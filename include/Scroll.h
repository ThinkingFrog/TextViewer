#pragma once
#include "View.h"

/*
* @brief Scroll window vertically
*
* @param[in] view Pointer to str_view struct that is connected to the window to be scrolled
*
* @param[in] wParam Scrolling message 
*/
void VertScroll(str_view* view, WPARAM wParam);

/*
* @brief Change scroll range and position with new window size
*
* @param[in] view Pointer to str_view struct that is connected to the window with resized scroll
*/
void ResizeVertScroll(str_view* view);

/*
* @brief Scroll window horizontally
*
* @param[in] view Pointer to str_view struct that is connected to the window to be scrolled
*
* @param[in] wParam Scrolling message 
*/
void HorzScroll(str_view* view, WPARAM wParam);

/*
* @brief Change scroll range and position with new window size
*
* @param[in] view Pointer to str_view struct that is connected to the window with resized scroll
*/
void ResizeHorzScroll(str_view* view);