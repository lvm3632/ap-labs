#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void quicksort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

/* Compare s1 and s2 numerically */
int numcomp(void* s1, void* s2){
	int n1, n2;
	n1 = atoi(s1);
	n2 = atoi(s2);
	if(n1 < n2){
		return -1;
	}else if(n1 > n2){
		return 1;
	}else{
		return 0;
	}
}
int strcomp(void* a, void * b){
	// Compare two strings
	return strcmp(a,b);
}

int main(int argc, char **argv)
{
	if(argc < 3){
		printf("Please provide at least 3 arguments.\n");
		printf("Example 1: \n");
		printf("./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
		printf("Example 2: \n");
		printf("./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return 0;
	}

	int numeric = 0; /* 1 if numeric sort */
	char *input_file_name;
	char *output_file_name;
	char *sortby;

	if(strcmp(argv[1], "-n") == 0){
		// Sorted by numbers
		numeric = 1;	
	}else{
		// Sorted by strings
		numeric = 0;
	}

	// Sort numbers
	if(numeric == 1){
		input_file_name = argv[2]; // numbers.txt
		char *punto = strrchr(input_file_name,'.');
		if(punto == NULL){
			printf("Please write a correct filename, provide a valid input filename extension.\nMust be .txt \n");
			return 0;
		}

		if(strcmp(punto,".txt") != 0) {
			printf("Please, provide a valid input filename extension.\nMust be .txt \n");
			return 0;
		}
		if(argv[3] != NULL){
			if(strcmp(argv[3], "-quicksort") == 0 || strcmp(argv[3], "-mergesort") == 0){
				sortby = argv[3]; // -quicksort or -mergesort
			}else{
				printf("Please, provide a valid type of sorting algorithm.\nValids options are: \n");
				printf("-mergesort or -quicksort\n");
				return 0;
			}
		}else{
			printf("Please don't leave it empty.\nWrite a valid type of algorithm sorting\n");
			printf("Example 1: \n");
			printf("./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
			printf("Example 2: \n");
			printf("./genericsort -n numbers.txt -mergesort -o qs_sorted_numbers.txt\n");
			return 0;
		}
		

		if(argv[4] != NULL){
			if(strcmp(argv[4],"-o") != 0){
				printf("Please, write -o as parameter by output filename flag\n");
				printf("Example 1: \n");
				printf("./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
				return 0;
			}
		}else{
			printf("Please don't leave it empty.\nWrite -o as parameter by output filename's flag\n");
			printf("Example 1: \n");
			printf("./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
			return 0;
		}

		if(argv[5] != NULL){
			output_file_name = argv[5]; // qs_sorted_numbers.txt
			char *punto2 = strrchr(output_file_name,'.');
			if(punto2 == NULL){
				printf("Please write a correct filename, provide a valid input filename extension.\nMust be .txt \n");
				return 0;
			}
			if(strcmp(punto2,".txt") != 0) {
				printf("Please, provide a valid output filename extension.\nMust be .txt \n");
				return 0;
			}
		}else{
			printf("Please don't leave it empty.\nProvide an output file's name with .txt after -o\n");
			return 0;
		}
		
	}else{

		input_file_name = argv[1]; // strings.txt
		char *punto = strrchr(input_file_name,'.');
		if(punto == NULL){
			printf("Please write a correct filename, provide a valid input filename extension.\nMust be .txt \n");
			return 0;
		}
		if(strcmp(punto,".txt") != 0) {
			printf("Please, provide a valid input filename extension.\nMust be .txt \n");
			return 0;
		}
		if(argv[2] != NULL){
			if(strcmp(argv[2], "-quicksort") == 0 || strcmp(argv[2], "-mergesort") == 0){
				sortby = argv[2]; // -quicksort or -mergesort
			}else{
				printf("Please, provide a valid type of sorting algorithm.\nValids options are: \n");
				printf("-mergesort or -quicksort\n");
				return 0;
			}
		}else{
			printf("Please don't leave it empty.\n Write a valid type of algorithm sorting\n");
			printf("Example 1: \n");
			printf("./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
			printf("Example 2: \n");
			printf("./genericsort -n numbers.txt -mergesort -o qs_sorted_numbers.txt\n");
			return 0;
		}
		if(argv[4] != NULL){
			output_file_name = argv[4]; // ms_sorted_strings.txt
			char *punto2 = strrchr(output_file_name,'.');
			if(strcmp(punto2,".txt") != 0) {
				printf("Please, provide a valid output filename extension.\nMust be .txt \n");
				return 0;
			}
		}else{
			printf("Please, provide an output file's name with .txt after -o\n");
			return 0;
		}
	}
	/** Count of lines **/
	FILE *foriginal;
	foriginal = fopen(input_file_name, "r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	int nlines = 0;
	while ((read = getline(&line, &len, foriginal)) != -1) {
		nlines++;
    }

	if (line){
        free(line);
	}
	
	fseek(foriginal, 0, SEEK_SET);
	char *lineptr[nlines-2];
	char *element;
	/** Array of chars or numbers **/
	int i = 0;
	while ((read = getline(&line, &len, foriginal)) != -1) {
			line[(int)read-1] = '\0';
			strcpy(element, line);
			lineptr[i] = strdup(element);
			i++;
			if(i==nlines){
				break;
			}
	}
	/** End - Array of chars or numbers **/
	fclose(foriginal);
	printf("... Results file can be found at ./%s\n", output_file_name);
	printf("... sorting %s file with %s", input_file_name, sortby);

	if(strcmp(sortby, "-mergesort") == 0){
		mergesort((void** )lineptr, 0, nlines-1, numeric ? numcomp : strcomp);
	}else if(strcmp(sortby, "-quicksort") == 0){
		quicksort((void** )lineptr, 0, nlines-1, numeric ? numcomp : strcomp);
	}

	FILE *output_file = fopen(output_file_name, "wb");

	for(int j=0;j<nlines;j++){
		lineptr[j] = strdup(lineptr[j]);
		fputs(strcat(lineptr[j],"\n"), output_file);
		//printf("%s", lineptr[j]);
		//printf("%s\n", lineptr[j]);
	}
	fclose(output_file);
	printf("\n");
    return 0;
}



