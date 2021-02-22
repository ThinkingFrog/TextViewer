#pragma once

/*
* @brief Model structure
*/
typedef struct {
    // Pointer to a string that holds the whole text as single line
    char *str;

    // Length of the text in 'str' field
    unsigned long len;
} str_model;

/*
* @brief Initialize a model
*
* Initialize blank model that doesn't contain a string
*
* @return Pointer to a str_model struct with NULL str field and 0 len field or NULL
*/
str_model* InitModel();

/*
* @brief Free memory from model
*
* Free str field and structure itself
*
* @param[in] model Pointer to str_model structure to free
*/
void DestroyModel(str_model* model);

/*
* @brief Empty model without freeing it
*
* Free str field, set it to NULL and set len to 0
*
* @param[in] model Pointer to str_model struct to empty
*/
void EmptyModel(str_model* model);

/*
* @brief Fill model with text from file
*
* Fills model's str and len fields based on file's contains
*
* @param[out] str Pointer to string to fill
*
* @param[out] len Pointer to length of the string to fill
*
* @param[in] filename Name of a file to fill the model with
*/
void AssignFileToModel(char** str, unsigned long *len, char* filename);