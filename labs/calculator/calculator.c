#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    int total = 0;
    if(operator == 1){
         total = 0;
        // Suma
        for(int i=0;i<nValues;i++){
            total+=values[i];
        }

    }else if(operator == 2){
         total = 0;
        // Resta
         for(int i=0;i<nValues;i++){
             if(i==0){
                total=values[i];
             }else{
                total=total-values[i];
             }
        }

    }else if(operator == 3){
         total = 1;
        // Mult
         for(int i=0;i<nValues;i++){
            total*=values[i];
        }
    }else{
        return -1;
    }
    return total;
}

int main(int argc, char **argv) {

    int len = argc;

    if(len <= 3){
        printf("You need to send at least two numbers separeted by a space and an operator\n");
        printf("Example\n");
        printf("./calculator <operator> <numbers separated by space>\n");
        printf("Expressions that are valid: \n");
        printf("./calculator add 1 2 3 4 5\n");
        printf("./calculator sub 1 2 3 4 5\n");
        printf("./calculator mult 1 2 3 4 5\n");
        return -1;
    }
  
    int arrSize = len-2;
    int *arr_numbers;
    arr_numbers = malloc(sizeof(int)*arrSize);

    for(int i=2, j=0;i<=arrSize+1;i++,j++){
        if(atoi(argv[i]) != 0){
            arr_numbers[j]=atoi(argv[i]);
        }else{
            printf("Solo números positivos y/o diferentes a 0\n");

        }
    }

    char *operator = argv[1];
    long res;

    if(strcmp(operator,"add")==0){
        int sum = 1;
        res = calc(sum,arrSize,arr_numbers);
        for(int i=0;i<arrSize;i++){
            char str_b[50];
            int b = arr_numbers[i];
            sprintf(str_b,"%d",b); 
            if(i==arrSize-1){
                printf("%s", str_b);
                printf(" = ");
            }else{
                printf("%s + ", str_b);
            }
        }
        printf("%ld",res);
    }else if(strcmp(operator,"sub")==0){
        int sub = 2;
        res = calc(sub,arrSize,arr_numbers);
        for(int i=0;i<arrSize;i++){
            char str_b[50];
            int b = arr_numbers[i];
            sprintf(str_b,"%d",b); 
            if(i==arrSize-1){
                printf("%s", str_b);
                printf(" = ");
            }else{
                printf("%s - ", str_b);
            }
        }
        printf("%ld",res);
    }else if(strcmp(operator,"mult")==0){
        int mult = 3;
        res = calc(mult,arrSize,arr_numbers);
        for(int i=0;i<arrSize;i++){
            char str_b[50];
            int b = arr_numbers[i];
            sprintf(str_b,"%d",b); 
            if(i==arrSize-1){
                printf("%s", str_b);
                printf(" = ");
            }else{
                printf("%s * ", str_b);
            }
        }
        printf("%ld",res);

    }else{
        printf("Expresión inválida, el programa solo soporta: add, sub y mult y/o parámetros pueden ser inválidos \n");
        printf("Ejemplo correcto: \n");
        printf("./calculator <operator> <numbers separated by space>\n");
        return -1;
    }
    printf("\n");
    return 0;
   
}
