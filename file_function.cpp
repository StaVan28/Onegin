
#include "global.h"
#include "file_header.h"

//-----------------------------------------------------------------------------

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
        while(isspace(buffer[index]) || isdigit(buffer[index])   \
             || (buffer[index] == '*') || (buffer[index] == 'X') \
             || (buffer[index] == 'L') || (buffer[index] == 'I') \
             || (buffer[index] == 'V') || (buffer[index] == '…') \
             || (buffer[index] == '.'))
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

void print_structs(line_t* arrstr, size_t n_structs, FILE* out)
{
    for(size_t j = 0; j < n_structs; j++)
        for(size_t i = 0; i < arrstr[j].length; i++)
            fprintf(out, "%c", arrstr[j].line[i]);
}

//-----------------------------------------------------------------------------

void free_memory(line_t* arrstr, char* buffer)
{
    free(arrstr);
    free(buffer);
}
