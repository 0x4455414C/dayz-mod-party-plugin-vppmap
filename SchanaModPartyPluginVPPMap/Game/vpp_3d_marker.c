modded class VPP3DMarker extends UIScriptedMenu {
    void VPP3DMarker (string MarkerName, string iconPath, vector MarkerPos, bool visible) {
        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.DoUpdate);
        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).CallLater (this.DoUpdate, 16, true);
    }
}