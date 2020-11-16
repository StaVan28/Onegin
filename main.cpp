
#include "global.h"
#include "sort_header.h"
#include "file_header.h"

//-----------------------------------------------------------------------------

int main()
{
    setlocale(LC_ALL, "Rus");

    FILE* in  = fopen("onegin.txt", "rb");
    assert(in);

	FILE* out = fopen("result.txt", "wb");
	assert(out);

//You can choose three types of sorting: "alphabet++" (sort ascending),
//"alphabet--" (descending sort), "rhyme" (sorting by rhyme)
	SortFile(in, out, "rhyme", "bred");

	fclose(in);
	assert(in);

	fclose(out);
	assert(out);

	print_error();

    return SUCCESS;
}
