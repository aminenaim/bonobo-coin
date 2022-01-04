/// \file transaction.c
/// \author Axel Delas, Amine Naim, Mamdou Djibo
/// \date février 2021
/// \brief Partie traitement consacré aux transactions

#include "annexe.h"

int indMinerRandom() {
/// \brief Indice random d'un mineur
/// \returns Indice random compris entre [0, 10];
  int random = rand()%MAX_USERS;

  return random;
}

int nbTransactionsRandom(int nbMaxofTx) {
/// \brief Nombre random
/// \param[in] nbMaxofTx Nombre de transactions maximum dans un bloc choisi par l'utilisateur
/// \returns Nombre de transactions random compris entre ]0, nbMaxofTx]
  int random;

  do {
    random=rand()%nbMaxofTx+1;
  } while (random==0);

  return random;
}

int indSenderReceiverRandom() {
/// \brief Indice random d'un mineur
/// \param[in] nbUsers Nombre d'utilisateurs
/// \returns Indice random compris entre [0, 10]
  int random = rand()%MAX_USERS;

  return random;
}

int nbAmountRandom() {
/// \brief Montant random
/// \returns Montant random compris entre ]0, 5000000000] (50 Bnb)
  int random;

  do {
    random=rand()%MAX_AMOUNT+1;
  } while (random==0);

  return random;
}

BYTE** initListofUsers(int nbUsers) {
/// \brief Génération tableau contenant la chaîne de chaque utilisateur
/// \param[in] nbUsers Nombre d'utilisateurs
/// \returns Tableau contenant la chaîne de chaque utilisateur généré
  BYTE **listofUsers = malloc(nbUsers * sizeof(BYTE*));

  listofUsers[0] = malloc(SIZE_CREATOR * sizeof(BYTE) +1);
  listofUsers[0] = (BYTE*)"Creator";

  for (int i=1; i<nbUsers; i++) {
    listofUsers[i] = malloc(SIZE_USER * sizeof(BYTE) +1);
    sprintf((char*)listofUsers[i], "user%d", i);
  }

  return listofUsers;
}

void pushBackTx(doubleLinkedBlock block, doubleLinkedTransaction newTx) {
/// \brief Ajout d'une transaction dans un bloc
/// \param[in] block Bloc dans lequel ajouter la transaction
/// \param[in] newTx Transaction à ajouter dans le bloc
  int index=newTx->index;

  /* Cas où le bloc ne contient aucune transaction */
  if (index==0) {
    /* Connectiques next et prev de la sentinelle sur la première transaction */
    block->sentinelTx->prevTx = newTx;
    block->sentinelTx->nextTx = newTx;
    /* Connectiques next et prev de la première transaction */
    newTx->prevTx = newTx;
    newTx->nextTx = newTx;
    /* Ajout de la première transaction dans le bloc */
    block->listOfTransactions[index] = newTx;
  }
  /* Cas où le bloc contient au moins une transaction */
  else {
    /* Nouvelles connectiques next et prev de l'ancienne dernière transaction du bloc */
    newTx->nextTx = block->sentinelTx;
    newTx->prevTx = newTx->nextTx->prevTx;
    /* Ajout de la première transaction dans le bloc */
    block->listOfTransactions[index] = newTx;
    /* Nouvelle connectique next de l'ancienne dernière transaction du bloc et nouvelle connectique prev de la sentinelle */
    block->listOfTransactions[index]->prevTx->nextTx = newTx;
    block->listOfTransactions[index]->nextTx->prevTx = newTx;
  }

  block->numberOfTransactions++;
}

doubleLinkedTransaction getTx(BYTE **listofUsers, int index) {
/// \brief Génération d'une transaction
/// \param[in] listofUsers Tableau contenant la chaîne de chaque utilisateur généré
/// \param[in] index Indice associé à la transaction générée
/// \returns Transaction générée
  int indSender = indSenderReceiverRandom();
  int indReceiver = indSenderReceiverRandom();
  long int amount = nbAmountRandom();

  doubleLinkedTransaction newTx = malloc(sizeof(struct s_doubleLinkedTransaction));
  newTx->index = index;
  newTx->sender = listofUsers[indSender];
  newTx->receiver = listofUsers[indReceiver];
  newTx->amount = amount;
  newTx->tx = malloc(SIZE_SIMPLIFIED_TXO * sizeof(BYTE) +1);

  sprintf((char*)newTx->tx, "Source %s-Destination : %s %ld", newTx->sender,
                                                              newTx->receiver,
                                                              newTx->amount);

  return newTx;
}

doubleLinkedTransaction getTxHelicopter(BYTE **listofUsers, int index) {
/// \brief Génération d'une transaction pendant la phase helicopter money : elles contiennent toutes un montant équivalent à 50 Bnb
/// \param[in] listofUsers Tableau contenant la chaîne de chaque utilisateur généré
/// \param[in] index Indice associé à la transaction générée
/// \returns Transaction associée à la phase helicopter money générée
  doubleLinkedTransaction newTx = malloc(sizeof(struct s_doubleLinkedTransaction));
  newTx->index = 0;
  newTx->sender = (BYTE*)"Creator";
  newTx->receiver = listofUsers[index];
  newTx->amount = MAX_AMOUNT;

  newTx->tx = malloc(SIZE_SIMPLIFIED_TXO * sizeof(BYTE) +1);
  sprintf((char*)newTx->tx, "Source %s-Destination : %s %ld", newTx->sender,
                                                              newTx->receiver,
                                                              newTx->amount);

  return newTx;
}

int nbofTotalTx(blockchain blockchain) {
/// \brief Nombre total de transactions dans la blockchain
/// \param[in] blockchain Blockchain contenant les blocs
/// \returns Entier qui est le nombre total de transactions dans la blockchain
  int nbofTx=0;

  for (int i=0; i<blockchain->numberOfBlocks; i++)
    nbofTx += blockchain->arrayofBlocks[i]->numberOfTransactions;

  return nbofTx;
}
