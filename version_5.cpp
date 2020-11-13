
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>

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

size_t NumberOfSimbols(FILE* fp);
size_t NumberOfStrings(size_t n_symbols, char* buffer);

char* MakeBuffer(FILE* in, size_t n_symbols);

size_t SortFile(FILE* in, FILE* out, const char* mode);
size_t FillingStructs(char* buffer, line_t* arrstr);

int (*choose_cmp(const char* mode))(const void*, const void*);
int alphabet_up_cmp  (const void* arg1, const void* arg2);
int alphabet_down_cmp(const void* arg1, const void* arg2);
int rhyme_cmp        (const void* arg1, const void* arg2);

inline void print_structs(line_t* arrstr, size_t n_structs, FILE* out);
inline void free_memory(line_t* arrstr, char* buffer);

//*****************************************************************************

int main()
{
	setlocale(LC_ALL, "Rus");

	FILE* in  = fopen("onegin.txt", "rb");
	if(!in)  return FILE_OPENING_ERROR;

	//!TODO if (errno != 0) perror (errno, "Error on line %d", __LINE__);

	FILE* out = fopen("result.txt", "wb");
	if(!out) return FILE_OPENING_ERROR;

//You can choose three types of sorting: "alphabet++" (sort ascending),
//"alphabet--" (descending sort), "rhyme" (sorting by rhyme)
	SortFile(in, out, "alphabet++");

	fclose(in);
	if(!in)  return FILE_CLOSING_ERROR;

	fclose(out);
	if(!out) return FILE_CLOSING_ERROR;

	return SUCCESS;
}

//*****************************************************************************

size_t NumberOfSimbols(FILE* fp)
{
	if(!fp) return FILE_OPENING_ERROR;

	size_t start_value = ftell(fp);
	
	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, start_value, SEEK_SET);

	return file_size;
}

//-----------------------------------------------------------------------------

size_t NumberOfStrings(size_t n_symbols, char* buffer)
{
	if(!n_symbols)        return CHARACTER_COUNT_ERROR;
	if(buffer == nullptr) return MEMORY_ALLOCATION_ERROR;

	size_t n_strings = 0;

	for (size_t index = 0; index < n_symbols; index++)
        if (buffer[index] == '\n')
            n_strings++;

	return n_strings;
}

//-----------------------------------------------------------------------------

char* MakeBuffer(FILE* in, size_t n_symbols)
{
    assert(in);
    assert(n_symbols);

	char*  buffer = (char*) calloc(n_symbols + 1, sizeof(char));
	assert(buffer);

	size_t compFread = fread(buffer, sizeof(char), n_symbols, in);
	assert(compFread == n_symbols);

	fseek(in, 0, SEEK_SET);

	return buffer;
}

//-----------------------------------------------------------------------------

size_t FillingStructs(char* buffer, line_t* arrstr)
{
	if(buffer == nullptr) return MEMORY_ALLOCATION_ERROR;
    if(arrstr == nullptr) return MEMORY_ALLOCATION_ERROR;

    size_t index = 0;
    size_t n_structs = 0;

    while(true)
    {
        while(isspace(buffer[index]))
		index++;

        arrstr[n_structs].line = &(buffer[index]);

        while(buffer[index] != '\n')
        {
            if(buffer[index] == '\0')
            {
                arrstr[n_structs].length = &(buffer[index]) - arrstr[n_structs].line;
                return n_structs + 1;
            }

            index++;
        }

        arrstr[n_structs].length = &(buffer[index]) - arrstr[n_structs].line;

        n_structs++;
    }
}

//-----------------------------------------------------------------------------

size_t SortFile(FILE* in, FILE* out, const char* mode)
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

    if(length1 == length2)
        return 0;
    else
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

    if(length1 == length2)
        return 0;
    else
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
        while(isspace(parg1->line[length1 - index_second]))
            index_second++;

	if(parg1->line[length1 - index_first] != parg2->line[length2 - index_second])
            return parg1->line[length1 - index_first] - parg2->line[length2 - index_second];

        index_first++;
        index_second++;
	}
    return 0;
}

//-----------------------------------------------------------------------------

inline void print_structs(line_t* arrstr, size_t n_structs, FILE* out)
{
    for(size_t j = 0; j < n_structs; j++)
        for(size_t i = 0; i < arrstr[j].length; i++)
            fprintf(out, "%c", arrstr[j].line[i]);
}

//-----------------------------------------------------------------------------

inline void free_memory(line_t* arrstr, char* buffer)
{
    free(arrstr);
    free(buffer);
}

//-----------------------------------------------------------------------------




