#include <stdio.h>
#include <stdlib.h>
#define SOUBOR "zavodnici.txt"
#include <string.h>
#define ODDELOVAC ".:"
#define VELIKOST 200
#define VYSTUP "vystup.txt"


int prevodCasu(const char *cas){
    char *pch;
    char pom_cas[15];
    int setiny;
    int kolo=0;
    //Zkopírování času do pom_cas
    strcpy(pom_cas,cas);
    pch = strtok (pom_cas,ODDELOVAC);  
    while (pch != NULL){
        switch(kolo){
            //Zpracováváme minuty
            case 0:
               setiny=atoi(pch)*6000;
               break;
            case 1:
            //Zpracování sekund
                setiny+=atoi(pch)*100;
                break;
            case 2:
                setiny+=atoi(pch);
                break;
        }
        pch = strtok (NULL,ODDELOVAC);
        kolo++;
    }
    return setiny;
}

int soucetCasu(const char *cas1,const char *cas2){
    return prevodCasu(cas1)+prevodCasu(cas2);
}

char * setinyNaCas(int cas_setiny){
    int minuty=cas_setiny/6000;
    int sekundy=(cas_setiny-minuty*6000)/100;
    int setiny=cas_setiny%100;
    char *cas = (char*)malloc(15*sizeof(char));
    sprintf(cas,"%02d:%02d.%02d",minuty,sekundy,setiny);
    return cas;
}


int main(int argc, char** argv) {
    typedef struct{
        int cislo;
        char prijmeni[20];
        char jmeno[20];
        char cas1[15];
        char cas2[15];
    }ZAVODNIK;
    
    ZAVODNIK *zavodnik=NULL;
    char *ukazatel;
    char *ukazatel_ztrata;
    char retezec[VELIKOST];
    int pocet=0;
    int i;
    int k;
    int ztrata;
    
    ZAVODNIK schovka; //Pro třídění, to tam schovám
    
    FILE * pFile;
     if ((pFile = fopen(SOUBOR,"r")) == NULL){ 
        printf("Nebyl otevren vas soubor %s. \n",SOUBOR);
        return EXIT_FAILURE;
    }
    
    //Přečtení prvního řádku
    fgets(retezec,VELIKOST,pFile);
    //Vytvoření paměti pro první závodnici
    zavodnik = (ZAVODNIK*) realloc (zavodnik, (pocet+1) * sizeof(ZAVODNIK));
    while(fscanf(pFile,"%d %s %s %s %s",&zavodnik[pocet].cislo,zavodnik[pocet].prijmeni,
            zavodnik[pocet].jmeno,zavodnik[pocet].cas1,zavodnik[pocet].cas2)!=EOF){
        //Vytvoření paměti pro další závodnici
        pocet++;
        zavodnik = (ZAVODNIK*) realloc (zavodnik, (pocet+1) * sizeof(ZAVODNIK));
        
    }
    
    if(fclose(pFile)==EOF){
        printf("Nepodarilo se zavrit soubor %s \n",SOUBOR);
    }
    
    printf("cislo  |   prijmeni   |   jmeno   |   cas1   |    cas2   \n");
    for(i=0;i<pocet;i++){
        printf("%2d %15s %15s %10s%10d %10s%10d\n",zavodnik[i].cislo,zavodnik[i].prijmeni,
                zavodnik[i].jmeno,zavodnik[i].cas1,prevodCasu(zavodnik[i].cas1),zavodnik[i].cas2,prevodCasu(zavodnik[i].cas2));   
    }
    
    //Třídění 
    for(i=0;i<pocet;i++){
        for(k=0;k<pocet-1;k++){
            if(soucetCasu(zavodnik[k].cas1,zavodnik[k].cas2)>
                    soucetCasu(zavodnik[k+1].cas1,zavodnik[k+1].cas2)){
                //Výměna dvou řádků
                schovka = zavodnik[k];
                zavodnik[k] = zavodnik[k+1];
                zavodnik[k+1] = schovka;
            }
        }
    }
    
    if ((pFile = fopen(VYSTUP,"w")) == NULL){ 
        printf("Nebyl otevren vas soubor %s. \n",VYSTUP);
        return EXIT_FAILURE;
    }
    
    for(i=0;i<pocet;i++){
        ztrata=soucetCasu(zavodnik[i].cas1,zavodnik[i].cas2)-
                soucetCasu(zavodnik[0].cas1,zavodnik[0].cas2);
        ukazatel_ztrata=setinyNaCas(ztrata);
        ukazatel=setinyNaCas(soucetCasu(zavodnik[i].cas1,zavodnik[i].cas2));
        fprintf(pFile,"%2d. %2d %15s %15s %10s %10s %10s %10s\n",i+1,zavodnik[i].cislo,zavodnik[i].prijmeni,
                zavodnik[i].jmeno,zavodnik[i].cas1,zavodnik[i].cas2,ukazatel,ukazatel_ztrata);
        free(ukazatel);
        free(ukazatel_ztrata);
        
    }
    
    
    
    if(fclose(pFile)==EOF){
        printf("Nepodarilo se zavrit soubor %s \n",VYSTUP);
    }else
        printf("Soubor %s byl vytvoren\n",VYSTUP);
    
    return (EXIT_SUCCESS);
}

