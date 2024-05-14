#include "Ingredient.h"

#include "IngredientPart.h"

dae::Ingredient::Ingredient(std::string path, dae::Scene& scene, glm::vec2 pos)
{
	const auto& pIngredient = std::make_shared<dae::GameObject>();

	std::unique_ptr<IngredientPart> newBurgerPart =  std::make_unique<IngredientPart>(pIngredient.get(), path, scene, pos);
}