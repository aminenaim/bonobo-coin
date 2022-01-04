/// \file block.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date janvier 2021
/// \brief Partie traitement consacré aux blocs

#include "annexe.h"

doubleLinkedBlock initGenesisBlock(int difficulty) {
/// \brief Génération du bloc génésis
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \returns Bloc génésis généré
  doubleLinkedBlock genesisBlock = malloc(sizeof(struct s_doubleLinkedBlock));
  genesisBlock->listOfTransactions = malloc(sizeof(struct s_doubleLinkedTransaction));
  genesisBlock->sentinelTx = malloc(sizeof(struct s_doubleLinkedTransaction));
  genesisBlock->index = 0;
  genesisBlock->previousBlockHash = (BYTE*)"0000000000000000000000000000000000000000000000000000000000000000";
  genesisBlock->numberOfTransactions = 1;
  genesisBlock->timestamp = NULL;

  genesisBlock->listOfTransactions[0] = malloc(sizeof(struct s_doubleLinkedTransaction));
  genesisBlock->listOfTransactions[0]->tx = (BYTE*)"0";

  genesisBlock->merkleRoot = getRoot(simplifyTx(genesisBlock->numberOfTransactions, genesisBlock->listOfTransactions), 1);
  genesisBlock = mining(genesisBlock, difficulty);

  return genesisBlock;
}

void pushBackBlock(blockchain blockchain, doubleLinkedBlock newBlock) {
/// \brief Ajout d'un bloc dans la blockchain
/// \param[in] blockchain Blockchain où ajouter le bloc
/// \param[in] newBlock Bloc à ajouter dans la blockchain
  int index=blockchain->numberOfBlocks;

  /* Cas où on ajoute le génésis à la blockchain donc la blockchain est vide */
  if (index == 0) {
    /* Connectiques next et prev de la sentinelle  sur le bloc génésis */
    blockchain->sentinelBlock->prevBlock = newBlock;
    blockchain->sentinelBlock->nextBlock = newBlock;
    /* Connectiques next et prev du bloc génésis */
    newBlock->prevBlock = newBlock;
    newBlock->nextBlock = newBlock;
    /* Ajout du bloc dans la blockchain */
    blockchain->arrayofBlocks[index] = newBlock;
  }
  /* Cas où on a déjà ajouté le génésis donc la blockchain n'est pas vide */
  else {
    /* Nouvelles connectiques next et prev du nouveau dernier bloc de la blockchain */
    newBlock->nextBlock = blockchain->sentinelBlock;
    newBlock->prevBlock = newBlock->nextBlock->prevBlock;
    /* Ajout du bloc dans la blockchain */
    blockchain->arrayofBlocks[index] = newBlock;
    /* Nouvelle connectique next de l'ancien dernier bloc de la blockchain et nouvelle connectique prev de la sentinelle */
    blockchain->arrayofBlocks[index]->prevBlock->nextBlock = newBlock;
    blockchain->arrayofBlocks[index]->nextBlock->prevBlock = newBlock;
  }

  blockchain->numberOfBlocks++;
}

doubleLinkedBlock getBlock(BYTE *previousBlockHash, int index, int nbTransactions) {
/// \brief Génération d'un bloc
/// \param[in] previousBlockHash Hash du bloc précédent
/// \param[in] index Indice associé à ce nouveau bloc
/// \param[in] nbTransactions Nombre de transactions que peut contenir le bloc
/// \returns Bloc généré
  doubleLinkedBlock newBlock = malloc(sizeof(struct s_doubleLinkedBlock));

  newBlock->listOfTransactions = malloc(nbTransactions * sizeof(struct s_doubleLinkedTransaction));
  newBlock->sentinelTx = malloc(sizeof(struct s_doubleLinkedTransaction));
  newBlock->index = index;
  newBlock->timestamp = NULL;

  newBlock->previousBlockHash = previousBlockHash;
  newBlock->numberOfTransactions = 0;

  return newBlock;
}