/*     Copyright 2019 Diligent Graphics LLC
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#pragma once

#include "BasicTypes.h"
#include "FlagEnum.h"

namespace Diligent
{

struct MouseState
{
    enum BUTTON_FLAGS : Uint8
    {
        BUTTON_FLAG_NONE   = 0x00,
        BUTTON_FLAG_LEFT   = 0x01,
        BUTTON_FLAG_MIDDLE = 0x02,
        BUTTON_FLAG_RIGHT  = 0x04,
        BUTTON_FLAG_WHEEL  = 0x08
    };

    Float32      PosX        = -1;
    Float32      PosY        = -1;
    BUTTON_FLAGS ButtonFlags = BUTTON_FLAG_NONE;
    Float32      WheelDelta  = 0;
};
DEFINE_FLAG_ENUM_OPERATORS(MouseState::BUTTON_FLAGS)


enum class InputKeys
{
    Unknown = 0,
    MoveLeft,
    MoveRight,
    MoveForward,
    MoveBackward,
    MoveUp,
    MoveDown,
    Reset,
    ControlDown,
    ShiftDown,
    AltDown,
    ZoomIn,
    ZoomOut,
    TotalKeys
};

enum INPUT_KEY_STATE_FLAGS : Uint8
{
    INPUT_KEY_STATE_FLAG_KEY_NONE     = 0x00,
    INPUT_KEY_STATE_FLAG_KEY_IS_DOWN  = 0x01,
    INPUT_KEY_STATE_FLAG_KEY_WAS_DOWN = 0x80
};
DEFINE_FLAG_ENUM_OPERATORS(INPUT_KEY_STATE_FLAGS)

class InputControllerBase
{
public:
    const MouseState& GetMouseState() const
    {
        return m_MouseState;
    }

    INPUT_KEY_STATE_FLAGS GetKeyState(InputKeys Key) const
    {
        return m_Keys[static_cast<size_t>(Key)];
    }

    bool IsKeyDown(InputKeys Key) const
    {
        return (GetKeyState(Key) & INPUT_KEY_STATE_FLAG_KEY_IS_DOWN) != 0;
    }

    void ClearState()
    {
        m_MouseState.WheelDelta = 0;

        for (Uint32 i = 0; i < static_cast<Uint32>(InputKeys::TotalKeys); ++i)
        {
            auto& KeyState = m_Keys[i];
            if (KeyState & INPUT_KEY_STATE_FLAG_KEY_WAS_DOWN)
            {
                KeyState &= ~INPUT_KEY_STATE_FLAG_KEY_WAS_DOWN;
            }
        }
    }

protected:
    MouseState            m_MouseState;
    INPUT_KEY_STATE_FLAGS m_Keys[static_cast<size_t>(InputKeys::TotalKeys)] = {};
};

} // namespace Diligent

// clang-format off
#if PLATFORM_WIN32
    #include "Win32/InputControllerWin32.h"
    namespace Diligent
    {
        using InputController = InputControllerWin32;
    }
#elif PLATFORM_UNIVERSAL_WINDOWS
    #include "UWP/InputControllerUWP.h"
    namespace Diligent
    {
        using InputController = InputControllerUWP;
    }
#elif PLATFORM_MACOS
    #include "MacOS/InputControllerMacOS.h"
    namespace Diligent
    {
        using InputController = InputControllerMacOS;
    }
#elif PLATFORM_IOS
    #include "iOS/InputControllerIOS.h"
    namespace Diligent
    {
        using InputController = InputControllerIOS;
    }
#elif PLATFORM_LINUX
    #include "Linux/InputControllerLinux.h"
    namespace Diligent
    {
        using InputController = InputControllerLinux;
    }
#elif PLATFORM_ANDROID
    #include "Android/InputControllerAndroid.h"
    namespace Diligent
    {
        using InputController = InputControllerAndroid;
    }
#else
    namespace Diligent
    {
        class DummyInputController
        {
        public:
            const MouseState& GetMouseState()const{return m_MouseState;}

            INPUT_KEY_STATE_FLAGS GetKeyState(InputKeys Key)const{return INPUT_KEY_STATE_FLAG_KEY_NONE;}

        private:
            MouseState m_MouseState;
        };
        using InputController = DummyInputController;
    }
#endif
// clang-format on