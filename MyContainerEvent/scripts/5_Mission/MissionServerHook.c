/*
  Arquivo: MissionServerHook.c
  Caminho (relativo ao mod): scripts/5_Mission/MissionServerHook.c
  Versão SDN:
  - CORREÇÃO: Atualizado tipo da variável de 'ContainerEventManager' para 'SDN_ContainerEventManager'.
  - MANTIDO: Lógica de injeção no OnInit e ciclo de OnUpdate.
*/

modded class MissionServer
{
    // [SDN] Variável atualizada para a nova arquitetura
    protected ref SDN_ContainerEventManager m_SDN_ContainerEventManager;

    override void OnInit()
    {
        super.OnInit();
        
        // [SDN] Instancia o Gerenciador de Eventos usando o novo nome da classe
        m_SDN_ContainerEventManager = new SDN_ContainerEventManager();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        // [SDN] Repassa o tick do servidor para o nosso manager processar os timers e eventos
        if (m_SDN_ContainerEventManager)
        {
            m_SDN_ContainerEventManager.OnUpdate(timeslice);
        }
    }
}