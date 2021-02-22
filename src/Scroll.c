#include "Model.h"
#include "View.h"

void VertScroll(str_view* view, WPARAM wParam) {
    int move = 0;
    int thumbPos = 0;
    double dividedPos = 0.0;

    switch (LOWORD(wParam)) {
        case SB_LINEUP:
            // Check if scroll is not going over document top side
            if ((int)view->vertScrollPos - (int)view->vertScrollDist < 0)
                move = -view->vertScrollPos;
            else
                move = -view->vertScrollDist;
            break;

        case SB_LINEDOWN:
            // Check if scroll is not going over document bottom side
            if (view->vertScrollPos + view->vertScrollDist > view->vertScrollMaxPos)
                move = view->vertScrollMaxPos - view->vertScrollPos;
            else
                move = view->vertScrollDist;
            break;

        case SB_PAGEUP:
            // Check if scroll is not going over document top side
            if ((int)view->vertScrollPos - (int)view->visibleLines < 0)
                move = -view->vertScrollPos;
            else
                move = -max(1, view->visibleLines);
            break;

        case SB_PAGEDOWN:
            // Check if scroll is not going over document bottom side
            if (view->vertScrollPos + view->visibleLines > view->vertScrollMaxPos)
                move = view->vertScrollMaxPos - view->vertScrollPos;
            else
                move = max(1, view->visibleLines);
            break;

        case SB_THUMBTRACK:
        //case SB_THUMBPOSITION:
            thumbPos = HIWORD(wParam);
            if (view->vertScrollMaxPos > view->thumbMaxRange) {
                dividedPos = (double)thumbPos / (double)view->thumbMaxRange;
                thumbPos = (int)(dividedPos * (double)view->vertScrollMaxPos);
            }
            move = thumbPos - view->vertScrollPos;
            break;

        default:
            move = 0;
            break;
    }

    if (move != 0) {
        view->vertScrollPos += move;
        thumbPos = view->vertScrollPos;

        ScrollWindow(view->hwnd, 0, -view->charHeight * move, NULL, NULL);

        if (view->vertScrollMaxPos > view->thumbMaxRange)  {
            dividedPos = (double)thumbPos / (double)view->vertScrollMaxPos;
            thumbPos = (int)(dividedPos *  (double)view->thumbMaxRange);
        }
        SetScrollPos(view->hwnd, SB_VERT, thumbPos, TRUE);

        InvalidateRect(view->hwnd, NULL, TRUE);
        UpdateWindow(view->hwnd);
    }
}

void HorzScroll(str_view* view, WPARAM wParam) {
    int move = 0;
    int thumbPos = 0;
    double dividedPos = 0.0;

    switch (LOWORD(wParam)) {
        case SB_LINEUP:
            // Check if scroll is not going over document top side
            if ((int)view->horzScrollPos - (int)view->horzScrollDist < 0)
                move = -view->horzScrollPos;
            else
                move = -view->horzScrollDist;
            break;

        case SB_LINEDOWN:
            // Check if scroll is not going over document bottom side
            if (view->horzScrollPos + view->horzScrollDist > view->horzScrollMaxPos)
                move = view->horzScrollMaxPos - view->horzScrollPos;
            else
                move = view->horzScrollDist;
            break;

        case SB_PAGEUP:
            // Check if scroll is not going over document top side
            if ((int)view->horzScrollPos - (int)view->charsInLine < 0)
                move = -view->horzScrollPos;
            else
                move = -max(1, view->charsInLine);
            break;

        case SB_PAGEDOWN:
            // Check if scroll is not going over document bottom side
            if (view->horzScrollPos + view->charsInLine > view->horzScrollMaxPos)
                move = view->horzScrollMaxPos - view->horzScrollPos;
            else
                move = max(1, view->charsInLine);
            break;

        case SB_THUMBTRACK:
        //case SB_THUMBPOSITION:
            thumbPos = HIWORD(wParam);
            if (view->horzScrollMaxPos > view->thumbMaxRange) {
                dividedPos = (double)thumbPos / (double)view->thumbMaxRange;
                thumbPos = (int)(dividedPos * (double)view->horzScrollMaxPos);
            }
            move = thumbPos - view->horzScrollPos;
            break;

        default:
            move = 0;
            break;
    }

    if (move != 0) {
        view->horzScrollPos += move;
        thumbPos = view->horzScrollPos;

        ScrollWindow(view->hwnd, -view->charWidth * move, 0, NULL, NULL);

        if (view->horzScrollMaxPos > view->thumbMaxRange)  {
            dividedPos = (double)thumbPos / (double)view->horzScrollMaxPos;
            thumbPos = (int)(dividedPos *  (double)view->thumbMaxRange);
        }
        SetScrollPos(view->hwnd, SB_HORZ, thumbPos, TRUE);

        InvalidateRect(view->hwnd, NULL, TRUE);
        UpdateWindow(view->hwnd);
    }
}

void ResizeVertScroll(str_view* view) {
    if (view->model->str == NULL) {
        SetScrollRange(view->hwnd, SB_VERT, 0, 0, FALSE);
        return;
    }

    view->vertScrollMaxPos = max(0, (int)view->linebreaksLen - (int)view->visibleLines);
    view->vertScrollPos = min(view->vertScrollPos, view->vertScrollMaxPos);

    SetScrollRange(view->hwnd, SB_VERT, 0, min(view->thumbMaxRange, view->vertScrollMaxPos), FALSE);
}

void ResizeHorzScroll(str_view* view) {
    unsigned longestLength, longestIndex, longestPixels;
    char* longestString;

    if (view->model->str == NULL) {
        SetScrollRange(view->hwnd, SB_HORZ, 0, 0, FALSE);
        return;
    }

    if (view->viewMode == layout) {
        view->horzScrollMaxPos = 0;
        view->horzScrollPos = 0;
        SetScrollRange(view->hwnd, SB_HORZ, 0, view->horzScrollMaxPos, FALSE);
        return;
    }

    GetLongestLine(view, &longestLength, &longestIndex, &longestPixels, &longestString);

    view->horzScrollMaxPos = max(0, (int)longestLength - (int)view->charsInLine);
    view->horzScrollPos = min(view->horzScrollPos, view->horzScrollMaxPos);

    SetScrollRange(view->hwnd, SB_HORZ, 0, min(view->thumbMaxRange, view->horzScrollMaxPos), FALSE);
    if (longestString != NULL)
        free(longestString);
}
