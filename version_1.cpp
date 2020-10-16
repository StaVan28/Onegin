
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------

FILE* OpenFile  ( FILE* fp , const char* NameOfFile, const char* mode );
void  CloseFile ( FILE* fp );

size_t QualitySimbolsInFile ( FILE* fp );
size_t QualityStringsInFile ( FILE* fp );

char* SelectMemory_Char ( size_t size );

void CopyFileInFile_fread ( FILE* fpIN , FILE* fpOUT );
void CopyFileInFile_fputc ( FILE* fpIN , FILE* fpOUT );

//***********************************************************************

int main()
{											         
	FILE* IN   = OpenFile ( IN   , "shekspir.txt"      , "rb" );
	FILE* OUT1 = OpenFile ( OUT1 , "result_fread.txt"  , "wb" );
	FILE* OUT2 = OpenFile ( OUT2 , "result_fputs.txt"  , "wb" );

	CopyFileInFile_fread ( IN , OUT1 );
	CopyFileInFile_fputc ( IN , OUT2 );
	
	CloseFile ( IN   );
	CloseFile ( OUT1 );
	CloseFile ( OUT2 );

	return 0;
}

//***********************************************************************

FILE* OpenFile ( FILE* fp , const char* NameOfFile, const char* mode )
{
	if( ( fp = fopen ( NameOfFile , mode ) ) == NULL )
	{
		printf ("Ошибка при открытии файла %s.\n", NameOfFile );
		exit(1);
	}
	return fp;
}

//-----------------------------------------------------------------------

void CloseFile ( FILE* fp )
{
	if( fclose ( fp ) )
	{
		printf ("Ошибка закрытия файла.\n");
		exit(2);
	}
}

//-----------------------------------------------------------------------

size_t QualitySimbolsInFile ( FILE* fp )
{
	size_t startValue = ftell ( fp );

	fseek ( fp , 0 , SEEK_END );
	size_t sizeOfFile = ftell ( fp );
	fseek ( fp , startValue , SEEK_SET );

	return sizeOfFile;
}

//-----------------------------------------------------------------------

size_t QualityStringsInFile ( FILE* fp )
{
	size_t sizeOfFile = 0;
	char*  string     = 0;

	while ( !feof(fp) && !ferror(fp) )		
	{
		fgets ( string , 100 , fp ); 
		sizeOfFile++;
	}

	return sizeOfFile;
}

//-----------------------------------------------------------------------
 
char* SelectMemory_Char ( size_t size )
{
	char* buffer = (char*) calloc ( ( size + 1 ) , sizeof(char) ); 
	if ( buffer == NULL )
	{
		printf ("Ошибка выделения  памяти\n");
		exit(3);
	}

	return buffer;
}

//-----------------------------------------------------------------------

void CopyFileInFile_fread ( FILE* fpIN , FILE* fpOUT )
{
	size_t sizeOfFile = QualitySimbolsInFile ( fpIN );
	char*  buffer     = SelectMemory_Char     ( sizeOfFile );

	size_t compFread = fread ( buffer, sizeof(char) , sizeOfFile , fpIN );
	if ( compFread != sizeOfFile )
	{
		printf ("Ошибка считывания файла в буфер\n");
		exit(4);
	}

	size_t compFwrite = fwrite ( buffer , sizeof(char) , sizeOfFile , fpOUT );
	if ( compFwrite != sizeOfFile )
	{
		printf ("Ошибка записи буфера в файл\n");
		exit(5);
	}
	
	fseek( fpIN , 0 , SEEK_SET );
	free ( buffer );
}

//-----------------------------------------------------------------------

void CopyFileInFile_fputc ( FILE* fpIN , FILE* fpOUT )
{
	size_t sizeOfFile = QualitySimbolsInFile ( fpIN );
	char*  buffer     = SelectMemory_Char    ( sizeOfFile );
	
	for ( size_t i = 0; i < sizeOfFile; i++ )
	{
		buffer[i] = fgetc ( fpIN );
		
		if ( buffer[i] == EOF )
			break;

		fputc( buffer[i] , fpOUT );
	}
	
	fseek( fpIN , 0 , SEEK_SET );
	free ( buffer );
}

//------------------------------------------------------------------------
