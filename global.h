#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <ctime>

//-----------------------------------------------------------------------------

const int EXIT               = 1;
const int SIZE_ONEGIN_STANZA = 7;

//-----------------------------------------------------------------------------

enum Errors
{
     SUCCESS                  ,
     FILE_OPENING_ERROR       ,
     FILE_CLOSING_ERROR       ,
     MEMORY_ALLOCATION_ERROR  ,
     FILE_READ_ERROR          ,
     FILLING_STRUCTURES_ERROR ,
     COMPILER_INPUT_ERROR     ,
     CHARACTER_COUNT_ERROR    ,
     STRINGS_COUNT_ERROR
};

//-----------------------------------------------------------------------------

struct line_t
{
    size_t length = 0;
    char*  line   = nullptr;
};

//-----------------------------------------------------------------------------

void print_error(void);

//-----------------------------------------------------------------------------

#endif // GLOBAL_H_INCLUDED
