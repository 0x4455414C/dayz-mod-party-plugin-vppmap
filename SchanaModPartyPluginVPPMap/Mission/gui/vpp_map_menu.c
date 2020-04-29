modded class VPPMapMenu extends UIScriptedMenu
{
	override void DisplayPlayerPosition()
	{
		super.DisplayPlayerPosition();

		DisplaySchanaPartyPositions();
	}

	void DisplaySchanaPartyPositions()
	{
		SchanaPartyManagerClient client = GetSchanaPartyManagerClient();

		if (client)
		{
			auto positions = client.GetPositions();
			if (positions)
			{
				foreach (string id, vector position : positions)
				{
					m_MapWidget.AddUserMark(position, GetSchanaPartySettings().GetName(id), ARGB(255, 255, 255, 0), "VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa");
				}
			}
		}
	}
}