// PASTA: 4_World
// ARQUIVO: SDN_Security_Persistence.c
/*
    SDN Security Persistence (Debug & Base Class Fix)
    - Persistência de ID Único (Chave <-> Container).
    - Modificação da classe PAI das chaves para garantir cobertura global.
    - Logs de Debug ativados para diagnóstico.
*/

// =========================================================
// 1. LÓGICA DAS CHAVES (Classe Base)
// =========================================================
// Em vez de modificar cada cor, modificamos a base de todas as chaves do mod.
modded class ShippingContainerKeys_ColorBase
{
    protected int m_SDN_EventID = 0;

    void ShippingContainerKeys_ColorBase() 
    { 
        RegisterNetSyncVariableInt("m_SDN_EventID"); 
    }

    void SetSDNEventID(int id) 
    { 
        m_SDN_EventID = id; 
        SetSynchDirty(); 
        // Print("[SDN Key] ID Set: " + id);
    }

    int GetSDNEventID() 
    { 
        return m_SDN_EventID; 
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SDN_EventID);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) 
            return false;
            
        if (!ctx.Read(m_SDN_EventID)) 
        {
            m_SDN_EventID = 0;
        }
            
        SetSynchDirty();
        return true;
    }
}

// =========================================================
// 2. LÓGICA DOS CONTAINERS (Land_ContainerLocked...)
// =========================================================
// Precisamos manter individual pois eles não compartilham uma classe base acessível via script fácil.


modded class Land_ContainerLocked_Blue_DE
{
    protected int m_SDN_EventID = 0;

    void Land_ContainerLocked_Blue_DE() 
    { 
        RegisterNetSyncVariableInt("m_SDN_EventID"); 
    }

    void SetSDNEventID(int id) 
    { 
        m_SDN_EventID = id; 
        SetSynchDirty(); 
    }

    int GetSDNEventID() 
    { 
        return m_SDN_EventID; 
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SDN_EventID);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) 
            return false;
            
        if (!ctx.Read(m_SDN_EventID)) 
        {
            m_SDN_EventID = 0;
        }
            
        SetSynchDirty();
        return true;
    }
}

modded class Land_ContainerLocked_Yellow_DE
{
    protected int m_SDN_EventID = 0;

    void Land_ContainerLocked_Yellow_DE() 
    { 
        RegisterNetSyncVariableInt("m_SDN_EventID"); 
    }

    void SetSDNEventID(int id) 
    { 
        m_SDN_EventID = id; 
        SetSynchDirty(); 
    }

    int GetSDNEventID() 
    { 
        return m_SDN_EventID; 
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SDN_EventID);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) 
            return false;
            
        if (!ctx.Read(m_SDN_EventID)) 
        {
            m_SDN_EventID = 0;
        }
            
        SetSynchDirty();
        return true;
    }
}

modded class Land_ContainerLocked_Orange_DE
{
    protected int m_SDN_EventID = 0;

    void Land_ContainerLocked_Orange_DE() 
    { 
        RegisterNetSyncVariableInt("m_SDN_EventID"); 
    }

    void SetSDNEventID(int id) 
    { 
        m_SDN_EventID = id; 
        SetSynchDirty(); 
    }

    int GetSDNEventID() 
    { 
        return m_SDN_EventID; 
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SDN_EventID);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) 
            return false;
            
        if (!ctx.Read(m_SDN_EventID)) 
        {
            m_SDN_EventID = 0;
        }
            
        SetSynchDirty();
        return true;
    }
}

modded class Land_ContainerLocked_Red_DE
{
    protected int m_SDN_EventID = 0;

    void Land_ContainerLocked_Red_DE() 
    { 
        RegisterNetSyncVariableInt("m_SDN_EventID"); 
    }

    void SetSDNEventID(int id) 
    { 
        m_SDN_EventID = id; 
        SetSynchDirty(); 
    }

    int GetSDNEventID() 
    { 
        return m_SDN_EventID; 
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_SDN_EventID);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) 
            return false;
            
        if (!ctx.Read(m_SDN_EventID)) 
        {
            m_SDN_EventID = 0;
        }
            
        SetSynchDirty();
        return true;
    }
}

// =========================================================
// 3. FIX ACTION: ActionUnlockShippingContainer
// =========================================================

modded class ActionUnlockShippingContainer
{
    // Validação de Segurança (Visual / Client Side)
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        // 1. Cast básico do Mod Original
        ContainerLockedBase shipCont;
        ShippingContainerKeys_ColorBase key;

        if (Class.CastTo(shipCont, target.GetObject()) && Class.CastTo(key, item))
        {
            // --- SDN CHECK ---
            int containerID = GetContainerID(shipCont);
            
            // Se o container tem ID (é protegido), verificamos a chave
            if (containerID > 0)
            {
                int keyID = key.GetSDNEventID();
                // Check on both Client and Server because variables are network synced.
                if (keyID != containerID) 
                {
                    return false; // Bloqueia ação (opção não aparece)
                }
            }
            // -----------------

            // Lógica original de cores
            int doorIndex = TranslateLockSelectionIntoDoorIdx(target);
            if (doorIndex != -1 && shipCont.IsDoorLocked(doorIndex))
            {
                return (shipCont.GetLockCompatibilityType(doorIndex) & key.GetKeyCompatibilityType()) != 0;
            }
        }
        return false;
    }

    // Validação Final (Server Side) - ONDE A MÁGICA ACONTECE
    override void OnFinishProgressServer(ActionData action_data)
    {
        ShippingContainerKeys_ColorBase key = ShippingContainerKeys_ColorBase.Cast(action_data.m_MainItem);
        ContainerLockedBase shipCont = ContainerLockedBase.Cast(action_data.m_Target.GetObject());

        if (!shipCont || !key) 
            return;

        // --- SDN CHECK FINAL ---
        int containerID = GetContainerID(shipCont);
        int keyID = key.GetSDNEventID();

        // Print de Debug: Veja isso no Log do Servidor (script.log)
        Print("[SDN DEBUG] Tentativa de Abertura -> ContainerID: " + containerID + " | KeyID: " + keyID);

        if (containerID > 0)
        {
            if (keyID != containerID)
            {
                Print("[SDN DEBUG] ACESSO NEGADO: IDs não batem.");
                return; // CANCELA TUDO. Não abre, não destrói chave.
            }
        }
        else
        {
             // Opcional: Se ContainerID for 0, é um container antigo ou spawnado por admin.
             // Você pode descomentar a linha abaixo se quiser BLOQUEAR containers sem ID:
             // Print("[SDN DEBUG] Container sem ID. Permitindo acesso legado.");
        }
        // -----------------------

        // Se passou, destranca
        UnlockDoor(action_data.m_Target);
        
        // Destrói a chave
        key.SetHealth(0);
        Print("[SDN DEBUG] Sucesso. Chave destruída.");
    }

    // Função Auxiliar para pegar ID de qualquer container
    int GetContainerID(Object obj)
    {
        Land_ContainerLocked_Blue_DE blue = Land_ContainerLocked_Blue_DE.Cast(obj);
        if (blue) return blue.GetSDNEventID();
        
        Land_ContainerLocked_Yellow_DE yellow = Land_ContainerLocked_Yellow_DE.Cast(obj);
        if (yellow) return yellow.GetSDNEventID();
        
        Land_ContainerLocked_Orange_DE orange = Land_ContainerLocked_Orange_DE.Cast(obj);
        if (orange) return orange.GetSDNEventID();
        
        Land_ContainerLocked_Red_DE red = Land_ContainerLocked_Red_DE.Cast(obj);
        if (red) return red.GetSDNEventID();
        
        return 0;
    }

    // Mantém lógica original
    override protected void UnlockDoor(ActionTarget target)
    {
        Building building;
        if (Class.CastTo(building, target.GetObject()))
        {
            int doorIndex = TranslateLockSelectionIntoDoorIdx(target);
            if (doorIndex != -1)
            {
                building.UnlockDoor(doorIndex, false);
            }
        }
    }

    protected int TranslateLockSelectionIntoDoorIdx(ActionTarget target)
    {
        ContainerLockedBase shipCont;
        if (Class.CastTo(shipCont, target.GetObject()))
        {
            string selectionName = shipCont.GetActionComponentName(target.GetComponentIndex());
            if (selectionName.Contains("_lock"))
            {
                return (selectionName.Substring(4, 1).ToInt() - 1);
            }
        }
        return -1;
    }
}