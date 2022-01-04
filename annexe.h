#ifndef ANNEXE_H
#define ANNEXE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "sha256_utils.h"
#include "sha256.h"

#define OK 0
#define MAX_NB_TRANSACTIONS 10  // Nombre de transactions max à générer dans un bloc
#define MAX_AMOUNT 5000000000   // Montant maximum pour une transaction (50 satoBnb)
#define SIZE_USER 9             // Taille allocation chaîne de caractères "user" + int
#define SIZE_CREATOR 7          // Taille allocation chaîne de caractères "Creator"
#define SIZE_SIMPLIFIED_TXO 50  // Taille allocation chaîne de caractère transaction simplifiée
#define SIZE_HASH 65            // Taille allocation hash
#define MAX_USERS 10            // Nombre d'utilisateurs max
#define SIZE_TIMESTAMP 26       // Taille de la timestamp en char
#define ERR_NBOFBLOCKS 1        // Erreur si le nombre de bloc(s) demandé n'est pas suffisant
#define ERR_DIFFICULTY 2        // Erreur si la difficulté demandée est inférieure à 0
#define ERR_NBOFBLOCKCHEATER 3  // Erreur si le numéro du bloc à supprimer est supérieur au nombre de blocs total
#define ERR_NBOFTXCHEATER 4     // Erreur si le numéro de la tx à supprimer est supérieur au nombre potentiel de tx total
#define ERR 5                   // Erreur paramètre(s)
#define ERR_NBOFMAXTX 6         // Erreur si le nombre max de tx est égal à 0 ou supérieur à 10

typedef unsigned char BYTE;

typedef struct s_doubleLinkedTransaction
   {
      int index;
      BYTE *tx;
      BYTE *sender;
      BYTE *receiver;
      long int amount;
      struct s_doubleLinkedTransaction *prevTx;
      struct s_doubleLinkedTransaction *nextTx;
   } *doubleLinkedTransaction;

typedef struct s_doubleLinkedBlock
   {
      int index;
      BYTE *timestamp;
      BYTE *previousBlockHash;
      int numberOfTransactions;
      doubleLinkedTransaction *listOfTransactions;
      doubleLinkedTransaction sentinelTx;
      BYTE *merkleRoot;
      BYTE *currentBlockHash;
      int nonce;
      struct s_doubleLinkedBlock *prevBlock;
      struct s_doubleLinkedBlock *nextBlock;
   } *doubleLinkedBlock;

typedef struct blockchain
   {
      int difficulty;
      int numberOfBlocks;
      doubleLinkedBlock *arrayofBlocks;
      doubleLinkedBlock sentinelBlock;
   } *blockchain;

/* ---------------------  MAIN  --------------------- */
char* getTimeStamp();
void freesParty(blockchain *newBlockchain, BYTE **listofUsers, int nbUsers);
blockchain inflationPhase(int nbofBlocks, blockchain blockchain, BYTE **listofUsers, int nbMaxofTx);
blockchain noInflationPhase(int nbofBlocks, blockchain blockchain, BYTE **listofUsers, int nbMaxofTx);
void helpMessage(void);

/* --------------------- BLOCK --------------------- */
doubleLinkedBlock initGenesisBlock(int difficulty);
void pushBackBlock(blockchain blockchain, doubleLinkedBlock block);
doubleLinkedBlock getBlock(BYTE *previousBlockHash, int index, int nbTransactions);

/* --------------------- BLOCKCHAIN --------------------- */
blockchain createBlockchain(doubleLinkedBlock genesisBlock, int difficulty, int nbBlocks);
void displayBlockchain(blockchain bc);

/* --------------------- TRANSACTION --------------------- */
int indMinerRandom();
int nbTransactionsRandom(int nbUsers);
int indSenderReceiverRandom();
int nbAmountRandom();
BYTE **initListofUsers(int nbUsers);
void pushBackTx(doubleLinkedBlock block, doubleLinkedTransaction newTx);
doubleLinkedTransaction getTx(BYTE **listofUsers, int index);
int nbofTotalTx(blockchain blockchain);

/* --------------------- MERKLE --------------------- */
BYTE *getRoot( BYTE** listofTx, int numberofTx);
BYTE **simplifyTx(int numberofTx, doubleLinkedTransaction *listofTx);

/* --------------------- MINING --------------------- */
bool hashValidation(BYTE *hashAttempt, int difficulty);
doubleLinkedBlock mining(doubleLinkedBlock block, int difficulty);
doubleLinkedTransaction getTxHelicopter(BYTE **listofUsers, int index);

/* --------------------- CHEATER --------------------- */
blockchain cheaterofBlock(blockchain blockchain, int blockNumber);
blockchain cheaterofTx(blockchain blockchain, int indexofBlock, int indexofTx);

/* ------------------- VALIDATION ------------------- */
bool testMining(doubleLinkedBlock block);
bool testRoot(doubleLinkedBlock block);
bool testNeighbor(doubleLinkedBlock currentBlock, BYTE* previousHash);
bool firstValidation(blockchain blockchain);
bool secondValidation(blockchain blockchain);

/* ------------------- BEACON ------------------- */
bool has_h_beacon(int argc, char **argv);
bool has_c_beacon(int argc, char **argv);
bool is_b_beacon(int i, char **argv);
bool is_d_beacon(int i, char **argv);
bool is_c_beacon(int i, char **argv);
bool is_t_beacon(int i, char **argv);

#endif
