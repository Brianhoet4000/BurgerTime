#include "Ingredient.h"

#include "IngredientComponent.h"
#include "IngredientPart.h"

dae::Ingredient::Ingredient(std::string path, dae::Scene& scene, glm::vec2 pos)
{
	m_pIngredient = std::make_shared<dae::GameObject>();

	const auto& pIngredientComp = std::make_shared<IngredientComponent>(m_pIngredient.get());
	m_pIngredient->AddComponent(pIngredientComp);
	m_pIngredient->SetRelativePosition(0, 0);
	scene.Add(m_pIngredient);

	std::unique_ptr<IngredientPart> newBurgerPart =  std::make_unique<IngredientPart>(m_pIngredient, path, scene, pos);
}