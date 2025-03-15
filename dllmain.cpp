// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "hook.h"
#include <Windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>

#pragma comment(lib, "comdlg32.lib")  // For file dialog functions
#pragma comment(lib, "shell32.lib")   // For ShellExecute

// Original MessageBoxA function type
typedef int (WINAPI* MessageBoxA_t)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

// Pointer to the original function
MessageBoxA_t Original_MessageBoxA = NULL;



//=====================================================================
// 1. VISUAL/UI HOOKS
//=====================================================================

// SetWindowText hook
typedef BOOL(WINAPI* SetWindowTextA_t)(HWND hWnd, LPCSTR lpString);
SetWindowTextA_t Original_SetWindowTextA = NULL;

BOOL WINAPI Detour_SetWindowTextA(HWND hWnd, LPCSTR lpString) {
    OutputDebugStringA("SetWindowTextA hook called!");

    if (!lpString) {
        return Original_SetWindowTextA(hWnd, "[HOOKED] NULL");
    }

    // Create modified string with prefix
    char newText[512];
    sprintf_s(newText, "[HOOKED] %s", lpString);

    // Call original with modified text
    return Original_SetWindowTextA(hWnd, newText);
}

// SetCursor hook
typedef HCURSOR(WINAPI* SetCursor_t)(HCURSOR hCursor);
SetCursor_t Original_SetCursor = NULL;

HCURSOR WINAPI Detour_SetCursor(HCURSOR hCursor) {
    OutputDebugStringA("SetCursor hook called!");

    // Change all cursors to an hourglass
    static HCURSOR hourglassCursor = NULL;
    if (!hourglassCursor) {
        hourglassCursor = LoadCursor(NULL, IDC_WAIT);
    }

    return Original_SetCursor(hourglassCursor);
}

// TextOutA hook
typedef BOOL(WINAPI* TextOutA_t)(HDC hdc, int x, int y, LPCSTR lpString, int c);
TextOutA_t Original_TextOutA = NULL;

BOOL WINAPI Detour_TextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c) {
    OutputDebugStringA("TextOutA hook called!");

    // Save current text color
    COLORREF oldColor = SetTextColor(hdc, RGB(255, 0, 0)); // Red text

    // Call original function
    BOOL result = Original_TextOutA(hdc, x, y, lpString, c);

    // Restore text color
    SetTextColor(hdc, oldColor);

    return result;
}

BOOL SetupVisualHooks() {
    BOOL result = TRUE;
    LPVOID pOriginal = NULL;
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    HMODULE hGdi32 = GetModuleHandleA("gdi32.dll");

    if (!hUser32 || !hGdi32) {
        OutputDebugStringA("Failed to get module handles");
        return FALSE;
    }

    // SetWindowTextA hook
    PVOID pSetWindowTextA = GetProcAddress(hUser32, "SetWindowTextA");
    if (pSetWindowTextA) {
        if (HookCreateHook(pSetWindowTextA, Detour_SetWindowTextA, &pOriginal)) {
            Original_SetWindowTextA = (SetWindowTextA_t)pOriginal;
            OutputDebugStringA("SetWindowTextA hook installed");
        }
        else {
            OutputDebugStringA("Failed to install SetWindowTextA hook");
            result = FALSE;
        }
    }

    // SetCursor hook
    PVOID pSetCursor = GetProcAddress(hUser32, "SetCursor");
    if (pSetCursor) {
        pOriginal = NULL;
        if (HookCreateHook(pSetCursor, Detour_SetCursor, &pOriginal)) {
            Original_SetCursor = (SetCursor_t)pOriginal;
            OutputDebugStringA("SetCursor hook installed");
        }
        else {
            OutputDebugStringA("Failed to install SetCursor hook");
            result = FALSE;
        }
    }

    // TextOutA hook
    PVOID pTextOutA = GetProcAddress(hGdi32, "TextOutA");
    if (pTextOutA) {
        pOriginal = NULL;
        if (HookCreateHook(pTextOutA, Detour_TextOutA, &pOriginal)) {
            Original_TextOutA = (TextOutA_t)pOriginal;
            OutputDebugStringA("TextOutA hook installed");
        }
        else {
            OutputDebugStringA("Failed to install TextOutA hook");
            result = FALSE;
        }
    }

    return result;
}

//=====================================================================
// 2. SOUND HOOKS
//=====================================================================

// PlaySound hook
typedef BOOL(WINAPI* PlaySoundA_t)(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound);
PlaySoundA_t Original_PlaySoundA = NULL;

BOOL WINAPI Detour_PlaySoundA(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound) {
    OutputDebugStringA("PlaySoundA hook called!");

    // Play a beep first
    MessageBeep(MB_ICONEXCLAMATION);

    // Let the original sound also play
    return Original_PlaySoundA(pszSound, hmod, fdwSound);
}

// Beep hook
typedef BOOL(WINAPI* Beep_t)(DWORD dwFreq, DWORD dwDuration);
Beep_t Original_Beep = NULL;

BOOL WINAPI Detour_Beep(DWORD dwFreq, DWORD dwDuration) {
    OutputDebugStringA("Beep hook called!");

    char dbgMsg[100];
    sprintf_s(dbgMsg, "Original: Freq=%d, Duration=%d", dwFreq, dwDuration);
    OutputDebugStringA(dbgMsg);

    // Double frequency but halve duration
    return Original_Beep(dwFreq * 2, dwDuration / 2);
}

BOOL SetupSoundHooks() {
    BOOL result = TRUE;
    LPVOID pOriginal = NULL;
    HMODULE hWinmm = LoadLibraryA("winmm.dll");
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");

    if (!hWinmm || !hKernel32) {
        OutputDebugStringA("Failed to get module handles");
        return FALSE;
    }

    // PlaySoundA hook
    PVOID pPlaySoundA = GetProcAddress(hWinmm, "PlaySoundA");
    if (pPlaySoundA) {
        if (HookCreateHook(pPlaySoundA, Detour_PlaySoundA, &pOriginal)) {
            Original_PlaySoundA = (PlaySoundA_t)pOriginal;
            OutputDebugStringA("PlaySoundA hook installed");
        }
        else {
            OutputDebugStringA("Failed to install PlaySoundA hook");
            result = FALSE;
        }
    }

    // Beep hook
    PVOID pBeep = GetProcAddress(hKernel32, "Beep");
    if (pBeep) {
        pOriginal = NULL;
        if (HookCreateHook(pBeep, Detour_Beep, &pOriginal)) {
            Original_Beep = (Beep_t)pOriginal;
            OutputDebugStringA("Beep hook installed");
        }
        else {
            OutputDebugStringA("Failed to install Beep hook");
            result = FALSE;
        }
    }

    return result;
}

//=====================================================================
// 3. DIALOG AND FILE HOOKS
//=====================================================================

// GetOpenFileName hook
typedef BOOL(WINAPI* GetOpenFileNameA_t)(LPOPENFILENAMEA lpofn);
GetOpenFileNameA_t Original_GetOpenFileNameA = NULL;

BOOL WINAPI Detour_GetOpenFileNameA(LPOPENFILENAMEA lpofn) {
    OutputDebugStringA("GetOpenFileNameA hook called!");

    // Modify the dialog title
    lpofn->lpstrTitle = "HOOKED Open File Dialog";

    // Call original
    return Original_GetOpenFileNameA(lpofn);
}

// ShellExecute hook
typedef HINSTANCE(WINAPI* ShellExecuteA_t)(HWND hwnd, LPCSTR lpOperation,
    LPCSTR lpFile, LPCSTR lpParameters,
    LPCSTR lpDirectory, INT nShowCmd);
ShellExecuteA_t Original_ShellExecuteA = NULL;

HINSTANCE WINAPI Detour_ShellExecuteA(HWND hwnd, LPCSTR lpOperation,
    LPCSTR lpFile, LPCSTR lpParameters,
    LPCSTR lpDirectory, INT nShowCmd) {
    OutputDebugStringA("ShellExecuteA hook called!");

    // Show a message before executing
    char message[512];
    sprintf_s(message, "About to execute: %s\nOperation: %s",
        lpFile ? lpFile : "NULL",
        lpOperation ? lpOperation : "NULL");

    MessageBoxA(hwnd, message, "HOOKED ShellExecute", MB_OK | MB_ICONINFORMATION);

    // Call original
    return Original_ShellExecuteA(hwnd, lpOperation, lpFile,
        lpParameters, lpDirectory, nShowCmd);
}

BOOL SetupDialogHooks() {
    BOOL result = TRUE;
    LPVOID pOriginal = NULL;
    HMODULE hComdlg32 = LoadLibraryA("comdlg32.dll");
    HMODULE hShell32 = LoadLibraryA("shell32.dll");

    if (!hComdlg32 || !hShell32) {
        OutputDebugStringA("Failed to get module handles");
        return FALSE;
    }

    // GetOpenFileNameA hook
    PVOID pGetOpenFileNameA = GetProcAddress(hComdlg32, "GetOpenFileNameA");
    if (pGetOpenFileNameA) {
        if (HookCreateHook(pGetOpenFileNameA, Detour_GetOpenFileNameA, &pOriginal)) {
            Original_GetOpenFileNameA = (GetOpenFileNameA_t)pOriginal;
            OutputDebugStringA("GetOpenFileNameA hook installed");
        }
        else {
            OutputDebugStringA("Failed to install GetOpenFileNameA hook");
            result = FALSE;
        }
    }

    // ShellExecuteA hook
    PVOID pShellExecuteA = GetProcAddress(hShell32, "ShellExecuteA");
    if (pShellExecuteA) {
        pOriginal = NULL;
        if (HookCreateHook(pShellExecuteA, Detour_ShellExecuteA, &pOriginal)) {
            Original_ShellExecuteA = (ShellExecuteA_t)pOriginal;
            OutputDebugStringA("ShellExecuteA hook installed");
        }
        else {
            OutputDebugStringA("Failed to install ShellExecuteA hook");
            result = FALSE;
        }
    }

    return result;
}

//=====================================================================
// 4. INPUT HANDLING HOOKS
//=====================================================================

// GetAsyncKeyState hook
typedef SHORT(WINAPI* GetAsyncKeyState_t)(int vKey);
GetAsyncKeyState_t Original_GetAsyncKeyState = NULL;

SHORT WINAPI Detour_GetAsyncKeyState(int vKey) {
    // Only log for letter keys to avoid flooding
    if (vKey >= 'A' && vKey <= 'Z') {
        char dbgMsg[100];
        sprintf_s(dbgMsg, "GetAsyncKeyState hook called for key: %c", vKey);
        OutputDebugStringA(dbgMsg);
    }

    // Invert key state for letter A (make it appear pressed when not, and vice versa)
    if (vKey == 'A') {
        SHORT original = Original_GetAsyncKeyState(vKey);
        return ~original;
    }

    return Original_GetAsyncKeyState(vKey);
}

// SetCursorPos hook
typedef BOOL(WINAPI* SetCursorPos_t)(int X, int Y);
SetCursorPos_t Original_SetCursorPos = NULL;

BOOL WINAPI Detour_SetCursorPos(int X, int Y) {
    char dbgMsg[100];
    sprintf_s(dbgMsg, "SetCursorPos hook called: X=%d, Y=%d", X, Y);
    OutputDebugStringA(dbgMsg);

    // Offset cursor position by 20 pixels
    return Original_SetCursorPos(X + 20, Y + 20);
}

BOOL SetupInputHooks() {
    BOOL result = TRUE;
    LPVOID pOriginal = NULL;
    HMODULE hUser32 = GetModuleHandleA("user32.dll");

    if (!hUser32) {
        OutputDebugStringA("Failed to get User32 module handle");
        return FALSE;
    }

    // GetAsyncKeyState hook
    PVOID pGetAsyncKeyState = GetProcAddress(hUser32, "GetAsyncKeyState");
    if (pGetAsyncKeyState) {
        if (HookCreateHook(pGetAsyncKeyState, Detour_GetAsyncKeyState, &pOriginal)) {
            Original_GetAsyncKeyState = (GetAsyncKeyState_t)pOriginal;
            OutputDebugStringA("GetAsyncKeyState hook installed");
        }
        else {
            OutputDebugStringA("Failed to install GetAsyncKeyState hook");
            result = FALSE;
        }
    }

    // SetCursorPos hook
    PVOID pSetCursorPos = GetProcAddress(hUser32, "SetCursorPos");
    if (pSetCursorPos) {
        pOriginal = NULL;
        if (HookCreateHook(pSetCursorPos, Detour_SetCursorPos, &pOriginal)) {
            Original_SetCursorPos = (SetCursorPos_t)pOriginal;
            OutputDebugStringA("SetCursorPos hook installed");
        }
        else {
            OutputDebugStringA("Failed to install SetCursorPos hook");
            result = FALSE;
        }
    }

    return result;
}

//=====================================================================
// MAIN HOOK SETUP FUNCTIONS
//=====================================================================

// This function can be called from DllMain to install all hooks
BOOL SetupAllHooks() {
    BOOL success = TRUE;

    // Install visual hooks
    if (!SetupVisualHooks()) {
        OutputDebugStringA("Not all visual hooks were installed successfully");
        success = FALSE;
    }

    // Install sound hooks
    if (!SetupSoundHooks()) {
        OutputDebugStringA("Not all sound hooks were installed successfully");
        success = FALSE;
    }

    // Install dialog and file hooks
    if (!SetupDialogHooks()) {
        OutputDebugStringA("Not all dialog hooks were installed successfully");
        success = FALSE;
    }

    // Install input hooks
    if (!SetupInputHooks()) {
        OutputDebugStringA("Not all input hooks were installed successfully");
        success = FALSE;
    }

    return success;
}


int WINAPI Detour_MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {

    char dbgMsg[100];
    sprintf_s(dbgMsg, "Detour called! Original at: 0x%p", Original_MessageBoxA);
    OutputDebugStringA(dbgMsg);

    // Test if original is valid
    if (Original_MessageBoxA == NULL) {
        OutputDebugStringA("ERROR: Original_MessageBoxA is NULL!");
        return 0;
    }

    // Modify the text and caption
    const char* newText = "This message was intercepted by our hook!";
    const char* newCaption = "Hooked MessageBox";

    // Call the original function with our modified parameters
    return Original_MessageBoxA(hWnd, newText, newCaption, uType);
}

BOOL SetupMessageBoxHook() {
    // Get the address of MessageBoxA
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    if (!hUser32) {
        hUser32 = LoadLibraryA("user32.dll");
        if (!hUser32) {
            OutputDebugStringA("Failed to load user32.dll");
            return FALSE;
        }
    }

    PVOID pMessageBoxA = GetProcAddress(hUser32, "MessageBoxA");
    if (!pMessageBoxA) {
        OutputDebugStringA("Failed to get MessageBoxA address");
        return FALSE;
    }

    // Fix: Use a temporary variable for the output parameter
    LPVOID pOriginal = NULL;
    BOOL result = HookCreateHook(pMessageBoxA, Detour_MessageBoxA, &pOriginal);
    if (result && pOriginal) {
        Original_MessageBoxA = (MessageBoxA_t)pOriginal;

        char dbgMsg[100];
        sprintf_s(dbgMsg, "Original function at: 0x%p", Original_MessageBoxA);
        OutputDebugStringA(dbgMsg);
    }
    if (!result || !pOriginal) {
        OutputDebugStringA("Failed to install MessageBoxA hook");
        return FALSE;
    }

    // Now assign the original function pointer
    Original_MessageBoxA = (MessageBoxA_t)pOriginal;

    char dbgMsg[100];
    sprintf_s(dbgMsg, "Hook installed. Original function at 0x%p", Original_MessageBoxA);
    OutputDebugStringA(dbgMsg);

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Install hook when DLL is loaded
        SetupAllHooks();
        break; // Add this break statement
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

