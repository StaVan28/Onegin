
#include <stdio.h>
#include <stdlib.h>
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

void SortFile(FILE* in , FILE* out);
void FillingStructs(char* buffer, struct line_t* arrays);

//*****************************************************************************

int main()
{
    setlocale(LC_ALL, "Rus");

    FILE* in  = fopen("shekspir.txt", "rb");
	assert(in );
	FILE* out = fopen("result.txt"  , "wb");
    assert(out);

	SortFile(in, out);

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

	fseek( in, 0, SEEK_SET );

	return buffer;
}

//-----------------------------------------------------------------------------

void FillingStructs(char* buffer, struct line_t* array_structs)
{
    size_t j = 0;
    size_t i = 0;

    while(buffer[i] != '\0')
    {
        while(!isalnum(buffer[i]))
            i++;
        array_structs[j].line = &(buffer[i]);
        while(buffer[i] == '\n' || buffer[i] == '\0')
            i++;
        array_structs[j].length = &(buffer[i]) - array_structs[j].line;
        j++;
    }
}

//-----------------------------------------------------------------------------

void SortFile(FILE* in, FILE* out)
{
    size_t simbols = NumberOfSimbols(in);
    assert(simbols);

	char* buffer = MakeBuffer(in, simbols);

	size_t strings = NumberOfStrings(simbols, buffer);
	assert(strings);

    struct line_t* arrstr = (struct line_t*) calloc(sizeof(struct line_t), strings);
    assert(arrstr);
    FillingStructs(buffer, arrstr);

	free (buffer);
}

//-----------------------------------------------------------------------------










