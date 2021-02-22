#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "View.h"
#include "Model.h"
#include "SupportModule.h"

str_view* InitView(str_model* model, HWND hwnd) {
    str_view* view;

    view = (str_view*)malloc(sizeof(str_view));
    if (view == NULL)
        return NULL;

    view->viewMode = noLayout;
    view->hwnd = hwnd;
    view->model = model;
    view->linebreaks = NULL;
    view->linebreaksLen = 0;

    view->vertScrollPos = 0;
    view->vertScrollMaxPos = 0;
    view->horzScrollPos = 0;
    view->horzScrollMaxPos = 0;

    view->thumbMaxRange = 65535;

    // fill array of indexes where we want a line break
    FillLineBreaks(view);
    RefillMetrics(view);
    InitDialog(view);

    return view;
}

void FillLineBreaks(str_view* view) {
    unsigned* linebreaks = (unsigned*)malloc(sizeof(unsigned));
    unsigned size, capacity;
    unsigned long i;

    if (linebreaks == NULL || view->model == NULL)
        return;

    if (view->linebreaksLen != 0) {
        view->topSymb = view->linebreaks[view->vertScrollPos];
        free(view->linebreaks);
        view->linebreaks = NULL;
        view->linebreaksLen = 0;
    }

    if (view->model->len == 0) {
        view->linebreaks = NULL;
        view->linebreaksLen = 0;
        return;
    }

    linebreaks[0] = 0;
    for (i = 0, size = 1, capacity = 1; i < view->model->len; ++i) {
        if (view->model->str[i] == '\n' || (view->viewMode == layout && (i - linebreaks[size - 1] >= view->charsInLine - 1)))
            AppendToArray(&linebreaks, &size, &capacity, i);
    }
    AppendToArray(&linebreaks, &size, &capacity, view->model->len);

    view->linebreaksLen = size;
    view->linebreaks = (unsigned*)malloc(sizeof(unsigned) * view->linebreaksLen);
    for (i = 0; i < view->linebreaksLen; ++i)
        view->linebreaks[i] = linebreaks[i];
    free(linebreaks);
}

void RefillMetrics(str_view* view) {
    unsigned longestLen, longestIndex, longestPixels;
    char *longestString;
    TEXTMETRIC metrics;
    RECT wndRect;

    HDC hdc = GetDC(view->hwnd);
    GetClientRect(view->hwnd, &wndRect);
    GetTextMetrics(hdc, &metrics);

    view->wndWidth = wndRect.right - wndRect.left;
    view->wndHeight = wndRect.bottom - wndRect.top;

    view->charHeight = metrics.tmHeight;
    view->charWidth = metrics.tmAveCharWidth;

    view->charsInLine = view->wndWidth / view->charWidth;
    view->visibleLines = view->wndHeight / view->charHeight;

    view->vertScrollDist = ceil(view->linebreaksLen / pow(2, 8 * sizeof(short)));

    GetLongestLine(view, &longestLen, &longestIndex, &longestPixels, &longestString);
    view->horzScrollDist = ceil(longestLen / pow(2, 8 * sizeof(short)));

    ReleaseDC(view->hwnd, hdc);
}

void InitDialog(str_view* view) {
    ZeroMemory(&view->ofn, sizeof(OPENFILENAME));
    view->ofn.lStructSize = sizeof(OPENFILENAME);
    view->ofn.hwndOwner = view->hwnd;
    view->ofn.lpstrFile = view->filename;
    view->ofn.lpstrFile[0] = '\0';
    view->ofn.nMaxFile = sizeof(view->filename);
    view->ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0";
    view->ofn.nFilterIndex = 1;
    view->ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

void AssignModelToView(str_view* view, str_model* model) {
    if (view == NULL || model == NULL)
        return;

    view->model = model;

    // manually reset scrolls
    view->vertScrollPos = 0;
    SetScrollPos(view->hwnd, SB_VERT, 0, TRUE);
    view->horzScrollPos = 0;
    SetScrollPos(view->hwnd, SB_HORZ, 0, TRUE);

    FillLineBreaks(view);
    RefillMetrics(view);
}

void GetLongestLine(str_view* view, unsigned* length, unsigned* index, unsigned* pixels, char** str) {
    unsigned i, len, maxLength = 0, maxIndex = 0;
    HDC hdc;
    SIZE strSize= {0};
    hdc = GetDC(view->hwnd);

    if (view->linebreaksLen == 0) {
        *length = 0;
        *pixels = 0;
        *index = 0;
        *str = NULL;
        return;
    }

    for (i = 1; i < view->linebreaksLen; ++i) {
        len = view->linebreaks[i] - view->linebreaks[i - 1];
        if (len > maxLength) {
            maxLength = len;
            maxIndex = i;
        }
    }

    *length = maxLength;
    *index = maxIndex;
    *str = ObtainSubString(view->model->str, view->linebreaks[maxIndex - 1], view->linebreaks[maxIndex]);
    GetTextExtentPoint32A(hdc, *str, strlen(*str), &strSize);
    *pixels = (unsigned)strSize.cx;

    ReleaseDC(view->hwnd, hdc);
}

void FixVertScroll(str_view* view) {
    unsigned i;

    // Move scroll so it points to the same string it did before changing mode
    for (i = 0; i < view->linebreaksLen - 1; ++i)
        if (view->topSymb >= view->linebreaks[i] && view->topSymb < view->linebreaks[i + 1]) {
            view->vertScrollPos = i;
            break;
        }
}

void ShowView(str_view* view) {
    PAINTSTRUCT ps;
    HDC hdc;

    char* tmp_str;
    unsigned i, paintAreaStart = 1, paintAreaEnd = view->linebreaksLen;

    unsigned curY = 0, curX = 0;

    hdc = BeginPaint(view->hwnd, &ps);
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

    if (view->vertScrollPos != 0) {
        paintAreaStart = max(1, view->vertScrollPos + ps.rcPaint.top / view->charHeight - 1);
        paintAreaEnd = min(view->linebreaksLen, view->vertScrollPos + ps.rcPaint.bottom / view->charHeight + 1);
    }

    // print text in lines
    if (view->model->str != NULL) {
        for (i = paintAreaStart; i < paintAreaEnd; ++i) {
            curX = view->charWidth * (1 - view->horzScrollPos);
            curY = view->charHeight * (i - view->vertScrollPos);
            tmp_str = ObtainSubString(view->model->str, view->linebreaks[i - 1], view->linebreaks[i]);
            TextOut(hdc, curX, curY, tmp_str, strlen(tmp_str));
            free(tmp_str);
        }
    }

    EndPaint(view->hwnd, &ps);
}

void EmptyView(str_view* view) {
    if (view == NULL)
        return;

    free(view->linebreaks);
    view->linebreaks = NULL;
    view->linebreaksLen = 0;
    view->charHeight = 0;
    view->charsInLine = 0;
    view->charWidth = 0;
    view->horzScrollMaxPos = 0;
    view->horzScrollPos = 0;
    view->horzScrollDist = 0;
    view->vertScrollMaxPos = 0;
    view->vertScrollPos = 0;
    view->vertScrollDist = 0;
    view->visibleLines = 0;
}

void DestroyView(str_view* view) {
    free(view->linebreaks);
    free(view);
}
