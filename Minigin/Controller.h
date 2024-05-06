namespace dae
{

    class Controller final
    {

    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;

    public:
        Controller(unsigned int controllerIndex);
        ~Controller();
        void Update();
        bool IsButtonDown(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;
        unsigned int GetIDOfController() const;

        enum class ControllerButton
        {
            DpadUp = 0x0001,
            DpadDown = 0x0002,
            DpadLeft = 0x0004,
            DpadRight = 0x0008,
            Start = 0x0010,
            Back = 0x0020,
            LeftThumb = 0x0040,
            RightThumb = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,
            ButtonA = 0x1000,
            ButtonB = 0x2002,
            ButtonX = 0x4000,
            ButtonY = 0x8000,
        };
    };

}