#include "IngredientPart.h"

#include "GameCollisionMngr.h"
#include "IngredientPartComponent.h"
#include "TextureComponent.h"

dae::IngredientPart::IngredientPart(std::shared_ptr<GameObject>& parent, std::string path, dae::Scene& scene, glm::vec2 pos)
{
	for (int i = 0; i < 4; ++i)
	{
		const auto& pIngredientPart = std::make_shared<dae::GameObject>("Ingredient");
		const auto& pIngredientTexture = std::make_shared<dae::TextureComponent>(pIngredientPart.get());
		glm::vec2 newPos = pos;

		pIngredientTexture->SetTexture("Burger/" + path + "/" + path + "_0" + std::to_string(i + 1) + ".png");
		pIngredientPart->AddComponent(pIngredientTexture);
		newPos = glm::vec2{ pos.x + (pIngredientTexture->GetSize().x * i), pos.y };

		pIngredientPart->SetRelativePosition(newPos);
		const auto& pCollider = std::make_shared<dae::GameCollisionComponent>(pIngredientPart.get(), newPos, pIngredientTexture->GetSize().x, pIngredientTexture->GetSize().y, true, true, false);
		pIngredientPart->AddComponent(pCollider);

		const auto& pIngredientComponent = std::make_shared<IngredientPartComponent>(pIngredientPart.get());
		pIngredientPart->AddComponent(pIngredientComponent);

		parent->AddChild(pIngredientPart);
		scene.Add(pIngredientPart);
	}
}
