#include "ScreenManager.h"
#include <SDL_scancode.h>
#include <glm/vec2.hpp>
#include "EnemyPrefab.h"
#include "Font.h"
#include "GameCommands.h"
#include "FPSCounterComponent.h"
#include "GameCollisionMngr.h"
#include "ConditionSingleCoopComponent.h"
#include "ConditionVersusComponent.h"
#include "HealthComponent.h"
#include "HighscoreComponent.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "PlayerOne.h"
#include "PlayerTwo.h"
#include "PointComponent.h"

namespace dae
{

	void dae::ScreenManager::CreateMenuScreen(dae::Scene& scene)
	{
		m_pFont = dae::ResourceManager::GetInstance().LoadFont(m_FontPath.data(), 20);

		const auto& pGameObjBackGround = std::make_shared<dae::GameObject>();
		const auto& pBackGroundLogo = std::make_shared<dae::TextureComponent>(pGameObjBackGround.get());
		pBackGroundLogo->SetTexture("background.png");
		pGameObjBackGround->SetRelativePosition(pGameObjBackGround->GetWorldPosition());
		pGameObjBackGround->AddComponent(pBackGroundLogo);
		scene.Add(pGameObjBackGround);

		const auto& pFont = dae::ResourceManager::GetInstance().LoadFont(m_FontPath.data(), 36);
		const auto& pGameType = std::make_shared<GameObject>();
		const auto& pGameTypeText = std::make_shared<TextComponent>("GameMode:", pFont, pGameType.get());
		pGameType->SetRelativePosition(m_Width / 2 - 100, 90);
		pGameType->AddComponent(pGameTypeText);
		scene.Add(pGameType);

		m_pGameModeDisplay = std::make_shared<GameObject>();
		m_pGameModeDisplayText = std::make_shared<TextComponent>("Single", pFont, m_pGameModeDisplay.get());
		m_pGameModeDisplay->AddComponent(m_pGameModeDisplayText);
		m_pGameModeDisplay->SetRelativePosition(m_Width / 2 - 100, 140);

		scene.Add(m_pGameModeDisplay);

		//Controls
		{
			const auto& pKeyboardControls = std::make_shared<dae::GameObject>();
			const auto& pKeyboard = std::make_shared<dae::TextureComponent>(pKeyboardControls.get());
			pKeyboard->SetTexture("Controls/WASDSpace.png");
			pKeyboardControls->SetRelativePosition(m_Width / 6, 300);
			pKeyboardControls->AddComponent(pKeyboard);
			scene.Add(pKeyboardControls);

			const auto& pControllerControls = std::make_shared<dae::GameObject>();
			const auto& pController = std::make_shared<dae::TextureComponent>(pControllerControls.get());
			pController->SetTexture("Controls/ControllerInput.png");
			pControllerControls->SetRelativePosition((m_Width - (m_Width/4)) - pController->GetSize().x, 300);
			pControllerControls->AddComponent(pController);
			scene.Add(pControllerControls);

			const auto& pTabIcon = std::make_shared<dae::GameObject>();
			const auto& pTab = std::make_shared<dae::TextureComponent>(pTabIcon.get());
			pTab->SetTexture("Controls/TabIcon.png");
			pTabIcon->AddComponent(pTab);
			pTabIcon->SetRelativePosition(40, 90);
			scene.Add(pTabIcon);

			const auto& pEAccept = std::make_shared<dae::GameObject>();
			const auto& pE = std::make_shared<dae::TextureComponent>(pEAccept.get());
			pE->SetTexture("Controls/EAcceptIcon.png");
			pEAccept->AddComponent(pE);
			pEAccept->SetRelativePosition(40 + pTab->GetSize().x/2 - pE->GetSize().x /2, 140);
			scene.Add(pEAccept);

			std::shared_ptr<GameCommands::SwitchGameMode> Switch = std::make_shared<GameCommands::SwitchGameMode>(pGameObjBackGround, m_pGameModeDisplay.get(), m_CurrentGameMode, this);
			std::shared_ptr<GameCommands::AcceptGameMode> Accept = std::make_shared<GameCommands::AcceptGameMode>();
			std::shared_ptr<GameCommands::SkipLevel> SkipLevel = std::make_shared<GameCommands::SkipLevel>();
			std::shared_ptr<GameCommands::ResetLevel> ResetLevel = std::make_shared<GameCommands::ResetLevel>();

			dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_TAB, Switch);
			dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_E, Accept);
			dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_Q, SkipLevel);
			dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_R, ResetLevel);
		}

		LoadInSounds();
	}

	void dae::ScreenManager::CreateGameScreen(dae::Scene& scene)
	{
		const auto& pGameObjBackGround = std::make_shared<dae::GameObject>();
		const auto& pBackGroundLogo = std::make_shared<dae::TextureComponent>(pGameObjBackGround.get());
		pBackGroundLogo->SetTexture("background.png");
		pGameObjBackGround->SetRelativePosition(pGameObjBackGround->GetWorldPosition());
		pGameObjBackGround->AddComponent(pBackGroundLogo);
		scene.Add(pGameObjBackGround);

		const auto& pGameObjLogo = std::make_shared<dae::GameObject>();
		const auto& pDaeLogo = std::make_shared<dae::TextureComponent>(pGameObjLogo.get());
		pDaeLogo->SetTexture("logo.tga");
		pGameObjLogo->SetRelativePosition(m_Width/2 - pDaeLogo->GetSize().x/2, 420);
		pGameObjLogo->AddComponent(pDaeLogo);
		scene.Add(pGameObjLogo);

		const auto& pGameObjFps = std::make_shared<dae::GameObject>();
		const auto& pFpsFont = dae::ResourceManager::GetInstance().LoadFont(m_FontPath.data(), 15);
		const auto& pFpsCounter = std::make_shared<dae::FPSCounterComponent>("FPS", pFpsFont, pGameObjFps.get());
		pGameObjFps->SetRelativePosition(0, 0);
		pGameObjFps->AddComponent(pFpsCounter);
		scene.Add(pGameObjFps);

		//Mute sound
		std::shared_ptr<GameCommands::MuteMusic> muteMusic = std::make_shared<GameCommands::MuteMusic>();
		dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_F2, muteMusic);

		CreateLevelZero(scene);
		CreateLevelOne(scene);
		CreateLevelTwo(scene);
	}

	void ScreenManager::CreateGameOverScreen(dae::Scene& scene)
	{
		auto GameObjBackGround = std::make_shared<dae::GameObject>();
		auto go = std::make_shared<dae::TextureComponent>(GameObjBackGround.get());
		go->SetTexture("background.png");
		GameObjBackGround->SetRelativePosition(GameObjBackGround->GetWorldPosition());
		GameObjBackGround->AddComponent(go);
		scene.Add(GameObjBackGround);

		auto GameObjLogo = std::make_shared<dae::GameObject>();
		auto go2 = std::make_shared<dae::TextureComponent>(GameObjLogo.get());
		go2->SetTexture("logo.tga");
		GameObjLogo->SetRelativePosition(glm::vec3{ 216, 500, 0 });
		GameObjLogo->AddComponent(go2);
		scene.Add(GameObjLogo);

		const auto pMainObj = std::make_shared<dae::GameObject>();
		scene.Add(pMainObj);

		auto font = dae::ResourceManager::GetInstance().LoadFont(m_FontPath.data(), 36);
		auto smallFont = dae::ResourceManager::GetInstance().LoadFont(m_FontPath.data(), 16);

		const auto gameOverObj = std::make_shared<dae::GameObject>();
		const auto gameOverText = std::make_shared<dae::TextComponent>("GAME OVER", font, gameOverObj.get());
		gameOverObj->AddComponent(gameOverText);
		gameOverObj->SetRelativePosition({ (m_Width - gameOverText->GetTextWidth()) / 2.f, 40.f });
		pMainObj->AddChild(gameOverObj);

		const auto resetObj = std::make_shared<dae::GameObject>("Explanation");
		const auto resetText = std::make_shared<dae::TextComponent>("Enter name (Press Enter when done)", smallFont, resetObj.get());
		resetObj->SetRelativePosition({ (m_Width - resetText->GetTextWidth()) / 2.f, 100.f });
		resetObj->AddComponent(resetText);
		pMainObj->AddChild(resetObj);

		const auto highObj = std::make_shared<dae::GameObject>();
		const auto highText = std::make_shared<dae::TextComponent>("HighScores: ", smallFont, highObj.get());
		highObj->SetRelativePosition({ 200, 145 });
		highObj->AddComponent(highText);
		pMainObj->AddChild(highObj);

		const auto pointsObj = std::make_shared<dae::GameObject>();
		const auto pointsText = std::make_shared<dae::TextComponent>("Points:", smallFont, pointsObj.get());
		pointsObj->SetRelativePosition({ 10, 250 });
		pointsObj->AddComponent(pointsText);
		pMainObj->AddChild(pointsObj);

		const auto scoreObj = std::make_shared<dae::GameObject>("Score");
		scoreObj->SetRelativePosition({ pointsObj.get()->GetRelativePosition().x + pointsText->GetTextWidth() + 10.f, 250 });

		const auto pp = std::make_shared<dae::HighscoreComponent>(scoreObj.get(), smallFont);
		scoreObj->AddComponent(pp);
		pMainObj->AddChild(scoreObj);

		scene.Add(pMainObj);
	}

	void ScreenManager::CreateLevelZero(dae::Scene& scene)
	{
		if(m_CurrentLevel == 0)
		{
			if (m_CurrentGameMode == GameMode::SinglePlayer)
			{
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level01");
				if (!m_AddedPlayers)
				{
					const auto& pPlayer_01 = std::make_shared<dae::PlayerOne>(scene);
					PlayerManager::GetInstance().AddPlayer(pPlayer_01->ReturnPlayer());
					m_AddedPlayers = true;
				}
				auto player = PlayerManager::GetInstance().GetPlayers();
				dae::SceneManager::GetInstance().GetActiveScene()->Add(player[0]);
				player[0]->SetRelativePosition(pLevel->GetSpawnPosition()[0]);
				
				//const auto& pSpawner = std::make_shared<dae::EnemySpawner>(*dae::SceneManager::GetInstance().GetActiveScene(), pLevel->GetEnemySpawnPosition(), 3);
				const auto& pWinLose = std::make_shared<dae::ConditionSingleCoopComponent>(pLevel->returnLevelObj().get());
				pLevel->returnLevelObj()->AddComponent(pWinLose);

				std::unique_ptr<EnemyPrefab> TestEnemy = std::make_unique<EnemyPrefab>(scene, glm::vec2{ 267, 422 }, "Egg");

				CreateUI(scene, player, false);
			}

			else if(m_CurrentGameMode == GameMode::Coop)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level01");

				if (!m_AddedPlayers)
				{
					const auto& pPlayer_01 = std::make_shared<dae::PlayerOne>(scene);
					PlayerManager::GetInstance().AddPlayer(pPlayer_01->ReturnPlayer());

					const auto& Player_02 = std::make_shared<dae::PlayerTwo>(scene, *pLevel, true);
					PlayerManager::GetInstance().AddPlayer(Player_02->ReturnPlayer());
					m_AddedPlayers = true;
				}

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}

			else if(m_CurrentGameMode == GameMode::Versus)
			{

				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level01");

				if (!m_AddedPlayers)
				{
					const auto& pPlayer_01 = std::make_shared<dae::PlayerOne>(scene);
					PlayerManager::GetInstance().AddPlayer(pPlayer_01->ReturnPlayer());

					const auto& Player_02 = std::make_shared<dae::PlayerTwo>(scene, *pLevel, false);
					PlayerManager::GetInstance().AddPlayer(Player_02->ReturnPlayer());
					m_AddedPlayers = true;
				}

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				const auto& pWinLose = std::make_shared<dae::ConditionVersusComponent>(pLevel->returnLevelObj().get());
				pLevel->returnLevelObj()->AddComponent(pWinLose);

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}
		}
	}

	void ScreenManager::CreateLevelOne(dae::Scene& scene)
	{
		if (m_CurrentLevel == 1)
		{
			if (m_CurrentGameMode == GameMode::SinglePlayer)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level02");

				auto player = PlayerManager::GetInstance().GetPlayers();
				dae::SceneManager::GetInstance().GetActiveScene()->Add(player[0]);
				player[0]->SetRelativePosition(pLevel->GetSpawnPosition()[0]);

				CreateUI(scene, player, false);
			}

			else if (m_CurrentGameMode == GameMode::Coop)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level02");

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}

			else if (m_CurrentGameMode == GameMode::Versus)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level02");

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				const auto& pWinLose = std::make_shared<dae::ConditionVersusComponent>(pLevel->returnLevelObj().get());
				pLevel->returnLevelObj()->AddComponent(pWinLose);

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}
		}
	}

	void ScreenManager::CreateLevelTwo(dae::Scene& scene)
	{
		if (m_CurrentLevel == 2)
		{
			if (m_CurrentGameMode == GameMode::SinglePlayer)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level03");

				//auto EnemySpawner = std::make_shared<dae::EnemySpawner>(scene, pLevel->GetEnemySpawnPosition(), 6);
				auto player = PlayerManager::GetInstance().GetPlayers();
				dae::SceneManager::GetInstance().GetActiveScene()->Add(player[0]);
				player[0]->SetRelativePosition(pLevel->GetSpawnPosition()[0]);

				CreateUI(scene, player, false);
			}

			else if (m_CurrentGameMode == GameMode::Coop)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level03");

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}

			else if (m_CurrentGameMode == GameMode::Versus)
			{
				//Level
				const auto& pLevel = std::make_shared<dae::LevelPrefab>(scene, "Level03");

				for (int i = 0; i < static_cast<int>(PlayerManager::GetInstance().GetPlayers().size()); ++i)
				{
					dae::SceneManager::GetInstance().GetActiveScene()->Add(PlayerManager::GetInstance().GetPlayers()[i]);
					PlayerManager::GetInstance().GetPlayers()[i]->SetRelativePosition(pLevel->GetSpawnPosition()[i]);
				}

				const auto& pWinLose = std::make_shared<dae::ConditionVersusComponent>(pLevel->returnLevelObj().get());
				pLevel->returnLevelObj()->AddComponent(pWinLose);

				auto players = PlayerManager::GetInstance().GetPlayers();
				CreateUI(scene, players, true);
			}
		}
	}

	void ScreenManager::CreateUI(dae::Scene& scene, std::vector<std::shared_ptr<GameObject>>& players, bool SecondPlayer)
	{
		const auto& pPlayerLivesFirst = std::make_shared<dae::GameObject>("PlayerLivesFirst");
		scene.Add(pPlayerLivesFirst);
		const auto& pPlayerLivesSecond = std::make_shared<dae::GameObject>("PlayerLivesSecond");
		scene.Add(pPlayerLivesSecond);

		for (int i = 0; i < players[0]->GetComponent<HealthComponent>()->GetAmount(); ++i)
		{
			const auto& pPlayerLives = std::make_shared<dae::GameObject>();
			const auto& pPlayerLiveTexture = std::make_shared<TextureComponent>(pPlayerLives.get());
			pPlayerLiveTexture->SetTexture("Misc/PepperHealth.png");
			pPlayerLives->AddComponent(pPlayerLiveTexture);

			pPlayerLives->SetRelativePosition(((710.f/2.f) - ((pPlayerLiveTexture->GetSize().x*3))) + (i * pPlayerLiveTexture->GetSize().x),  6.f);
			scene.Add(pPlayerLives);
			pPlayerLivesFirst->AddChild(pPlayerLives);

			if (SecondPlayer)
			{
				const auto& pPlayerLives2 = std::make_shared<dae::GameObject>();
				const auto& pPlayerLiveTexture2 = std::make_shared<TextureComponent>(pPlayerLives2.get());
				pPlayerLiveTexture2->SetTexture("Misc/SallyHealth.png");
				pPlayerLives2->AddComponent(pPlayerLiveTexture2);

				pPlayerLives2->SetRelativePosition(((710.f / 2.f) - ((pPlayerLiveTexture2->GetSize().x * 3))) + (i * pPlayerLiveTexture2->GetSize().x), 34.f);
				scene.Add(pPlayerLives2);
				pPlayerLivesSecond->AddChild(pPlayerLives2);

			}
		}

		const auto& pPlayerPoints = std::make_shared<GameObject>("PlayerOnePoints");
		const auto& pPlayerPointsText = std::make_shared<TextComponent>(std::to_string(players[0]->GetComponent<PointComponent>()->GetAmount()),
			m_pFont, pPlayerPoints.get());
		pPlayerPoints->AddComponent(pPlayerPointsText);
		pPlayerPoints->SetRelativePosition((710.f / 2.f), 6.f);
		scene.Add(pPlayerPoints);

		if(SecondPlayer)
		{
			const auto& pPlayerPoints2 = std::make_shared<GameObject>("PlayerTwoPoints");
			const auto& pPlayerPointsText2 = std::make_shared<TextComponent>(std::to_string(players[1]->GetComponent<PointComponent>()->GetAmount()),
				m_pFont, pPlayerPoints2.get());
			pPlayerPoints2->AddComponent(pPlayerPointsText2);
			pPlayerPoints2->SetRelativePosition((710.f / 2.f), 34.f);
			scene.Add(pPlayerPoints2);
		}
	}

	void ScreenManager::PlayerKilledResetLevelAndStats(dae::GameCollisionComponent* ownerbox) const
	{
		ownerbox->GetOwner()->GetComponent<HealthComponent>()->DecreaseAmount(1);
		ownerbox->GetOwner()->GetComponent<PointComponent>()->SetAmount(0);
		
		const auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
		
		if (ownerbox->GetOwner()->GetTag() == "Player_01")
		{
			const auto& points = GetInstance().GetGameObjectInScene(*scene, "PlayerOnePoints");
			points->GetComponent<TextComponent>()->SetText(std::to_string(ownerbox->GetOwner()->GetComponent<PointComponent>()->GetAmount()));
		}
		else
		{
			const auto& points = GetInstance().GetGameObjectInScene(*scene, "PlayerTwoPoints");
			points->GetComponent<TextComponent>()->SetText(std::to_string(ownerbox->GetOwner()->GetComponent<PointComponent>()->GetAmount()));
		}

		dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
		dae::GameCollisionMngr::GetInstance().ClearAll();
		GetInstance().CreateGameScreen(*SceneManager::GetInstance().GetActiveScene());
	}

	GameObject* ScreenManager::GetGameObjectInScene(dae::Scene& scene, std::string tag)
	{
		for (const auto& gameObjects : scene.GetGameObjects())
		{
			if(gameObjects->GetTag() == tag)
			{
				return gameObjects.get();
			}
		}
		return nullptr;
	}

	void ScreenManager::SkipToGameOverLevel()
	{
		switch (m_CurrentLevel)
		{
		case 0:
		{
			SceneManager::GetInstance().NextScene();
			dae::GameCollisionMngr::GetInstance().ClearAll();
			IncrementCurrentLevel();

			SceneManager::GetInstance().NextScene();
			IncrementCurrentLevel();

			SceneManager::GetInstance().NextScene();
			IncrementCurrentLevel();

			break;
		}

		case 1:
		{
			SceneManager::GetInstance().NextScene();
			dae::GameCollisionMngr::GetInstance().ClearAll();
			IncrementCurrentLevel();

			SceneManager::GetInstance().NextScene();
			dae::GameCollisionMngr::GetInstance().ClearAll();
			IncrementCurrentLevel();
			break;
		}
		case 2:
		{
			SceneManager::GetInstance().NextScene();
			dae::GameCollisionMngr::GetInstance().ClearAll();
			IncrementCurrentLevel();
			break;
		}
		}
	}

	void ScreenManager::ProceedNextLevel() const
	{
		dae::SceneManager::GetInstance().NextScene();

		if (dae::SceneManager::GetInstance().GetActiveSceneName() != "GameOver")
		{
			dae::GameCollisionMngr::GetInstance().ClearAll();
			dae::ScreenManager::GetInstance().IncrementCurrentLevel();
			dae::ScreenManager::GetInstance().CreateGameScreen(*dae::SceneManager::GetInstance().GetActiveScene());
		}
	}

	void ScreenManager::IncreasePoint(GameObject* player, int points)
	{
		Scene* scene = dae::SceneManager::GetInstance().GetActiveScene();
		if (player->GetTag() == "Player_01")
		{
			player->GetComponent<PointComponent>()->IncreaseAmount(points);
			const auto& pointPlayerOnePoints = dae::ScreenManager::GetInstance().GetGameObjectInScene(*scene, "PlayerOnePoints");
			pointPlayerOnePoints->GetComponent<TextComponent>()->SetText(std::to_string(player->GetComponent<PointComponent>()->GetAmount()));
		}
		else if (player->GetTag() == "Player_02")
		{
			player->GetComponent<PointComponent>()->IncreaseAmount(points);
			const auto& pointPlayerTwoPoints = dae::ScreenManager::GetInstance().GetGameObjectInScene(*scene, "PlayerTwoPoints");
			pointPlayerTwoPoints->GetComponent<TextComponent>()->SetText(std::to_string(player->GetComponent<PointComponent>()->GetAmount()));
		}
	}

	void ScreenManager::LoadInSounds()
	{
		dae::servicelocator::register_sound_system(std::make_unique<dae::SoundSystem>());
		dae::servicelocator::get_sound_system().Load(0, "11_main.ogg");
		dae::servicelocator::get_sound_system().Load(1, "0_burger_going_down.ogg");
		dae::servicelocator::get_sound_system().Load(2, "1_burger_touching_floor.ogg");
		dae::servicelocator::get_sound_system().Load(3, "16_stepping_on_burger.ogg");
		dae::servicelocator::get_sound_system().Load(4, "7_enemy_crushed.ogg");
		dae::servicelocator::get_sound_system().Load(5, "5_coin_inserted.ogg");
	}

}
