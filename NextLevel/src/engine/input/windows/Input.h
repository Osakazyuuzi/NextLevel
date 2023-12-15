#ifndef ___SRC_ENGINE_INPUT_WINDOWS_INPUT_H___
#define ___SRC_ENGINE_INPUT_WINDOWS_INPUT_H___

#include <Windows.h>
#undef max
#undef min

HRESULT InitInput();
void UninitInput();
void UpdateInput();

bool IsKeyPress(BYTE key);
bool IsKeyTrigger(BYTE key);
bool IsKeyRelease(BYTE key);
bool IsKeyRepeat(BYTE key);

#endif //!___SRC_ENGINE_INPUT_WINDOWS_INPUT_H___
