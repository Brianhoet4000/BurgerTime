#pragma once
#include <glm/vec2.hpp>
#include "GameObject.h"

namespace dae
{

    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void Execute(float) = 0;
        void SetKeyPressed(bool keyPressed) { m_KeyPressed = keyPressed; }
        bool GetKeyPressed() const { return m_KeyPressed; }
    protected:
        std::shared_ptr<dae::GameObject> GetGameActor() const { return m_pGameObject; }
        std::shared_ptr<dae::GameObject> m_pGameObject{};
        bool m_KeyPressed = false;
    };

    class MoveCommand : public Command
    {
    private:
        glm::vec2 m_Dir{};
    public:
        MoveCommand(std::shared_ptr<dae::GameObject>, const glm::vec2& dir);
        virtual void Execute(float deltaTime) override;
    };

}