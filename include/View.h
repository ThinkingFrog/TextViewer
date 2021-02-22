#pragma once

#include <windows.h>
#include "Model.h"

/*
* @brief List of possible view modes
*/
typedef enum {
    noLayout,
    layout,
    modes_count
} modes;

/*
* @brief View structure for str_model
*/
typedef struct {
    // Current mode (either with layout or without)
    modes viewMode;
    // Handle to window to be displayed at
    HWND hwnd;
    // str_model connected to current view
    str_model *model;

    // Size of the window
    unsigned wndWidth, wndHeight;
    // Amount of lines visible on the window at once
    unsigned visibleLines;
    // Maximum amount of characters visible on one line on the window at once
    unsigned charsInLine;
    // Height of each character (and string accordingly) in pixels
    unsigned charHeight;
    // Average width of a single character in pixels
    unsigned charWidth;
    // Index of first symbol of first visible line
    unsigned topSymb;

    // Position of a scroll
    unsigned vertScrollPos, horzScrollPos;
    // Maximum position a scroll can reach
    unsigned vertScrollMaxPos, horzScrollMaxPos;
    // Amount of characters scroll travels
    unsigned vertScrollDist, horzScrollDist;

    // Array that holds indexes in string to change line at
    unsigned *linebreaks;
    // Length of the array that holds indexes in string to change line at
    unsigned linebreaksLen;

    // Magic number that equals to 2^16 - 1, which is maximum of HIWORD(wParam) and limits scrollbar position on the sidebar
    unsigned thumbMaxRange;

    // Structure for dialogue menu
    OPENFILENAME ofn;
    // Filename for dialogue menu
    char filename[MAX_PATH];
} str_view;

/*
* @brief Initialize str_view connected with given model and window
*
* @return Filled str_view struct or NULL
*/
str_view* InitView(str_model* model, HWND hwnd);

/*
* @brief Func to display view on a window connected to it
*
* @param[in] view Pointer to a str_view to display
*/
void ShowView(str_view* view);

/*
* @brief Free memory from a str_view struct
*
* @param[in] Pointer to str_view to free
*/
void DestroyView(str_view* view);

/*
* @brief Fill the linebreaks field array of a given str_view
*
* @param[in] view Pointer to a view to fill
*/
void FillLineBreaks(str_view* view);

/*
* @brief Return parameters of the longest string of the given view
*
* @param[in] view Pointer to a str_view to operate on
*
* @param[out] length Pointer to a length of the longest string in characters
*
* @param[out] index Pointer to an index of the longest string starting from 1
*
* @param[out] pixels Pointer to a length of the longest string in pixels
*
* @param[out] str Pointer to the longest string
*/
void GetLongestLine(str_view* view, unsigned* length, unsigned* index, unsigned* pixels, char** str);

/*
* @brief Initialize dialogue menu parameters
*
* @param[in] view Pointer to a str_view structure connected with dialogue menu
*/
void InitDialog(str_view* view);

/*
* @brief Connect a model to a view
*
* @param[in] view Pointer to str_view to connect to
*
* @param[in] model Pointer to str_model to connect
*/
void AssignModelToView(str_view* view, str_model* model);

/*
* @brief Erase model from view and everything corresponded
*
* @param[in] view Pointer to a view to empty
*/
void EmptyView(str_view* view);

/*
* @brief Refresh metrics stored in str_view that are connected to the window
*
* @param[in] view Pointer to str_view to refresh metrics at
*/
void RefillMetrics(str_view* view);

/*
* @brief Adjust scroll position after view mode change
*
* @param[in] view Pointer to str_view that connects to scroll to be fixed
*/
void FixVertScroll(str_view* view);
