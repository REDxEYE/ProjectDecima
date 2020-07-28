#ifndef OPENMC_INPUT_H
#define OPENMC_INPUT_H

#include <array>
#include <memory>
#include <GLFW/glfw3.h>
#include <utility>
#include <glm/vec2.hpp>

namespace omc {
    namespace buttons {
        enum class keyboard {
            Space = 32,
            Apostrophe = 39,
            Comma = 44,
            Minus = 45,
            Period = 46,
            Slash = 47,
            Zero = 48,
            One = 49,
            Two = 50,
            Three = 51,
            Four = 52,
            Fixe = 53,
            Six = 54,
            Seven = 55,
            Eight = 56,
            Nine = 57,
            Semicolon = 59,
            Equal = 61,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LeftBracket = 91,
            Backslash = 92,
            RightRracket = 93,
            GraveAccent = 96,
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            PageUp = 266,
            PageDown = 267,
            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            KeyPadZero = 320,
            KeyPadOne = 321,
            KeyPadTwo = 322,
            KeyPadThree = 323,
            KeyPadFour = 324,
            KeyPadFixe = 325,
            KeyPadSix = 326,
            KeyPadSeven = 327,
            KeyPadEight = 328,
            KeyPadNine = 329,
            KeyPadDecimal = 330,
            KeyPadDivide = 331,
            KeyPadMultiply = 332,
            KeyPadSubtract = 333,
            KeyPadAdd = 334,
            KeyPadEnter = 335,
            KeyPadEqual = 336,
            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControL = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348
        };
        enum class mouse {
            Button1 = 0,
            Button2 = 1,
            Button3 = 2,
            Button4 = 3,
            Button5 = 4,
            Button6 = 5,
            Button7 = 6,
            Button8 = 7,
            Left = Button1,
            Right = Button2,
            Middle = Button3,
        };
    }
}

namespace omc {
    enum class key_state : uint8_t {
        Release,
        Press,
        Free,
        Hold
    };
    class application;
    class key_info {
    private:
        key_state state;

    public:
        inline explicit key_info(key_state state) : state(state) {}

        inline key_info() : state(key_state::Free) {}

        inline bool release() const { return this->state == key_state::Release; };

        inline bool press() const { return this->state == key_state::Press; };

        inline bool free() const { return this->state == key_state::Free; };

        inline bool hold() const { return this->state == key_state::Hold; };
    };

    class input_manager {
    private:
        omc::application* app;
        std::array<key_info, GLFW_KEY_LAST + 1> keyboard {};
        std::array<key_info, GLFW_MOUSE_BUTTON_LAST + 1> mouse {};
    public:
        explicit input_manager(omc::application* app):app(app){};

        void update();

        inline key_info get(buttons::keyboard value) const { return this->keyboard[static_cast<int>(value)]; }

        inline key_info operator[](buttons::keyboard value) const { return this->keyboard[static_cast<int>(value)]; }

        inline key_info operator[](buttons::mouse value) const { return this->mouse[static_cast<int>(value)]; }

        inline key_info key(buttons::keyboard value) const{return this->keyboard[static_cast<int>(value)];}
        inline key_info key(buttons::mouse value) const{return this->mouse[static_cast<int>(value)];}
    };
}

#endif //OPENMC_INPUT_H
