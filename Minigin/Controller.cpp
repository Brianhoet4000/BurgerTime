#include "Controller.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

class dae::Controller::ControllerImpl
{
public:
    ControllerImpl(unsigned int controllerIndex) :
        m_ControllerIndex{ controllerIndex }
    {
        ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    }

    void Update()
    {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_ControllerIndex, &m_CurrentState);

        m_ButtonsPressedThisFrame = m_CurrentState.Gamepad.wButtons & ~m_PreviousState.Gamepad.wButtons;
    }
    bool IsButtonDown(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
    bool IsUpThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
    bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }
    unsigned int GetIDOfController() const { return m_ControllerIndex; }

private:
    XINPUT_STATE m_CurrentState{};
    XINPUT_STATE m_PreviousState{};
    int m_ButtonsPressedThisFrame{};
    int m_ButtonsReleasedFrame{};

    unsigned int m_ControllerIndex{};
};

dae::Controller::Controller(unsigned int controllerIdx)
{
    m_pImpl = new ControllerImpl(controllerIdx);
}

dae::Controller::~Controller()
{
    delete m_pImpl;
}

void dae::Controller::Update()
{
    m_pImpl->Update();
}

bool dae::Controller::IsButtonDown(unsigned int button) const
{
    return m_pImpl->IsButtonDown(button);
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
    return m_pImpl->IsUpThisFrame(button);
}

bool dae::Controller::IsPressed(unsigned int button) const
{
    return m_pImpl->IsPressed(button);
}

unsigned int dae::Controller::GetIDOfController() const
{
    return m_pImpl->GetIDOfController();
}
