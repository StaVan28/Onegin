#ifndef FILE_HEADER_H_INCLUDED
#define FILE_HEADER_H_INCLUDED

size_t NumberOfSimbols(FILE* fp);

size_t NumberOfStrings(size_t n_symbols, char* buffer);

char* MakeBuffer(FILE* in, size_t n_symbols);

size_t FillingStructs(char* buffer, line_t* arrstr);

void print_structs(line_t* arrstr, size_t n_structs, FILE* out);

void free_memory(line_t* arrstr, char* buffer);

#endif // FILE_HEADER_H_INCLUDED
