#ifndef __BLOCK__H__
#include <stdbool.h>

/* Structures */
typedef void *Adresse;
typedef struct block
{
    int size;
    Adresse adresse;
    bool est_occupe;
    struct block *suivant;
} * Block;

/* Prototypes */
void createTable(void *adresse, int size, bool occupe);
void insertBlock(void *adresse, int size, bool occupe);
void suppBlock(Block block);
Adresse getBlockIndice(int indice);
Block getBlock(void *adress);
Block getPrecedent(Block block);
Block getDernier();

/* Tools */
char *memoire;
Block table_pages;
int size_memoire;
#endif
