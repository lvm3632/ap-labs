#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <unistd.h>
#define REPORT_FILE "report.txt"
/** Start of Hashtable **/ 
struct nlist {
    struct nlist *next;
    char *name;
    char *value;
};
void perror(const char *msg);
// Si el tamaño, no es el apropiado nuestro hashing puede guardar más valores
// En la categoría equivocada
#define HASHSIZE 5000
static struct nlist *hashtab[HASHSIZE]; // Pointer table
// Función de hashing
unsigned hash(char *str){
    unsigned hashval = 7;
    for(hashval = 0; *str != '\0'; str++){
        hashval = *str + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

/** Start of Utils **/
char *strcopy(char *s)
{
	char *p;
	p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}
/** End of utils **/

// Función para regresar el apuntador a la key inicial, si un elemento fue encontrado
struct nlist* lookup(char *s)
{
	struct nlist *np;
	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

struct nlist* install(char *name, char *value)
{
    struct nlist *np;
    unsigned hashval;

    np = (struct nlist *) malloc(sizeof(*np));
    
    if(np == NULL || (np->name = strcopy(name)) == NULL){
        return NULL;
    }

    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;

    if((np->value = strcopy(value)) == NULL)
            return NULL;
    
    return np;
}

/** End of Hashtable **/
void analizeLog(char *logFile, char *report);
char *findstr(char *string, char *patrn, int *begin, int *end, int sub, int topOrEnd) {     
        int i, w=0, len;                  
        char *word = NULL;
        regex_t rgT;
        regmatch_t match;
        regcomp(&rgT,patrn,REG_EXTENDED);
        if ((regexec(&rgT,string,1,&match,0)) == 0) {
                // Si hay que restar caracteres
            if(sub!=0){
                // Resta al top
                if(topOrEnd == 1){
                    *begin = (int)match.rm_so+sub;
                    *end = (int)match.rm_eo;   
                }else{
                    *begin = (int)match.rm_so;
                    *end = (int)match.rm_eo+sub;
                }
            }else{
                // Si es 0, no se resta nada.
                *begin = (int)match.rm_so;
                *end = (int)match.rm_eo;
            }
                len = *end-*begin;
                word=malloc(len+1);
               
                for (i=*begin; i<*end; i++) {
                    word[w] = string[i];
                    w++; 
                }
                word[w]=0;
        }else{
            //printf("no hizo match");
        }
        regfree(&rgT);
        return word;
}

void initializeHash(int nLines, char* arr_logs[]){
    struct nlist *np;
    for(int i=0;i<nLines;i++){
        int a,b,c,d,e,f;
        char *timestamp = findstr(arr_logs[i],"([[\\.]  .[0-9].[0-9]{6}]|\\[[a-z]{3}[0-9]{2} [0-9]{2}:[0-9]{2}])", &c,&d,0,0);
	    char *category = findstr(arr_logs[i],"( [a-zA-Z])(.*): ",&a,&b,1,1);
        char *description = findstr(arr_logs[i],"[:] (.*)", &e,&f,1,1);
        if(category != NULL){
          int maxLength = strlen(timestamp)+strlen(description)+1;
          char timeDescription[maxLength];
          timeDescription[0] = '\0';
          strcat(timeDescription,timestamp);
          strcat(timeDescription,description);
          install(category, timeDescription);        
        }else{
          install("General: ", arr_logs[i]);
        }
    }
}

void printLog(char *category){
    struct nlist *head, *ptr;
    int k=0;
    ptr = lookup(category);
    printf("%s\n", category);
    while(ptr != NULL){
		printf("%d: %s", k,ptr->value);
		ptr = ptr->next;
        k++;
	}
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage:./dmesg-analizer dmesg.txt\n");
        return 1;
    }
    
    analizeLog(argv[1], REPORT_FILE); 
    return 0;
}


void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    // Implement your solution here.
    printf("Report is generated at: [%s]\n", report);
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;

    inputFd = open(logFile, O_RDONLY);

    if(inputFd == -1){
        printf("Hubo un error con el archivo input (open): %s", logFile);
        perror("open");
        exit(EXIT_FAILURE);
    }

    openFlags = O_WRONLY | O_CREAT | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                 S_IROTH | S_IWOTH;  

    outputFd = open(REPORT_FILE, openFlags, filePerms);
    if (outputFd == -1){
        printf("Hubo un error con el archivo output (open): %s", REPORT_FILE);
        perror("open");
        exit(EXIT_FAILURE);
    }
    int max_char = 0;
    int before = 0;
    int nLines = 0;
    char buffer;
    // Guardamos el valor máximo para crear nuestro arreglo
    while(read(inputFd, &buffer,1)){
        if(buffer == '\n'){
            nLines++;
            max_char = 0;
        }
        max_char++;
    }
    int arr_mem[nLines];
    lseek(inputFd, 0, SEEK_SET);
    int i=0;
     while(read(inputFd, &buffer,1)){
        if(buffer == '\n'){
            arr_mem[i]=max_char;
            max_char = 0;
            i++;
        }
            max_char++;
    }
    lseek(inputFd, 0, SEEK_SET);
    int j=0;
    char *arr_logs[nLines-1];
    // El buffer debe ser lo suficiente grande para no leer otros datos.
    char *element = (char*)malloc(500000*sizeof(char));
    while(read(inputFd, &buffer, 1) > 0){
        strncat(element, &buffer,1);
        if(buffer == '\n'){
            arr_logs[j] = strdup(element);
            j++;
            *element = '\0';
        }
        if(j==nLines){
				break;
		}
    }
    initializeHash(nLines,arr_logs);
    struct nlist *head, *ptr;
    //printf("\nHash Table List \n");
    int header = 0;
	for (i = 0; i < HASHSIZE; i++) {
		head = hashtab[i];
		for (ptr = head; ptr != NULL; ptr = ptr->next) {
            if(header == 0){
                //printf("%s\n", ptr->name);
                write(outputFd, ptr->name, strlen(ptr->name));
                write(outputFd, "\n", 1);
                header=1;
            }
			//printf("%s",ptr->value);
            write(outputFd,"  ", 2);
            write(outputFd, ptr->value, strlen(ptr->value));
		}
        header = 0;
	}
    /*printf("\nHash Table\n");
	printLog("pci 0000:00:1c.4: ");
    printLog("e820: ");
    printLog("General: ");
    printLog("Bluetooth: ");
    printLog("usb 2-4: ");*/
    if(numRead == -1){
        printf("read");
    }

    if(close(inputFd) == -1){
        /* Code to handle the error */
        printf("Hubo un error con el archivo input (close): %s", logFile);
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (close(outputFd) == -1){
        /* Code to handle the error */
        printf("Hubo un error con el archivo output (close): %s", REPORT_FILE);
        perror("close");
        exit(EXIT_FAILURE);
    }
}





