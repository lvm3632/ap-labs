#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

static char *name[] = {
        "Illegal month",
       "Jan", "Feb", "Mar",
       "Apr", "May", "June",
       "July", "Aug", "Sept",
       "Oct", "Nov", "Dec"
    };

char *get_month(int *n){
     return (*n < 1 || *n > 12) ? name[0] : name[*n];
}

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // 365: leap == 0   
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} // 366: leap == 1

};

static int daysumtab[2][13] = {
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}, // 365: leap == 0   
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366} // 366: leap == 1
};

int day_of_year(int year,int *month ,int day){
    int i, leap;
    if(year <= 0 || day <= 0 || day >= 367){
        printf("Año ínvalido y/o día fuera del rango");
        return 0;
    }
    // 1 or 0
    leap = year%4 == 0 && year%100 != 0 || year % 400 == 0;
    if(leap == 0 && day >= 366){
        printf("El día es inválido, porque no es un año bisiesto\n");
        return 0;
    }
 
    // Sacar month con el dia
    if(day >= 1 && day <= 31){
            *month = 1;
     }else{
         for(int j=1; j<12;j++){
        // Intervalo de dias entre meses
            if(day >= daysumtab[leap][j] && day <= daysumtab[leap][j+1]){
                *month = ++j;
                break;
            } 
        }
     }

    int lastday = day;
    int n=0;
    for(int m = daysumtab[leap][*month-1]+1; m <= lastday; m++){
        n = n + 1;
    }
    
    return n;
}


void month_day(int year, int yearday, int *pmonth, int *pday){

    *pday = day_of_year(year,pmonth,yearday);
    // En la función day_of_Year cambio el valor del apuntador de pmonth
    // Aquí ya es el mes de acuerdo con el día
    char *month = get_month(pmonth);

}

int main(int argc, char **argv) {

    /* Input */
    if(argc < 2){
        printf("Please write at least two arguments. \nExample: \n");
        printf("# ./month_day <year> <yearday> \n");
        printf("# Example for Feb 2nd, 2019:\n");     
        printf("./month_day 2019 33\n");
        printf("Output: Feb 02, 2019\n");
        return 0;
    }

    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    int pmonth = -1;
    int pday = -1;

    month_day(year, yearday, &pmonth, &pday);
    if(pmonth != -1 && pday != -1){
        printf("%s %02d, %d", name[pmonth], pday, year);
    }
    printf("\n");

    return 0;
}
