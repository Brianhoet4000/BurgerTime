#include "CollisionBoxManager.h"
#include "GameObject.h"

namespace dae
{
    class GoldStateComponent;

    void dae::CollisionBoxManager::AddCollisionBox(dae::GameObject* owner, CollisionBoxComponent* box)
    {
        m_pOwners.push_back(owner);
        m_pCollisonBoxes.push_back(box);
    }

    void dae::CollisionBoxManager::RemoveCollisionBox(CollisionBoxComponent* box)
    {
        m_pCollisonBoxes.erase(std::remove(m_pCollisonBoxes.begin(), m_pCollisonBoxes.end(), box), m_pCollisonBoxes.end());
    }

    bool dae::CollisionBoxManager::CheckForCollision(const CollisionBoxComponent* box) const
    {
        for (const auto& otherBox : m_pCollisonBoxes)
        {
            if (otherBox == box)
                continue;

            if (box->GetCollisionRect().x < otherBox->GetCollisionRect().x + otherBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > otherBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < otherBox->GetCollisionRect().y + otherBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > otherBox->GetCollisionRect().y)
            {
                return true;
            }
        }

        return false;


    }

    dae::CollisionBoxComponent* dae::CollisionBoxManager::CheckForCollisionComponent(const CollisionBoxComponent* box) const
    {
        for (const auto& otherBox : m_pCollisonBoxes)
        {
            if (otherBox == box)
                continue;

            if (box->GetCollisionRect().x < otherBox->GetCollisionRect().x + otherBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > otherBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < otherBox->GetCollisionRect().y + otherBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > otherBox->GetCollisionRect().y)
            {
                return otherBox;
            }
        }
        return nullptr;
    }

    bool dae::CollisionBoxManager::Raycast(glm::vec2 startpos, glm::vec2 direction, dae::CollisionBoxComponent* collisionbox) const
    {
        glm::vec2 startPos = startpos;
        startPos.x += collisionbox->GetCollisionRect().w / 2.0f;
        startPos.y += collisionbox->GetCollisionRect().h / 2.0f;

        glm::vec2 dir = glm::normalize(direction);
        float distance = collisionbox->GetCollisionRect().w / 2.f;
        const float offset{ 1.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pCollisonBoxes)
        {

            if (startPos.x + (dir.x * distance + offset) <= boxes->GetCollisionRect().x + boxes->GetCollisionRect().w &&
                startPos.x + dir.x * distance - offset >= boxes->GetCollisionRect().x &&
                startPos.y + (dir.y * distance + offset) <= boxes->GetCollisionRect().y + boxes->GetCollisionRect().h &&
                startPos.y + dir.y * distance - offset >= boxes->GetCollisionRect().y)
            {
                return false;
            }

        }
        return true;
    }

}