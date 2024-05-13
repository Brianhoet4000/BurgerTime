#include "IngredientPart.h"

#include "TextureComponent.h"

dae::IngredientPart::IngredientPart(std::string path, dae::Scene& scene, glm::vec2 pos)
{
	for (int i = 0; i < 4; ++i)
	{
		const auto& pIngredientPart = std::make_shared<dae::GameObject>("Ingredient");
		const auto& pIngredientTexture = std::make_shared<dae::TextureComponent>(pIngredientPart.get());
		glm::vec2 newPos;

		std::cout << "Burger/" + path + "/" + path + "_0" + std::to_string(i + 1) + ".png" << '\n';
		pIngredientTexture->SetTexture("Burger/" + path + "/" + path + "_0" + std::to_string(i + 1) + ".png");
		
		newPos = glm::vec2{ pos.x + (24 * i), pos.y }; //(pIngredientTexture->GetSize().x * i);

		pIngredientPart->AddComponent(pIngredientTexture);

		pIngredientPart->SetRelativePosition(newPos);
		const auto& pCollider = std::make_shared<dae::GameCollisionComponent>(pIngredientPart.get(), newPos, pIngredientTexture->GetSize().x, pIngredientTexture->GetSize().y, true, true, false);
		pIngredientPart->AddComponent(pCollider);

		scene.Add(pIngredientPart);
	}
}
