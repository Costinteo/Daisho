#ifndef __DAI_STDLIB_FILES
#define __DAI_STDLIB_FILES
#include "../PreStart/PreStart.h"
#include "String.h"

/****************/
/* File Reading */
/****************/

/*
 * Returns SIZE_MAX on error. There's no worry of that being a legitimate value.
 * Since Stilts only works on 64 bit, we know that size_t is 64 bits.
 * That's over 18 Exabytes.
 */
__DAI_FN size_t
__Dai_fileSize(char* filePath) {
    struct stat st;
    return (!stat(filePath, &st)) ? (size_t)st.st_size : SIZE_MAX;
}

__DAI_FN __Dai_String_View
__Dai_readFile(char* filePath) {
    __Dai_String_View err = {.str = NULL, .len = 0};

    /* Ask for the length of the file */
    size_t fsize = __Dai_fileSize(filePath);
    if (fsize == SIZE_MAX) return err;

    /* Open the file */
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) return err;

    /* Allocate exactly enough memory. */
    char* buffer = (char*)__DAI_MALLOC(fsize + 1);
    if (!buffer) {
        close(fd);
        return err;
    }

    /* Read the file into a buffer and close it. */
    size_t bytes_read = read(fd, buffer, fsize);
    int close_err = close(fd);
    if ((bytes_read != fsize) | close_err) {
        __DAI_FREE(buffer);
        return err;
    }

    /* Write null terminator */
    buffer[fsize] = '\0';

    return (__Dai_String_View){.str = buffer, .len = fsize};
}

#endif /* __DAI_STDLIB_FILES */