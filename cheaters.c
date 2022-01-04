/// \file cheaters.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date février 2021
/// \brief Partie traitement consacré aux cheaters

#include "annexe.h"

blockchain cheaterofBlock(blockchain blockchain, int indexofBlock)
/// \brief Cheater de bloc
/// \details Supprime le numéro de bloc donné puis recalcule les hash de tous les blocs suivant pour effacer toute trace avec affichage du temps de calcul final
/// \param[in] blockchain Blockchain contenant les blocs
/// \param[in] indexofBlock Indice du bloc à supprimer
/// \returns Blockchain avec le bloc supprimé et les hash recalculés
   {
      if (indexofBlock <= 0 || blockchain->numberOfBlocks < indexofBlock){
         fprintf(stderr,"\nErreur Lancement Cheater de Bloc : bloc demandé inexistant.\n");
         return blockchain;
      }
      printf("\nLancement Cheater de Bloc sur le bloc n°%d\n", indexofBlock);

      clock_t begin = clock();

      blockchain->arrayofBlocks[indexofBlock]->nextBlock->prevBlock = blockchain->arrayofBlocks[indexofBlock]->prevBlock;
      blockchain->arrayofBlocks[indexofBlock]->prevBlock->nextBlock = blockchain->arrayofBlocks[indexofBlock]->nextBlock;

      blockchain->numberOfBlocks -= 1;

      while ( indexofBlock < blockchain->numberOfBlocks){

         memcpy(blockchain->arrayofBlocks[indexofBlock], blockchain->arrayofBlocks[indexofBlock+1],sizeof *(blockchain->arrayofBlocks[indexofBlock+1]) );

         blockchain->arrayofBlocks[indexofBlock]->index -= 1;

         blockchain->arrayofBlocks[indexofBlock]->previousBlockHash = blockchain->arrayofBlocks[indexofBlock-1]->currentBlockHash;

         printf("[Bloc %d] Reminage\n", blockchain->arrayofBlocks[indexofBlock]->index);

         blockchain->arrayofBlocks[indexofBlock] = mining(blockchain->arrayofBlocks[indexofBlock], blockchain->difficulty);

         indexofBlock++;
      }

      free(blockchain->arrayofBlocks[indexofBlock]);

      double timeofExecution = (double)(clock() - begin) / CLOCKS_PER_SEC;

      printf("TEMPS D'EXECUTION CHEATER DE BLOC : %lf sec.\n", timeofExecution);
      return blockchain;
   }


blockchain cheaterofTx(blockchain blockchain, int indexofBlock, int indexofTx)
/// \brief Cheater de transaction
/// \details Supprime le numéro de transaction donné puis recalcule les hash de tous les blocs suivant pour effacer toute trace avec affichage du temps de calcul final
/// \param[in] blockchain Blockchain contenant les blocs
/// \param[in] indexofBlock Indice du bloc à supprimer
/// \param[in] indexofTx Indice de la transaction à supprimer
/// \returns Blockchain avec la transaction supprimée du numéro bloc donné avec les hash recalculés
  {
     clock_t begin = clock();
     doubleLinkedTransaction deletedTx = blockchain->arrayofBlocks[indexofBlock]->listOfTransactions[indexofTx];

     if (indexofTx >= 0 && indexofTx<=blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions-1)
     {
        deletedTx->nextTx->prevTx = deletedTx->prevTx;
        deletedTx->prevTx->nextTx = deletedTx->nextTx;
     }
     else
     {
        fprintf(stderr, "\nErreur Cheater de Transaction : transaction ou bloc inexistant\n");
        return blockchain;
     }
     printf("\nLancement Cheater de Transaction sur le bloc n°%d et la transaction n°%d\n", indexofBlock, indexofTx);

     blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions -= 1;

     if (blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions==0)
     {
        blockchain->arrayofBlocks[indexofBlock]->merkleRoot = (BYTE*)"0000000000000000000000000000000000000000000000000000000000000000";
     }
     else
     {
        for (int i=indexofTx; i<blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions-1; i++)
        {
           memcpy(blockchain->arrayofBlocks[indexofBlock]->listOfTransactions[i], blockchain->arrayofBlocks[indexofBlock]->listOfTransactions[i+1], sizeof * (blockchain->arrayofBlocks[indexofBlock]->listOfTransactions[i+1]));
        }

        blockchain->arrayofBlocks[indexofBlock]->merkleRoot = getRoot(simplifyTx(blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions, blockchain->arrayofBlocks[indexofBlock]->listOfTransactions), blockchain->arrayofBlocks[indexofBlock]->numberOfTransactions);
     }

     while (indexofBlock < blockchain->numberOfBlocks)
     {

        memcpy(blockchain->arrayofBlocks[indexofBlock]->previousBlockHash, blockchain->arrayofBlocks[indexofBlock]->prevBlock->currentBlockHash, SIZE_HASH);

        printf("[Bloc %d] Reminage\n", indexofBlock);

        blockchain->arrayofBlocks[indexofBlock] = mining(blockchain->arrayofBlocks[indexofBlock], blockchain->difficulty);

        indexofBlock++;
     }

     double timeofExecution = (double)(clock() - begin) / CLOCKS_PER_SEC;

     printf("TEMPS D'EXECUTION CHEATER DE TRANSACTION : %lf sec.\n", timeofExecution);
     return blockchain;
  }
