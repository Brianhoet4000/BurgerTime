#pragma once
#include <string>
#include "GameObject.h"
#include "GameCollisionComponent.h"
#include "Scene.h"

namespace dae
{

    class IngredientPart
    {
    public:

        IngredientPart(GameObject* parent, std::string path, dae::Scene& scene, glm::vec2 pos);
        ~IngredientPart() = default;
        IngredientPart(const IngredientPart&) = delete;
        IngredientPart& operator=(const IngredientPart&) = delete;
        IngredientPart(IngredientPart&&) = default;
        IngredientPart& operator=(IngredientPart&&) = default;

        void leftPart(GameObject* parent, std::string path, dae::Scene& scene, glm::vec2 pos);
        void middleleftPart(GameObject* parent, std::string path, dae::Scene& scene, glm::vec2 pos);
        void middlerightPart(GameObject* parent, std::string path, dae::Scene& scene, glm::vec2 pos);
        void rightPart(GameObject* parent, std::string path, dae::Scene& scene, glm::vec2 pos);

    private:
        
    };

}