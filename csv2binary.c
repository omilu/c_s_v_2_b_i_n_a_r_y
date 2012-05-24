/*
 * 	File:		csv2binary.c
 * 	Programmer:	BForsyth
 * 	Date:		20111122
 * 	Description:	Takes a csv file of numbers and converts them to 
 * 			floats, skipping over newlines
 * 			invoke with ./csvbinary infile outfile
 *
 * 			need to run this twice, on the first run
 * 			set the macro MIN to 0, the program will check
 * 			every sample and report the min
 * 			run the program again, but replace MIN macro
 * 			with the min reported from the first run
 * 			this will normalize everything so there are no
 * 			negatives and you can report the number with 
 * 			a uint16_t which is what image magick needs
 * 			it cannot handle signed numbers or interpret
 * 			floats even though you can tell it to.  The
 * 			version of imagemagick i have only handles
 * 			unsigned integers
 */

#include	<stdio.h>
#include	<stdlib.h>
#define 	MAX_DIGITS	50
#define		MIN		-10000	

int	getNextCsvValue (char outArray[], int *cols, int *rows, FILE * inFile);
// reads chars from inFile until comma, puts chars into str outArray,
// updates rows when it reads a newline
//
int main (int argc, char *argv[])
{
	FILE 	*inFile;
	FILE	*outFile;
	char 	val[MAX_DIGITS];   //string of digits
	int 	count;
	int 	rows = 0;
       	int 	cols = 0;
	int	min = 0;
	int 	max = 0;
	
	if (argc != 3)	{
		printf("\nError: need infile and outfile");
		exit(1);
	}

	inFile = fopen(argv[1], "r");
	if (inFile == 0)	{
		printf("\nError: could not open infile");
		exit(1);
	}
	
	outFile = fopen(argv[2], "w");
	if (outFile == 0)	{
		printf("\nError: could not open outfile");
		exit(1);
	}
	
	while (getNextCsvValue (val, &cols, &rows, inFile) != EOF)
	{	
		cols++;
		int n = atoi(val) - MIN;		//convert the csv value
		if (min > n)				//string to int	
			min = n;		
		if (max < n)
			max = n;	
						
		uint16_t y = (uint16_t) n;	//convert the int
	       					//to a float and scale it
		fwrite(&y, sizeof(y), 1, outFile); //write the float to file
	}
	printf("\nmin is %d", min);
	printf("\nmax is %d", max);
	fclose(inFile);
	fclose(outFile);
	return 0;
}


int	getNextCsvValue (char outArray[], int *cols, int *rows, FILE * inFile)
{
	char dig, nextdig;
	int flag;	//to see if new line or end of file
	int count =0; 	//restart the count	
	int max_cols = 0;

	while ((dig = fgetc (inFile)) != ',')
	{	
		if (dig == '\n')
		{
			
			*rows = *rows + 1;		//increment row
			max_cols = *cols + 1;		//increment col
			*cols = 0;			//reset the cols
			outArray[count] = '\0';
			return 0;
		}
		if (dig == EOF) 
		{
			*cols = max_cols;	//get rid of col and
			*rows = *rows;	//row due to newline	
			printf("\ncols = %d, rows = %d", *cols, *rows);
			outArray[count] = '\0';	
			//terminate the string with NULL
			return EOF;
		}
		
		outArray[count] = dig;
		if (count++ == MAX_DIGITS)
			return 0;
	}

	outArray[count] = '\0';		//terminate the string with NULL
	return 0;
}
