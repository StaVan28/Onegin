
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
//errno - gloval var
//perror

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
size_t NumberOfStrings(size_t simbols, char* buffer);
//названия переменных
char* MakeBuffer(FILE* in, size_t simbols);

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
	SortFile(in, out, "rhyme");

	fclose(in);
	if(!in)  return FILE_CLOSING_ERROR;

	fclose(out);
	if(!out) return FILE_CLOSING_ERROR;

	return SUCCESS;
}

//*****************************************************************************

inline void print_hello(void)
{
    printf("    Hello!\n You can choose three types of sorting:\n");
    printf(" alphabet++ (sort ascending),\n");
    printf(" alphabet-- (descending sort),\n rhyme      (sorting by rhyme).\n\n");
    printf("    Please choose one:\n");
}

//-----------------------------------------------------------------------------

size_t NumberOfSimbols(FILE* fp)
{
    assert(fp);

    size_t start_value = ftell(fp);

    fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, start_value, SEEK_SET);

	return file_size;
}

//-----------------------------------------------------------------------------

size_t NumberOfStrings(size_t simbols, char* buffer)
{//!TODO FIX VARIABLE'S NAME
    assert(simbols);
    assert(buffer);

	size_t strings = 0;

	for (size_t i = 0; i < simbols; i++)
        if (buffer[i] == '\n')
            strings++;

	return strings;
}

//-----------------------------------------------------------------------------

char* MakeBuffer(FILE* in, size_t simbols)
{
    assert(in);
    assert(simbols);

	char*  buffer = (char*) calloc(simbols + 1, sizeof(char));
	assert(buffer);

	size_t compFread = fread(buffer, sizeof(char), simbols, in);
	assert(compFread == simbols);

	fseek(in, 0, SEEK_SET);

	return buffer;
}

//-----------------------------------------------------------------------------

size_t FillingStructs(char* buffer, line_t* arrstr)
{
    assert(buffer);
    assert(arrstr);

    size_t i = 0;
    size_t n_structs = 0;

    while(true)
    {
        while(isspace(buffer[i])) i++;
	    
        arrstr[n_structs].line = &(buffer[i]);
	    
        while(buffer[i] != '\n')
        {
		
            if(buffer[i] == '\0')
            {
                arrstr[n_structs].length = &(buffer[i]) - arrstr[n_structs].line;
                return n_structs + 1;
            }
		
            i++;
		
        }
	    
        arrstr[n_structs].length = &(buffer[i]) - arrstr[n_structs].line;
        n_structs++;
    }
}

//-----------------------------------------------------------------------------

size_t SortFile(FILE* in, FILE* out, const char* mode)
{//assert files
    size_t simbols = NumberOfSimbols(in);
    if(!simbols) return CHARACTER_COUNT_ERROR;

	char* buffer = MakeBuffer(in, simbols);
	if(buffer == nullptr) return MEMORY_ALLOCATION_ERROR;

    size_t strings = NumberOfStrings(simbols, buffer);
	if(!strings) return STRINGS_COUNT_ERROR;

    line_t* arrstr = (line_t*) calloc(sizeof(line_t), strings);
    if(buffer == nullptr) return MEMORY_ALLOCATION_ERROR;

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

    size_t min = length1 <= length2 ? length1 : length2;
	 
    for(size_t i = 0; i < min; i++)
    {
        if(parg1->line[i] != parg2->line[i])
            return parg1->line[i] - parg2->line[i];
    }
	 
    if(length1 == length2)
        return 0;
    else
        return length1 - length2;
 //   return strcmp(parg1->line, parg2->line);
 }

//-----------------------------------------------------------------------------

 int alphabet_down_cmp(const void* arg1, const void* arg2)
 {
    const line_t* parg1 = (const line_t*)arg1;
    const line_t* parg2 = (const line_t*)arg2;

    size_t length1 = parg1->length;
    size_t length2 = parg2->length;

    size_t min = length1 <= length2 ? length1 : length2;
    for(size_t i = 0; i < min; i++)
    {
        if(parg1->line[i] != parg2->line[i])
            return parg2->line[i] - parg1->line[i];
    }
    if(length1 == length2)
        return 0;
    else
        return length2 - length1;

//   return strcmp(parg2->line, parg1->line);
 }

//-----------------------------------------------------------------------------

int rhyme_cmp(const void* arg1, const void* arg2)
{
   	const line_t* parg1 = (const line_t*)arg1;
	const line_t* parg2 = (const line_t*)arg2;

	size_t length1 = parg1->length;
	size_t length2 = parg2->length;

	size_t min = length1 <= length2 ? length1 : length2;

	size_t i = 0;//CharIndexFirstLine
	size_t j = 0;

	while(i < min && j < min)
	{
        while(isspace(parg1->line[length1 - i]))
            i++;
        while(isspace(parg1->line[length1 - j]))
            j++;

		if(parg1->line[length1 - i] != parg2->line[length2 - i])
                return parg1->line[length1 - i] - parg2->line[length2 - i];
        i++;
        j++;
	}
    return 0;
}

//-----------------------------------------------------------------------------

inline void print_structs(line_t* arrstr, size_t n_structs, FILE* out)
{//переписать
    for(size_t j = 0; j < n_structs; j++)//посимвольный вывод - гавно
        for(size_t i = 0; i < arrstr[j].length; i++)
            fprintf(out, "%c", arrstr[j].line[i]);//fprintf - гавно. Меняем на fwrite
}

//-----------------------------------------------------------------------------

inline void free_memory(line_t* arrstr, char* buffer)
{
    free(arrstr);
    free(buffer);
}

//-----------------------------------------------------------------------------

