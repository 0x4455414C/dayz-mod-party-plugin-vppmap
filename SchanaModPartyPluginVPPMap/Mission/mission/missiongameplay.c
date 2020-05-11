modded class MissionGameplay extends MissionBase {
    override void OnInit () {
        super.OnInit ();
        delete g_SchanaPartyMarkerManagerClient;
        GetSchanaPartyMarkerManagerClient ();
    }
}