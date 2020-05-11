modded class VPPMapMenu extends UIScriptedMenu {
	private ref array<ref MarkerInfo> m_SchanaPartyMarkers = new ref array<ref MarkerInfo>;

	override void DisplayPlayerPosition () {
		DisplayClientMarkers ();
		DisplayServerMarkers ();
		super.DisplayPlayerPosition ();

		DisplaySchanaPartyPositions ();
		m_Adapter.UpdateContent ();
	}

	void DisplaySchanaPartyPositions () {
		auto client = GetSchanaPartyManagerClient ();

		if (client) {
			auto positions = client.GetPositions ();
			if (positions) {
				foreach (string id, vector position : positions) {
					m_MapWidget.AddUserMark (position, GetSchanaPartySettings ().GetName (id), ARGB (255, 255, 255, 0), "VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa");
				}
			}
		}

		auto partyMarkerClient = GetSchanaPartyMarkerManagerClient ();

		if (partyMarkerClient) {
			auto serverMarkers = partyMarkerClient.GetServerMarkers ();
			if (serverMarkers) {
				foreach (auto serverMarker : serverMarkers) {
					m_MapWidget.AddUserMark (serverMarker.GetPosition (), serverMarker.GetName (), ARGB (255, 255, 255, 0), "VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa");
				}
			}
		}
	}

	override void DisplayClientMarkers () {
		m_MapWidget.ClearUserMarks ();
		g_Game.SetClient3dMarkers (NULL); //Clear client 3d markers array
		g_Game.SetServer3dMarkers (NULL); //Clear server 3d markers array
		HeaderItem headerItem = m_Adapter.GetClientMarkersHeaderItem ();
		BuildClientHeaderItem (headerItem, m_ClientMarkers);
		headerItem = m_Adapter.GetCustomServerMarkersHeaderItem ();
		BuildClientHeaderItem (headerItem, m_CustomServerMarkers);

		headerItem = m_Adapter.GetSchanaPartyMarkersHeaderItem ();
		BuildClientHeaderItem (headerItem, m_SchanaPartyMarkers);

		m_Adapter.UpdateContent ();
	}

	override void MapDoubleClick (Widget w, int x, int y, int button) {
		if (button == MouseState.LEFT) {
			//string name, string path, vector color, vector pos, bool isActive = true, bool is3DActive = false
			int index;
			string suffix;
			if (m_LastEditedMarker) {
				string name = m_LastEditedMarkerName;
				if (m_MarkerSuffix > 0) {
					suffix = " [" + m_MarkerSuffix + "]";
					name += suffix;
				}
				m_MarkerSuffix++;
				index = m_ClientMarkers.Insert (new MarkerInfo (name, m_LastEditedMarker.GetIconPath (), m_LastEditedMarker.GetColor (), GetMapClickPos (), true, true));
			} else {
				index = m_ClientMarkers.Insert (new MarkerInfo ("Marker", "dz\\gear\\navigation\\data\\map_border_cross_ca.paa", Vector (255, 255, 255), GetMapClickPos (), true, true));
			}
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_ClientMarkers);
			g_Game.GetClientMarkersCache ().SaveCache (); //Save directly to json
			//Redraw markers
			ReloadMarkers ();
			ShowDialog (index, false, true, false, suffix);
		} else if (button == MouseState.RIGHT) {
			//Remove Marker by positon
			RemoveMarker (GetMapClickPos ());
		}
	}

	override void RemoveMarkerExact (vector pos, bool isCustomServer) {
		bool found = false;
		for (int i = 0; i < m_SchanaPartyMarkers.Count (); ++i) {
			MarkerInfo marker = m_SchanaPartyMarkers.Get (i);
			if (marker.GetPosition () == pos) {
				found = true;
				m_SchanaPartyMarkers.Remove (i);
				break;
			}
		}
		if (!found) {
			super.RemoveMarkerExact (pos, isCustomServer);
		} else {
			SchanaUpdatePartyMarkers ();
			ReloadMarkers ();
		}
	}

	override void RemoveMarker (vector relativePos) {
		float minDistance = 86, distance;
		ref MarkerInfo nearestMarker;
		bool isCustomServerMarker = false;
		foreach (MarkerInfo clientMarker : m_ClientMarkers) {
			distance = vector.Distance (relativePos, clientMarker.GetPosition ());
			if (distance < minDistance) {
				minDistance = distance;
				nearestMarker = clientMarker;
			}
		}
		foreach (MarkerInfo marker : m_CustomServerMarkers) {
			distance = vector.Distance (relativePos, marker.GetPosition ());
			if (distance < minDistance) {
				minDistance = distance;
				nearestMarker = marker;
				isCustomServerMarker = true;
			}
		}
		foreach (MarkerInfo partyMarker : m_SchanaPartyMarkers) {
			distance = vector.Distance (relativePos, partyMarker.GetPosition ());
			if (distance < minDistance) {
				minDistance = distance;
				nearestMarker = partyMarker;
			}
		}
		if (nearestMarker) {
			RemoveMarkerExact (nearestMarker.GetPosition (), isCustomServerMarker);
		}
	}

	void ShowDialog (int markerIndex, bool isCustomServer, bool isNewMarker, bool isSchanaPartyMarker, string markerSuffix = "") {
		g_Game.HideAll3dMarkers ();
		if (!m_EditDialog) {
			m_EditDialog = new EditDialog (this, m_PanelEditDialog);
		}
		m_MapWidget.Show (false);
		m_MapFakeBg.Show (true);
		m_PanelEditDialog.Show (true);
		ref array<ref MarkerInfo> markers;
		if (isSchanaPartyMarker) {
			markers = m_SchanaPartyMarkers;
		} else if (isCustomServer) {
			markers = m_CustomServerMarkers;
		} else {
			markers = m_ClientMarkers;
		}
		MarkerInfo marker = markers.Get (markerIndex);
		m_EditDialog.SetMarker (markerIndex, marker, false, isNewMarker, markerSuffix, isSchanaPartyMarker);
	}

	ref array<ref MarkerInfo> GetSchanaPartyMarkers () {
		return m_SchanaPartyMarkers;
	}

	void EditMarkerVisibility (array<int> indexes, bool show2D, bool show3D, bool isClientMarker, bool isCustomServerMarker, bool isSchanaPartyMarker) {
		array<ref MarkerInfo> markers;
		foreach (int index : indexes) {
			if (isSchanaPartyMarker) {
				markers = m_SchanaPartyMarkers;
			} else if (isClientMarker) {
				markers = m_ClientMarkers;
			} else if (isCustomServerMarker) {
				markers = m_CustomServerMarkers;
			} else {
				markers = m_ServerMarkers;
			}
			MarkerInfo marker = markers.Get (index);
			marker.SetActive (show2D);
			marker.Set3DActive (show3D);
		}

		if (isClientMarker || isCustomServerMarker) {
			string addressPort = "";
			if (isCustomServerMarker) {
				addressPort = m_AddressPort;
			}
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (markers, addressPort);
			g_Game.GetClientMarkersCache ().SaveCache (); //Save directly to json
		}
		ReloadMarkers (); //Reload all markers
	}

	void EditMarkerAttributes (int index, string name, string markerSuffix, string iconPath, vector color, bool wasCustomServer, bool isCustomServer, bool isSchanaPartyMarker, bool isNewMarker) {
		ref array<ref MarkerInfo> markers;
		if (isSchanaPartyMarker) {
			markers = m_SchanaPartyMarkers;
		} else if (wasCustomServer) {
			markers = m_CustomServerMarkers;
		} else {
			markers = m_ClientMarkers;
		}

		if (isNewMarker && isSchanaPartyMarker) {
			m_LastEditedMarker = m_ClientMarkers.Get (index);
			m_ClientMarkers.Remove (index);
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_ClientMarkers);
			m_SchanaPartyMarkers.Insert (m_LastEditedMarker);
		} else {
			m_LastEditedMarker = markers.Get (index);
		}

		m_LastEditedMarker.SetName (name);
		m_LastEditedMarker.SetIconPath (iconPath);
		m_LastEditedMarker.SetColor (color);

		if (isSchanaPartyMarker) {
			SchanaUpdatePartyMarkers ();
		}

		string markerName = name;
		if (markerSuffix) {
			markerName.Replace (markerSuffix, "");
		}
		if (!m_LastEditedMarkerName || m_LastEditedMarkerName != markerName) {
			m_MarkerSuffix = 1;
			m_LastEditedMarkerName = markerName;
		}

		if (wasCustomServer && !isCustomServer) {
			g_Game.GetClientMarkersCache ().RemoveCachedMarker (m_LastEditedMarker.GetPosition (), m_AddressPort);
			m_ClientMarkers.Insert (m_LastEditedMarker);
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_ClientMarkers);
		} else if (!wasCustomServer && isCustomServer) {
			g_Game.GetClientMarkersCache ().RemoveCachedMarker (m_LastEditedMarker.GetPosition ());
			m_CustomServerMarkers.Insert (m_LastEditedMarker);
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_CustomServerMarkers, m_AddressPort);
		} else if (isCustomServer && wasCustomServer) {
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_CustomServerMarkers, m_AddressPort);
		} else if (!isCustomServer && !wasCustomServer) {
			g_Game.GetClientMarkersCache ().UpdateCachedMarkerInfos (m_ClientMarkers);
		}
		g_Game.GetClientMarkersCache ().SaveCache (); //Save directly to json
		ReloadMarkers (); //Reload all markers
	}

	void SchanaUpdatePartyMarkers () {
		auto client = GetSchanaPartyMarkerManagerClient ();
		if (client) {
			client.Reset ();
			foreach (MarkerInfo partyMarker : m_SchanaPartyMarkers) {
				client.Add (new SchanaPartyMarkerInfo (partyMarker.GetName (), partyMarker.GetPosition ()));
			}
			client.Send ();
		}
	}
}