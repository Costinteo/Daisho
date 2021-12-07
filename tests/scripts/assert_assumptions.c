#include "../../stilts-stdlib/Native/Stilts.h"

/*****************************************/
/* LIST OF PLATFORM SPECIFIC ASSUMPTIONS */
/*****************************************/

/*
 *
 *
 *
 *
 */

#define ENDIANNESS_OTHER 0
#define ENDIANNESS_LITTLE 1
#define ENDIANNESS_BIG 2

static inline void
types_valid(void) {
    if (sizeof(uint32_t) != 4 * sizeof(uint8_t)) {
        fprintf(stderr,
                "The Stilts standard library is not supported on platforms "
                "where sizeof(uint32_t) != 4 * sizeof(uint8_t).");
        exit(1);
    }
}

static inline void
get_endianness(void) {
    union {
        uint32_t a;
        uint8_t b[4];
    } endi = {0};
    endi.a = 0x00000001;

    int endianness;
    switch (endi.b[0]) {
        case 0x01:
            endianness = ENDIANNESS_LITTLE;
            break;
        default:
            endianness = ENDIANNESS_OTHER;
            break;
    }

    if (endianness != ENDIANNESS_LITTLE) {
        fprintf(stderr,
                "The Stilts standard library is not supported on "
                "big-endian or unknown endianness platforms.\n");
        exit(1);
    }
}

static inline void
uint8_char(void) {
    if (sizeof(uint8_t) != 1) {
        fprintf(stderr,
                "The Stilts standard library assumes that the size of uint8_t "
                "on the machine is one.\n");
        exit(1);
    }
}

static inline void
size_size(void) {
    if (!(SIZE_MAX <= UINT64_MAX)) {
        fprintf(stderr,
                "The Stilts standard library assumes that size_t's max value "
                "is less than or equal to uint64_t's max value.\n");
        exit(1);
    }
}

static inline void
utf8_locale(void) {
    if (!setlocale(LC_ALL, "C.UTF-8")) {
        fprintf(stderr, "Could not set locale to utf8.\n");
        exit(1);
    }
}

// Returns cleanly prints error message and exits on failure.
typedef void (*Test)(void);

Test tests[] = {types_valid, get_endianness, uint8_char, size_size,
                utf8_locale};

int
main(void) {
    static size_t num_tests = (sizeof(tests) / sizeof(Test));
    for (size_t i = 0; i < num_tests; i++) tests[i]();

    puts("SUCCESS");
}
