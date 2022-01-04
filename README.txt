README

   Bonobo Central Bank est un outil de simulation de creation monétaire avec économie de marché.
   Ce projet est réalisé dans le cadre de la validation de l'UE Projet S4.
   Nous développons plus bas l'utilisation de cet outil.

   Le calcul du sha256 se fait en utilisant le code fourni sur Moodle :
   sha256.h, sha256.c, sha256_utils.h, sha256_utils.c
   (initialement développé par Brat Conte et adapté par Mr Vincent DUGAT aux besoins du projet)

   ----------------------------------------------------------------------------------------------------------------

   Bonobo Central Bank is a tool to simulate monetary creation with all the dynamics related to the monetary system.
   Find out how to use this tool by reading the following page.

   The sha256 computation has been made using the code available on Moodle :
   sha256.h, sha256.c, sha256_utils.h, sha256_utils.c
   (originally coded by Brat Conte and adapted by Vincent DUGAT for the needs of this project)


NOTICE

   Usage : bc DIFFICULTY BLOCKS NBMAXTX [CHEATERS]                                                
   Generates blockchain with monetary creation.                                               

   -d difficulty         Hash difficulty to use
   -b blocks             Blocks number to generate                                        
   -t transactions       Max transactions number for each bloc
   -c cheaters           Activate or not cheaters : first int to choose block number, optionnal second int to choose transaction number

   Here is an example to copy/paste in the command line : 
   ./bc -d 4 -b 1000 -t 10 -c 800 