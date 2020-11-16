#ifndef SORT_HEADER_H_INCLUDED
#define SORT_HEADER_H_INCLUDED

size_t SortFile(FILE* in, FILE* out, const char* mode, const char* bred_mode);

int (*choose_cmp(const char* mode))(const void*, const void*);

int alphabet_up_cmp  (const void* arg1, const void* arg2);

int alphabet_down_cmp(const void* arg1, const void* arg2);

int rhyme_cmp        (const void* arg1, const void* arg2);

size_t bred_generator(line_t* arrstr, size_t n_structs, const char* bred_mode);

#endif // SORT_HEADER_H_INCLUDED
