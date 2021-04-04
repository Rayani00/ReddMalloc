#include "Block.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Elle crée l'espace qui representera notre mémoire, elle retourne 0 en cas d'erreur 
 * la taille de l'espace alloué en cas de succées
 * @param size : La taille de notre espace mémoire
 * @return int 
 */
int initMemory(int size)
{
    if (memoire == NULL)
    {
        memoire = malloc(size);
        if (memoire == NULL)
        {
            return 0;
        }
        size_memoire = size;
        return size;
    }
    return 0;
}

/**
 * @brief Libére la mémoire, retourne la taille de mémoire libérée en cas de succées
 * -1 si la mémoire est inexistante (ou non allouée) 
 * 
 * @return int 
 */
int freeMemory()
{

    if (memoire == NULL)
    {
        return -1;
    }

    Block b = table_pages;
    while (b->suivant != NULL)
    {
        Block q = b->suivant;
        Adresse a = b->adresse;
        a = NULL;
        free(a);
        b = NULL;
        free(b);
        b = q;
    }
    memoire = NULL;
    free(memoire);
    table_pages = NULL;
    free(table_pages);
    return size_memoire;
}

/**
 * @brief Alloue size de mémoire
 * 
 * @param size : La taille de la mémoire à allouer
 * @return void* l'adresse du block alloué
 */
void *myAlloc(int size)
{
    if (table_pages == NULL)
    {

        if (memoire == NULL)
        {
            printf("########################\n");
            puts("Mémoire non initialisée.");
            printf("########################\n\n");
            return NULL;
        }
        if (size > size_memoire)
        {
            printf("########################\n");
            puts("Taille demandée supérieure a taille de mémoire");
            printf("########################\n\n");
            return NULL;
        }
        createTable(memoire, size, true);
        printf("########################\n");
        printf("---Table des pages initialisée---\nTaille du block : %d octets.\nAdresse du block : %p.\n------------------------\n", size, memoire);
        printf("########################\n\n");
        return memoire;
    }
    Block premier = NULL;
    int n = 0;
    int min;

    /* chercher le premier block pouvant contenir la taille demandée */
    for (Block b = table_pages; b != NULL; b = b->suivant)
    {
        if (b->est_occupe == false && b->size >= size)
        {
            min = b->size;
            premier = b;
        }
    }

    /* Si on ne trouve pas le block alors on le crée
    on doit trouver l'adresse stockée dans le dernier block ainsi que sa taille
    L'adresse mémoire retournée correspondra à l'adresse contenue dans le dernier block+sa_taille*/
    if (premier == NULL)
    {
        Block dernier = getDernier();
        printf("########################\n");
        printf("Aucun block existant ne peut contenir cette taille.\nNouveau block crée :\nAdresse : %p\nTaille : %d\n", dernier->adresse + dernier->size, size);
        insertBlock(dernier->adresse + dernier->size, size, true);
        printf("########################\n\n");
        return dernier->adresse + dernier->size;
    }

    /* Application de l'algorithme best fit */
    Block best = NULL;
    for (Block b = premier; b != NULL; b = b->suivant)
    {
        if (b->est_occupe == false && b->size >= size && b->size < min)
        {
            best = b;
        }
    }

    /* s'il y a un seul block qui peut contenir la taille demandée */
    if (best == NULL)
    {
        best = premier;

        /* si la taille du block == à celle qu'on demande à allouer */
        if (size == best->size)
        {
            printf("########################\n");
            printf("Adresse du meilleur block trouvé : %p\n", best->adresse);
            printf("Le block convient exactement à la taille demandé ( %d )\n", size);
            printf("########################\n\n");
            best->est_occupe = true;
            return best->adresse;
        }

        /* sinon on divise le block existant en 2 : 1=taille  */
        int nv_size = best->size - size;
        int b_size = best->size;
        best->size = size;
        best->est_occupe = true;

        Block nv = malloc(sizeof(Block));
        Adresse nvad = malloc(sizeof(Adresse));
        nvad = best->adresse + best->size;
        nv->adresse = nvad;
        nv->size = nv_size;
        nv->est_occupe = false;
        nv->suivant = best->suivant;
        best->suivant = nv;
        printf("########################\n");
        printf("Le meilleur block trouvé\nTaille : %d  Adresse : %p\n\n", b_size, best->adresse);
        printf("Le block a été divisé en 2.\nBlock non occupé : \nTaille : %d Adresse : %p\n", nv->size, nv->adresse);
        printf("########################\n\n");
        return best->adresse;
    }
}

/**
 * @brief Libére l'espace mémoire de l'adresse passée en parametre
 * 
 * @param adress 
 * @return int retourne la taille liberée en cas de reussite, -1 en cas d'échec.
 */
int myFree(void *adress)
{

    /* 
    1- cas on de mande a liberer une zone non alloué-> erreur
    2- cas on demande a liberer une zone deja liberer-> erreur
    3- on libere{
        fin -> on supprime
        deb/mil -> bloc->est_occupe = false
    }
    */

    Block dernier = getDernier();

    /* Hors memoire physique */
    if ((char *)adress < memoire || (char *)adress > memoire + size_memoire)
    {
        printf("########################\n");
        puts("Adresse inexistante dans la memoire.");
        printf("Debut de la memoire : %p\nFin de la memoire : %p\n", memoire, memoire + size_memoire);
        printf("Adresse du block que vous voulez libérer : %p\n", adress);
        printf("########################\n\n");
        return -1;
    }

    /* Hors table des pages */
    if (adress < table_pages->adresse || adress > getDernier()->adresse)
    {
        printf("########################\n");
        puts("Adresse déja liberée ou Out of bounds \n");
        printf("########################\n\n");
        return -1;
    }

    /* 1er Block */
    if (adress == memoire)
    {
        if (table_pages->est_occupe == false)
        {
            printf("########################\n");
            puts("Block déjà liberé");
            printf("########################\n\n");
            return -1;
        }
        printf("########################\n");
        printf("Le block se situant à l'adresse  : %p, de taille %d vient d'être liberé.\n\n", table_pages->adresse, table_pages->size);
        printf("########################\n\n");
        table_pages->est_occupe = false;
        return table_pages->size;
    }

    /* Block precedent occupé */
    Block b = getBlock(adress);
    if (b == NULL)
    {
        printf("########################\n");
        printf("Erreur\n");
        printf("########################\n\n");
        return -1;
    }
    if (getPrecedent(b)->est_occupe == true)
    {
        printf("########################\n");
        printf("Le block se situant à l'adresse : %p, de taille %d vient d'être liberé.\n", adress, b->size);
        printf("########################\n\n");
        b->est_occupe = false;
        return b->size;
    }

    Block prec = getPrecedent(b);
    prec->size = prec->size + b->size;
    prec->suivant = b->suivant;
    Adresse ad_r = b->adresse;
    int siz = b->size;
    printf("########################\n");
    printf("Le block à l'adresse : %p, de taille %d vient d'être recuperé et fusionné avec son précedent.\n", b->adresse, b->size);
    printf("Un espace total de %d à l'adresse %p vient d'être recuperé.\n\n", prec->size, prec->adresse);

    ad_r = NULL;
    free(ad_r);
    b = NULL;
    free(b);

    Block suiv_suiv = prec->suivant;

    if (suiv_suiv == NULL || suiv_suiv->est_occupe == true)
    {
        printf("########################\n\n");
        return siz;
    }

    prec->size = prec->size + suiv_suiv->size;
    Adresse ad_suiv = suiv_suiv->adresse;
    ad_suiv = NULL;
    free(ad_suiv);
    prec->suivant = suiv_suiv->suivant;
    suiv_suiv = NULL;
    free(suiv_suiv);
    printf("Le block à l'adresse : %p, taille : %d, vient d'être liberé et fusionné avec son suivant\n", adress, siz);
    printf("Taille espace total : %d\nAdresse : %p\nStatut : recuperé\n", prec->size, prec->adresse);
    printf("########################\n\n");
    return siz;
}

void bilan()
{

    if (memoire == NULL)
    {
        printf("Mémoire non initialisée.\n");
        return;
    }

    if(table_pages == NULL){
        printf("La table des pages non initialisée\n");
        return; 
    }
    void *debut = table_pages->adresse;
    ;
    void *fin;
    int nb_b_occ = 0;
    int nb_b_lib = 0;
    int nb_b_tot = 0;
    int size_libre = 0;
    int size_occ = 0;

    Block b = table_pages;
    while (b != NULL)
    {
        if (b->est_occupe == true)
        {
            size_occ += b->size;
            nb_b_occ += 1;
        }
        else
        {
            size_libre += b->size;
            nb_b_lib += 1;
        }
        nb_b_tot += 1;
        fin = b;
        b = b->suivant;
    }
    printf("Adresse début de mémoire : %p\nAdresse fin de mémoire : %p\nTaille de mémoire : %d octets\nNombre total de blocks : %d\nBlocks occupés : %d\nBlocks libres : %d\nEspace libre : %d octets\nEspace occupé : %d octets\n\n", debut, fin, size_memoire, nb_b_tot, nb_b_occ, nb_b_lib, size_libre, size_occ);
}
