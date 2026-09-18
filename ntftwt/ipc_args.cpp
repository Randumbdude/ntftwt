#include "pch.h"
#include <Windows.h>
#include "ipc_args.h"
#include "sha1.h"

#define ID_CANCEL 1001

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // Message
        CreateWindowW(
            L"STATIC",
            L"The promised future never happened.\nPress OK to continue.",
            WS_VISIBLE | WS_CHILD,
            20, 20, 300, 50,
            hwnd,
            NULL,
            NULL,
            NULL
        );

        // Cancel button
        CreateWindowW(
            L"BUTTON",
            L"Cancel",
            WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            120, 85, 100, 30,
            hwnd,
            (HMENU)ID_CANCEL,
            NULL,
            NULL
        );

        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_CANCEL)
        {
            DestroyWindow(hwnd);
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}


void ShowDialog()
{
    HINSTANCE hInstance = GetModuleHandleW(NULL);

    // Register window class
    WNDCLASSW wc = {};
    wc.lpfnWndProc = DialogProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"error";
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    // Create window
    HWND hwnd = CreateWindowExW(
        WS_EX_DLGMODALFRAME,
        L"error",
        L"Command Failed",
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        340, 160,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

int32_t ipoc_arg(const char* cmd) {
	if (strcmp(cmd, "f931d1290e11f230a684c06ba04b9bc7938e7b02") == 0) {
        ShowDialog();
		return 0;
	}
}