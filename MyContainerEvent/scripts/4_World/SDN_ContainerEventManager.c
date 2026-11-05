class SDN_SceneObjectTemplate
{
    string ClassName;
    vector OffsetPos;
    vector OffsetOri; // SEMPRE armazenado em GRAUS (Yaw, Pitch, Roll)

    // oriInRadians = true  -> o vetor foi exportado em RADIANOS (ex: "-0.532299 0.000000 1.570796") e é convertido aqui.
    void SDN_SceneObjectTemplate(string cls, vector pos, vector ori, bool oriInRadians = false)
    {
        ClassName = cls;
        OffsetPos = pos;

        if (oriInRadians)
        {
            OffsetOri = Vector(ori[0] * Math.RAD2DEG, ori[1] * Math.RAD2DEG, ori[2] * Math.RAD2DEG);
        }
        else
        {
            OffsetOri = ori;
        }
    }
}

class ActiveEventInstance
{
    int EventID;
    vector Position;
    string LocationName;
    
    EntityAI Container; 
    ItemBase LootCrate;
    
    // Controle Visual (Fumaça)
    ItemBase VisualEffect; 
    float SmokeTimer; // Cronômetro para trocar a granada antes que apague
    string CurrentSmokeColor; // Para saber se precisamos trocar a cor
    
    int InitialLootCount; 
    
    string ContainerType;
    string KeyType;
    int Tier; 
    
    ref array<EntityAI> Zombies;
    ref array<Object> SceneObjects;
    bool IsActive;      
    bool IsCompleted;   
    float EventTimer;   
    float LifeTime;     
    float DebugTimer; 

    void ActiveEventInstance()
    {
        Zombies = new array<EntityAI>;
        SceneObjects = new array<Object>;
        IsActive = false;
        IsCompleted = false;
        EventTimer = 0;
        LifeTime = 0;
        KeyType = "";
        DebugTimer = 0;
        InitialLootCount = 0;
        
        SmokeTimer = 0;
        CurrentSmokeColor = "";
    }

    void Cleanup()
    {
        if (VisualEffect) GetGame().ObjectDelete(VisualEffect);
        if (LootCrate) GetGame().ObjectDelete(LootCrate);
        if (Container) GetGame().ObjectDelete(Container);
        
        if (Zombies)
        {
            foreach (EntityAI zmb : Zombies)
            {
                if (zmb && zmb.IsAlive()) GetGame().ObjectDelete(zmb);
            }
        }
        Zombies.Clear();
        if (SceneObjects)
        {
            foreach (Object sc : SceneObjects)
            {
                if (sc)
                {
                    ItemBase sc_ib = ItemBase.Cast(sc);
                    if (sc_ib)
                    {
                        // Descongela a física e força sync de rede antes de apagar,
                        // evitando Network Ghosting (ítens flutuando no client).
                        dBodyDynamic(sc_ib, true);
                        dBodyActive(sc_ib, ActiveState.ACTIVE);
                    }
                    GetGame().ObjectDelete(sc);
                }
            }
        }
        SceneObjects.Clear();
    }
}

enum SDN_NotificationRPC
{
    SDN_EVENT_MSG = 8410225,
    SDN_EVENT_CRASH_SOUND = 8410227
}

class SDN_ContainerEventManager
{
    protected ref SDN_ContainerEventConfig m_Config;
    protected ref array<ref ActiveEventInstance> m_ActiveEvents;
    
    protected float m_UpdateTimer;
    protected float m_SpawnTimer;        
    protected float m_CurrentTargetTime; 
    protected float m_SaveTimer;
    
    // --- SAFE START ---
    protected bool m_IsInitialized;      
    protected float m_InitDelayTimer;    
    
    protected int m_EventsSpawnedInWave; 
    protected string m_LastSpawnedLocName;

    protected ref array<string> m_HistoryLocations;      
    protected ref array<string> m_CurrentWaveLocations;
    protected ref array<ref SDN_SceneObjectTemplate> m_SceneTemplate;  
    protected ref array<vector> m_GhostLocations;

    // [SDN] Nova estrutura de pastas padronizada
    const string MOD_FOLDER_ROOT = "$profile:SDN_MODS";
    const string CONFIG_ROOT = "$profile:SDN_MODS/ContainerEvent";
    const string CONFIG_PATH = "$profile:SDN_MODS/ContainerEvent/EventConfig.json";
    const string STATE_PATH = "$profile:SDN_MODS/ContainerEvent/EventState.json";
    const string LOG_PATH = "$profile:SDN_MODS/ContainerEvent/EventLog.txt";

    const string ICON_PATH = "MyContainerEvent/data/gui/icon_containe_event.edds";

    void SDN_ContainerEventManager()
    {
        m_ActiveEvents = new array<ref ActiveEventInstance>;
        m_HistoryLocations = new array<string>;
        m_CurrentWaveLocations = new array<string>;
        m_GhostLocations = new array<vector>;
        
        m_SpawnTimer = 0;
        m_EventsSpawnedInWave = 0;
        m_LastSpawnedLocName = "";
        m_SaveTimer = 0;
        
        m_IsInitialized = false;
        m_InitDelayTimer = 0;
        
        if (!FileExist(MOD_FOLDER_ROOT))
        {
            MakeDirectory(MOD_FOLDER_ROOT);
        }
        
        if (!FileExist(CONFIG_ROOT))
        {
            MakeDirectory(CONFIG_ROOT);
        }
        
        LoadConfig();
        InitSceneTemplate();
        Log("Mod loaded. Waiting for server stabilization (Safe Start)...");
    }

    void RunDelayedInit()
    {
        Log("Server stabilized. Starting event and ghost scan...");
        LoadState();        
        HandleCrashGhosts(); 
        m_IsInitialized = true;
    }

    // =========================================================================
    // UTILITÁRIOS BÁSICOS
    // =========================================================================

    void Log(string msg)
    {
        string timestamp = GetDate(); 
        string logLine = string.Format("[%1] %2", timestamp, msg);
        Print("[ContainerEvent] " + msg);
        
        FileHandle file = OpenFile(LOG_PATH, FileMode.APPEND);
        
        if (file != 0) 
        {
            FPrintln(file, logLine);
            CloseFile(file);
        }
    }
    
    string GetDate()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        return string.Format("%1-%2-%3 %4:%5:%6", year.ToStringLen(4), month.ToStringLen(2), day.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2));
    }

    // =========================================================================
    // SEGURANÇA E IDs (SetIDSafe)
    // =========================================================================

    void SetIDSafe(Object obj, int id)
    {
        if (!obj) 
        {
            return;
        }

        ShippingContainerKeys_ColorBase key = ShippingContainerKeys_ColorBase.Cast(obj);
        
        if (key) 
        { 
            key.SetSDNEventID(id); 
            return; 
        }

        Land_ContainerLocked_Blue_DE blue = Land_ContainerLocked_Blue_DE.Cast(obj);
        
        if (blue) 
        { 
            blue.SetSDNEventID(id); 
            return; 
        }

        Land_ContainerLocked_Yellow_DE yellow = Land_ContainerLocked_Yellow_DE.Cast(obj);
        
        if (yellow) 
        { 
            yellow.SetSDNEventID(id); 
            return; 
        }

        Land_ContainerLocked_Orange_DE orange = Land_ContainerLocked_Orange_DE.Cast(obj);
        
        if (orange) 
        { 
            orange.SetSDNEventID(id); 
            return; 
        }

        Land_ContainerLocked_Red_DE red = Land_ContainerLocked_Red_DE.Cast(obj);
        
        if (red) 
        { 
            red.SetSDNEventID(id); 
            return; 
        }
    }

    // =========================================================================
    // WEBHOOK DO DISCORD
    // =========================================================================
    
    void SendDiscordWebhook(string title, string message)
    {
        if (!m_Config)
        {
            return;
        }
        
        if (m_Config.DiscordWebhookURL == "")
        {
            return;
        }

        RestApi restApi = GetRestApi();
        
        if (!restApi)
        {
            restApi = CreateRestApi();
        }

        if (restApi)
        {
            RestContext ctx = restApi.GetRestContext(m_Config.DiscordWebhookURL);
            
            if (ctx)
            {
                message.Replace("\"", "'");
                message.Replace("\n", " ");
                message.Replace("\r", "");
                title.Replace("\"", "'");
                title.Replace("\n", " ");
                title.Replace("\r", "");
                
                string payload = "{\"username\": \"SDN Container Event\", \"content\": \"**" + title + "**\\n" + message + "\"}";
                
                ctx.SetHeader("application/json");
                
                ctx.POST(null, "", payload);
            }
        }
    }

    // =========================================================================
    // CRIAÇÃO DE CONFIGURAÇÃO
    // =========================================================================

    void CreateDefaultConfig()
    {
        m_Config = new SDN_ContainerEventConfig();
        m_Config.EventActive = 1;
        m_Config.EnableSound = true;
        m_Config.ServerTimeOffset = 0;
        
        m_Config.DiscordWebhookURL = "";
        
        m_Config.MinPlayers = 1;
        m_Config.SpawnStartDelay = 5; 
        m_Config.MaxSimultaneousEvents = 2;
        m_Config.DelayBetweenSpawns = 25; 
        m_Config.RespawnTime = 60;        
        m_Config.ActivationRadius = 50.0;
        m_Config.TimeToActivate = 30; 
        m_Config.TimeAfterActivation = 20; 
        m_Config.ZombieCount = 20;
        m_Config.MinLoot = 10;
        m_Config.MaxLoot = 12;
        m_Config.MaxHistoryLocations = 8;
        
        m_Config.ChanceBlue = 45; 
        m_Config.ChanceYellow = 35; 
        m_Config.ChanceOrange = 15; 
        m_Config.ChanceRed = 5;    
        
        m_Config.LootContainerObject = "StorageBox_Wooden";
        
        m_Config.TitleSpawn = "NEW EVENT";
        m_Config.TitleActive = "EVENT ACTIVATED";
        m_Config.TitleComplete = "SUCCESS";
        m_Config.TitleFail = "FAILURE";
        
        m_Config.MsgSpawn = "ATTENTION: A container has been left in the %1 area. The key is with the local patrol!";
        m_Config.MsgActive = "ALERT: Movement detected in the container area!";
        m_Config.MsgComplete = "SUCCESS: A brave survivor looted the container in!";
        m_Config.MsgFail = "FAILURE: No one dared to go to %1. The container was destroyed.";
        
        m_Config.RestartTimes = new array<string>;
        m_Config.RestartTimes.Insert("00:00"); 
        m_Config.RestartTimes.Insert("04:00");
        m_Config.RestartTimes.Insert("08:00"); 
        m_Config.RestartTimes.Insert("12:00");
        m_Config.RestartTimes.Insert("16:00"); 
        m_Config.RestartTimes.Insert("20:00");
        
        m_Config.RestartSafetyMinutes = 15;
        m_Config.GhostCleanupTime = 10; 

        SDN_EventLocation loc;
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Altar Radio"; 
        loc.Coordinates = "8141 0 9153"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Kabanino Fields"; 
        loc.Coordinates = "5131 0 8616"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Gorka East"; 
        loc.Coordinates = "10083 0 8867"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Stary Sobor Road"; 
        loc.Coordinates = "6697 0 7538"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Mogilevka Road"; 
        loc.Coordinates = "7653 0 4915"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Green Mtn. Road"; 
        loc.Coordinates = "3728 0 6013"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Vyshnoye"; 
        loc.Coordinates = "6411 0 6199"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Kamenca Road"; 
        loc.Coordinates = "2203 0 2248"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Balota AirStrip"; 
        loc.Coordinates = "5074 0 2394"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Dolina"; 
        loc.Coordinates = "11232 0 6551"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Shakhovka Road"; 
        loc.Coordinates = "9617 0 6583"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Staroy Gas Station"; 
        loc.Coordinates = "10148 0 5216"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Kamensk Mine Road"; 
        loc.Coordinates = "7029 0 14482"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Severograd Quarry"; 
        loc.Coordinates = "7953 0 12693"; 
        m_Config.Locations.Insert(loc);
        
        loc = new SDN_EventLocation(); 
        loc.Name = "Drozhino Village"; 
        loc.Coordinates = "3297 0 5079"; 
        m_Config.Locations.Insert(loc);
    }

    void AddDefaultItem(SDN_LootTierConfig cfg, string name, int prob, array<string> atts)
    {
        SDN_EventLootItem item = new SDN_EventLootItem();
        item.ItemName = name; 
        item.Probability = prob; 
        item.Attachments = atts;
        cfg.Items.Insert(item);
    }

    void CreateDefaultLootTiers()
    {
        int i; 
        string path; 
        SDN_LootTierConfig tierCfg;
        
        for (i = 1; i <= 4; i++) 
        {
            path = CONFIG_ROOT + "/LootTier" + i + ".json";
            
            if (!FileExist(path)) 
            {
                tierCfg = new SDN_LootTierConfig();
                
                switch(i) 
                {
                    case 1: 
                    {
                        AddDefaultItem(tierCfg, "SodaCan_Pepsi", 100, {}); 
                        AddDefaultItem(tierCfg, "TunaCan", 100, {});
                        AddDefaultItem(tierCfg, "BandageDressing", 100, {}); 
                        AddDefaultItem(tierCfg, "DuctTape", 80, {});
                        AddDefaultItem(tierCfg, "Battery9V", 80, {}); 
                        AddDefaultItem(tierCfg, "Headtorch_Grey", 70, {"Battery9V"});
                        AddDefaultItem(tierCfg, "HuntingKnife", 60, {}); 
                        AddDefaultItem(tierCfg, "Machete", 60, {});
                        AddDefaultItem(tierCfg, "MountainBag_Green", 50, {}); 
                        AddDefaultItem(tierCfg, "Raincoat_Green", 60, {});
                        AddDefaultItem(tierCfg, "CargoPants_Green", 60, {}); 
                        AddDefaultItem(tierCfg, "HikingBoots_Brown", 60, {});
                        AddDefaultItem(tierCfg, "IJ70", 50, {"Mag_IJ70_8Rnd"}); 
                        AddDefaultItem(tierCfg, "BK18", 50, {});
                        AddDefaultItem(tierCfg, "AmmoBox_357_20Rnd", 50, {}); 
                        AddDefaultItem(tierCfg, "AmmoBox_762x39_20Rnd", 50, {});
                        AddDefaultItem(tierCfg, "NailBox", 40, {}); 
                        AddDefaultItem(tierCfg, "Hammer", 40, {});
                        AddDefaultItem(tierCfg, "Handsaw", 40, {}); 
                        AddDefaultItem(tierCfg, "Shovel", 30, {}); 
                        break;
                    }
                    case 2: 
                    {
                        AddDefaultItem(tierCfg, "BakedBeansCan", 100, {}); 
                        AddDefaultItem(tierCfg, "Canteen", 100, {});
                        AddDefaultItem(tierCfg, "Morphine", 80, {}); 
                        AddDefaultItem(tierCfg, "Epinephrine", 80, {});
                        AddDefaultItem(tierCfg, "SalineBagIV", 70, {}); 
                        AddDefaultItem(tierCfg, "PoliceVest", 60, {});
                        AddDefaultItem(tierCfg, "PoliceJacket", 60, {}); 
                        AddDefaultItem(tierCfg, "PolicePants", 60, {});
                        AddDefaultItem(tierCfg, "CombatBoots_Black", 60, {}); 
                        AddDefaultItem(tierCfg, "Mich2001Helmet", 50, {});
                        AddDefaultItem(tierCfg, "MP5K", 50, {"Mag_MP5_30Rnd", "MP5_PlasticHndgrd", "MP5_RailHndgrd"});
                        AddDefaultItem(tierCfg, "Glock19", 50, {"Mag_Glock_15Rnd"}); 
                        AddDefaultItem(tierCfg, "Mosin9130", 40, {});
                        AddDefaultItem(tierCfg, "PUScope", 40, {}); 
                        AddDefaultItem(tierCfg, "AmmoBox_9x19_25Rnd", 60, {});
                        AddDefaultItem(tierCfg, "DZ\weapons\ammunition\762_20roundbox.p3d", 50, {}); 
                        AddDefaultItem(tierCfg, "Handcuffs", 40, {});
                        AddDefaultItem(tierCfg, "HandcuffKeys", 40, {}); 
                        AddDefaultItem(tierCfg, "CombatKnife", 50, {});
                        AddDefaultItem(tierCfg, "AliceBag_Black", 40, {}); 
                        break;
                    }
                    case 3: 
                    {
                        AddDefaultItem(tierCfg, "TacticalBacon", 100, {}); 
                        AddDefaultItem(tierCfg, "FilteredWaterBottle", 90, {});
                        AddDefaultItem(tierCfg, "PlateCarrierVest", 50, {"PlateCarrierHolster", "PlateCarrierPouches"});
                        AddDefaultItem(tierCfg, "HighCapacityVest_Olive", 60, {}); 
                        AddDefaultItem(tierCfg, "M65Jacket_Olive", 60, {});
                        AddDefaultItem(tierCfg, "CargoPants_Beige", 60, {}); 
                        AddDefaultItem(tierCfg, "JungleBoots_Olive", 60, {});
                        AddDefaultItem(tierCfg, "BallisticHelmet_Green", 50, {}); 
                        AddDefaultItem(tierCfg, "AKM", 40, {"AK_WoodBttstck", "AK_WoodHndgrd", "Mag_AKM_30Rnd"});
                        AddDefaultItem(tierCfg, "M4A1", 30, {"M4_OEBttstck", "M4_PlasticHndgrd", "M4_CarryHandleOptic", "Mag_STANAG_30Rnd|Mag_STANAG_60Rnd"});
                        AddDefaultItem(tierCfg, "AK101", 30, {"AK_PlasticBttstck", "AK_PlasticHndgrd", "Mag_AK101_30Rnd"});
                        AddDefaultItem(tierCfg, "PSO1Optic", 40, {}); 
                        AddDefaultItem(tierCfg, "KobraOptic", 40, {});
                        AddDefaultItem(tierCfg, "DZ\weapons\ammunition\556_looserounds.p3d", 60, {}); 
                        AddDefaultItem(tierCfg, "AmmoBox_762x39_20Rnd", 60, {});
                        AddDefaultItem(tierCfg, "Grenade_RGD5", 30, {}); 
                        AddDefaultItem(tierCfg, "LandMineTrap", 30, {});
                        AddDefaultItem(tierCfg, "Rangefinder", 40, {"Battery9V"}); 
                        AddDefaultItem(tierCfg, "TortillaBag", 40, {});
                        AddDefaultItem(tierCfg, "NVGHeadstrap", 50, {}); 
                        break;
                    }
                    case 4: 
                    {
                        AddDefaultItem(tierCfg, "Bear_SteakMeat", 100, {}); 
                        AddDefaultItem(tierCfg, "Canteen", 100, {});
                        AddDefaultItem(tierCfg, "PlateCarrierVest", 70, {"PlateCarrierHolster", "PlateCarrierPouches"});
                        AddDefaultItem(tierCfg, "GorkaEJacket_Summer", 60, {}); 
                        AddDefaultItem(tierCfg, "GorkaPants_Summer", 60, {});
                        AddDefaultItem(tierCfg, "CombatBoots_Grey", 60, {}); 
                        AddDefaultItem(tierCfg, "Mich2001Helmet", 40, {"NVGHeadstrap", "NVGoggles"});
                        AddDefaultItem(tierCfg, "FAL", 30, {"Fal_OeBttstck", "Mag_FAL_20Rnd"});
                        AddDefaultItem(tierCfg, "DZ\\weapons\\firearms\\svd\\svd.p3d", 30, {"Mag_SVD_10Rnd", "PSO1Optic", "AK_Suppressor"});
                        AddDefaultItem(tierCfg, "M70_Tundra", 30, {"HuntingOptic"});
                        AddDefaultItem(tierCfg, "VSS", 30, {"Mag_VSS_10Rnd", "PSO11Optic"});
                        AddDefaultItem(tierCfg, "AmmoBox_308Win_20Rnd", 60, {}); 
                        AddDefaultItem(tierCfg, "DZ\weapons\ammunition\762_20roundbox.p3d", 60, {});
                        AddDefaultItem(tierCfg, "Grenade_M67", 40, {}); 
                        AddDefaultItem(tierCfg, "Plastic_Explosive", 20, {});
                        AddDefaultItem(tierCfg, "RemoteDetonator", 20, {}); 
                        AddDefaultItem(tierCfg, "NVGoggles", 40, {"Battery9V"});
                        AddDefaultItem(tierCfg, "CoyoteBag_Brown", 40, {}); 
                        AddDefaultItem(tierCfg, "GhillieSuit_Woodland", 20, {});
                        AddDefaultItem(tierCfg, "LargeTent", 10, {}); 
                        break;
                    }
                }
                
                JsonFileLoader<SDN_LootTierConfig>.JsonSaveFile(path, tierCfg);
            }
        }
    }

    // =========================================================================
    // LÓGICA DE SPAWN (Zumbis, Loot, Container)
    // =========================================================================

    void SpawnZombies(ActiveEventInstance evt, string keyName, int eventID)
    {
        if (!m_Config) 
        {
            return;
        }
        
        int i; 
        vector spawnPos; 
        float angle, dist; 
        EntityAI zmb;
        int keyHolderIndex = Math.RandomInt(0, 4); 
        string bossBerets[] = {"MilitaryBeret_UN", "MilitaryBeret_Red", "MilitaryBeret_ChdkZ", "MilitaryBeret_CDF"};
        
        for (i = 0; i < m_Config.ZombieCount; i++)
        {
            spawnPos = evt.Position;
            angle = Math.RandomFloat(0, Math.PI * 2);
            dist = Math.RandomFloat(5, 25); 
            
            if (i >= 4) 
            {
                dist = Math.RandomFloat(15, 35); 
            }
            
            spawnPos[0] = spawnPos[0] + Math.Cos(angle) * dist;
            spawnPos[2] = spawnPos[2] + Math.Sin(angle) * dist;
            spawnPos[1] = GetGame().SurfaceY(spawnPos[0], spawnPos[2]);
            
            zmb = EntityAI.Cast(GetGame().CreateObject("ZmbM_PatrolNormal_PautRev", spawnPos, false, true));
            
            if (zmb) 
            {
                zmb.SetLifetime(3600);

                evt.Zombies.Insert(zmb);
                
                if (i < 4) 
                {
                    zmb.GetInventory().CreateAttachment(bossBerets[i]);
                    
                    if (i == keyHolderIndex) 
                    {
                        EntityAI keyObj = zmb.GetInventory().CreateInInventory(keyName);
                        
                        if (keyObj)
                        {
                            SetIDSafe(keyObj, eventID);
                        }
                    }
                }
            }
        }
    }

    void FillLootCrate(EntityAI crate, int tier)
    {
        int lootCount = 0; 
        int maxAttempts = m_Config.MaxLoot * 10; 
        int attempts = 0;
        
        SDN_EventLootItem lootConfig; 
        EntityAI item, attachment; 
        Magazine mag; 
        ItemBase itemBase;
        TStringArray options; 
        SDN_LootTierConfig tierLoot = new SDN_LootTierConfig();
        
        string path = CONFIG_ROOT + "/LootTier" + tier + ".json";
        
        if (FileExist(path)) 
        {
            JsonFileLoader<SDN_LootTierConfig>.JsonLoadFile(path, tierLoot); 
        }
        else 
        {
            return;
        }
        
        if (!crate || !tierLoot.Items) 
        {
            return;
        }
        
        while (lootCount < m_Config.MinLoot && attempts < maxAttempts) 
        {
            attempts++; 
            
            if (lootCount >= m_Config.MaxLoot) 
            {
                break;
            }
            
            lootConfig = tierLoot.Items.GetRandomElement();
            
            if (Math.RandomFloat01() <= (lootConfig.Probability / 100.0)) 
            {
                item = crate.GetInventory().CreateEntityInCargo(lootConfig.ItemName);
                
                if (!item) 
                {
                    item = crate.GetInventory().CreateInInventory(lootConfig.ItemName);
                }
                
                if (item) 
                {
                    if (lootConfig.Attachments) 
                    {
                        foreach (string rawAttName : lootConfig.Attachments) 
                        {
                            string selectedAtt = rawAttName;
                            
                            if (rawAttName.Contains("|")) 
                            {
                                options = new TStringArray; 
                                rawAttName.Split("|", options);
                                selectedAtt = options.GetRandomElement();
                            }
                            
                            attachment = item.GetInventory().CreateAttachment(selectedAtt);
                            
                            if (!attachment) 
                            {
                                attachment = item.GetInventory().CreateInInventory(selectedAtt);
                            }
                            
                            if (attachment) 
                            { 
                                mag = Magazine.Cast(attachment); 
                                
                                if (mag) 
                                {
                                    mag.ServerSetAmmoCount(mag.GetAmmoMax()); 
                                }
                            }
                        }
                    }
                    
                    itemBase = ItemBase.Cast(item);
                    
                    if (itemBase && itemBase.CanBeSplit()) 
                    { 
                        itemBase.SetQuantity(Math.RandomFloat(itemBase.GetQuantityMax() * 0.5, itemBase.GetQuantityMax())); 
                    }
                    
                    lootCount++;
                }
            }
        }
    }

    void CleanGroundItems(vector pos, float radius)
    {
        array<Object> objects = new array<Object>;
        array<CargoBase> proxy = new array<CargoBase>;
        GetGame().GetObjectsAtPosition(pos, radius, objects, proxy);
        
        int deletedCount = 0;

        foreach (Object obj : objects)
        {
            EntityAI ent = EntityAI.Cast(obj);
            
            if (!ent) continue;
            if (ent.IsInherited(Man)) continue;             
            if (ent.IsInherited(CarScript)) continue;       
            if (ent.IsInherited(TentBase)) continue;         
            if (ent.IsInherited(BaseBuildingBase)) continue; 

            string type = ent.GetType();
            
            if (type.Contains("M18SmokeGrenade") || ent.IsInherited(DayZInfected)) 
            {
                GetGame().ObjectDelete(ent);
                deletedCount++;
            }
        }
    }

    void HandleCrashGhosts()
    {
        if (m_GhostLocations.Count() > 0)
        {
            Log("Checking " + m_GhostLocations.Count() + " completed events pre-restart...");
            array<Object> objects = new array<Object>;
            array<CargoBase> cargos = new array<CargoBase>;
            string lootName = m_Config.LootContainerObject;
            
            if (lootName == "") 
            {
                lootName = "StorageBox_Wooden";
            }
            
            int cleaned = 0;

            foreach (vector pos : m_GhostLocations)
            {
                CleanGroundItems(pos, 10.0);

                objects.Clear(); 
                cargos.Clear();
                
                GetGame().GetObjectsAtPosition(pos, 10.0, objects, cargos);
                
                foreach (Object obj : objects)
                {
                    EntityAI ent = EntityAI.Cast(obj);
                    
                    if (ent)
                    {
                        string type = ent.GetType();
                        
                        if (type.Contains("Land_ContainerLocked") || type.Contains("SDN_Test_ContainerBase") || type == lootName)
                        {
                            float cleanupTime = m_Config.GhostCleanupTime * 60;
                            
                            if (cleanupTime <= 0) 
                            {
                                cleanupTime = 300; 
                            }
                            
                            ent.SetLifetime(cleanupTime);
                            
                            if (type == lootName) 
                            {
                                ItemBase chestItem = ItemBase.Cast(ent);
                                
                                if (chestItem) 
                                { 
                                    chestItem.SetTakeable(false); 
                                }
                            }
                            
                            cleaned++;
                        }
                    }
                }
            }
            
            if (cleaned > 0) 
            {
                Log("Adjusted and Locked " + cleaned + " ghost objects.");
            }
            
            m_GhostLocations.Clear();
            SaveState();
        }
    }

    bool IsRestartComing()
    {
        if (!m_Config.RestartTimes || m_Config.RestartTimes.Count() == 0) 
        {
            return false;
        }
        
        int hour, minute, second;
        GetHourMinuteSecond(hour, minute, second);
        
        if (m_Config.ServerTimeOffset != 0)
        {
            hour = hour + m_Config.ServerTimeOffset;
            
            if (hour >= 24) hour = hour - 24;
            else if (hour < 0) hour = hour + 24;
        }

        int currentTotalMinutes = (hour * 60) + minute;
        int safetyMargin = m_Config.RestartSafetyMinutes;
        
        foreach (string timeStr : m_Config.RestartTimes)
        {
            TStringArray parts = new TStringArray;
            timeStr.Split(":", parts);
            
            if (parts.Count() == 2)
            {
                int rHour = parts.Get(0).ToInt();
                int rMinute = parts.Get(1).ToInt();
                int restartTotalMinutes = (rHour * 60) + rMinute;
                int diff = restartTotalMinutes - currentTotalMinutes;
                
                if (diff > 0 && diff <= safetyMargin) 
                {
                    Log("Spawn blocked! Restart scheduled for " + timeStr + " (" + diff + " min remaining).");
                    return true;
                }
            }
        }
        
        return false;
    }

    void UpdateEventSmoke(ActiveEventInstance evt, float timeslice)
    {
        evt.SmokeTimer += timeslice;
        string targetColor = "M18SmokeGrenade_Green"; 
        
        if (evt.IsCompleted) 
        {
            targetColor = "M18SmokeGrenade_Red";
        }
        else if (evt.IsActive) 
        {
            targetColor = "M18SmokeGrenade_Yellow";
        }
        
        bool timeToRefresh = evt.SmokeTimer >= 40.0;
        bool colorChanged = evt.CurrentSmokeColor != targetColor;
        bool smokeMissing = !evt.VisualEffect;
        
        if (timeToRefresh || colorChanged || smokeMissing)
        {
            if (evt.VisualEffect) 
            {
                GetGame().ObjectDelete(evt.VisualEffect);
            }
            
            vector smokePos = evt.Position;
            if (evt.Container)
            {
                smokePos = evt.Container.GetPosition();
                // O origin do Container fica no centro dele.
                // Como ele tem cerca de 2.6m de altura total, adicionar 1.3m coloca a fumaça exatamente no teto.
                smokePos[1] = smokePos[1] + 1.3; 
            }
            else 
            {
                smokePos[1] = smokePos[1] + 2.5; 
            }
            
            EntityAI smokeObj = EntityAI.Cast(GetGame().CreateObject(targetColor, smokePos, false, true));
            
            if (smokeObj) 
            {
                ItemBase smokeItem = ItemBase.Cast(smokeObj);
                
                if (smokeItem) 
                {
                    smokeItem.SetTakeable(false); 
                    smokeItem.SetLifetime(3600);
                    
                    if (smokeItem.GetCompEM()) 
                    {
                        smokeItem.GetCompEM().SwitchOn(); 
                    }
                    
                    evt.VisualEffect = smokeItem;
                    evt.CurrentSmokeColor = targetColor;
                    evt.SmokeTimer = 0; 
                }
            }
        }
    }

    void InitSceneTemplate()
    {
        m_SceneTemplate = new array<ref SDN_SceneObjectTemplate>;
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_Furniture_PaperBox_01_small_closed", "4449.7773 341.0856 10252.7685", "-30.1870 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_Furniture_PaperBox_01_small_closed", "4449.7812 340.6738 10252.7714", "-30.1870 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.1928 341.8095 10248.8222", "58.5493 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_Wreck_Trailer_Single_Container", "4452.5424 338.9119 10249.4560", "-30.4984 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.1069 340.7459 10248.9365", "60.7354 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.1420 341.1004 10248.9238", "58.5493 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("Land_Wreck_LR_Armed_TK", "4452.2304 338.9104 10255.7294", "-50.0080 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_Misc_Gunrack", "4451.2788 340.6111 10252.8300", "-30.4984 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_Furniture_PaperBox_01_small_closed", "4449.7792 340.2620 10252.7753", "-30.1870 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.3168 342.3014 10248.8955", "58.5514 -60.0000 1.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("Land_Wreck_Truck02_Green", "4454.0161 338.9647 10245.8378", "-102.9063 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\vehicles\\wheeled\\offroad_02\\proxy\\offroad_02_wheel_destroyed.p3d", "4452.951660 339.241821 10253.967773", "-0.715726 -0.000000 -0.000000", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_smoke_medium_camp_01", "4453.3989 339.7124 10253.7236", "-142.0000 -90.0000 -180.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\vehicles\\wheeled\\truck_01\\proxy\\truck_01_wheel_ruined.p3d", "4449.305176 339.445038 10253.696289", "0.544809 -0.000908 -1.364146", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("Land_Wreck_Uaz", "4457.0146 339.7254 10247.8974", "3.0000 11.0000 -177.9299"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.1381 341.4550 10248.9189", "58.5493 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("StaticObj_ammoboxes_single", "4452.8701 340.7622 10249.3222", "59.1701 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("Land_Wreck_Pickup01_Armed_TK", "4456.8764 339.9931 10241.3017", "-156.3408 -1.7598 -106.3918"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\vehicles\\wheeled\\offroad_02\\proxy\\offroad_02_wheel_destroyed.p3d", "4452.951660 339.241821 10253.967773", "-0.715726 -0.000000 -0.000000", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_fire_tirepile", "4454.8652 340.6574 10242.7226", "2.0000 -2.0000 1.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("FireplaceLight", "4454.9340 340.6475 10242.7500", "62.8300 55.0000 -102.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\vehicles\\wheeled\\truck_01\\proxy\\truck_01_wheeldouble_ruined.p3d", "4448.880371 339.300720 10253.483398", "2.024747 0.000583 -0.001196", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_fire_medium_barrel_01", "4457.3730 339.7337 10240.7353", "-94.7636 0.0000 0.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_fire_medium_oven_01", "4453.1352 339.3692 10253.6337", "2.0000 -3.0000 3.0000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_smoke_medium_house_01", "4457.0737 340.1774 10247.0771", "62.8661 -1.2000 1.7000"));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("SDN_P_smoke_small_house_01", "4457.3847 339.7087 10241.0009", "34.3593 -2.0000 9.0000"));

        // ---------------------------------------------------------------------
        // ITENS .p3d FIXOS (export do Editor: ypr em RADIANOS -> flag true converte p/ graus)
        // Armas: ypr = (yaw, 0, 90 graus) = posição vertical como montada no Editor
        // ---------------------------------------------------------------------
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\9mm_looserounds.p3d", "4450.153809 340.286407 10252.887695", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\9mm_25rnd_box.p3d", "4450.653320 340.337097 10252.257812", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\556_looserounds.p3d", "4451.423828 340.281860 10251.529297", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\308win_looserounds.p3d", "4451.099121 340.294281 10252.814453", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762_20roundbox.p3d", "4450.880371 340.294556 10252.345703", "3.033371687 1.570796371 -1.391064286", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\40mm_grenade.p3d", "4451.283203 340.327301 10251.497070", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\556_20roundbox.p3d", "4450.885742 340.275787 10252.133789", "2.696503401 1.570796371 -1.867559314", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\308win_looserounds.p3d", "4449.823730 341.501495 10252.803711", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\9x39_looserounds.p3d", "4450.059570 340.305084 10251.979492", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\308win_looserounds.p3d", "4450.416016 340.294281 10252.798828", "-0.532298744 -0.000000030 -0.000000015", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\9x39_looserounds.p3d", "4450.617676 340.281799 10252.150391", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\9mm_25rnd_box.p3d", "4450.663086 340.332428 10251.589844", "-0.218139619 0.680678546 0.087266453", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\556_20roundbox.p3d", "4450.817871 340.264008 10252.435547", "2.726381779 1.570450902 -1.610787034", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762_looserounds.p3d", "4449.763672 341.500214 10252.652344", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\12ga_00buck.p3d", "4449.635254 341.503937 10252.794922", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762_20roundbox.p3d", "4449.923828 341.538513 10252.898438", "-0.338313967 1.570796371 1.520435452", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\12ga_00buck.p3d", "4449.818848 341.507294 10252.910156", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.321777 341.803497 10248.888672", "1.021916270 -1.047197461 0.017453935", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\m14\\m14.p3d", "4451.632324 340.710236 10252.684570", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd.p3d", "4451.334473 340.648743 10252.622070", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd.p3d", "4451.104004 340.648743 10252.496094", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd.p3d", "4451.287109 340.648743 10252.715820", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.149902 340.650818 10252.973633", "-0.572996855 -0.000000060 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.386719 340.650818 10253.119141", "-0.572996557 -0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.106934 340.650818 10253.067383", "-0.572996557 -0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.343750 340.650818 10253.212891", "-0.572997153 -0.000000060 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.059570 340.650818 10253.164062", "-0.572996438 -0.000000000 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762x39_looserounds.p3d", "4451.426270 339.839233 10251.718750", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\556_looserounds.p3d", "4451.241211 340.265472 10251.875000", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\545x39_20roundbox.p3d", "4449.943848 341.509674 10252.942383", "2.733694792 1.570796371 -3.141546726", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\545x39_20roundbox.p3d", "4449.870117 341.504364 10252.667969", "2.778995275 1.570796371 -3.096246243", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\545x39_20roundbox.p3d", "4450.087402 340.322327 10252.107422", "2.588175297 1.570796371 2.996118307", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.146973 340.602539 10248.916992", "1.021878719 0.000000006 0.000000059", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.111816 340.248016 10248.929688", "1.060032964 -0.000000019 0.000000059", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\sv98\\sv98.p3d", "4451.296387 340.650818 10253.310547", "-0.572996557 -0.000000060 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.197754 341.311584 10248.815430", "1.021878362 -0.000000009 0.000000092", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762x39_20roundbox.p3d", "4450.834473 341.127472 10251.007812", "-1.513275385 0.000000030 -0.000000045", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762x39_20roundbox.p3d", "4450.909668 341.127472 10251.225586", "-1.513275385 0.000000030 -0.000000045", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762x39_20roundbox.p3d", "4450.717285 340.319031 10252.209961", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762_20roundbox.p3d", "4450.888672 341.080750 10251.375000", "1.743286729 1.570450902 -1.455827236", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\556_looserounds.p3d", "4451.394531 340.281860 10251.577148", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\00buck_10roundbox.p3d", "4449.572266 341.550323 10252.705078", "-0.532298684 -0.000000060 -0.000000017", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.875000 340.264282 10249.315430", "1.032713652 -0.000000016 0.000000092", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4452.143066 340.957062 10248.912109", "1.021878362 -0.000000009 0.000000092", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\structures_bliss\\underground\\storage\\proxy\\ammoboxes_single.p3d", "4451.287109 341.488037 10250.307617", "-2.085239649 -0.000000000 0.959931135", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd_old.p3d", "4451.205566 340.634064 10252.327148", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd_old.p3d", "4451.389160 340.634064 10252.546875", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\m14\\m14.p3d", "4451.525391 340.710236 10252.858398", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\svd\\svd.p3d", "4451.151367 340.648743 10252.402344", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\firearms\\m14\\m14.p3d", "4451.567871 340.710236 10252.763672", "-0.532298684 0.000000030 1.570796371", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\00buck_10roundbox.p3d", "4450.764160 341.126007 10250.959961", "-1.513275385 0.000000030 -0.000000045", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\762_looserounds.p3d", "4450.835938 341.076569 10251.228516", "-1.513275385 0.000000030 -0.000000045", true));
        m_SceneTemplate.Insert(new SDN_SceneObjectTemplate("DZ\\weapons\\ammunition\\00buck_10roundbox.p3d", "4449.643555 341.550934 10252.661133", "0.707818568 -0.000000015 0.000000047", true));
    }

    void LoadConfig()
    {
        if (FileExist(CONFIG_PATH)) 
        {
            JsonFileLoader<SDN_ContainerEventConfig>.JsonLoadFile(CONFIG_PATH, m_Config);
            
            if (m_Config && m_Config.LootContainerObject == "CONTAINER_Chest")
            {
                m_Config.LootContainerObject = "StorageBox_Wooden";
                JsonFileLoader<SDN_ContainerEventConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
                Log("Updated legacy config: Loot box changed to StorageBox_Wooden.");
            }
            Log("Configuration loaded.");
        } 
        else 
        {

            CreateDefaultConfig();
            JsonFileLoader<SDN_ContainerEventConfig>.JsonSaveFile(CONFIG_PATH, m_Config);
            Log("New default configuration created.");
        }
        
        CreateDefaultLootTiers();
        
        if (m_Config) 
        {
            if (m_CurrentTargetTime == 0) 
            {
                m_CurrentTargetTime = m_Config.SpawnStartDelay * 60;
            }
        }
    }

    void SaveState()
    {
        if (!m_IsInitialized) 
        {
            return;
        }

        SDN_ContainerEventGlobalState state = new SDN_ContainerEventGlobalState();
        state.SpawnTimer = m_SpawnTimer;
        state.CurrentTargetTime = m_CurrentTargetTime;
        state.EventsSpawnedInWave = m_EventsSpawnedInWave;
        state.LastRoundLocations = m_HistoryLocations;
        state.CurrentWaveLocations = m_CurrentWaveLocations; 
        
        state.CompletedGhostLocations = m_GhostLocations;
        
        foreach (ActiveEventInstance evt : m_ActiveEvents)
        {
            if (evt.IsCompleted) 
            {
                continue;
            }
            
            SDN_SavedEventData data = new SDN_SavedEventData();
            data.Position = evt.Position;
            data.LocationName = evt.LocationName;
            data.ContainerType = evt.ContainerType;
            data.KeyType = evt.KeyType;
            data.Tier = evt.Tier;
            data.LifeTime = evt.LifeTime;
            data.InitialLootCount = evt.InitialLootCount;
            state.ActiveEvents.Insert(data);
        }
        
        JsonFileLoader<SDN_ContainerEventGlobalState>.JsonSaveFile(STATE_PATH, state);
    }
    
    void LoadState()
    {
        if (!FileExist(STATE_PATH)) 
        {
            return;
        }
        
        SDN_ContainerEventGlobalState state = new SDN_ContainerEventGlobalState();
        JsonFileLoader<SDN_ContainerEventGlobalState>.JsonLoadFile(STATE_PATH, state);
        
        m_SpawnTimer = state.SpawnTimer;
        m_CurrentTargetTime = state.CurrentTargetTime;
        m_EventsSpawnedInWave = state.EventsSpawnedInWave;
        
        if (state.LastRoundLocations) 
        {
            m_HistoryLocations = state.LastRoundLocations;
        }
        
        if (state.CurrentWaveLocations) 
        {
            m_CurrentWaveLocations = state.CurrentWaveLocations;
        }
        
        if (state.CompletedGhostLocations) 
        {
            m_GhostLocations = state.CompletedGhostLocations;
        }
        
        if (state.ActiveEvents)
        {
            Log("Trying to reconnect " + state.ActiveEvents.Count() + " persistent events...");
            
            foreach (SDN_SavedEventData data : state.ActiveEvents)
            {
                SpawnEventFromSave(data);
            }
        }
    }

    void SpawnEventFromSave(SDN_SavedEventData data)
    {
        SDN_EventLocation loc = new SDN_EventLocation();
        loc.Name = data.LocationName;
        loc.Coordinates = data.Position;
        SpawnEventInternal(loc, data.ContainerType, data.KeyType, data.Tier, 0, data.InitialLootCount, true);
    }

    // --- MÉTODOS DE LOOP PRINCIPAL ---

    void OnUpdate(float timeslice)
    {
        if (!m_Config || m_Config.EventActive == 0) 
        {
            return;
        }

        m_UpdateTimer += timeslice;
        
        if (!m_IsInitialized)
        {
            m_InitDelayTimer += timeslice;
            
            if (m_InitDelayTimer >= 25.0) 
            {
                RunDelayedInit();
            }
            return; 
        }

        if (m_UpdateTimer >= 1.0)
        {
            CheckActiveEvents(1.0); 
            m_SpawnTimer += 1.0;
            m_SaveTimer += 1.0;
            
            if (m_SaveTimer >= 60.0) 
            { 
                SaveState(); 
                m_SaveTimer = 0; 
            }
            
            if (m_SpawnTimer >= m_CurrentTargetTime)
            {
                if (IsRestartComing()) 
                {
                    m_SpawnTimer = m_CurrentTargetTime - 60;
                    m_UpdateTimer = 0;
                    return;
                }

                array<Man> players = new array<Man>;
                GetGame().GetPlayers(players);

                if (players.Count() >= m_Config.MinPlayers)
                {
                    if (TrySpawnEvent()) 
                    {
                        m_EventsSpawnedInWave++;
                        
                        if (m_EventsSpawnedInWave >= m_Config.MaxSimultaneousEvents) 
                        {
                            m_HistoryLocations.InsertAll(m_CurrentWaveLocations);
                            
                            int historyLimit = 20; 
                            
                            if (m_Config.MaxHistoryLocations > 0) 
                            {
                                historyLimit = m_Config.MaxHistoryLocations;
                            }
                            
                            while (m_HistoryLocations.Count() > historyLimit) 
                            { 
                                m_HistoryLocations.Remove(0); 
                            }
                            
                            m_CurrentWaveLocations.Clear(); 
                            m_CurrentTargetTime = m_Config.RespawnTime * 60;
                            m_EventsSpawnedInWave = 0; 
                            Log("Wave finished. History updated (Max " + historyLimit + "). Next in " + m_Config.RespawnTime + " min.");
                            SaveState(); 
                        } 
                        else 
                        {
                            m_CurrentTargetTime = m_Config.DelayBetweenSpawns * 60;
                            Log("Event spawned. Next in " + m_Config.DelayBetweenSpawns + " min.");
                        }
                        
                        m_SpawnTimer = 0;
                        SaveState();
                    } 
                    else 
                    { 
                        m_SpawnTimer = m_CurrentTargetTime - 60; 
                    }
                } 
                else 
                { 
                    m_SpawnTimer = m_CurrentTargetTime - 60; 
                }
            }
            
            m_UpdateTimer = 0;
        }
    }

    bool TrySpawnEvent()
    {
        array<Man> players = new array<Man>;
        SDN_EventLocation randomLoc;
        int maxRetries = 10;
        int attempt = 0;
        bool foundUnique = false;
        
        if (!m_Config) 
        {
            return false;
        }
        
        if (m_ActiveEvents.Count() >= m_Config.MaxSimultaneousEvents) 
        {
            return false;
        }
        
        if (!m_Config.Locations || m_Config.Locations.Count() == 0) 
        {
            return false;
        }

        while (attempt < maxRetries)
        {
            attempt++;
            randomLoc = m_Config.Locations.Get(Math.RandomInt(0, m_Config.Locations.Count()));
            
            if (randomLoc.Name == m_LastSpawnedLocName) 
            {
                continue;
            }
            
            if (m_CurrentWaveLocations.Find(randomLoc.Name) != -1) 
            {
                continue;
            }
            
            if (m_HistoryLocations.Find(randomLoc.Name) != -1) 
            {
                continue;
            }
            
            bool tooClose = false;
            
            foreach (ActiveEventInstance existing : m_ActiveEvents) 
            {
                if (vector.Distance(existing.Position, randomLoc.Coordinates) < 200) 
                {
                    tooClose = true; 
                    break;
                }
            }
            
            if (tooClose) 
            {
                continue;
            }
            
            foundUnique = true;
            break;
        }
        
        if (!foundUnique) 
        {
            return false;
        }
        
        SpawnEvent(randomLoc);
        return true; 
    }

    void SpawnEvent(SDN_EventLocation loc)
    {
        m_LastSpawnedLocName = loc.Name;
        m_CurrentWaveLocations.Insert(loc.Name);
        SaveState(); 
        
        string cType, kType;
        int tier;
        int roll = Math.RandomInt(0, 100);
        int currentChance = 0;
        
        currentChance += m_Config.ChanceBlue;
        
        if (roll < currentChance) 
        { 
            tier = 1; 
            cType = "Land_ContainerLocked_Blue_DE"; 
            kType = "ShippingContainerKeys_Blue"; 
        }
        else 
        {
            currentChance += m_Config.ChanceYellow;
            
            if (roll < currentChance) 
            { 
                tier = 2; 
                cType = "Land_ContainerLocked_Yellow_DE"; 
                kType = "ShippingContainerKeys_Yellow"; 
            }
            else 
            {
                currentChance += m_Config.ChanceOrange;
                
                if (roll < currentChance) 
                { 
                    tier = 3; 
                    cType = "Land_ContainerLocked_Orange_DE"; 
                    kType = "ShippingContainerKeys_Orange"; 
                }
                else 
                { 
                    tier = 4; 
                    cType = "Land_ContainerLocked_Red_DE"; 
                    kType = "ShippingContainerKeys_Red"; 
                }
            }
        }
        
        SpawnEventInternal(loc, cType, kType, tier, 0, 0, false);
        
        string msg = m_Config.MsgSpawn;
        
        if (msg.Contains("%1")) 
        {
            msg = string.Format(msg, loc.Name);
        }
        else 
        {
            msg = msg + " [" + loc.Name + "]";
        }

        SDN_SendNotificationToAll(m_Config.TitleSpawn, msg, ICON_PATH, 10.0);
    }
    
    void SpawnEventInternal(SDN_EventLocation loc, string cType, string kType, int tier, float lifeTime, int initialLoot, bool isRestoration)
    {
        ActiveEventInstance newEvent = new ActiveEventInstance();
        
        // A âncora do evento no mundo real (representa o SarmaCan no chão)
        vector anchorPos = loc.Coordinates;
        anchorPos[1] = GetGame().SurfaceY(anchorPos[0], anchorPos[2]);

        vector anchorOri = loc.Orientation;

        // Enforce requires explicit check for zeros
        if (anchorOri[0] == 0 && anchorOri[1] == 0 && anchorOri[2] == 0)
        {
            anchorOri[0] = Math.RandomFloat(0, 360);
        }

        // --- CÁLCULO DE ALINHAMENTO DO TERRENO (SLOPE ALIGNMENT) ---
        // Pega 4 pontos a 1 metro de distância do centro para criar 2 vetores cruzados
        float pX = anchorPos[0];
        float pZ = anchorPos[2];
        float d = 1.0;

        vector pN = Vector(pX, GetGame().SurfaceY(pX, pZ + d), pZ + d);
        vector pS = Vector(pX, GetGame().SurfaceY(pX, pZ - d), pZ - d);
        vector pE = Vector(pX + d, GetGame().SurfaceY(pX + d, pZ), pZ);
        vector pW = Vector(pX - d, GetGame().SurfaceY(pX - d, pZ), pZ);

        vector vNS = pN - pS;
        vector vEW = pE - pW;

        // Vetor Normal da superfície: (Cross Product nativo matematicamente, uma linha)
        vector surfaceNormal = Vector((vNS[1]*vEW[2]) - (vNS[2]*vEW[1]), (vNS[2]*vEW[0]) - (vNS[0]*vEW[2]), (vNS[0]*vEW[1]) - (vNS[1]*vEW[0]));
        surfaceNormal.Normalize();

        // Direção de Yaw que desejamos (Vetor Forward Z-axis extraído diretamente dos ângulos)
        vector dirAngles = Vector(anchorOri[0], 0, 0);
        vector desiredDir = dirAngles.AnglesToVector();

        // Calcula nova matriz baseada na direção e na inclinação do chão
        vector finalAnchorMat[4];
        Math3D.DirectionAndUpMatrix(desiredDir, surfaceNormal, finalAnchorMat);
        anchorOri = Math3D.MatrixToAngles(finalAnchorMat);
        // -------------------------------------------------------------
        
        newEvent.LocationName = loc.Name;
        newEvent.Position = anchorPos;
        newEvent.ContainerType = cType;
        newEvent.KeyType = kType;
        newEvent.Tier = tier;
        newEvent.LifeTime = lifeTime;

        int eventID = Math.RandomInt(100000, 999999);

        // --- MATRIZ DA ÂNCORA (SarmaCan no Editor) ---
        vector editorOriginPos = "4451.6596 339.0072 10250.7070";
        vector editorOriginOri = "0 0 0";
        
        vector editorOriginMat[4];
        Math3D.YawPitchRollMatrix(editorOriginOri, editorOriginMat);
        editorOriginMat[3] = editorOriginPos;

        vector invEditorOriginMat[4];
        invEditorOriginMat[0][0] = editorOriginMat[0][0]; invEditorOriginMat[0][1] = editorOriginMat[1][0]; invEditorOriginMat[0][2] = editorOriginMat[2][0];
        invEditorOriginMat[1][0] = editorOriginMat[0][1]; invEditorOriginMat[1][1] = editorOriginMat[1][1]; invEditorOriginMat[1][2] = editorOriginMat[2][1];
        invEditorOriginMat[2][0] = editorOriginMat[0][2]; invEditorOriginMat[2][1] = editorOriginMat[1][2]; invEditorOriginMat[2][2] = editorOriginMat[2][2];
        
        vector t = editorOriginMat[3];
        invEditorOriginMat[3][0] = -(invEditorOriginMat[0][0]*t[0] + invEditorOriginMat[1][0]*t[1] + invEditorOriginMat[2][0]*t[2]);
        invEditorOriginMat[3][1] = -(invEditorOriginMat[0][1]*t[0] + invEditorOriginMat[1][1]*t[1] + invEditorOriginMat[2][1]*t[2]);
        invEditorOriginMat[3][2] = -(invEditorOriginMat[0][2]*t[0] + invEditorOriginMat[1][2]*t[1] + invEditorOriginMat[2][2]*t[2]);
        
        // --- MATRIZ DE DESTINO (No mapa do jogo) ---
        vector targetSpawnMat[4];
        Math3D.YawPitchRollMatrix(anchorOri, targetSpawnMat);
        targetSpawnMat[3] = anchorPos;

        EntityAI containerObj = null;

        if (isRestoration)
        {
            array<Object> nearbyObjs = new array<Object>;
            array<CargoBase> proxies = new array<CargoBase>;
            GetGame().GetObjectsAtPosition(anchorPos, 10.0, nearbyObjs, proxies);
            
            foreach (Object found : nearbyObjs)
            {
                if (found.GetType() == cType)
                {
                    containerObj = EntityAI.Cast(found);
                    Log(" Container found (Safe Start)! Reconnecting event at " + loc.Name);
                    
                    Land_ContainerLocked_Blue_DE blue = Land_ContainerLocked_Blue_DE.Cast(containerObj);
                    if (blue) eventID = blue.GetSDNEventID();
                    
                    Land_ContainerLocked_Yellow_DE yellow = Land_ContainerLocked_Yellow_DE.Cast(containerObj);
                    if (yellow) eventID = yellow.GetSDNEventID();
                    
                    Land_ContainerLocked_Orange_DE orange = Land_ContainerLocked_Orange_DE.Cast(containerObj);
                    if (orange) eventID = orange.GetSDNEventID();
                    
                    Land_ContainerLocked_Red_DE red = Land_ContainerLocked_Red_DE.Cast(containerObj);
                    if (red) eventID = red.GetSDNEventID();
                    
                    break;
                }
            }
        }

        if (!containerObj)
        {
            CleanGroundItems(anchorPos, 15.0);

            array<Object> objects_nearby = new array<Object>;
            array<CargoBase> proxy_cargos = new array<CargoBase>;
            
            string lootObjName = m_Config.LootContainerObject;
            
            if (lootObjName == "") 
            {
                lootObjName = "StorageBox_Wooden";
            }
            
            GetGame().GetObjectsAtPosition(anchorPos, 15.0, objects_nearby, proxy_cargos);
            
            foreach (Object obj : objects_nearby) 
            {
                EntityAI ent = EntityAI.Cast(obj);
                
                if (ent) 
                {
                    string entType = ent.GetType();
                    
                    if (entType.Contains("Land_ContainerLocked") || entType.Contains("SDN_Test_ContainerBase") || entType == lootObjName || entType == "Barrel_Red") 
                    {
                        GetGame().ObjectDelete(ent);
                    }
                }
            }

            vector containerOriginalPos = "4451.6401 341.3586 10250.9335";
            vector containerOriginalOri = "-30.4984 0.0000 0.0000";
            
            vector containerOriginalMat[4];
            Math3D.YawPitchRollMatrix(containerOriginalOri, containerOriginalMat);
            containerOriginalMat[3] = containerOriginalPos;
            
            vector localOffsetMatC[4];
            Math3D.MatrixMultiply4(invEditorOriginMat, containerOriginalMat, localOffsetMatC);

            vector finalMatC[4];
            Math3D.MatrixMultiply4(targetSpawnMat, localOffsetMatC, finalMatC);
            
            vector containerWorldPos = finalMatC[3];
            vector containerWorldOri = Math3D.MatrixToAngles(finalMatC);

            containerObj = EntityAI.Cast(GetGame().CreateObject(cType, containerWorldPos, false, true));
            
            if (!containerObj) 
            {
                return;
            }
            
            containerObj.SetPosition(containerWorldPos);
            containerObj.SetOrientation(containerWorldOri);
            containerObj.Update();
            
            SetIDSafe(containerObj, eventID);
        }

        newEvent.Container = containerObj;
        UpdateEventSmoke(newEvent, 100.0); 

        EntityAI crate = null;
        
        string chestName = m_Config.LootContainerObject;
        if (chestName == "") chestName = "StorageBox_Wooden";

        if (isRestoration) 
        {
             array<Object> nearbyChests = new array<Object>;
             array<CargoBase> cProxies = new array<CargoBase>;
             GetGame().GetObjectsAtPosition(anchorPos, 10.0, nearbyChests, cProxies);
             
             foreach (Object cObj : nearbyChests) 
             {
                 if (cObj.GetType() == chestName) 
                 {
                     crate = EntityAI.Cast(cObj);
                     break;
                 }
             }
        }

        if (!crate) 
        {
            vector crateOriginalPos = "4451.1645 340.2743 10250.6005";
            vector crateOriginalOri = "-121.4474 0.0000 0.0000";
            
            vector crateOriginalMat[4];
            Math3D.YawPitchRollMatrix(crateOriginalOri, crateOriginalMat);
            crateOriginalMat[3] = crateOriginalPos;
            
            vector localOffsetMatCr[4];
            Math3D.MatrixMultiply4(invEditorOriginMat, crateOriginalMat, localOffsetMatCr);

            vector finalMatCr[4];
            Math3D.MatrixMultiply4(targetSpawnMat, localOffsetMatCr, finalMatCr);

            crate = EntityAI.Cast(GetGame().CreateObjectEx(chestName, finalMatCr[3], ECE_NOLIFETIME | ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS));
            if (crate) 
            {
                crate.SetPosition(finalMatCr[3]);
                crate.SetOrientation(Math3D.MatrixToAngles(finalMatCr));
                crate.Update();
                
                dBodyActive(crate, ActiveState.INACTIVE);
                
                FillLootCrate(crate, tier);
            }
        }

        if (crate) 
        {
            ItemBase itemCrate = ItemBase.Cast(crate);
            
            if (itemCrate) 
            {
                itemCrate.SetTakeable(false); 
                itemCrate.SetAllowDamage(false); 
                itemCrate.SetLifetime(7200); 
            }
            
            newEvent.LootCrate = itemCrate;
            
            if (initialLoot == 0) 
            {
                newEvent.InitialLootCount = crate.GetInventory().CountInventory();
            }
            else 
            {
                newEvent.InitialLootCount = initialLoot;
            }
        }

        SpawnZombies(newEvent, kType, eventID);
        if (!isRestoration) SpawnSceneObjects(newEvent, anchorPos, anchorOri, containerObj);
        
        m_ActiveEvents.Insert(newEvent);

        if (m_Config && m_Config.MarkerSettings)
        {
            SDN_MarkerManager.AddEventMarker(loc.Name, anchorPos, m_Config.MarkerSettings);
        }

        // [SDN] Disparar áudio 3D da colisão/queda (Raio de 1km conforme Config.cpp)
        if (!isRestoration && m_Config.EnableSound)
        {
            array<Man> players = new array<Man>;
            GetGame().GetPlayers(players);
            foreach (Man p : players)
            {
                if (p && p.GetIdentity())
                {
                    ScriptRPC rpcSound = new ScriptRPC();
                    rpcSound.Write(anchorPos);
                    rpcSound.Send(p, SDN_NotificationRPC.SDN_EVENT_CRASH_SOUND, true, p.GetIdentity());
                }
            }
        }

        Log("Event Active and Linked at " + loc.Name + " [ID: " + eventID + "]");
    }

    
    // =========================================================================
    // CENA DO EVENTO: composição EXATA de matrizes (Editor -> âncora real)
    // =========================================================================
    // TODA conversão Euler <-> matriz é feita pela PRÓPRIA ENGINE através de um objeto auxiliar
    // (SetOrientation + GetTransform  /  SetTransform + GetOrientation). Assim não dependemos da
    // convenção do Math3D.YawPitchRollMatrix / MatrixToAngles, que não bate com o SetOrientation.
    //
    // Convenção do GetTransform: mat[0]=eixo X (direita), mat[1]=eixo Y (cima), mat[2]=eixo Z (frente), mat[3]=posição.
    //   1) local = item expresso no referencial do Editor (origem da cena)
    //   2) world = local reexpresso no referencial da âncora real do mapa
    //
    // Objetos TIPADOS : SetTransform(finalMat) (matriz exata).
    // Objetos .p3d    : a Enfusion não move p3d estático depois de criado, então o Euler (vindo da engine)
    //                   é passado já na criação (CreateStaticObjectUsingP3D), em RADIANOS.
    void SpawnSceneObjects(ActiveEventInstance evt, vector basePos, vector baseOri, EntityAI containerObj)
    {
        if (!m_SceneTemplate || !containerObj) return;

        vector editorOriginPos = "4451.6596 339.0072 10250.7070";
        vector editorOriginOri = "0 0 0";

        // Objeto auxiliar usado só como conversor de rotação (apagado no final)
        Object conv = GetGame().CreateObjectEx("StaticObj_Furniture_PaperBox_01_small_closed", basePos, ECE_NOLIFETIME);

        if (!conv)
        {
            Log("[Scene] Falha ao criar objeto conversor de rotação. Cena cancelada.");
            return;
        }

        vector originMat[4];
        vector anchorMat[4];
        vector itemMat[4];
        vector finalMat[4];
        vector chkMat[4];
        vector lr0;
        vector lr1;
        vector lr2;
        vector relPos;
        vector localPos;
        vector finalPos;
        vector finalOri;
        vector finalOriRad;
        float align;
        bool isP3D;
        int p3dMode = 2; // CONFIRMADO pelo log: CreateStaticObjectUsingP3D usa RADIANOS (1 = graus, 0 = calibrar automaticamente)
        Object obj;
        ItemBase ib;

        // Rotação da origem do Editor e da âncora real, na convenção da engine
        conv.SetOrientation(editorOriginOri);
        conv.GetTransform(originMat);

        conv.SetOrientation(baseOri);
        conv.GetTransform(anchorMat);

        foreach (SDN_SceneObjectTemplate tmpl : m_SceneTemplate)
        {
            // --- Rotação do item no referencial do Editor (Euler em graus -> matriz, pela engine) ---
            conv.SetOrientation(tmpl.OffsetOri);
            conv.GetTransform(itemMat);

            // --- 1) ROTAÇÃO LOCAL: eixos do item expressos no referencial da origem do Editor ---
            lr0 = Vector(vector.Dot(itemMat[0], originMat[0]), vector.Dot(itemMat[0], originMat[1]), vector.Dot(itemMat[0], originMat[2]));
            lr1 = Vector(vector.Dot(itemMat[1], originMat[0]), vector.Dot(itemMat[1], originMat[1]), vector.Dot(itemMat[1], originMat[2]));
            lr2 = Vector(vector.Dot(itemMat[2], originMat[0]), vector.Dot(itemMat[2], originMat[1]), vector.Dot(itemMat[2], originMat[2]));

            // --- 1) POSIÇÃO LOCAL: offset do item em relação à origem do Editor ---
            relPos = tmpl.OffsetPos - editorOriginPos;
            localPos = Vector(vector.Dot(relPos, originMat[0]), vector.Dot(relPos, originMat[1]), vector.Dot(relPos, originMat[2]));

            // --- 2) MATRIZ FINAL no mundo (referencial da âncora real) ---
            finalMat[0] = (anchorMat[0] * lr0[0]) + (anchorMat[1] * lr0[1]) + (anchorMat[2] * lr0[2]);
            finalMat[1] = (anchorMat[0] * lr1[0]) + (anchorMat[1] * lr1[1]) + (anchorMat[2] * lr1[2]);
            finalMat[2] = (anchorMat[0] * lr2[0]) + (anchorMat[1] * lr2[1]) + (anchorMat[2] * lr2[2]);
            finalMat[3] = (anchorMat[0] * localPos[0]) + (anchorMat[1] * localPos[1]) + (anchorMat[2] * localPos[2]) + basePos;

            finalPos = finalMat[3];

            // --- Matriz final -> Euler, também pela engine (trata gimbal lock internamente) ---
            conv.SetTransform(finalMat);
            finalOri = conv.GetOrientation();

            // --- CRIAÇÃO ---
            obj = null;
            isP3D = tmpl.ClassName.Contains(".p3d");

            if (isP3D)
            {
                // CALIBRAÇÃO AUTOMÁTICA: descobre se o CreateStaticObjectUsingP3D usa GRAUS ou RADIANOS
                // comparando a rotação lida de volta (GetTransform) com a matriz desejada.
                if (p3dMode != 2)
                {
                    obj = GetGame().CreateStaticObjectUsingP3D(tmpl.ClassName, finalPos, finalOri, 1.0, false);

                    if (obj)
                    {
                        obj.GetTransform(chkMat);
                        align = vector.Dot(chkMat[0], finalMat[0]) + vector.Dot(chkMat[1], finalMat[1]) + vector.Dot(chkMat[2], finalMat[2]);

                        if (align >= 2.998)
                        {
                            if (p3dMode == 0)
                            {
                                Log("[Scene] p3d: engine aceitou ângulos em GRAUS.");
                            }

                            p3dMode = 1;
                        }
                        else if (p3dMode == 0)
                        {
                            GetGame().ObjectDelete(obj);
                            obj = null;
                        }
                    }
                }

                if (!obj && p3dMode != 1)
                {
                    finalOriRad = finalOri * Math.DEG2RAD;
                    obj = GetGame().CreateStaticObjectUsingP3D(tmpl.ClassName, finalPos, finalOriRad, 1.0, false);

                    if (obj)
                    {
                        obj.GetTransform(chkMat);
                        align = vector.Dot(chkMat[0], finalMat[0]) + vector.Dot(chkMat[1], finalMat[1]) + vector.Dot(chkMat[2], finalMat[2]);

                        if (align >= 2.998)
                        {
                            if (p3dMode == 0)
                            {
                                Log("[Scene] p3d: engine aceitou ângulos em RADIANOS.");
                            }

                            p3dMode = 2;
                        }
                        else if (p3dMode == 0)
                        {
                            // Nenhuma das duas unidades bateu: volta para graus e segue (ver log de divergência abaixo)
                            Log("[Scene] p3d: nem graus nem radianos bateram em " + tmpl.ClassName);
                            GetGame().ObjectDelete(obj);
                            obj = GetGame().CreateStaticObjectUsingP3D(tmpl.ClassName, finalPos, finalOri, 1.0, false);
                        }
                    }
                }
            }
            else
            {
                obj = GetGame().CreateObjectEx(tmpl.ClassName, finalPos, ECE_NOLIFETIME | ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
            }

            if (!obj)
            {
                Log("[Scene] Falha ao criar objeto: " + tmpl.ClassName);
                continue;
            }

            if (!isP3D)
            {
                obj.SetTransform(finalMat);
                obj.Update();
            }

            // --- Leitura de volta: a rotação aplicada bate com a desejada? (3.0 = idêntico) ---
            obj.GetTransform(chkMat);
            align = vector.Dot(chkMat[0], finalMat[0]) + vector.Dot(chkMat[1], finalMat[1]) + vector.Dot(chkMat[2], finalMat[2]);

            if (align < 2.998)
            {
                Log("[Scene] Rotação divergente (" + align + ") em " + tmpl.ClassName);

                if (!isP3D)
                {
                    obj.SetPosition(finalPos);
                    obj.SetOrientation(finalOri);
                    obj.Update();
                }
            }

            // --- Itens: travar na matriz (sem a física reassentar no terreno) ---
            ib = ItemBase.Cast(obj);

            if (ib)
            {
                ib.SetTakeable(false);
                ib.SetAllowDamage(false);

                if (ib.GetInventory())
                {
                    ib.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
                }

                dBodyActive(ib, ActiveState.INACTIVE);
                dBodyDynamic(ib, false);
            }

            evt.SceneObjects.Insert(obj);
        }

        GetGame().ObjectDelete(conv);
    }

    void CheckActiveEvents(float delta)
    {
        int i; 
        ActiveEventInstance evt; 
        bool isContainerOpen = false; 
        string msg; 
        int j;
        string anims[] = {"DoorsTwin1", "DoorsTwin2", "Doors1", "Doors2", "Door_1_1", "Door_1_2", "Door_2_1"};

        for (i = m_ActiveEvents.Count() - 1; i >= 0; i--)
        {
            evt = m_ActiveEvents.Get(i);
            evt.LifeTime += delta; 
            evt.DebugTimer += delta;
            
            UpdateEventSmoke(evt, delta);
            
            if (evt.LootCrate) 
            {
                evt.LootCrate.SetTakeable(false);
            }

            if (!evt.IsCompleted && evt.Container)
            {
                isContainerOpen = false;
                
                for (j = 0; j < 7; j++) 
                { 
                    if (evt.Container.GetAnimationPhase(anims[j]) > 0.5) 
                    { 
                        isContainerOpen = true; 
                        break; 
                    } 
                }
                
                if (!isContainerOpen) 
                { 
                    ItemBase ib = ItemBase.Cast(evt.Container); 
                    
                    if (ib && ib.IsOpen()) 
                    {
                        isContainerOpen = true; 
                    }
                }
                
                if (!isContainerOpen && evt.LootCrate && evt.LifeTime > 5.0) 
                {
                     if (evt.LootCrate.GetInventory().CountInventory() < evt.InitialLootCount) 
                     { 
                         isContainerOpen = true; 
                     }
                }

                if (isContainerOpen)
                {
                    evt.IsCompleted = true; 
                    if (evt.LootCrate) 
                    {
                        evt.LootCrate.SetTakeable(true);
                        dBodyActive(evt.LootCrate, ActiveState.ACTIVE);
                    }
                    
                    msg = m_Config.MsgComplete;
                    
                    if (msg.Contains("%1")) 
                    {
                        msg = string.Format(msg, evt.LocationName);
                    }
                    else 
                    {
                        msg = msg + " [" + evt.LocationName + "]";
                    }

                    SDN_SendNotificationToAll(m_Config.TitleComplete, msg, ICON_PATH, 10.0);
                    
                    Log("Event Completed at " + evt.LocationName);
                    
                    CleanGroundItems(evt.Position, 10.0);
                    
                    if (m_Config && m_Config.MarkerSettings)
                    {
                        SDN_MarkerManager.RemoveEventMarker(evt.LocationName, m_Config.MarkerSettings);
                    }

                    m_GhostLocations.Insert(evt.Position);
                    evt.EventTimer = 0; 
                    evt.IsActive = true; 
                    SaveState(); 
                }
            }
            
            if (!evt.IsActive && !evt.IsCompleted)
            {
                array<Man> players = new array<Man>; 
                GetGame().GetPlayers(players);
                
                foreach (Man p : players) 
                {
                    if (!p || !p.IsAlive()) 
                    {
                        continue;
                    }
                    
                    if (vector.Distance(p.GetPosition(), evt.Position) <= m_Config.ActivationRadius) 
                    {
                        evt.IsActive = true;
                        
                        string msgActive = m_Config.MsgActive;
                        
                        if (msgActive.Contains("%1")) 
                        {
                            msgActive = string.Format(msgActive, evt.LocationName);
                        }
                        else 
                        {
                            msgActive = msgActive + " [" + evt.LocationName + "]";
                        }
                        
                        SDN_SendNotificationToAll(m_Config.TitleActive, msgActive, ICON_PATH, 8.0);

                        Log("Activated by player at " + evt.LocationName);
                        evt.EventTimer = 0; 
                        SaveState(); 
                        break;
                    }
                }
                
                if (!evt.IsActive && evt.LifeTime >= (m_Config.TimeToActivate * 60)) 
                {
                    
                    string msgFail = m_Config.MsgFail;
                    
                    if (msgFail.Contains("%1"))
                    {
                        msgFail = string.Format(msgFail, evt.LocationName);
                    }
                    else
                    {
                        msgFail = msgFail + " [" + evt.LocationName + "]";
                    }

                    SDN_SendNotificationToAll(m_Config.TitleFail, msgFail, ICON_PATH, 8.0);
                    
                    CleanGroundItems(evt.Position, 10.0);
                    
                    if (m_Config && m_Config.MarkerSettings)
                    {
                        SDN_MarkerManager.RemoveEventMarker(evt.LocationName, m_Config.MarkerSettings);
                    }

                    evt.Cleanup(); 
                    m_ActiveEvents.Remove(i); 
                    SaveState(); 
                    continue;
                }
            }
            else 
            {
                evt.EventTimer += delta;
                
                if (evt.EventTimer >= (m_Config.TimeAfterActivation * 60)) 
                {
                    if (!evt.IsCompleted)
                    {
                        string msgFailPost = m_Config.MsgFail;
                        
                        if (msgFailPost.Contains("%1"))
                        {
                            msgFailPost = string.Format(msgFailPost, evt.LocationName);
                        }
                        else
                        {
                            msgFailPost = msgFailPost + " [" + evt.LocationName + "]";
                        }
                        
                        SDN_SendNotificationToAll(m_Config.TitleFail, msgFailPost, ICON_PATH, 8.0);
                    }
                    
                    CleanGroundItems(evt.Position, 10.0);
                    
                    if (m_Config && m_Config.MarkerSettings)
                    {
                        SDN_MarkerManager.RemoveEventMarker(evt.LocationName, m_Config.MarkerSettings);
                    }

                    evt.Cleanup(); 
                    m_ActiveEvents.Remove(i); 
                    SaveState(); 
                }
            }
        }
    }

    void SDN_SendNotificationToAll(string title, string message, string iconPath, float duration)
    {
        if (!GetGame().IsServer()) 
        {
            return;
        }
        
        if (message == "") 
        {
            return;
        }

        bool soundEnabled = true;
        
        if (m_Config) 
        {
            soundEnabled = m_Config.EnableSound;
        }

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        foreach (Man player : players)
        {
            if (player && player.GetIdentity())
            {
                ScriptRPC rpc = new ScriptRPC();
                rpc.Write(title);
                rpc.Write(message);
                rpc.Write(iconPath);
                rpc.Write(duration);
                rpc.Write(soundEnabled);
                
                rpc.Send(player, SDN_NotificationRPC.SDN_EVENT_MSG, true, player.GetIdentity());
            }
        }
        
        // Dispara o Webhook do Discord em segundo plano
        SendDiscordWebhook(title, message);
    }
    
    void SendGlobalMessage(string msg)
    {
        // Desativado
    }
}