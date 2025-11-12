#include "stb_image.h"
#include <stdlib.h>

unsigned char* stbi_load(const char*, int* x, int* y, int* comp, int req_comp) {
    if (x) *x = 1;
    if (y) *y = 1;
    if (comp) *comp = req_comp > 0 ? req_comp : 4;
    unsigned char* data = (unsigned char*)malloc(4);
    if (data) data[0] = data[1] = data[2] = data[3] = 255;
    return data;
}

void stbi_image_free(void* data) {
    free(data);
}
