// PASTA: 4_World
// ARQUIVO: SDN_PlayerListener.c

modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        // Segurança 1: Apenas Client
        if (!GetGame().IsClient()) return;

        // Segurança 2: Apenas Jogador Local
        if (GetGame().GetPlayer() != this) return;
        
        // ID RPC: 8410225
        if (rpc_type == 8410225)
        {
            string title;
            if (!ctx.Read(title)) { return; }

            string text;
            if (!ctx.Read(text)) { return; }

            string icon;
            if (!ctx.Read(icon)) { return; }

            float time;
            if (!ctx.Read(time)) { return; }

            // [SDN] Ler configuração de som enviada pelo servidor
            bool enableSound;
            if (!ctx.Read(enableSound)) { return; }

            // [SDN] Tocar Som de Notificação APENAS se ativado na config
            if (enableSound && GetGame().GetPlayer())
            {
                SEffectManager.PlaySoundOnObject("notification_SoundSet", GetGame().GetPlayer(), 0, 0.5, false);
            }

            // Chamada com assinatura INVERTIDA (Tempo primeiro) baseada no seu mod de referência
            // Restaurando a chamada direta. O mod VPPNotifications provavelmente não tem a flag #ifdef.
            // Caso dê erro de compilação futuramente por falta do mod, avise o dono do servidor para usar VPP.
            NotificationSystem.AddNotificationExtended(time, title, text, icon);
        }
    }
}