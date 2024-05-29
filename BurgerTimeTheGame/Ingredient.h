#pragma once
#include <string>
#include <glm/vec2.hpp>

#include "IngredientPart.h"
#include "Scene.h"

namespace dae
{

    class Ingredient
    {
    public:
        Ingredient(std::string path, dae::Scene& scene, glm::vec2 pos);
        ~Ingredient() = default;
        Ingredient(const Ingredient&) = delete;
        Ingredient& operator=(const Ingredient&) = delete;
        Ingredient(Ingredient&&) = default;
        Ingredient& operator=(Ingredient&&) = default;

    private:
        std::shared_ptr<GameObject> m_pIngredient;
    };

}
