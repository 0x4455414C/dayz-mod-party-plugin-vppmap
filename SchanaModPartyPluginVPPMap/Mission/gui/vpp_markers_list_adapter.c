modded class MarkersListAdapter extends ListAdapter {
    private ref HeaderItem m_SchanaPartyMarkersHeaderItem;

    void MarkersListAdapter (VPPMapMenu menu, Widget root) {
        m_SchanaPartyMarkersHeaderItem = new HeaderItem ("SchanaParty Markers");
        m_HeaderItems.Insert (m_SchanaPartyMarkersHeaderItem);
    }

    HeaderItem GetSchanaPartyMarkersHeaderItem () {
        return m_SchanaPartyMarkersHeaderItem;
    }

    override void OnItemClicked (string widgetName, VPPMapItem item) {
        HeaderItem headerItem;
        if (widgetName == "collapse_icon" || widgetName == "expand_icon") {
            if (item.IsInherited (HeaderItem)) {
                headerItem = HeaderItem.Cast (item);
                headerItem.ToggleExpandCollapse ();
                UpdateContent ();
            }
        } else if (widgetName == "visible_button") {
            array<VerticalItem> items = new array<VerticalItem>;
            if (item.IsInherited (HeaderItem)) { // User clicked a HeaderItem
                headerItem = HeaderItem.Cast (item);
                items.Insert (headerItem);
                foreach (ListItem listItem : headerItem.m_ListItems) { // Add all children
                    items.Insert (listItem);
                }
            } else if (item.IsInherited (ListItem)) { // User clicked a ListItem
                items.Insert (VerticalItem.Cast (item));
            }
            if (items.Count () > 0 && item.IsInherited (VerticalItem)) {
                VerticalItem verticalItem = VerticalItem.Cast (item);
                switch (verticalItem.m_VisibleState) {
                    case VisibleState.NOT_VISIBLE:
                        SetVisible (items, VisibleState.VISIBLE_ONLY_2D);
                        break;
                    case VisibleState.VISIBLE_ONLY_2D:
                        SetVisible (items, VisibleState.VISIBLE_2D_AND_3D);
                        break;
                    case VisibleState.VISIBLE_2D_AND_3D:
                        SetVisible (items, VisibleState.NOT_VISIBLE);
                        break;
                }
            }
        } else if ((widgetName == "image" || widgetName == "text") && item.IsInherited (ListItem)) {
            ListItem list_item = ListItem.Cast (item);
            if (list_item.m_HeaderParent == GetClientMarkersHeaderItem ()) {
                m_Menu.ShowDialog (list_item.m_MarkerIndex, false, false, false);
            } else if (list_item.m_HeaderParent == GetCustomServerMarkersHeaderItem ()) {
                m_Menu.ShowDialog (list_item.m_MarkerIndex, true, false, false);
            } else if (list_item.m_HeaderParent == GetSchanaPartyMarkersHeaderItem ()) {
                m_Menu.ShowDialog (list_item.m_MarkerIndex, false, false, true);
            }
        }
    }

    override void SetVisible (array<VerticalItem> items, VisibleState visibleState) {
        array<int> markerIndexes = new array<int>;
        bool isClientMarker = false;
        bool isCustomServerMarker = false;
        bool isSchanaPartyMarker = false;
        foreach (VerticalItem item : items) {
            item.SetVisible (visibleState);
            if (item.IsInherited (ListItem)) {
                ListItem listItem = ListItem.Cast (item);
                if (listItem.m_HeaderParent == GetClientMarkersHeaderItem ()) {
                    isClientMarker = true;
                } else if (listItem.m_HeaderParent == GetCustomServerMarkersHeaderItem ()) {
                    isCustomServerMarker = true;
                } else if (listItem.m_HeaderParent == GetSchanaPartyMarkersHeaderItem ()) {
                    isSchanaPartyMarker = true;
                }
                markerIndexes.Insert (listItem.m_MarkerIndex);
            }
        }
        switch (visibleState) {
            case VisibleState.NOT_VISIBLE:
                m_Menu.EditMarkerVisibility (markerIndexes, false, false, isClientMarker, isCustomServerMarker, isSchanaPartyMarker);
                break;
            case VisibleState.VISIBLE_ONLY_2D:
                m_Menu.EditMarkerVisibility (markerIndexes, true, false, isClientMarker, isCustomServerMarker, isSchanaPartyMarker);
                break;
            case VisibleState.VISIBLE_2D_AND_3D:
                m_Menu.EditMarkerVisibility (markerIndexes, true, true, isClientMarker, isCustomServerMarker, isSchanaPartyMarker);
                break;
        }
    }
}