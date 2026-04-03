<<<<<<< Updated upstream
cc = gcc

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

%.o : %.c
	$(cc) -o $@ -c $<


simu.exe : test_simulateur.o simulateur.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o
	$(cc) test_simulateur.o simulateur.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o -o simu.exe -L. -lftd2xx


usb.exe : main_usb.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o
	$(cc) main_usb.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o -o usb.exe -L. -lftd2xx

=======

simu: test_simu.exe
usb: test_usb.exe



# 1. L'EDITION DE LIENS (Creation des executables finaux)


# (Assemblage les .o avec test_simulateur.o ET simulateur.o)
test_simu.exe : test_simulateur.o simulateur.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o
	gcc test_simulateur.o simulateur.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o -o test_simu.exe -L. -lftd2xx

# Pour l'USB (Assemblage les .o avec main_usb.o)
test_usb.exe : main_usb.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o
	gcc main_usb.o commande.o consigne.o regulation.o releve.o visualisationC.o visualisationT.o -o test_usb.exe -L. -lftd2xx


# 2. LA COMPILATION (Creation des fichiers objets .o)

test_simulateur.o : test_simulateur.c
	gcc -c test_simulateur.c -o test_simulateur.o


main_usb.o : main_usb.c
	gcc -c main_usb.c -o main_usb.o


commande.o : commande.c
	gcc -c commande.c -o commande.o

consigne.o : consigne.c
	gcc -c consigne.c -o consigne.o

regulation.o : regulation.c
	gcc -c regulation.c -o regulation.o

releve.o : releve.c
	gcc -c releve.c -o releve.o

visualisationC.o : visualisationC.c
	gcc -c visualisationC.c -o visualisationC.o

visualisationT.o : visualisationT.c
	gcc -c visualisationT.c -o visualisationT.o

simulateur.o : simulateur.c
	gcc -c simulateur.c -o simulateur.o


# 3. LE NETTOYAGE
>>>>>>> Stashed changes
clean :
	del /S /Q *.o *.exe .verrouData 2>nul

.PHONY: clean