/// \file validations.c
/// \author Axel Delas, Amine Naim, Mamdou Djibo
/// \date février 2021
/// \brief Partie traitement consacré aux tests de cohésion des données

#include "annexe.h"

bool testMining(doubleLinkedBlock block)
/// \brief Vérification si le hash du bloc est correct
/// \param[in] block Bloc contenant le hash à tester
/// \returns Booléen True ou False
   {
      BYTE *blockHeader = malloc(SHA256_BLOCK_SIZE*5 + 1);
      BYTE *hash = malloc(SHA256_BLOCK_SIZE*2 + 1);

      sprintf((char*)blockHeader, "%d%s%s%s%d", block->index,
                                                block->timestamp,
                                                block->merkleRoot,
                                                block->previousBlockHash,
                                                block->nonce);
      sha256ofString(blockHeader, hash);

      free(blockHeader); blockHeader=NULL;

      return !memcmp(hash,block->currentBlockHash,SIZE_HASH);
   }

bool testRoot(doubleLinkedBlock block)
/// \brief Vérification si le merkle root du bloc est correct
/// \param[in] block Bloc contenant le merkle root à tester
/// \returns Booléen True ou False
   {
      return !memcmp(
               getRoot( simplifyTx(block->numberOfTransactions, block->listOfTransactions),
               block->numberOfTransactions),
               block->merkleRoot,
               SIZE_HASH);
   }

bool testNeighbor(doubleLinkedBlock currentBlock, BYTE* previousBlockHash)
/// \brief Vérification si le bloc contient correctement le hash du bloc précédent
/// \param[in] block Bloc contenant le hash du bloc précédent à tester
/// \param[in] previousBlockHash Hash du bloc précédent
/// \returns Booléen True ou False
   {
      return !memcmp(currentBlock->previousBlockHash, previousBlockHash, SIZE_HASH);
   }

bool firstValidation(blockchain blockchain)
/// \brief Vérification si le génésis et les hash contenus dans les blocs de la blockchain sont corrects
/// \param[in] blockchain Blockchain contenant les blocs avec les données à tester
/// \returns Booléen True ou False
   {
      printf("\nContrôle de cohérence Genesis & Hash\n");
      for (int i=0; i<blockchain->numberOfBlocks; i++)
      {
         if (!testMining(blockchain->arrayofBlocks[i]) ||
             (i == 0 ? !testNeighbor(blockchain->arrayofBlocks[i], (BYTE*)"0000000000000000000000000000000000000000000000000000000000000000") : !testNeighbor(blockchain->arrayofBlocks[i], blockchain->arrayofBlocks[i-1]->currentBlockHash)))
         {
            fprintf(stderr, "\terreur: incohérence detectée au bloc[%d].\n",i);
            return false;
         }
      }
      fprintf(stderr, "\tmessage: aucune erreur detectée.\n");
      return true;
   }

bool secondValidation(blockchain blockchain)
/// \brief Vérification les merkles roots contenus dans les blocs de la blockchain sont corrects
/// \param[in] blockchain Blockchain contenant les blocs avec les merkle roots à tester
/// \returns Booléen True ou False
   {
      printf("\nContrôle de cohérence des Merkle Roots\n");
      for (int i=0; i<blockchain->numberOfBlocks; i++)
      {
         //printf("ctrl of b[%d] with %d Tx and root:%s\n",i,blockchain->arrayofBlocks[i]->numberOfTransactions, blockchain->arrayofBlocks[i]->merkleRoot);
         if (blockchain->arrayofBlocks[i]->numberOfTransactions == 0 &&
             memcmp(blockchain->arrayofBlocks[i]->merkleRoot,"0000000000000000000000000000000000000000000000000000000000000000",65))
         {
            fprintf(stderr, "\terreur: incohérence trouvée au bloc[%d].\n",i);
            return false;
         }
         if (!testRoot(blockchain->arrayofBlocks[i])  )
         {
            fprintf(stderr, "\terreur: incohérence trouvée au bloc[%d].\n",i);
            return false;
         }
      }
      fprintf(stderr, "\tmessage: aucune erreur detectée.\n");
      return true;
   }
