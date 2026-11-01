/*
  Arquivo: SDN_MarkerManager.c
  Caminho (no mod principal): scripts/4_World/SDN_MarkerManager.c
  Versão SDN: Universal Core. Suporta Expansion, BasicMap e VPPMap.
  - FIX: O Expansion gera UIDs próprios ao usar CreateServerMarker. Criado um mapa para armazenar e remover o UID correto.
*/

#ifdef EXPANSIONMOD
#define SDN_HAS_EXPANSION_MARKERS
#endif

#ifdef EXPANSIONMODNAVIGATION
#define SDN_HAS_EXPANSION_MARKERS
#endif

class SDN_VPPMarkerData
{
    string uid;
    vector pos;
    string name;
    string icon;
    int color;
}

class SDN_MarkerManager
{
    static ref map<string, string> m_SDN_ActiveMarkers = new map<string, string>();
    // [SDN_FIX] Cofre seguro para guardar os IDs aleatórios que o Expansion gera
    static ref map<string, string> m_SDN_ExpansionUIDs = new map<string, string>(); 
    static ref map<string, ref SDN_VPPMarkerData> m_SDN_VPPMarkers = new map<string, ref SDN_VPPMarkerData>();

    static void AddEventMarker(string locationName, vector pos, SDN_MarkerSettings config)
    {
        if (!GetGame().IsServer()) return;
        if (!config) return;

        string uid = "SDN_Event_" + locationName;

        #ifdef SDN_HAS_EXPANSION_MARKERS
        if (config.EnableExpansionMarkers)
        {
            ExpansionMarkerModule expansion_module;
            if (Class.CastTo(expansion_module, CF_ModuleCoreManager.Get(ExpansionMarkerModule)))
            {
                string expIcon = config.MarkerIconName;
                if (expIcon == "Deliver" || expIcon == "waypointeditor_CA.paa" || expIcon == "") {
                    expIcon = "Airdrop"; 
                }
                
                int color = ARGB(255, config.MarkerColorR, config.MarkerColorG, config.MarkerColorB);
                
                ExpansionMarkerData marker = expansion_module.CreateServerMarker(config.MarkerName, expIcon, pos, color, true);
                if (marker)
                {
                    // [SDN_FIX] Lê o ID que o Expansion gerou e guarda-o associado ao nome do local!
                    m_SDN_ExpansionUIDs.Insert(locationName, marker.GetUID());
                }
            }
        }
        #endif

        #ifdef BASICMAP
        if (config.EnableBasicMapMarkers)
        {
            array<ref BasicMapMarker> basic_markers = new array<ref BasicMapMarker>();
            TIntArray bColor = new TIntArray();
            bColor.Insert(255);
            bColor.Insert(config.MarkerColorR);
            bColor.Insert(config.MarkerColorG);
            bColor.Insert(config.MarkerColorB);
            
            // [SDN_FIX] Reversão para a API 2D nativa e segura do BasicMap (removido o parâmetro 'true').
            BasicMapMarker bMarker = new BasicMapMarker(config.MarkerName, pos, "BasicMap\\gui\\images\\marker.paa", bColor);
            basic_markers.Insert(bMarker);
            BasicMap().SetMarkersRemote(uid, basic_markers, null);
            m_SDN_ActiveMarkers.Insert(locationName, uid);
        }
        #endif

        if (config.EnableVPPMarkers)
        {
            SDN_VPPMarkerData vppData = new SDN_VPPMarkerData();
            vppData.uid = uid;
            vppData.pos = pos;
            vppData.name = config.MarkerName;
            
            vppData.icon = config.MarkerIconName;
            if (vppData.icon == "" || vppData.icon == "Deliver") {
                vppData.icon = "waypointeditor_CA.paa"; 
            }
            
            vppData.color = ARGB(255, config.MarkerColorR, config.MarkerColorG, config.MarkerColorB);
            
            m_SDN_VPPMarkers.Insert(uid, vppData);
            SyncVPPMarkersToAll();
            m_SDN_ActiveMarkers.Insert(locationName, uid);
        }
    }

    static void RemoveEventMarker(string locationName, SDN_MarkerSettings config)
    {
        if (!GetGame().IsServer()) return;
        if (!config) return;

        string uid = "SDN_Event_" + locationName;

        #ifdef SDN_HAS_EXPANSION_MARKERS
        if (config.EnableExpansionMarkers)
        {
            ExpansionMarkerModule expansion_module;
            if (Class.CastTo(expansion_module, CF_ModuleCoreManager.Get(ExpansionMarkerModule)))
            {
                // [SDN_FIX] Vai ao nosso cofre procurar qual foi o ID gerado pelo Expansion
                string expUid;
                if (m_SDN_ExpansionUIDs.Find(locationName, expUid))
                {
                    // Ordena ao Expansion para remover o ID exato dele!
                    expansion_module.RemoveServerMarker(expUid);
                    m_SDN_ExpansionUIDs.Remove(locationName); // Limpa o cofre
                }
            }
        }
        #endif

        #ifdef BASICMAP
        if (config.EnableBasicMapMarkers)
        {
            array<ref BasicMapMarker> empty_markers = new array<ref BasicMapMarker>();
            BasicMap().SetMarkersRemote(uid, empty_markers, null);
        }
        #endif
        
        if (config.EnableVPPMarkers)
        {
            if (m_SDN_VPPMarkers.Contains(uid))
            {
                m_SDN_VPPMarkers.Remove(uid);
                SyncVPPMarkersToAll();
            }
        }

        if (m_SDN_ActiveMarkers.Contains(locationName))
            m_SDN_ActiveMarkers.Remove(locationName);
    }

    static void SyncVPPMarkersToAll()
    {
        if (!GetGame().IsServer()) return;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        foreach (Man p : players)
        {
            PlayerBase pb = PlayerBase.Cast(p);
            if (pb && pb.GetIdentity())
            {
                ScriptRPC rpc = new ScriptRPC();
                rpc.Write(m_SDN_VPPMarkers.Count());
                foreach (string id, SDN_VPPMarkerData data : m_SDN_VPPMarkers)
                {
                    rpc.Write(data.uid); rpc.Write(data.pos); rpc.Write(data.name); rpc.Write(data.icon); rpc.Write(data.color);
                }
                rpc.Send(pb, 8410226, true, pb.GetIdentity());
            }
        }
    }

    static void SyncVPPMarkersToPlayer(PlayerIdentity identity)
    {
        if (!GetGame().IsServer() || !identity) return;
        PlayerBase pb = PlayerBase.Cast(identity.GetPlayer());
        if (!pb) return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(m_SDN_VPPMarkers.Count());
        foreach (string id, SDN_VPPMarkerData data : m_SDN_VPPMarkers)
        {
            rpc.Write(data.uid); rpc.Write(data.pos); rpc.Write(data.name); rpc.Write(data.icon); rpc.Write(data.color);
        }
        rpc.Send(pb, 8410226, true, identity);
    }
}

modded class PlayerBase
{
    override void OnPlayerLoaded()
    {
        super.OnPlayerLoaded();
        if (GetGame().IsServer())
        {
            SDN_MarkerManager.SyncVPPMarkersToPlayer(this.GetIdentity());
        }
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);
        if (rpc_type == 8410226 && GetGame().IsClient()) 
        {
            int count;
            if (!ctx.Read(count)) return;
            SDN_MarkerManager.m_SDN_VPPMarkers.Clear();
            for (int i = 0; i < count; i++)
            {
                SDN_VPPMarkerData data = new SDN_VPPMarkerData();
                if (!ctx.Read(data.uid)) return;
                if (!ctx.Read(data.pos)) return;
                if (!ctx.Read(data.name)) return;
                if (!ctx.Read(data.icon)) return;
                if (!ctx.Read(data.color)) return;
                SDN_MarkerManager.m_SDN_VPPMarkers.Insert(data.uid, data);
            }
        }
    }
}