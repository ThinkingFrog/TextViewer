#include <stdio.h>
#include <stdlib.h>
#include "Model.h"

str_model* InitModel() {
    str_model* model;

    model = (str_model*)malloc(sizeof(str_model));
    if (model == NULL)
        return NULL;

    model->str = NULL;
    model->len = 0;

    return model;
}

void EmptyModel(str_model* model) {
    if (model == NULL)
        return;

    free(model->str);
    model->str = NULL;
    model->len = 0;
}

void DestroyModel(str_model* model) {
    free(model->str);
    free(model);
}

void AssignFileToModel(char** str, unsigned long *len, char* filename) {
    FILE *F;

    F = fopen(filename, "rb");
    if (F == NULL) {
        *str = NULL;
        *len = 0;
        return;
    }

    // find amount of characters in file
    fseek(F, 0, SEEK_END);
    *len = ftell(F);
    fseek(F, 0, SEEK_SET);

    // put whole file in a single string
    *str = (char*)malloc(sizeof(char) * (*len));
    if (*str == NULL)
        return;

    fread(*str, sizeof(char), *len, F);
    (*str)[*len - 1] = '\0';

    fclose(F);
}