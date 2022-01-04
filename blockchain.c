/// \file blockchain.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date janvier 2021
/// \brief Partie traitement consacré à la blockchain

#include "annexe.h"

blockchain createBlockchain(doubleLinkedBlock genesisBlock, int difficulty, int nbBlocks) {
/// \brief Génération du corps de la blockchain
/// \param[in] genesisBlock Bloc génésis de la blockchain
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \param[in] nbBlocks Nombre de blocs à générer choisi par l'utilisateur
/// \returns Blockchain générée
  blockchain newBlockchain = malloc(sizeof(struct blockchain));
  newBlockchain->arrayofBlocks = malloc(nbBlocks * sizeof(struct s_doubleLinkedBlock));
  newBlockchain->sentinelBlock = malloc(sizeof(struct s_doubleLinkedBlock));
  newBlockchain->difficulty = difficulty;
  newBlockchain->numberOfBlocks = 0;

  pushBackBlock(newBlockchain, genesisBlock);

  return newBlockchain;
}

void displayBlockchain(blockchain blockchain)  {
/// \brief Affichage informations de tous les blocs de la blockchain
/// \param[in] blockchain Blockchain contenant les blocs
  doubleLinkedBlock block;

  for (int i=0; i<blockchain->numberOfBlocks; i++) {
    block = blockchain->arrayofBlocks[i];
    printf("\n---------------------------- Bloc n°%d -------------------------\n", block->index);
    printf("Timestamp : %s", block->timestamp);
    printf("Nb de transaction(s) : %d\n", block->numberOfTransactions);
    printf("Bloc hash précédent : %s\n", block->previousBlockHash);
    printf("Bloc hash : %s\n", block->currentBlockHash);
    printf("Merkleroot : %s\n", block->merkleRoot);
    printf("Nonce : %d\n", block->nonce);
    printf("-----------------------------------------------------------------\n");
  }
}
