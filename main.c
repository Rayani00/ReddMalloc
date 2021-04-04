/**
 * @file main.c
 * @author Houari, Haddadi, Kasmi
 * @brief Interface d'utilisateur
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Block.h"
#include "memoire.h"
#include <string.h>

FILE *conf;
int main(int argc, char const *argv[])
{

    if (argc > 1)
    {
        if (strcmp(argv[1], "-i") == 0)
        {
            printf("Mode intéractif : \n");
            int choix;
            printf("[1]- Allouer l'espace mémoire\n");
            printf("[2]- Liberer l'espace memoire\n");
            printf("[3]- Allouer un block\n");
            printf("[4]- Libérer un block\n");
            printf("[5]- Bilan\n");
            printf("[0]- Quitter\n");
            while (1)
            {
                scanf("%d", &choix);
                int mem;
                switch (choix)
                {
                case 1:
                    puts("Allocation de memoire en cours..");
                    int siz;
                    puts("Veuillez indiquer la taille de la mémoire : ");
                    scanf("%d", &siz);
                    if (initMemory(siz) == 0)
                    {
                        puts("Erreur d'initialisation de la mémoire.");
                    }
                    else
                    {
                        printf("Mémoire de %d octets allouée avec succés\n", siz);
                    }
                    break;

                case 2:
                    mem = freeMemory();
                    if (mem == -1)
                    {
                        puts("Erreur lors de la libération de la memoire.");
                    }
                    else
                    {
                        puts("Memoire librée avec succés.");
                    }
                    break;

                case 3:
                    puts("Allocation d'un block..");
                    puts("Veuillez indiquer la taille du block à allouer : ");
                    int taille;
                    scanf("%d", &taille);
                    myAlloc(taille);

                    break;

                case 4:
                    puts("Liberation du block: ");
                    int indice;
                    puts("Veuillez indiquer l'indice du block à désallouer : ");
                    scanf("%d", &indice);
                    myFree(getBlockIndice(indice));

                    break;

                case 5:
                    bilan();

                    break;

                case 0:
                    puts("Fin du programme.");
                    return 0;

                default:
                    puts("Veuillez choisir un nombre entre 0 et 4");
                }
                if (choix == 0)
                {
                    return 0;
                }
            }

            return 0;
        }

        if (strcmp(argv[1], "-f") == 0)
        {
            printf("Mode ligne de commande\n");
            printf("Mode d'utilisation :\n1- Initialiser la table de pages : initMemory(taille_de_memoire_a_allouer)\n2- Allouer de la mémoire : malloc(taille_de_memoire_a_allouer)\n3- Liberer de la mémoire : free(taille_de_memoire_a_liberer)\n4- Liberer la table de pages : deinitMemory()\n");
            if (argv[2] == NULL)
            {
                printf("Fichier de configuration manquant.\n");
                return 0;
            }
            char *fichier;
            strcpy(fichier, argv[2]);
            fichier[strlen(fichier) + 1] = 0;
            conf = fopen(fichier, "r");
            if (conf == NULL)
            {
                printf("Fichier introuvable.\n");
            }

            printf("Fichier ouvert avec succes.\n");

            char *line;
            size_t len;
            int nr;
            int i = 1;
            while (nr = getline(&line, &len, conf) != -1)
            {
                int siz;
                int ind;
                if (fscanf(conf, "malloc(%d)", &siz) == 1)
                {
                    myAlloc(siz);
                }

                if (fscanf(conf, "initMemory(%d)", &siz) == 1)
                {
                    initMemory(siz);
                }

                if (fscanf(conf, "free(%d)", &ind) == 1)
                {
                    myFree(getBlockIndice(ind));
                }

                if (fscanf(conf, "deinitMemory()"))
                {
                    printf("%d \n", freeMemory());
                }
            }
            fclose(conf);
            return 0;
        }
    }
    return 0;
}
