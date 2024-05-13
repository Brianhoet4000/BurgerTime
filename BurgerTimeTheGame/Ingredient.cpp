#include "Ingredient.h"

#include "IngredientPart.h"

dae::Ingredient::Ingredient(std::string path, dae::Scene& scene, glm::vec2 pos)
{
	std::unique_ptr<IngredientPart> newBurgerPart =  std::make_unique<IngredientPart>(path, scene, pos);
}