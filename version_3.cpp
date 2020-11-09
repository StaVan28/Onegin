#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>

//-----------------------------------------------------------------------------

struct line_t
{
    size_t length;
    char*  line;
};

//-----------------------------------------------------------------------------

size_t NumberOfSimbols(FILE* fp);
size_t NumberOfStrings(size_t simbols, char* buffer);

char* MakeBuffer(FILE* in, size_t simbols);

void SortFile(FILE* in, FILE* out, const char* mode);
size_t FillingStructs(char* buffer, line_t* arrstr);

int (*choose_cmp(const char* mode))(const void*, const void*);
int alphabet_up_cmp  (const void* arg1, const void* arg2);
int alphabet_down_cmp(const void* arg1, const void* arg2);
int rhyme_cmp        (const void* arg1, const void* arg2);

void print_structs(line_t* arrstr, size_t n_structs, FILE* out);
inline void free_memory(line_t* arrstr, char* buffer);

//*****************************************************************************

int main()
{
    setlocale(LC_ALL, "Rus");

    FILE* in  = fopen("shekspir.txt", "rb");
	assert(in );
	FILE* out = fopen("result.txt", "wb");
	assert(out);

	SortFile(in, out, "rhyme");

	fclose(in );
	fclose(out);

	return 0;
}

//*****************************************************************************

size_t NumberOfSimbols(FILE* fp)
{
    size_t start_value = ftell(fp); 

    fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, start_value, SEEK_SET);

	return file_size;
}

//-----------------------------------------------------------------------------

size_t NumberOfStrings(size_t simbols, char* buffer)
{
	size_t strings = 0;

	for (size_t i = 0; i < simbols; i++)
        if (buffer[i] == '\n')
            strings++;

	return strings;
}

//-----------------------------------------------------------------------------

char* MakeBuffer(FILE* in, size_t simbols)
{
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
    size_t i = 0;
    size_t n_structs = 0;

    while(true)
    {
        while(isspace(buffer[i]))
            i++;
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

void SortFile(FILE* in, FILE* out, const char* mode)
{
    size_t simbols = NumberOfSimbols(in);
    assert(simbols);

	char* buffer = MakeBuffer(in, simbols);
    size_t strings = NumberOfStrings(simbols, buffer);
	assert(strings);

    line_t* arrstr = (line_t*) calloc(sizeof(line_t), strings);
    assert(arrstr);
    size_t n_structs = FillingStructs(buffer, arrstr);

	int (*compare)(const void*, const void*) = choose_cmp(mode);

    qsort(arrstr, n_structs, sizeof(line_t), compare);

    print_structs(arrstr, n_structs, out);

    free_memory(arrstr, buffer);
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
}

//-----------------------------------------------------------------------------

 int alphabet_up_cmp(const void* arg1, const void* arg2)
 {
     const line_t* parg1 = (const line_t*)arg1;
     const line_t* parg2 = (const line_t*)arg2;
     return strcmp(parg1->line, parg2->line);
 }

//-----------------------------------------------------------------------------

 int alphabet_down_cmp(const void* arg1, const void* arg2)
 {
     const line_t* parg1 = (const line_t*)arg1;
     const line_t* parg2 = (const line_t*)arg2;
     return strcmp(parg2->line, parg1->line);
 }

//-----------------------------------------------------------------------------

int rhyme_cmp(const void* arg1, const void* arg2)
{
    const line_t* parg1 = (const line_t*)arg1;
	const line_t* parg2 = (const line_t*)arg2;
	
	size_t min = parg1->length <= parg2->length ? parg1->length : parg2->length;
	
	for(size_t i = 0; i < min; i++)
	{
		while(isspace(parg1->line[parg1->length - i]) ||  isspace(parg2->line[parg2->length - i]))
			i++;
		if(parg1->line[parg1->length - i] != parg2->line[parg2->length - i])
			return parg1->line[parg1->length - i] - parg2->line[parg2->length - i];
	}

	if(parg1->length == parg2->length)
		return 0;

	if(parg1->length > min)
		return  1;
	else
		return -1;
}
//-----------------------------------------------------------------------------

void print_structs(line_t* arrstr, size_t n_structs, FILE* out)
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



