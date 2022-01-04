/// \file merkleroot.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date février 2021
/// \brief Partie traitement consacré au merkle root

#include "annexe.h"

BYTE ** simplifyTx(int numberofTx, doubleLinkedTransaction *listofTx)
/// \brief Conversion de la chaîne de chaque transaction sous forme de hash
/// \param[in] numberofTx Nombre de transactions
/// \param[in] listofTx Tableau contenant la chaîne de caractères de chaque transaction
/// \returns Tableau contenant la chaîne de caractères hashée de chaque transaction
  {

    BYTE ** simplifiedListofTx = malloc(  (numberofTx==0?1:numberofTx)* sizeof(BYTE*));

    for (int i = 0; i < numberofTx; i++){

      simplifiedListofTx[i] = malloc(SHA256_BLOCK_SIZE *2* sizeof(BYTE) +1);

      sha256ofString(listofTx[i]->tx,simplifiedListofTx[i]);

    }
    return simplifiedListofTx;
  }


BYTE* getRoot( BYTE** listofTx, int numberofTx)
/// \brief Génération du merkle root
/// \param[in] listofTx Tableau contenant la chaîne de caractères hashée de chaque transaction
/// \param[in] numberofTx Nombre de transactions
/// \returns Merkle Root sous forme d'une chaîne de caractères généré
  {
    int newSizeofList = numberofTx;

    while (newSizeofList != 1)
    {
      int indexofWriting = 0, indexofReading=0;

      while (indexofReading<newSizeofList-1)
      {

        BYTE* concatenation = malloc(SHA256_BLOCK_SIZE*4+1);

        sprintf((char *)concatenation, "%s%s", listofTx[indexofReading], listofTx[indexofReading+1]);

        sha256ofString(concatenation, listofTx[indexofWriting]);

        free(concatenation);  concatenation = NULL;

        indexofReading+=2;
        indexofWriting++;
      }

      if (newSizeofList%2 == 1)
      {
        listofTx[indexofWriting] = listofTx[newSizeofList-1];
        indexofWriting += 1;
      }

      while (indexofWriting < newSizeofList)
      {
        listofTx[indexofWriting] = NULL;
        indexofWriting++;
      }
      newSizeofList = (newSizeofList%2==0? newSizeofList/2 : (newSizeofList+1)/2 );
    }
    return listofTx[0];
  }
