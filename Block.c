#include "Block.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Procedure qui initialise la table des pages lorsqu'on appelle pour 
 * la premiére fois la fonction myAllc()
 * 
 * @param adresse : La premiere adresse de la mémoire 
 * @param size : La taille du premier block, correspondant à la taille que l'on veut allouer initialement
 * @param occupe : Booléen qui est vrai lorsque le block est alloué, faux sinon
 */
void createTable(void *adresse, int size, bool occupe)
{
    Block bl = malloc(sizeof(Block));
    Adresse a = malloc(sizeof(Adresse));
    a = adresse;
    bl->adresse = a;
    bl->size = size;
    bl->est_occupe = occupe;
    bl->suivant = NULL;

    table_pages = bl;
}

/**
 * @brief Fonction qui crée un block dans la mémoire, et l'insere dans la tables des pages
 * 
 * @param adresse : L'adresse de l'espace que l'on vient d'allouer
 * @param size  : Taille du block à inserer
 * @param occupe  : Booléen qui est vrai lorsque le block est alloué, faux sinon
 */
void insertBlock(void *adresse, int size, bool occupe)
{

    Block block = malloc(sizeof(Block));
    Block b = table_pages;
    Adresse a = malloc(sizeof(Adresse));
    a = adresse;
    block->adresse = a;
    block->size = size;
    block->est_occupe = occupe;
    block->suivant = NULL;
    Block inter;
    for (Block bb = table_pages; bb != NULL; bb = bb->suivant)
    {
        inter = bb;
    }
    inter->suivant = block;
}

/**
 * @brief Recupére le block dans lequel l'adresse donnée est stocké 
 * 
 * @param adress ; Adresse du block
 * @return Block 
 */
Block getBlock(void *adress)
{
    for (Block bl = table_pages; bl != NULL; bl = bl->suivant)
    {
        if (bl->adresse == adress)
        {
            return bl;
        }
    }
    return NULL;
}

/**
 * @brief Donne l'adresse du block qui precede le block indiqué
 * 
 * @param block : Le block dont on cherche le predecesseur
 * @return Block 
 */
Block getPrecedent(Block block)
{
    Block b = table_pages;
    while (b->suivant != NULL)
    {
        if (b->suivant == block)
        {
            return b;
        }
        b = b->suivant;
    }
    puts("Block precedent non trouvé.");
    return NULL;
}

/**
 * @brief Recupere l'espace mémoire occupé par le block donné
 * 
 * @param block Le bloc dont on veut libérer l'éspace
 */
void suppBlock(Block block)
{
    Block b = block;
    Block c = getPrecedent(block);
    if (block->suivant == NULL)
    {
        b = NULL;
        free(b);
        c->suivant = NULL;
    }
    else
    {
        block->est_occupe = false;
    }
}

/**
 * @brief Pointe sur le dernier block de la table de pages
 * 
 * @return Block 
 */
Block getDernier()
{
    Block dernier = NULL;
    for (Block b = table_pages; b != NULL; b = b->suivant)
    {
        dernier = b;
    }
    return dernier;
}

Adresse getBlockIndice(int indice)
{

    int i = 0;
    Adresse adresse;
    for (Block b = table_pages; b != NULL; b = b->suivant)
    {
        if (i == indice)
        {
            adresse = b->adresse;
            break;
        }
        i++;
    }
    return adresse;
}