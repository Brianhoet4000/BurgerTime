#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#pragma once
#include "Minigin.h"
#include <SDL.h>
#include "ResourceManager.h"
#include "Scene.h"
#include <iostream>

#include "GameObject.h"
#include "PlayerManager.h"
#include "PlayerTwo.h"
#include "ScreenManager.h"

void load()
{
	std::cout << "--Tab to switch gamemode--\n";
	std::cout << "--E To Accept--\n";
	std::cout << "--F2 To pause and resume music--\n";

	std::cout << "--How To Play--\n";
	std::cout << "WASD to move character\n";
	//std::cout << "Space to shoot\n";
	std::cout << "Dpad to move secondPlayer\n";
	//std::cout << "A to shoot\n";


	auto& startscene = dae::SceneManager::GetInstance().CreateScene("MainMenu");
	dae::ScreenManager::GetInstance().CreateMenuScreen(startscene);
	startscene.SetActive(true);

	dae::SceneManager::GetInstance().CreateScene("Level01");


	dae::SceneManager::GetInstance().CreateScene("Level02");


	dae::SceneManager::GetInstance().CreateScene("Level03");


	auto& sceneGameOver = dae::SceneManager::GetInstance().CreateScene("GameOver");
	dae::ScreenManager::GetInstance().CreateGameOverScreen(sceneGameOver);

}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}