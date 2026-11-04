/*
  Ficheiro: SDN_ContainerEventConfig.c
  Caminho (relativo ao mod): scripts/4_World/SDN_ContainerEventConfig.c
  Versão: SDN_MODS Folder + Discord Webhook + Custom Titles + Expanded Formatting
*/

class SDN_EventLocation
{
    string Name;
    vector Coordinates;
    vector Orientation; // {Yaw, Pitch, Roll} (Yaw = Direção/Rotação)
}

class SDN_EventLootItem
{
    string ItemName;
    ref array<string> Attachments;
    int Probability; 
    
    void SDN_EventLootItem()
    {
        Attachments = new array<string>;
    }
}

class SDN_LootTierConfig
{
    ref array<ref SDN_EventLootItem> Items;
    
    void SDN_LootTierConfig()
    {
        Items = new array<ref SDN_EventLootItem>;
    }
}

// --- PERSISTÊNCIA (SAVE) ---
class SDN_SavedEventData
{
    vector Position;
    string LocationName;
    string ContainerType;
    string KeyType;
    int Tier;
    float LifeTime;
    int InitialLootCount;
}

class SDN_ContainerEventGlobalState
{
    float SpawnTimer;
    float CurrentTargetTime;
    int EventsSpawnedInWave;
    ref array<ref SDN_SavedEventData> ActiveEvents;
    
    ref array<string> LastRoundLocations;
    ref array<string> CurrentWaveLocations;
    
    ref array<vector> CompletedGhostLocations; 
    
    void SDN_ContainerEventGlobalState()
    {
        ActiveEvents = new array<ref SDN_SavedEventData>;
        LastRoundLocations = new array<string>;
        CurrentWaveLocations = new array<string>;
        CompletedGhostLocations = new array<vector>;
    }
}

// --- CONFIGURAÇÃO DE MARCADORES ---
class SDN_MarkerSettings
{
    bool EnableExpansionMarkers;
    bool EnableBasicMapMarkers;
    bool EnableVPPMarkers;
    string MarkerName;
    string MarkerIconName; 
    int MarkerColorR;
    int MarkerColorG;
    int MarkerColorB;

    void SDN_MarkerSettings()
    {
        EnableExpansionMarkers = true;
        EnableBasicMapMarkers = true;
        EnableVPPMarkers = false;
        MarkerName = "Evento de Container";
        MarkerIconName = "Deliver"; 
        MarkerColorR = 255;
        MarkerColorG = 0;
        MarkerColorB = 0;
    }
}

// --- CONFIGURAÇÃO PRINCIPAL (JSON) ---
class SDN_ContainerEventConfig
{
    int EventActive;
    bool EnableSound;
    int ServerTimeOffset;
    
    // [SDN] Webhook do Discord
    string DiscordWebhookURL;

    int MinPlayers;                 
    int SpawnStartDelay;            
    int MaxSimultaneousEvents; 
    int DelayBetweenSpawns;     
    int RespawnTime;            
    float ActivationRadius;         
    int TimeToActivate;             
    int TimeAfterActivation;        
    int ZombieCount;
    int MinLoot;
    int MaxLoot;
    int MaxHistoryLocations;
    
    int ChanceBlue;   
    int ChanceYellow; 
    int ChanceOrange; 
    int ChanceRed;    
    
    string LootContainerObject; 
    
    // [SDN] Títulos Personalizáveis
    string TitleSpawn;
    string TitleActive;
    string TitleComplete;
    string TitleFail;
    
    string MsgSpawn;
    string MsgActive;
    string MsgComplete;
    string MsgFail;
    
    ref SDN_MarkerSettings MarkerSettings;
    
    ref array<ref SDN_EventLocation> Locations;
    
    ref array<string> RestartTimes;
    int RestartSafetyMinutes;
    int GhostCleanupTime; 

    void SDN_ContainerEventConfig()
    {
        Locations = new array<ref SDN_EventLocation>;
        RestartTimes = new array<string>;
        MarkerSettings = new SDN_MarkerSettings();
        
        EventActive = 1;
        EnableSound = true;
        ServerTimeOffset = 0;
        
        DiscordWebhookURL = "";
        
        TitleSpawn = "NEW EVENT";
        TitleActive = "EVENT ACTIVATED";
        TitleComplete = "SUCCESS";
        TitleFail = "FAILURE";
    }
}