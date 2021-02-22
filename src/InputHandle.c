#include <windows.h>
#include "View.h"

void HandleArrows(str_view* view, WPARAM wParam) {
    switch (wParam) {
        case VK_UP:
            SendMessage(view->hwnd, WM_VSCROLL, SB_LINEUP, (LPARAM)NULL);
            break;
        case VK_RIGHT:
            SendMessage(view->hwnd, WM_HSCROLL, SB_LINEDOWN, (LPARAM)NULL);
            break;
        case VK_DOWN:
            SendMessage(view->hwnd, WM_VSCROLL, SB_LINEDOWN, (LPARAM)NULL);
            break;
        case VK_LEFT:
            SendMessage(view->hwnd, WM_HSCROLL, SB_LINEUP, (LPARAM)NULL);
            break;
        case VK_PRIOR:
            SendMessage(view->hwnd, WM_VSCROLL, SB_PAGEUP, (LPARAM)NULL);
            break;
        case VK_NEXT:
            SendMessage(view->hwnd, WM_VSCROLL, SB_PAGEDOWN, (LPARAM)NULL);
            break;
    }
}

void HandleKeys(str_view* view, WPARAM wParam) {
    switch (wParam) {
        case 'w':
            SendMessage(view->hwnd, WM_VSCROLL, SB_LINEUP, (LPARAM)NULL);
            break;
        case 'd':
            SendMessage(view->hwnd, WM_HSCROLL, SB_LINEDOWN, (LPARAM)NULL);
            break;
        case 's':
            SendMessage(view->hwnd, WM_VSCROLL, SB_LINEDOWN, (LPARAM)NULL);
            break;
        case 'a':
            SendMessage(view->hwnd, WM_HSCROLL, SB_LINEUP, (LPARAM)NULL);
            break;
    }
}
