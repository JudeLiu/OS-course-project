 /****************************************************************************
        Description:
                This program is used to read in a file and copy the content of this file.

        Algorithm:
                read a character from source file, and save it into destination file, loop
        until read EOF.

        Sample input:
        -----------------------------------------------------
        ~/project1$ ./MyCopy data.in data.out
        -----------------------------------------------------
        The sample input means that "data.in" is source file, and "data.out" is destination
        file.
        Sample Output:
        -----------------------------------------------------
        MyCopy Time used:0.037600 miliisecond(s)
        -----------------------------------------------------
        Output indicates the execution time.

        Notice that you should change "data.in" before running this program.
****************************************************************************/
#include "header.h"

int main(int argc, char *argv[])
{
	clock_t start,end;//for timing
	double elapsed;
	char ch;//to read a single character from source file and save into dest file

	start = clock();//start timing

	FILE *src;//source file name
	FILE *dest;//destination file name

	//Open source file
	src = fopen(argv[1],"r");

	//open destination file
	dest = fopen(argv[2],"w+");

	//Check for file error
	if(src == NULL)
	{
		printf("Error: Could not open file'%s'.\n",argv[1]);
		exit(-1);
	}

	if(dest == NULL)
	{
		printf("Error:Coule not open file'%s'.\n",argv[2]);
		fclose(src);
		exit(-1);
	}

        //read a single character from source file, and put it to destination file, until read EOF
	while((ch=fgetc(src))!= EOF)
	{
		fputc(ch,dest);
	}

	fclose(src);
	fclose(dest);

	end = clock();//stop timing
	elapsed = ((double)(end-start)) / CLOCKS_PER_SEC * 100;
	printf("MyCopy Time used:%f miliisecond(s)\n",elapsed);

	return 1;
}
