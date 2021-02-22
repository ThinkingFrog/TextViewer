#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SupportModule.h"
#include "View.h"
#include "Model.h"

char* ObtainSubString(char* str, unsigned begin, unsigned end) {
    unsigned res_i, str_i;
    char cur_symb;

    char* res = (char*)malloc(sizeof(char) * (end - begin + 1));
    if (res == NULL)
        return NULL;

    for (res_i = 0, str_i = begin; str_i < end; ++str_i) {
        cur_symb = str[str_i];
        if (cur_symb == '\n' || cur_symb == '\r')
            continue;
        else
            res[res_i++] = cur_symb;
    }
    res[res_i] = '\0';

    return res;
}

unsigned* ReallocateUINT(unsigned* arr, unsigned size, unsigned new_size) {
    unsigned i;
    unsigned* new_arr;

    new_arr = (unsigned*)malloc(sizeof(unsigned) * new_size);
    if (new_arr == NULL)
        return NULL;

    for (i = 0; i < size; ++i) {
        new_arr[i] = arr[i];
    }
    free(arr);

    return new_arr;
}

void AppendToArray(unsigned** arr, unsigned* size, unsigned* capacity, unsigned item) {
    unsigned i;
    unsigned* new_arr;
    float cap_multiplier = 1.5f;

    if ((*size) + 1 > (*capacity)) {
        (*capacity) = ceil((*capacity) * cap_multiplier);

        new_arr = (unsigned*)malloc(sizeof(unsigned) * (*capacity));
        if (new_arr == NULL) {
            free(*arr);
            *arr = NULL;
            return;
        }

        for (i = 0; i < (*size); ++i)
            new_arr[i] = (*arr)[i];

        free(*arr);
        *arr = new_arr;
    }

    (*arr)[(*size)] = item;
    (*size) += 1;
}
