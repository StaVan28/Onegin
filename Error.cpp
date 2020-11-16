
#include "global.h"

//-----------------------------------------------------------------------------

void print_error(void)
{
    switch(errno)
    {
        case SUCCESS:
            break;
        case FILE_OPENING_ERROR:
            printf("FILE OPENING ERROR 1 in file (%s)", __FILE__);
            break;
        case FILE_CLOSING_ERROR:
            printf("FILE CLOSING ERROR 2 in file (%s)", __FILE__);
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("MEMORY ALLOCATION ERROR 3 in file (%s)", __FILE__);
            break;
        case FILE_READ_ERROR:
            printf("FILE READ ERROR 4 in file (%s)", __FILE__);
            break;
        case FILLING_STRUCTURES_ERROR:
            printf("FILLING STRUCTURES ERROR 5 in file (%s)", __FILE__);
            break;
        case COMPILER_INPUT_ERROR:
            printf("COMPILER INPUT ERROR 6 in file (%s)", __FILE__);
            break;
        case CHARACTER_COUNT_ERROR:
            printf("FILE OPENING ERROR 7 in file (%s)", __FILE__);
            break;
        case STRINGS_COUNT_ERROR:
            printf("FILE OPENING ERROR 8 in file (%s)", __FILE__);
            break;
        default:
            printf("UNKNOW ERROR in file (%s)", __FILE__);
            break;
    }
}
