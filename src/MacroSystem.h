#pragma once
#include <Windows.h>
#include <thread>
#include <atomic>

class MacroSystem {
private:
    std::atomic<bool> isQuickPeekActive = false;
    std::atomic<bool> isDiffuseActive = false;
    std::atomic<bool> shouldStopThread = false;
    std::atomic<bool> isTriggerActive = false;
    std::thread macroThread;
    std::thread diffuseThread;
    std::thread triggerThread;
    std::atomic<bool> isCapturingKey = false;
    int* captureTarget = nullptr;

    void SimulateKeyPress(WORD key);
    void SimulateKeyRelease(WORD key);
    void QuickPeekLoop(int type, float speed);
    void NormalQuickPeek(float speed);
    void ShaikoQuickPeek(float speed);
    void FastQuickPeek(float speed);
    void DiffuseLoop();
    void TriggerLoop(float threshold, float delay, float fovSize);
    void SimulateMouseClick();

public:
    MacroSystem();
    ~MacroSystem();
    
    void StartQuickPeek(int type, float speed);
    void StopQuickPeek();
    void StartDiffuse();
    void StopDiffuse();
    void StartTrigger(float threshold, float delay, float fovSize);
    void StopTrigger();
    bool IsQuickPeekActive() const { return isQuickPeekActive; }
    bool IsDiffuseActive() const { return isDiffuseActive; }
    bool IsTriggerActive() const { return isTriggerActive; }
    
    void StartKeyCapture(int* target);
    void StopKeyCapture();
    bool IsCapturingKey() const { return isCapturingKey; }
    int* GetCaptureTarget() const { return captureTarget; }
    void ProcessKeyPress(WPARAM key);
    void ProcessKeyRelease(WPARAM key);
}; 