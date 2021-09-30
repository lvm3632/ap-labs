#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char **argv) {
    if(argc <=  3){
        printf("You need to send at least 3 arguments \nExample add:");
        printf("./main.o -add 'Initial String ' 'This is the rest to be added'\n");
        printf("Example find: ");
        printf("./main.o -find 'Initial String ' 'Init'\n");
        return -1;
    }
    char *operator_function = argv[1]; // -add or -find
    char *palabra_original = argv[2];  // Origin word
    char *segunda_palabra = argv[3];  // Append or find
    if(strcmp(operator_function,"-add")==0){
        char *palabra_final = mystradd(palabra_original,segunda_palabra);
        printf("Initial Lenght      : %d\n",mystrlen(palabra_original));
        printf("New String          : %s\n",palabra_final);
        printf("New length          : %d\n",mystrlen(palabra_final));
    }else if(strcmp(operator_function, "-find")==0){
       int position = mystrfind(palabra_original, segunda_palabra);
       if(position != -1){
            printf("['%s'] string was found at [%d] position\n", segunda_palabra, position);
       }else{
           return -1;
       }
    }else{
        printf("\nFunción inválida, solo se permiten las funciones -add y -find\n");
        return -1;
    }
    return 0;
}
