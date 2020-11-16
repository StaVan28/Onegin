
#include "global.h"
#include "sort_header.h"
#include "file_header.h"

//-----------------------------------------------------------------------------

size_t SortFile(FILE* in, FILE* out, const char* mode, const char* bred_mode)
{
	if(!in ) return FILE_OPENING_ERROR;
	if(!out) return FILE_OPENING_ERROR;

    size_t n_symbols = NumberOfSimbols(in);
    if(!n_symbols) return CHARACTER_COUNT_ERROR;

	char* buffer = MakeBuffer(in, n_symbols);
	if(buffer == nullptr) return MEMORY_ALLOCATION_ERROR;

    size_t n_strings = NumberOfStrings(n_symbols, buffer);
	if(!n_strings) return STRINGS_COUNT_ERROR;

    line_t* arrstr = (line_t*) calloc(sizeof(line_t), n_strings);
    if(arrstr == nullptr) return MEMORY_ALLOCATION_ERROR;

    size_t n_structs = FillingStructs(buffer, arrstr);
    if(n_structs == 1) return FILLING_STRUCTURES_ERROR;

	int (*compare)(const void*, const void*) = choose_cmp(mode);

    qsort(arrstr, n_structs, sizeof(line_t), compare);

    bred_generator(arrstr, n_structs, bred_mode);

    print_structs(arrstr, n_structs, out);

    free_memory(arrstr, buffer);

    return SUCCESS;
}

//-----------------------------------------------------------------------------

int (*choose_cmp(const char* mode))(const void*, const void*)
{
	if(!strcmp(mode, "alphabet++"))
		return alphabet_up_cmp;
	else if(!strcmp(mode, "alphabet--"))
		return alphabet_down_cmp;
	else if(!strcmp(mode, "rhyme"))
		return rhyme_cmp;

    exit(COMPILER_INPUT_ERROR);
}

//-----------------------------------------------------------------------------

 int alphabet_up_cmp(const void* arg1, const void* arg2)
 {
    const line_t* parg1 = (const line_t*)arg1;
    const line_t* parg2 = (const line_t*)arg2;

    size_t length1 = parg1->length;
    size_t length2 = parg2->length;

    size_t min_length = length1 <= length2 ? length1 : length2;

    for(size_t index = 0; index < min_length; index++)
    {
        if(parg1->line[index] != parg2->line[index])
            return parg1->line[index] - parg2->line[index];
    }

    return length1 - length2;
 }

//-----------------------------------------------------------------------------

 int alphabet_down_cmp(const void* arg1, const void* arg2)
 {
    const line_t* parg1 = (const line_t*)arg1;
    const line_t* parg2 = (const line_t*)arg2;

    size_t length1 = parg1->length;
    size_t length2 = parg2->length;

    size_t min_length = length1 <= length2 ? length1 : length2;

    for(size_t index = 0; index < min_length; index++)
    {
        if(parg1->line[index] != parg2->line[index])
            return parg2->line[index] - parg1->line[index];
    }

    return length2 - length1;
 }

//-----------------------------------------------------------------------------

int rhyme_cmp(const void* arg1, const void* arg2)
{
   	const line_t* parg1 = (const line_t*)arg1;
	const line_t* parg2 = (const line_t*)arg2;

	size_t length1 = parg1->length;
	size_t length2 = parg2->length;

	size_t min_length = length1 <= length2 ? length1 : length2;

	size_t index_first  = 0;
	size_t index_second = 0;

	while(index_first < min_length && index_second < min_length)
	{
        while(isspace(parg1->line[length1 - index_first ]))
            index_first++ ;
        while(isspace(parg2->line[length2 - index_second]))
            index_second++;

        while(!isalnum(parg1->line[length1 - index_first ]))
            index_first++ ;
        while(!isalnum(parg2->line[length2 - index_second]))
            index_second++;

		if(parg1->line[length1 - index_first] != parg2->line[length2 - index_second])
            return parg1->line[length1 - index_first] - parg2->line[length2 - index_second];

        index_first++;
        index_second++;
	}
    return 0;
}

//-----------------------------------------------------------------------------

