modded class EditDialog extends ScriptedWidgetEventHandler {
    CheckBoxWidget m_CheckboxSchanaPartyMarker;
    bool m_isSchanaPartyMarker = false;
    bool m_isSchanaPartyNewMarker = false;

    void EditDialog (VPPMapMenu menu, Widget root) {
        m_Menu = menu;
        m_Root = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModPartyPluginVPPMap/GUI/Layouts/EditDialog.layout", root);
        m_EditBoxName = EditBoxWidget.Cast (m_Root.FindAnyWidget ("editbox_name"));
        m_EditBoxNameOverlay = m_Root.FindAnyWidget ("editbox_name_overlay");
        m_EditBoxNameOverlay.Show (false);
        m_PanelColor = m_Root.FindAnyWidget ("panel_color");
        m_PanelIcon = m_Root.FindAnyWidget ("panel_icon");
        m_ButtonDelete = ButtonWidget.Cast (m_Root.FindAnyWidget ("button_delete"));
        m_ButtonOK = ButtonWidget.Cast (m_Root.FindAnyWidget ("button_ok"));
        m_PanelClose = m_Root.FindAnyWidget ("panel_close");
        m_ButtonClose = ButtonWidget.Cast (m_Root.FindAnyWidget ("button_close"));
        m_CheckboxCustomServer = CheckBoxWidget.Cast (m_Root.FindAnyWidget ("checkbox_customserver"));
        m_CheckboxSchanaPartyMarker = CheckBoxWidget.Cast (m_Root.FindAnyWidget ("checkbox_schanapartymarker"));

        m_IconListAdapter = new IconListAdapter (m_PanelIcon);
        m_ColorListAdapter = new ColorListAdapter (this, m_PanelColor);

        m_Root.SetHandler (this);
    }

    void SetMarker (int markerIndex, MarkerInfo marker, bool isCustomServer, bool isNewMarker, string markerSuffix, bool isSchanaPartyMarker) {
        SetMarker (markerIndex, marker, isCustomServer, isNewMarker, markerSuffix);
        m_isSchanaPartyMarker = isSchanaPartyMarker;
        m_CheckboxSchanaPartyMarker.SetChecked (m_isSchanaPartyMarker);
        m_isSchanaPartyNewMarker = isNewMarker;
    }

    override bool OnMouseButtonDown (Widget w, int x, int y, int button) {
        if (button == MouseState.LEFT) {
            if (w == m_PanelClose || w == m_ButtonClose) {
                m_Menu.HideDialog ();
                return true;
            } else if (w == m_ButtonDelete) {
                m_Menu.RemoveMarkerExact (m_Marker.GetPosition (), m_IsCustomServerMarker);
                m_Menu.HideDialog ();
                return true;
            } else if (w == m_ButtonOK) {
                string name = m_EditBoxName.GetText ();
                string iconPath = m_IconListAdapter.GetSelectedIcon ();
                vector color = m_ColorListAdapter.GetSelectedColor ();
                bool checked = m_CheckboxCustomServer.IsChecked ();
                bool isSchanaPartyMarker = m_CheckboxSchanaPartyMarker.IsChecked ();
                m_Menu.EditMarkerAttributes (m_MarkerIndex, name, m_MarkerSuffix, iconPath, color, m_IsCustomServerMarker, checked, isSchanaPartyMarker, m_isSchanaPartyNewMarker);
                m_Menu.HideDialog ();
                return true;
            } else if (w == m_EditBoxNameOverlay) {
                m_EditBoxName.SetText ("");
                m_EditBoxNameOverlay.Show (false);
                return false;
            }
        }
        return false;
    }
}