class CfgPatches
{
	class SchanaModPartyPluginVPPMap
	{
		requiredAddons[] = {};
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class SchanaModPartyPluginVPPMap
	{
		name = "SchanaModPartyPluginVPPMap";
		action = "https://github.com/schana/dayz-mod-party-plugin-vppmap";
		author = "schana";
		type = "mod";
		dependencies[] =
		{
			"Mission"
		};
		class defs
		{
			class missionScriptModule
			{
				value = "";
				files[] = {
					"SchanaModPartyPluginVPPMap/Mission"
				};
			};
		};
	};
};
