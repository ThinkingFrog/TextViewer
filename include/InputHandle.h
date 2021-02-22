#pragma once

/*
* @brief Scroll by keyboard arrows
*
* Scroll vertically if arrows up or down are pressed and horizontally if left or right arrows are pressed
*
* @param[in] view str_view pointer to a view to be scrolled
*
* @param[in] wParam VK message (VK_UP, VK_DOW, VK_LEFT, VK_RIGHT)
*/
void HandleArrows(str_view* view, WPARAM wParam);

/*
* @brief Scroll by keyboard character keys
*
* Scroll vertically if keys 'w' or 's' are pressed and horizontally if keys 'a' or 'd' are pressed
*
* @param[in] view str_view pointer to a view to be scrolled
*
* @param[in] wParam keyborad character that was pressed
*/
void HandleKeys(str_view* view, WPARAM wParam);