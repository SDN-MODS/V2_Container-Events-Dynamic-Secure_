class SDN_Particle_ByName extends House
{
	protected Particle m_SDNFx;

	void SDN_Particle_ByName()
	{
		if (GetGame() && !GetGame().IsDedicatedServer())
			SpawnFromClassName();
	}

	protected void SpawnFromClassName()
	{
		string t = GetType(); 
		string prefix = "SDN_P_";
		int idx = t.IndexOf(prefix);
		if (idx != 0) return;

		string pname = t.Substring(prefix.Length(), t.Length() - prefix.Length());
		int pid = ParticleList.GetParticleIDByName(pname);

		if (ParticleList.IsValidId(pid))
		{
			vector pos = "0 0 0";
			if (MemoryPointExists("light"))
				pos = GetMemoryPointPos("light");

			m_SDNFx = ParticleManager.GetInstance().PlayOnObject(pid, this, pos);
		}
		else
		{
			Print("[SDN] Particle не знайдено: " + pname);
		}
	}


	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (GetGame() && !GetGame().IsDedicatedServer())
		{
			if (m_SDNFx)
			{
				m_SDNFx.Stop();
				m_SDNFx = null;
			}
		}
	}
}
class SDN_P_fire_small_camp_01_start     extends SDN_Particle_ByName {}
class SDN_P_fire_small_camp_01           extends SDN_Particle_ByName {}
class SDN_P_fire_medium_camp_01          extends SDN_Particle_ByName {}
class SDN_P_smoke_small_camp_01          extends SDN_Particle_ByName {}
class SDN_P_smoke_medium_camp_01         extends SDN_Particle_ByName {}
class SDN_P_fire_small_camp_01_end       extends SDN_Particle_ByName {}
class SDN_P_steam_medium_camp_2end       extends SDN_Particle_ByName {}
class SDN_P_default_01                   extends SDN_Particle_ByName {} 
class SDN_P_fire_extinguish_wind         extends SDN_Particle_ByName {}

class SDN_P_fire_small_stove_01          extends SDN_Particle_ByName {}
class SDN_P_fire_small_stove_01_start    extends SDN_Particle_ByName {}
class SDN_P_fire_small_stove_01_end      extends SDN_Particle_ByName {}

class SDN_P_fire_small_house_01_start    extends SDN_Particle_ByName {}
class SDN_P_fire_small_house_01          extends SDN_Particle_ByName {}
class SDN_P_smoke_small_house_01         extends SDN_Particle_ByName {}
class SDN_P_fire_medium_house_01         extends SDN_Particle_ByName {}
class SDN_P_smoke_medium_house_01        extends SDN_Particle_ByName {}
class SDN_P_fire_small_house_01_end      extends SDN_Particle_ByName {}
class SDN_P_steam_medium_house_2end      extends SDN_Particle_ByName {}

class SDN_P_fire_small_barrel_01_start   extends SDN_Particle_ByName {}
class SDN_P_fire_small_barrel_01         extends SDN_Particle_ByName {}
class SDN_P_smoke_small_barrel_01        extends SDN_Particle_ByName {}
class SDN_P_fire_medium_barrel_01        extends SDN_Particle_ByName {}
class SDN_P_smoke_medium_barrel_01       extends SDN_Particle_ByName {}
class SDN_P_fire_small_barrel_01_end     extends SDN_Particle_ByName {}

class SDN_P_fire_small_oven_01_start     extends SDN_Particle_ByName {}
class SDN_P_fire_small_oven_01           extends SDN_Particle_ByName {}
class SDN_P_fire_medium_oven_01          extends SDN_Particle_ByName {}
class SDN_P_fire_small_ovenl_01_end      extends SDN_Particle_ByName {} 

class SDN_P_fire_bonfire                  extends SDN_Particle_ByName {}
class SDN_P_smoke_bonfire                 extends SDN_Particle_ByName {}
class SDN_P_fire_tirepile                 extends SDN_Particle_ByName {}

class SDN_P_smoke_heli_wreck_01           extends SDN_Particle_ByName {}
class SDN_P_smoke_santa_wreck             extends SDN_Particle_ByName {}
class SDN_P_smoke_generic_wreck           extends SDN_Particle_ByName {}
class SDN_P_menu_engine_fire              extends SDN_Particle_ByName {}

class SDN_P_menu_evaporation              extends SDN_Particle_ByName {}

class SDN_P_cooking_boiling_empty  extends SDN_Particle_ByName {}
class SDN_P_cooking_boiling_start  extends SDN_Particle_ByName {}
class SDN_P_cooking_boiling_done   extends SDN_Particle_ByName {}
class SDN_P_cooking_baking_start   extends SDN_Particle_ByName {}
class SDN_P_cooking_baking_done    extends SDN_Particle_ByName {}
class SDN_P_cooking_drying_start   extends SDN_Particle_ByName {}
class SDN_P_cooking_drying_done    extends SDN_Particle_ByName {}
class SDN_P_cooking_burning_done   extends SDN_Particle_ByName {}
class SDN_P_item_hot_vapor         extends SDN_Particle_ByName {}

class SDN_P_fire_small_torch_01           extends SDN_Particle_ByName {}
class SDN_P_fire_small_torch_02           extends SDN_Particle_ByName {}
class SDN_P_fire_small_torch_03           extends SDN_Particle_ByName {}

class SDN_P_fire_small_broom_torch_01     extends SDN_Particle_ByName {}
class SDN_P_fire_small_broom_torch_02     extends SDN_Particle_ByName {}
class SDN_P_fire_small_broom_torch_03     extends SDN_Particle_ByName {}

class SDN_P_fire_small_roadflare_red_01   extends SDN_Particle_ByName {}
class SDN_P_fire_small_roadflare_red_02   extends SDN_Particle_ByName {}
class SDN_P_fire_small_roadflare_red_03   extends SDN_Particle_ByName {}
class SDN_P_fire_small_roadflare_red_04   extends SDN_Particle_ByName {} 

class SDN_P_fire_small_flare_yellow_01    extends SDN_Particle_ByName {}
class SDN_P_fire_small_flare_red_01       extends SDN_Particle_ByName {}
class SDN_P_fire_small_flare_green_01     extends SDN_Particle_ByName {}
class SDN_P_fire_small_flare_blue_01      extends SDN_Particle_ByName {}

class SDN_P_digging_ground extends SDN_Particle_ByName {}

class SDN_P_smoke_RDG2_black_01  extends SDN_Particle_ByName {} // START
class SDN_P_smoke_RDG2_black_02  extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_RDG2_black_03  extends SDN_Particle_ByName {} // END

class SDN_P_smoke_RDG2_white_01  extends SDN_Particle_ByName {} // START
class SDN_P_smoke_RDG2_white_02  extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_RDG2_white_03  extends SDN_Particle_ByName {} // END


class SDN_P_smoke_M18_green_01   extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_green_02   extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_green_03   extends SDN_Particle_ByName {} // END

class SDN_P_smoke_M18_purple_01  extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_purple_02  extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_purple_03  extends SDN_Particle_ByName {} // END

class SDN_P_smoke_M18_red_01     extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_red_02     extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_red_03     extends SDN_Particle_ByName {} // END

class SDN_P_smoke_M18_white_01   extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_white_02   extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_white_03   extends SDN_Particle_ByName {} // END

class SDN_P_smoke_M18_yellow_01  extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_yellow_02  extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_yellow_03  extends SDN_Particle_ByName {} // END

class SDN_P_smoke_M18_black_01   extends SDN_Particle_ByName {} // START
class SDN_P_smoke_M18_black_02   extends SDN_Particle_ByName {} // LOOP
class SDN_P_smoke_M18_black_03   extends SDN_Particle_ByName {} // END

class SDN_P_explosion_M84_01            extends SDN_Particle_ByName {}

class SDN_P_explosion_RGD5_01           extends SDN_Particle_ByName {}
class SDN_P_explosion_M67_01            extends SDN_Particle_ByName {}

class SDN_P_explosion_grenade_snow      extends SDN_Particle_ByName {}
class SDN_P_explosion_grenade_ice       extends SDN_Particle_ByName {}

class SDN_P_explosion_landmine_01       extends SDN_Particle_ByName {}
class SDN_P_explosion_placeholder       extends SDN_Particle_ByName {}
class SDN_P_explosion_goat              extends SDN_Particle_ByName {}

class SDN_P_explosion_claymore_01       extends SDN_Particle_ByName {}
class SDN_P_explosion_plastic_01        extends SDN_Particle_ByName {}

class SDN_P_smoke_small_generator_01  extends SDN_Particle_ByName {} // POWER_GENERATOR_SMOKE
class SDN_P_electro_shortc2          extends SDN_Particle_ByName {} // BARBED_WIRE_SPARKS
class SDN_P_electro_sparks           extends SDN_Particle_ByName {} // LEVER_SPARKS
class SDN_P_easter_egg_activate      extends SDN_Particle_ByName {} // EASTER_EGG_ACTIVATE

class SDN_P_blood_bleeding_01       extends SDN_Particle_ByName {}
class SDN_P_blood_bleeding_02       extends SDN_Particle_ByName {}
class SDN_P_blood_surface_drops     extends SDN_Particle_ByName {}
class SDN_P_blood_surface_chunks    extends SDN_Particle_ByName {}

class SDN_P_character_vomit_01      extends SDN_Particle_ByName {}
class SDN_P_character_vomit_puddle  extends SDN_Particle_ByName {}
class SDN_P_character_vomitBlood_01 extends SDN_Particle_ByName {}

class SDN_P_breath_vapour_light     extends SDN_Particle_ByName {}
class SDN_P_breath_vapour_medium    extends SDN_Particle_ByName {}
class SDN_P_breath_vapour_heavy     extends SDN_Particle_ByName {}

class SDN_P_weapon_shot_Flame_3D_4star      extends SDN_Particle_ByName {} // SVD STAR
class SDN_P_weapon_shot_Flame_3D            extends SDN_Particle_ByName {} // Solo muzzle flash

class SDN_P_weapon_shot_fnx_01              extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_fnx_02              extends SDN_Particle_ByName {} // suppressed

class SDN_P_weapon_shot_cz75_01             extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_akm_01              extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_akm_02              extends SDN_Particle_ByName {} // suppressed
class SDN_P_weapon_shot_izh18_01            extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_izh18_02            extends SDN_Particle_ByName {} // suppressed
class SDN_P_weapon_shot_mp5k_01             extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_mp5k_02_boris       extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_mp5k_02             extends SDN_Particle_ByName {} // suppressed/compensator
class SDN_P_weapon_shot_ump45_01            extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_m4a1_01             extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_mp133_01            extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_pellets             extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_mosin9130_01        extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_mosin_compensator_01 extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_cz527_01            extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_sks_01              extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_winch70_01          extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_vss_01              extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_ak74_01             extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_ak101_01            extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_magnum_01           extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_cz61_01             extends SDN_Particle_ByName {}

class SDN_P_weapon_shot_chamber_smoke       extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_chamber_smoke_raise extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_chamber_spark       extends SDN_Particle_ByName {} // SMARKS_CHAMBER

class SDN_P_weapon_shot_winded_smoke        extends SDN_Particle_ByName {}
class SDN_P_weapon_shot_winded_smoke_small  extends SDN_Particle_ByName {}

class SDN_P_smoking_barrel                  extends SDN_Particle_ByName {}
class SDN_P_smoking_barrel_small            extends SDN_Particle_ByName {}
class SDN_P_smoking_barrel_heavy            extends SDN_Particle_ByName {}
class SDN_P_smoking_barrel_steam            extends SDN_Particle_ByName {}
class SDN_P_smoking_barrel_steam_small      extends SDN_Particle_ByName {}

class SDN_P_Hatchback_coolant_overheating  extends SDN_Particle_ByName {}
class SDN_P_Hatchback_coolant_overheated   extends SDN_Particle_ByName {}
class SDN_P_Hatchback_engine_failing       extends SDN_Particle_ByName {}
class SDN_P_Hatchback_engine_failure       extends SDN_Particle_ByName {}
class SDN_P_Hatchback_exhaust              extends SDN_Particle_ByName {}

class SDN_P_vehicle_wheel_snow             extends SDN_Particle_ByName {}
class SDN_P_vehicle_wheel_gravel           extends SDN_Particle_ByName {}
class SDN_P_vehicle_wheel_asphalt          extends SDN_Particle_ByName {}

class SDN_P_contaminated_area_gas_around_tiny  extends SDN_Particle_ByName {}
class SDN_P_contaminated_area_gas_around       extends SDN_Particle_ByName {}
class SDN_P_contaminated_area_gas_bigass       extends SDN_Particle_ByName {}
class SDN_P_contaminated_area_gas_ground       extends SDN_Particle_ByName {}
class SDN_P_contaminated_area_gas_shell        extends SDN_Particle_ByName {}
class SDN_P_contaminated_area_gas_bigass_debug extends SDN_Particle_ByName {}

class SDN_P_spooky_mist                        extends SDN_Particle_ByName {}

class SDN_P_fireworks_small_01         extends SDN_Particle_ByName {} // SHOT
class SDN_P_fireworks_small_02         extends SDN_Particle_ByName {} // AFTERBURN_START
class SDN_P_fireworks_small_03         extends SDN_Particle_ByName {} // AFTERBURN_END
class SDN_P_fireworks_small_04         extends SDN_Particle_ByName {} // FUSE

class SDN_P_fireworks_large_01_Red     extends SDN_Particle_ByName {}
class SDN_P_fireworks_large_01_Green   extends SDN_Particle_ByName {}
class SDN_P_fireworks_large_01_Blue    extends SDN_Particle_ByName {}
class SDN_P_fireworks_large_01_Yellow  extends SDN_Particle_ByName {}
class SDN_P_fireworks_large_01_Pink    extends SDN_Particle_ByName {}

class SDN_P_fireworks_ThankYou_anim    extends SDN_Particle_ByName {}
class SDN_P_fireworks_For10_anim       extends SDN_Particle_ByName {}
class SDN_P_fireworks_Amazing_anim     extends SDN_Particle_ByName {}
class SDN_P_fireworks_Years_anim       extends SDN_Particle_ByName {}
class SDN_P_fireworks_Dayz_anim        extends SDN_Particle_ByName {}

class SDN_P_hotspring_watervapor   extends SDN_Particle_ByName {}
class SDN_P_geyser_normal          extends SDN_Particle_ByName {}
class SDN_P_geyser_strong          extends SDN_Particle_ByName {}
class SDN_P_geyser_strong_splash   extends SDN_Particle_ByName {}
class SDN_P_geyser_bubbles         extends SDN_Particle_ByName {}
class SDN_P_volcano_smoke          extends SDN_Particle_ByName {}

class SDN_P_water_jet              extends SDN_Particle_ByName {}
class SDN_P_water_jet_weak         extends SDN_Particle_ByName {}
class SDN_P_water_spilling         extends SDN_Particle_ByName {}

class SDN_P_breath_bubbles         extends SDN_Particle_ByName {} 
class SDN_P_fishing_signal_splash  extends SDN_Particle_ByName {}  

class SDN_P_tree_falling_snow              extends SDN_Particle_ByName {}
class SDN_P_tree_soft_falling_snow         extends SDN_Particle_ByName {}
class SDN_P_tree_soft_large_falling_snow   extends SDN_Particle_ByName {}
class SDN_P_tree_small_falling_snow        extends SDN_Particle_ByName {}
class SDN_P_tree_falling_needle            extends SDN_Particle_ByName {}
class SDN_P_tree_falling_leaf              extends SDN_Particle_ByName {}
class SDN_P_bush_falling_snow              extends SDN_Particle_ByName {}

class SDN_P_tree_passing_snow              extends SDN_Particle_ByName {}
class SDN_P_bush_passing_snow              extends SDN_Particle_ByName {}

class SDN_P_step_snow                      extends SDN_Particle_ByName {}

class SDN_P_env_fly_swarm_01               extends SDN_Particle_ByName {}

class SDN_P_cupid_bolt  extends SDN_Particle_ByName {} 
class SDN_P_cupid_hit   extends SDN_Particle_ByName {} 

class SDN_P__test_orientation            extends SDN_Particle_ByName {}
class SDN_P_debug_dot                    extends SDN_Particle_ByName {}
class SDN_P_debug_dot5m                  extends SDN_Particle_ByName {}

class SDN_P_contaminated_area_gas_grenade extends SDN_Particle_ByName {}
