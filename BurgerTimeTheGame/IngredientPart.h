#pragma once
#include <string>
#include "GameObject.h"
#include "GameCollisionComponent.h"
#include "IngredientComponent.h"
#include "Scene.h"

namespace dae
{

    class IngredientPart
    {
    public:

        IngredientPart(std::shared_ptr<GameObject>& parent, std::string path, dae::Scene& scene, glm::vec2 pos);
        ~IngredientPart() = default;
        IngredientPart(const IngredientPart&) = delete;
        IngredientPart& operator=(const IngredientPart&) = delete;
        IngredientPart(IngredientPart&&) = default;
        IngredientPart& operator=(IngredientPart&&) = default;

    private:
        
    };

}