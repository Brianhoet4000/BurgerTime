#include "Commands.h"
#include "GameObject.h"
#include "Counter.h"

namespace dae
{

    MoveCommand::MoveCommand(std::shared_ptr<dae::GameObject> owner, const glm::vec2& dir)
    {
        m_pGameObject = owner;
        m_Dir = dir;
    }

    void MoveCommand::Execute(float deltaTime)
    {
        if (m_pGameObject->ReturnDeleting()) return;

        glm::vec2 pos = m_pGameObject->GetRelativePosition();
        pos.x += m_Dir.x * deltaTime;
        pos.y += m_Dir.y * deltaTime;

        m_pGameObject->SetRelativePosition(pos);
    }
    
}