#include <stdio.h>
#include <stdlib.h>


int mystrlen(char *str){
    char *word = str;
    int count = 0;
    while(word[count] != '\0'){
        count++;
    }
    return count;
}

char *mystradd(char *origin, char *addition){
    int len_origin = mystrlen(origin);
    int len_addition = mystrlen(addition);

    int size = len_origin+len_addition;

    char *palabra_final;
    palabra_final = malloc(sizeof(int)*size);

    for(int i=0;i<len_origin;i++){
        palabra_final[i] = origin[i];
    }

    for(int i=0;i<len_addition;i++){
        palabra_final[len_origin + i] = addition[i];
    }

    return palabra_final;
}

int mystrfind(char *origin, char *substr){

    int len_origin = mystrlen(origin);
    int len_substr = mystrlen(substr);

    char *word_origin = origin;
    char *word_substr = substr;

    int count = 0;
    int found = 0;
    if(len_substr > len_origin){
        printf("\n['%s'] string was not found at any position\n", substr);
        return -1;
    }else{
        for(int i=0;i<len_origin;i++){
            if(word_origin[i] == ' ')count++;
            if(word_origin[i] != ' '){
                if(word_origin[i] == word_substr[0]){
                    int j=0;
                    int k = i;
                    while(k < len_origin && word_origin[k] == word_substr[j]){
                            j++;
                            k++; 
                    }
                    if(j >= len_substr){
                        k = k - i;
                    }else{
                        j = -1;
                        k = -2;
                    }
                    if(k == j){
                        if(k == 1 && j == 1){
                            if(word_origin[i]==word_substr[j-1]){
                                 found = 1;
                                 break;
                            }
                        }else{
                             found = 1;
                             break;
                        }
                    }
                }
            }
        }
        if(found == 1){
                 //printf("['%s'] string was found at [%d] position", word_substr, count);
            return count;
        }else{
                printf("\n['%s'] string was not found at any position\n", substr);
                printf("Count: %d", count);
                printf("\n");
                return -1;
        }
    }
    return 0;
}