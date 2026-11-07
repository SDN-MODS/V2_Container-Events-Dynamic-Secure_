// =============================================================================
// SDN Container Event - LIMPEZA DE FANTASMAS .p3d NO CLIENTE
// Colocar na mesma pasta do sdn_containereventmanager.c (scripts/4_World).
//
// O servidor apaga os objetos .p3d estáticos do cenário, mas o cliente que estava por perto
// pode continuar com a cópia local (objetos flutuando). Quando o evento acaba, o servidor
// manda as posições desses objetos e o cliente remove o que sobrou nelas.
// =============================================================================
modded class PlayerBase
{
    // Raio de busca ao redor de cada posição enviada (o pivot do p3d pode diferir do ponto de criação)
    protected const float SDN_GHOST_SEARCH_RADIUS = 1.5;

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type != SDN_NotificationRPC.SDN_EVENT_CLEANUP_P3D)
        {
            return;
        }

        if (!GetGame().IsClient())
        {
            return;
        }

        int count;

        if (!ctx.Read(count))
        {
            return;
        }

        array<vector> ghostPositions = new array<vector>;
        vector readPos;
        int i;

        for (i = 0; i < count; i++)
        {
            if (!ctx.Read(readPos))
            {
                return;
            }

            ghostPositions.Insert(readPos);
        }

        // Duas passadas: agora e 3s depois (o destroy de rede do servidor pode chegar depois do RPC)
        SDN_CleanP3DGhosts(ghostPositions, true);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SDN_CleanP3DGhosts, 3000, false, ghostPositions, false);
    }

    void SDN_CleanP3DGhosts(array<vector> positions, bool logResult)
    {
        if (!positions)
        {
            return;
        }

        array<Object> found;
        array<CargoBase> proxies;
        Object candidate;
        string candidateType;
        int removed = 0;
        int skipped = 0;
        string skippedTypes = "";
        int idx;

        foreach (vector pos : positions)
        {
            found = new array<Object>;
            proxies = new array<CargoBase>;
            GetGame().GetObjectsAtPosition(pos, SDN_GHOST_SEARCH_RADIUS, found, proxies);

            for (idx = 0; idx < found.Count(); idx++)
            {
                candidate = found.Get(idx);

                if (!candidate)
                {
                    continue;
                }

                // Nunca mexer em jogadores, itens, veículos ou qualquer entidade
                if (candidate.IsInherited(Man) || candidate.IsInherited(EntityAI))
                {
                    continue;
                }

                candidateType = candidate.GetType();

                // Objetos criados por CreateStaticObjectUsingP3D não têm classe no CfgVehicles (tipo vazio)
                if (candidateType != "")
                {
                    skipped++;

                    if (skipped <= 5)
                    {
                        skippedTypes = skippedTypes + " [" + candidateType + "]";
                    }

                    continue;
                }

                candidate.SetScale(0.00001); // garante que fique invisível mesmo se o delete falhar
                GetGame().ObjectDelete(candidate);
                removed++;
            }
        }

        if (logResult)
        {
            Print("[SDN] Limpeza de fantasmas p3d no cliente: removidos=" + removed + " ignorados=" + skipped + skippedTypes);
        }
    }
}
