/*
  Arquivo: SDN_Container_Security.c
  Caminho (relativo ao mod): scripts/4_World/SDN_Container_Security.c
*/

// Define a classe do script para o objeto contenedor de loot definido no config.cpp
// IMPORTANTE: Atualize seu config.cpp para que a classe herde com este exato nome.
class CONTAINER_Chest extends SeaChest
{
    // --- PROTEÇÕES DE INVENTÁRIO ---

    // Impede colocar nas mãos
    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    // Impede colocar dentro de outros inventários (Mochilas, Carros, etc)
    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    // Impede anexar em slots
    override bool CanPutAsAttachment(EntityAI parent)
    {
        return false;
    }
    
    // Impede desanexar (caso já esteja anexado por algum motivo forçado)
    override bool CanDetachAttachment(EntityAI parent)
    {
        return false;
    }
    
    // Impede troca rápida
    override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
    {
        return false;
    }

    // Nota: Nenhuma lógica de deleção (Timer) foi adicionada aqui.
    // O ciclo de vida é controlado exclusivamente pelo SDN_ContainerEventManager.c
    // para evitar conflitos e crashes do servidor.
}