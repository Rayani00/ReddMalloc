#ifndef __MEMOIRE__H__

/* Prototypes */
void *myAlloc(int size);
int myFree(void *adress);
int initMemory(int size);
int freeMemory();
void bilan();

/* Tools */
char *memoire;
Block table_pages;
int size_memoire;

#endif
