# Conception d'un Système de Régulation Thermique
Projet réalisé à l'ISEN en 2026.

# Description
Développement en C d'un algorithme de régulation thermique (PID et Tout-Ou-Rien) pour contrôler la température d'une pièce.

Le projet comprend :
- la communication USB avec une carte STM32 Nucleo,
- l'extraction et le traitement bas niveau de trames issues d'un capteur,
- une architecture modulaire validée en simulation puis sur maquette physique,
- un mécanisme de synchronisation entre le code C et l'IHM Python.
- une interface de visualisation en Python,
  
# Technologies
- C
- Python
- STM32
- USB
- Makefile
- Bitwise
- Régulation PID

# Dépendances
- FTDI D2XX (ftd2xx) pour la communication USB
- La bibliothèque `ftd2xx.lib` est requise pour la compilation
  
  NB : Selon votre environnement, il peut être nécessaire d'installer les drivers FTDI correspondants.

# Exécution
Compilation avec Makefile :

make

./programme

Lancement de l’interface Python :
python gestiontemp.py

# Contexte
Projet académique réalisé dans le cadre de la formation ISEN.
