#pragma once
#include "Model.h"
#include "View.h"

/*
* @brief Get a part of a string
*
* Get new string that is created from original string cut between two indexes
*
* @param[in] str The original string
*
* @param[in] begin Index of the first symbol of the substring
*
* @param[in] end Index of the last symbol of the substring
*
* @return Substring or NULL
*/
char* ObtainSubString(char* str, unsigned begin, unsigned end);

/*
* @brief realloc() alternative for unsigned int arrays
*
* @param[in] arr Array to be reallocated
*
* @param[in] size Size of the original array
*
* @param[in] new_size Size of the reallocated array
*
* @return Reallocated array or NULL
*/
unsigned* ReallocateUINT(unsigned* arr, unsigned size, unsigned new_size);

/*
* @brief Append item to the end of unsigned array
*
* Requires array to have 2 supportive values: size and capacity
*
* Only works with unsigned arrays
*
* Capacity gets multiplied by 1.5, size added by one
*
* @param[out] arr Pointer to array, may return as NULL
*
* @param[out] size Pointer to size of array
*
* @param[out] capacity Pointer to capacity of array
*
* @param[in] item Item to be appended
*/
void AppendToArray(unsigned** arr, unsigned* size, unsigned* capacity, unsigned item);
