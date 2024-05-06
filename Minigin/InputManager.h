#pragma once
#include "Singleton.h"
#include "Controller.h"
#include "Commands.h"
#include <SDL.h>
#include <memory>
#include <map>
#include <vector>

namespace dae
{
    class GameObject;

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput(float deltaTime);
        void AddController(unsigned int id);
        void BindControllerToCommand(unsigned int controllerId, Controller::ControllerButton& button, std::shared_ptr<Command> command);
        void BindKeyToCommand(const Uint8& key, std::shared_ptr<Command> command);
        void UnBindKey(const std::shared_ptr<Command>& command);
        void UnBindAllKeys();
        void UpdateControllers();
        


    private:
        //Controller
        using ControllerKey = std::pair<unsigned int, Controller::ControllerButton>;
        using CommandsMap = std::map<ControllerKey, std::shared_ptr<Command>>;

        CommandsMap m_Commands{};
        std::vector<std::shared_ptr<Controller>> m_Controllers{};

        //Keyboard
        using KeyboardCommands = std::map<Uint8, std::shared_ptr<Command>>;
        KeyboardCommands m_KeyCommands;


        void ProcessControllerInput(float deltaTime);
        void ProcessKeyboardInput(float deltaTime);

        
    };

}