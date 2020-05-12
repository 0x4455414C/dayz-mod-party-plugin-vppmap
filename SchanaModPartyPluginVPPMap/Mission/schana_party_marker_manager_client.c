class SchanaPartyMarkerManagerClient {
    private ref array<ref SchanaPartyMarkerInfo> markers;
    private ref array<ref SchanaPartyMarkerInfo> serverMarkers;

    void SchanaPartyMarkerManagerClient () {
        SchanaPartyUtils.LogMessage ("PartyMarker Client Init " + MissionBase.SCHANA_PARTY_PLUGIN_VPP_MAP_VERSION);
        markers = new ref array<ref SchanaPartyMarkerInfo>;
        serverMarkers = new ref array<ref SchanaPartyMarkerInfo>;

        GetRPCManager ().AddRPC ("SchanaModPartyPluginVPPMap", "ClientUpdatePartyMarkersRPC", this, SingleplayerExecutionType.Both);
    }

    void ClientUpdatePartyMarkersRPC (CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        Param1<ref array<ref SchanaPartyMarkerInfo>> data;
        if (!ctx.Read (data))
            return;

        ClientUpdatePartyMarkers (data.param1);
    }

    void ClientUpdatePartyMarkers (ref array<ref SchanaPartyMarkerInfo> newServerMarkers) {
        SchanaPartyUtils.LogMessage ("ClientUpdatePartyMarkers");
        serverMarkers = newServerMarkers;
    }

    void Add (SchanaPartyMarkerInfo marker) {
        markers.Insert (marker);
    }

    void Reset () {
        markers.Clear ();
    }

    ref array<ref SchanaPartyMarkerInfo> GetServerMarkers () {
        return serverMarkers;
    }

    void Send () {
        SchanaPartyUtils.LogMessage ("SendMarkers");
        auto data = new Param1<ref array<ref SchanaPartyMarkerInfo>> (markers);
        GetRPCManager ().SendRPC ("SchanaModPartyPluginVPPMap", "ServerRegisterMarkersRPC", data);
    }
}

static ref SchanaPartyMarkerManagerClient g_SchanaPartyMarkerManagerClient;
static ref SchanaPartyMarkerManagerClient GetSchanaPartyMarkerManagerClient () {
    if (g_Game.IsClient () && !g_SchanaPartyMarkerManagerClient) {
        g_SchanaPartyMarkerManagerClient = new SchanaPartyMarkerManagerClient;
    }
    return g_SchanaPartyMarkerManagerClient;
}