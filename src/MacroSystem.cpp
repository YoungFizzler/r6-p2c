#include "MacroSystem.h"
#include <Windows.h>

MacroSystem::MacroSystem() {}

MacroSystem::~MacroSystem() {
    shouldStopThread = true;
    if (macroThread.joinable()) {
        macroThread.join();
    }
    if (diffuseThread.joinable()) {
        diffuseThread.join();
    }
    if (triggerThread.joinable()) {
        triggerThread.join();
    }
}

void MacroSystem::SimulateKeyPress(WORD key) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
}

void MacroSystem::SimulateKeyRelease(WORD key) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void MacroSystem::NormalQuickPeek(float speed) {
    SimulateKeyPress('Q');
    Sleep(static_cast<DWORD>(10 / speed));
    SimulateKeyRelease('Q');
    Sleep(static_cast<DWORD>(10 / speed));
    SimulateKeyPress('E');
    Sleep(static_cast<DWORD>(10 / speed));
    SimulateKeyRelease('E');
    Sleep(static_cast<DWORD>(50 / speed));
}

void MacroSystem::ShaikoQuickPeek(float speed) {
    SimulateKeyPress('Q');
    Sleep(static_cast<DWORD>(5 / speed));
    SimulateKeyRelease('Q');
    Sleep(static_cast<DWORD>(5 / speed));
    SimulateKeyPress('E');
    Sleep(static_cast<DWORD>(5 / speed));
    SimulateKeyRelease('E');
    Sleep(static_cast<DWORD>(25 / speed));
}

void MacroSystem::FastQuickPeek(float speed) {
    SimulateKeyPress('Q');
    Sleep(static_cast<DWORD>(2 / speed));
    SimulateKeyRelease('Q');
    SimulateKeyPress('E');
    Sleep(static_cast<DWORD>(2 / speed));
    SimulateKeyRelease('E');
    Sleep(static_cast<DWORD>(15 / speed));
}

void MacroSystem::QuickPeekLoop(int type, float speed) {
    while (!shouldStopThread && isQuickPeekActive) {
        switch (type) {
            case 0:
                NormalQuickPeek(speed);
                break;
            case 1:
                ShaikoQuickPeek(speed);
                break;
            case 2:
                FastQuickPeek(speed);
                break;
        }
    }
}

void MacroSystem::StartQuickPeek(int type, float speed) {
    if (!isQuickPeekActive) {
        isQuickPeekActive = true;
        shouldStopThread = false;
        if (macroThread.joinable()) {
            macroThread.join();
        }
        macroThread = std::thread(&MacroSystem::QuickPeekLoop, this, type, speed);
    }
}

void MacroSystem::StopQuickPeek() {
    isQuickPeekActive = false;
}

void MacroSystem::DiffuseLoop() {
    isDiffuseActive = true;
    SimulateKeyPress('F');
    
    // Hold for 7 seconds
    for (int i = 0; i < 70 && isDiffuseActive; i++) {
        Sleep(100);  // Check every 100ms if we should stop
    }
    
    SimulateKeyRelease('F');
    isDiffuseActive = false;
}

void MacroSystem::StartDiffuse() {
    if (!isDiffuseActive) {
        if (diffuseThread.joinable()) {
            diffuseThread.join();
        }
        diffuseThread = std::thread(&MacroSystem::DiffuseLoop, this);
    }
}

void MacroSystem::StopDiffuse() {
    isDiffuseActive = false;
}

void MacroSystem::StartKeyCapture(int* target) {
    if (isCapturingKey) {
        StopKeyCapture();
    }
    captureTarget = target;
    isCapturingKey = true;
}

void MacroSystem::StopKeyCapture() {
    isCapturingKey = false;
    captureTarget = nullptr;
}

void MacroSystem::ProcessKeyPress(WPARAM key) {
    if (!isCapturingKey || !captureTarget) return;
    
    // Handle ESC key first - always cancels key capture
    if (key == VK_ESCAPE) {
        *captureTarget = 0;  // Clear the key binding
        StopKeyCapture();
        return;
    }
    
    // Convert common keys to our internal representation
    int mappedKey = 0;
    
    // Handle mouse buttons
    if (key == VK_LBUTTON) mappedKey = VK_LBUTTON;  // Left mouse
    else if (key == VK_RBUTTON) mappedKey = VK_RBUTTON;  // Right mouse
    else if (key == VK_MBUTTON) mappedKey = VK_MBUTTON;  // Middle mouse
    else if (key == VK_XBUTTON1) mappedKey = VK_XBUTTON1;  // Mouse4
    else if (key == VK_XBUTTON2) mappedKey = VK_XBUTTON2;  // Mouse5
    // Handle keyboard keys
    else if ((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9')) mappedKey = static_cast<int>(key);
    else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU) mappedKey = VK_MENU;  // Alt
    else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT) mappedKey = VK_SHIFT;
    else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL) mappedKey = VK_CONTROL;
    else if (key == VK_TAB) mappedKey = VK_TAB;
    else if (key == VK_SPACE) mappedKey = VK_SPACE;
    
    if (mappedKey) {
        *captureTarget = mappedKey;
        StopKeyCapture();
    }
}

void MacroSystem::ProcessKeyRelease(WPARAM /*key*/) {
    // No need to handle key release for capture
    // The key is captured on press
}

void MacroSystem::SimulateMouseClick() {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    Sleep(10);
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void MacroSystem::TriggerLoop(float threshold, float delay, float fovSize) {
    isTriggerActive = true;
    
    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // Calculate FOV region
    int fovWidth = static_cast<int>(fovSize);
    int fovHeight = static_cast<int>(fovSize);
    int fovX = (screenWidth - fovWidth) / 2;
    int fovY = (screenHeight - fovHeight) / 2;
    
    HDC hdc = GetDC(NULL);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, fovWidth, fovHeight);
    SelectObject(memDC, hBitmap);
    
    while (!shouldStopThread && isTriggerActive) {
        // Capture screen region
        BitBlt(memDC, 0, 0, fovWidth, fovHeight, hdc, fovX, fovY, SRCCOPY);
        
        // Check center pixel color
        COLORREF centerColor = GetPixel(memDC, fovWidth/2, fovHeight/2);
        int r = GetRValue(centerColor);
        int g = GetGValue(centerColor);
        int b = GetBValue(centerColor);
        
        // Simple color threshold check (can be improved)
        float colorIntensity = (r + g + b) / 3.0f;
        if (colorIntensity > threshold) {
            SimulateMouseClick();
            Sleep(static_cast<DWORD>(delay));
        }
        
        Sleep(1); // Prevent high CPU usage
    }
    
    DeleteObject(hBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, hdc);
    
    isTriggerActive = false;
}

void MacroSystem::StartTrigger(float threshold, float delay, float fovSize) {
    if (!isTriggerActive) {
        shouldStopThread = false;
        if (triggerThread.joinable()) {
            triggerThread.join();
        }
        triggerThread = std::thread(&MacroSystem::TriggerLoop, this, threshold, delay, fovSize);
    }
}

void MacroSystem::StopTrigger() {
    isTriggerActive = false;
} 