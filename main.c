#include <stdio.h>
#include <stdlib.h>
#include "affiche_tas.h"
#include <string.h>


// Macros
#define buffer_dans_tas(ptr)         ptr<(tas+TAILTAS-1)
#define buffer_libre(ptr)            *(ptr+1)==-1
#define taille_buffer_ok(taille,ptr) taille<*ptr



// Strategie first fit
char* first_fit(unsigned int taille)
{
    char* ptr = tas;
    unsigned int taille_cumulee = 0;
    while (buffer_dans_tas(ptr) ) // tant que le pointeur est dans le tas
    {
        // s'il y a la place pour allouer
        // et que c'est libre
        if(taille_buffer_ok(taille,ptr) && buffer_libre(ptr))
        {
            return ptr;
        }
        else
        {
            // sinon on avance de la taille de l'espace alloué
            ptr = tas + taille_cumulee;
            taille_cumulee = taille_cumulee + *ptr + 1;
        }
    }
    printf("Espace indisponible\n");
    return ptr;
}

// Strategie best fit
char* best_fit(unsigned int taille)
{
    char* ptr = tas;
    char* petit_ptr = tas;
    int min_place = 127;
    unsigned int taille_cumulee = 0;
    while (buffer_dans_tas(ptr)) // tant que le pointeur est dans le tas
    {
        // s'il y a la place pour allouer
        // et que c'est libre
        // et que c'est inférieur à la plus petite place sauvegardée
        if(taille_buffer_ok(taille,ptr) && buffer_libre(ptr) && *ptr < min_place)
        {
            min_place = *ptr;
            petit_ptr = ptr;
        }
        // on avance de la taille de l'espace alloué
        ptr = tas + taille_cumulee;
        taille_cumulee = taille_cumulee + *ptr + 1;
    }
    return petit_ptr;
}

// Strategie worst fit
char* worst_fit(unsigned int taille)
{
    char* ptr = tas;
    char* grand_ptr = tas;
    int max_place = 0;
    unsigned int taille_cumulee = 0;
    while (buffer_dans_tas(ptr)) // tant que le pointeur est dans le tas
    {
        // s'il y a la place pour allouer
        // et que c'est libre
        // et que c'est supérieur à la plus grande place sauvegardée
        if(taille_buffer_ok(taille,ptr) && buffer_libre(ptr) && *ptr > max_place)
        {
            max_place = *ptr;
            grand_ptr = ptr;
        }
        // on avance de la taille de l'espace alloué
        ptr = tas + taille_cumulee;
        taille_cumulee = taille_cumulee + *ptr + 1;
    }
    return grand_ptr;
}



/* Fonction malloc qui prend en argument la taille à allouer ainsi
    qu'un pointeur de fonction de fonction, représentant la strategie choisie */
char* tas_malloc_fct(unsigned int taille, char* (pt_fct)(unsigned int))
{

    // recuperation de l'adresse via le pointeur de fonction selon la strategie choisie
    char* pos_allo = (*pt_fct)(taille);
    if(pos_allo == (tas + TAILTAS -1))// vérifie qu'il y a toujours de l'espace dans le tas
    {
        printf("Espace insuffisant\n");
        return pos_allo;
    }

    if(TAILTAS - *pos_allo == libre) // verification du dernier buffer libre
    {
        libre = libre + taille + 1; // mise à jour de la variable libre
        *(pos_allo + taille + 1) = TAILTAS - libre; // notation de l'espace libre restant à la suite
        *(pos_allo + taille + 2) = -1; // informe que cet espace est libre

    }
    // on ajoute une case si elle se retrouve seule entre deux buffers ou a la fin
    // car il faut au moins deux cases pour etre considere libre
    if(*pos_allo - taille == 1)
    {
       taille++;
    }
    *pos_allo = taille; // attribution de la taille allouee

    return pos_allo + 1; // retour de l'adresse de la position juste apres
}


void tas_free(char* ptr)
{
    *ptr = -1; // le pointeur pointe sur le début du bloc, juste après sa taille
}


int main()
{
    // initialisation du tas
    tas_init();

/* Pointeurs des fonctions best-fits, worst-fits, first-fits */
    char* (*pt_fct_best)(unsigned int) = &best_fit;
    char* (*pt_fct_first)(unsigned int) = &first_fit;
    char* (*pt_fct_worst)(unsigned int) = &worst_fit;



/* Affichage de l'énoncé avec des pointeurs de fonctions */

    char *p1, *p2, *p3, *p4;

    p1 = (char *) tas_malloc_fct(10,pt_fct_best);
    strcpy( p1, "tp 1" );

    p2 = (char *) tas_malloc_fct(9,pt_fct_best);
    strcpy( p2, "tp 2" );

    p3 = (char *) tas_malloc_fct(5,pt_fct_best);
    strcpy( p3, "tp 3" );

    tas_free(p2);

    p4 = (char *) tas_malloc_fct(8,pt_fct_best);
    strcpy( p4, "systeme" );

// Affichage du tas
    afficher_tas();


    return 0;
}
