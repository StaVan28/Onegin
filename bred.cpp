
#include "global.h"
#include "sort_header.h"

//-----------------------------------------------------------------------------

size_t bred_generator(line_t* arrstr, size_t n_structs, const char* bred_mode)
{
    if(strcmp("bred", bred_mode))
       return EXIT;

    if(arrstr == nullptr) return MEMORY_ALLOCATION_ERROR;
    if(!n_structs)       return FILLING_STRUCTURES_ERROR;

    FILE* bred_file = fopen("bred.txt", "wb");
    if(!bred_file) return FILE_OPENING_ERROR;

    srand(time(nullptr));

    /*AbAb*/
    size_t random1 = rand() % n_structs;
    size_t random2 = rand() % n_structs;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);

    random1++;
    random2++;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);


    /*CCdd*/
    random1 = rand() % n_structs;
    random2 = rand() % n_structs;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    random1++;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);

    random2++;

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);


    /*EffE      */
    random1 = rand() % n_structs;
    random2 = rand() % n_structs;
    if(isdigit(arrstr[random1].line))
        random1 = rand() % n_structs;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);

    random1++;
    random2++;

    for (size_t j = 0; j < arrstr[random2].length; j++)
        fprintf (bred_file, "%c", arrstr[random2].line[j]);

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);


    /*gg*/
     random1 = rand() % n_structs;
    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);

    random1++;

    for (size_t j = 0; j < arrstr[random1].length; j++)
        fprintf (bred_file, "%c", arrstr[random1].line[j]);
    
    fprintf(bred_file, "\n");
    
    fclose(bredfile);

}
