#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include "Model.h"
#include "View.h"
#include "Scroll.h"
#include "InputHandle.h"
#include "menu.h"

// Declare Windows procedure
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//  Make the class name into a global variable
TCHAR szClassName[ ] = _T("WinAPIApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow) {
    HWND hwnd;               // Window handler
    MSG messages;            // Storage for messages
    WNDCLASSEX wincl;        // window class structure

    // The Window structure
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      // This function is called by windows
    wincl.style = CS_DBLCLKS | CS_CLASSDC;                 // Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

    // Use default icon and mouse-pointer
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "menu";                 // No menu
    wincl.cbClsExtra = 0;                      // No extra bytes after the window class
    wincl.cbWndExtra = 0;                      // structure or the window instance
    // Use Windows's default colour as the background of the window
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx (&wincl))
        return 0;

    // Creating app window
    hwnd = CreateWindowEx (
               0,                   // Extended possibilites for variation
               szClassName,         // Classname
               _T("Text Viewer"),   // Title Text
               WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, // default window
               CW_USEDEFAULT,       // Windows decides the position
               CW_USEDEFAULT,       // where the window ends up on the screen
               800,                // The programs width
               600,                 // and height in pixels
               HWND_DESKTOP,        // The window is a child-window to desktop
               NULL,                // No menu
               hThisInstance,       // Program Instance handler
               lpszArgument         // No Window Creation data
           );

    // Make the window visible on the screen
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop. It will run until GetMessage() returns 0
    while (GetMessage (&messages, NULL, 0, 0)) {
        // Translate virtual-key messages into character messages
        TranslateMessage(&messages);
        // Send message to WindowProcedure
        DispatchMessage(&messages);
    }

    // The program return-value is 0 - The value that PostQuitMessage() gave
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Declare local variables
    CREATESTRUCT* st;
    static unsigned wm_size_second_call = 0;

    // Model used to store text as single string
    static str_model* model;
    // View that represents str_model instance on the screen
    static str_view* view;
    // Handle to app menu
    HMENU hMenu;

    int thumbPos;
    double dividedPos;

    switch (message) {                // handle the messages
        // Window creation
        case WM_CREATE:
            // Make lParam a readable structure
            st = (CREATESTRUCT*)lParam;

            // Create a model and view from text file given in arguments
            model = InitModel();
            AssignFileToModel(&model->str, &model->len, (char*)st->lpCreateParams);
            view = InitView(model, hwnd);

            // Disable menu close button on startup if no file is loaded
            hMenu = GetMenu(view->hwnd);
            if (model->str == NULL)
                EnableMenuItem(hMenu, IDM_CLOSE, MF_GRAYED);

            break;

        case WM_PAINT:
            // Show view on window it's attached to
            ShowView(view);
            break;

        case WM_SIZE:
            // Refresh metrics stored in view related to screen size
            RefillMetrics(view);

            // If layout mode is enabled, refresh array of linebreaks on each resizing
            // SetScrollRange() from scrolls resizing functions sends additional WM_SIZE messages
            // To prevent FillLineBreaks() from recounting every time, flag is used
            if (view->viewMode == layout && wm_size_second_call == 0) {
                FillLineBreaks(view);
                FixVertScroll(view);
                wm_size_second_call = 1;
            }

            // Change scrolls' ranges and positions with new window size
            ResizeVertScroll(view);
            ResizeHorzScroll(view);

            // Redraw window
            InvalidateRect(view->hwnd, NULL, TRUE);

            // Drop flag value
            wm_size_second_call = 0;
            break;

        case WM_COMMAND:
            // Get a handle to app's menu
            hMenu = GetMenu(view->hwnd);

            switch(LOWORD(wParam)) {
                case IDM_OPEN:
                    // Show 'open file' dialog menu
                    // If it successes, empty current model, assign new file to it, and connect model with current view
                    if (GetOpenFileNameA(&view->ofn)) {
                        EmptyModel(model);
                        AssignFileToModel(&model->str, &model->len, view->ofn.lpstrFile);
                        AssignModelToView(view, model);
                        EnableMenuItem(hMenu, IDM_CLOSE, MF_ENABLED);
                    }
                    SendMessage(view->hwnd, WM_SIZE, 0, (LPARAM)NULL);
                    break;

                case IDM_CLOSE:
                    // On file close empty both model and view parameters
                    // It still holds viewMode parameter, so new file will open in the same mode as the old one
                    EmptyModel(model);
                    EmptyView(view);
                    EnableMenuItem(hMenu, IDM_CLOSE, MF_GRAYED);
                    SendMessage(view->hwnd, WM_SIZE, 0, (LPARAM)NULL);
                    break;

                case IDM_QUIT:
                    // Simply send WM_DESTROY message to quit application
                    SendMessage(view->hwnd, WM_DESTROY, 0, (LPARAM)NULL);
                    break;

                case IDM_LAYOUT:
                    // Switch layout checkbox and layout on/off mode
                    if (view->viewMode == layout) {
                        CheckMenuItem(hMenu, IDM_LAYOUT, MF_UNCHECKED);
                        view->viewMode = noLayout;
                        FillLineBreaks(view);
                        FixVertScroll(view);
                    }
                    else {
                        CheckMenuItem(hMenu, IDM_LAYOUT, MF_CHECKED);
                        view->viewMode = layout;
                    }
                    SendMessage(view->hwnd, WM_SIZE, 0, (LPARAM)NULL);
                    // Force set position to prevent bugs that may appear with wrong scroll positioning
                    thumbPos = view->vertScrollPos;
                    if (view->vertScrollMaxPos > view->thumbMaxRange)  {
			dividedPos = (double)thumbPos / (double)view->vertScrollMaxPos;
			thumbPos = (int)(dividedPos *  (double)view->thumbMaxRange);
		    }
		    SetScrollPos(view->hwnd, SB_VERT, thumbPos, TRUE);
                    break;
            }

            break;

        case WM_VSCROLL:
            // Scroll vertically
            VertScroll(view, wParam);
            break;

        case WM_HSCROLL:
            // Scroll horizontally
            HorzScroll(view, wParam);
            break;

        case WM_KEYDOWN:
            // Handle keyboard arrows input
            HandleArrows(view, wParam);
            break;

        case WM_CHAR:
            // Handle keyboard characters input
            HandleKeys(view, wParam);
            break;

        // Free memory and send quit message to WinMain
        case WM_DESTROY:
            DestroyModel(model);
            DestroyView(view);
            PostQuitMessage(0);       // send a WM_QUIT to the message queue
            break;

        default:                      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
