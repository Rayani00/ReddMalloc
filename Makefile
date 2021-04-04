all: clean go

OBJ = main.o Block.o memoire.o

go : $(OBJ)
	cc -o go $(OBJ)
	@echo "Make done"
	@echo "------------------------------------------"
	@echo "Mode interactif : ./go -i"
	@echo "Mode ligne de commandes : ./go -f nom_du_fichier.txt"
	@echo "En mode ligne de commandes commencez par un commentaire ex '# Ceci est un commentaire.' "

main.o : Block.h memoire.h
	cc -c main.c

Block.o :  Block.h
	cc -c Block.c

memoire.o : memoire.h Block.h
	cc -c memoire.c

clean : 
	rm -f *.o go
	@echo "Clean done"

