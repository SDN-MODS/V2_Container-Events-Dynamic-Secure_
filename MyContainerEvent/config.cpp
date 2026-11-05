class CfgPatches
{
	class MyContainerEvent_Scripts
	{
		units[]=
		{
			"CONTAINER_Chest"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Camping"
		};
	};
};
class CfgMods
{
	class MyContainerEvent
	{
		dir="MyContainerEvent";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Container Event Mod";
		credits="SafeDoNordeste Thiago Santana";
		author="SafeDoNordeste Thiago Santana";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World",
			"Mission"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"MyContainerEvent/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"MyContainerEvent/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{

	class HouseNoDestruct;
	class SDN_P_fire_small_camp_01_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_camp_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_medium_camp_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_small_camp_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_medium_camp_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_camp_01_end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_steam_medium_camp_2end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_default_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_extinguish_wind: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_stove_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_stove_01_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_stove_01_end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_house_01_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_house_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_small_house_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_medium_house_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_medium_house_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_house_01_end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_steam_medium_house_2end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_barrel_01_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_barrel_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_small_barrel_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_medium_barrel_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_medium_barrel_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_barrel_01_end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_oven_01_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_oven_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_medium_oven_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_ovenl_01_end: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_bonfire: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_bonfire: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_tirepile: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_heli_wreck_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_santa_wreck: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_generic_wreck: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_menu_engine_fire: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_menu_evaporation: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_boiling_empty: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_boiling_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_boiling_done: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_baking_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_baking_done: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_drying_start: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_drying_done: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cooking_burning_done: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_item_hot_vapor: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_torch_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_torch_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_torch_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_broom_torch_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_broom_torch_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_broom_torch_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_roadflare_red_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_roadflare_red_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_roadflare_red_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_roadflare_red_04: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_flare_yellow_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_flare_red_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_flare_green_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fire_small_flare_blue_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_digging_ground: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_black_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_black_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_black_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_white_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_white_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_RDG2_white_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_green_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_green_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_green_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_purple_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_purple_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_purple_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_red_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_red_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_red_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_white_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_white_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_white_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_yellow_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_yellow_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_yellow_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_black_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_black_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_M18_black_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_M84_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_RGD5_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_M67_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_grenade_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_grenade_ice: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_landmine_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_placeholder: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_goat: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_claymore_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_explosion_plastic_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoke_small_generator_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_electro_shortc2: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_electro_sparks: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_easter_egg_activate: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_blood_bleeding_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_blood_bleeding_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_blood_surface_drops: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_blood_surface_chunks: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_character_vomit_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_character_vomit_puddle: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_character_vomitBlood_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_breath_vapour_light: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_breath_vapour_medium: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_breath_vapour_heavy: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_Flame_3D_4star: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_Flame_3D: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_fnx_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_fnx_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_cz75_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_akm_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_akm_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_izh18_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_izh18_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mp5k_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mp5k_02_boris: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mp5k_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_ump45_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_m4a1_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mp133_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_pellets: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mosin9130_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_mosin_compensator_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_cz527_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_sks_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_winch70_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_vss_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_ak74_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_ak101_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_magnum_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_cz61_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_chamber_smoke: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_chamber_smoke_raise: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_chamber_spark: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_winded_smoke: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_weapon_shot_winded_smoke_small: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoking_barrel: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoking_barrel_small: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoking_barrel_heavy: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoking_barrel_steam: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_smoking_barrel_steam_small: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_Hatchback_coolant_overheating: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_Hatchback_coolant_overheated: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_Hatchback_engine_failing: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_Hatchback_engine_failure: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_Hatchback_exhaust: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_vehicle_wheel_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_vehicle_wheel_gravel: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_vehicle_wheel_asphalt: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_around_tiny: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_around: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_bigass: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_ground: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_shell: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_contaminated_area_gas_bigass_debug: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_spooky_mist: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_small_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_small_02: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_small_03: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_small_04: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_large_01_Red: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_large_01_Green: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_large_01_Blue: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_large_01_Yellow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_large_01_Pink: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_ThankYou_anim: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_For10_anim: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_Amazing_anim: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_Years_anim: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fireworks_Dayz_anim: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_hotspring_watervapor: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_geyser_normal: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_geyser_strong: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_geyser_strong_splash: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_geyser_bubbles: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_volcano_smoke: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_water_jet: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_water_jet_weak: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_water_spilling: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_breath_bubbles: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_fishing_signal_splash: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_falling_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_soft_falling_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_soft_large_falling_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_small_falling_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_falling_needle: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_falling_leaf: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_bush_falling_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_tree_passing_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_bush_passing_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_step_snow: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_env_fly_swarm_01: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cupid_bolt: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_cupid_hit: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P__test_orientation: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_debug_dot: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	class SDN_P_debug_dot5m: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};
	
    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    

    
class SDN_P_contaminated_area_gas_grenade: HouseNoDestruct
	{
		scope=1;
		model="MyContainerEvent\data\SDN_Light.p3d";
	};

	class Inventory_Base;
	class SeaChest;
	class BaseBuildingBase: Inventory_Base
	{
	};
	class CONTAINER_Chest: SeaChest
	{
		scope=2;
		displayName="Reward Chest";
		descriptionShort="Chest containing rewards CONTAINER!";
		weight=10000;
		itemBehaviour=0;
		itemSize[]={10,5};
		canBeDigged=0;
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"MyContainerEvent\data\sea_chest_evente_container.paa"
		};
		attachments[]={};
		class DamageSystem
		{
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,50};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
	};
};
class CfgSoundSets
{
	class notification_SoundSet
	{
		soundShaders[]=
		{
			"notification_SoundShader"
		};
		volumeFactor=1;
	};
	class SDN_ContainerEvent_Crash_SoundSet
	{
		soundShaders[]=
		{
			"SDN_ContainerEvent_Crash_SoundShader"
		};
		volumeFactor=1;
		spatial=1;
		doppler=0;
		loop=0;
		distanceFilterInfo="defaultDistanceFilterInfo";
	};
};
class CfgSoundShaders
{
	class notification_SoundShader_Base
	{
		samples[]={};
		frequency=1;
		range=35;
		volume=0.75;
	};
	class notification_SoundShader: notification_SoundShader_Base
	{
		samples[]=
		{
			
			{
				"MyContainerEvent\Sounds\Sound01",
				1
			}
		};
	};
	class SDN_ContainerEvent_Crash_SoundShader_Base
	{
		samples[]={};
		frequency=1;
		range=1000;
		volume=1.0;
	};
	class SDN_ContainerEvent_Crash_SoundShader: SDN_ContainerEvent_Crash_SoundShader_Base
	{
		samples[]=
		{
			{
				"MyContainerEvent\Sounds\Sound_Event_Container",
				1
			}
		};
	};
};
