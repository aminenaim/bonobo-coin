/// \file main.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date janvier 2021
/// \brief Programme principal qui traite les balises utilisées par l'utilisateur

#include "annexe.h"

char* getTimeStamp() {
/// \brief Récupération de la timestamp
/// \returns Timestamp sous forme d'une chaîne de caractères
  time_t ltime;
  time(&ltime);
  return ctime(&ltime);
}

void freesParty(blockchain *newBlockchain, BYTE **listofUsers, int nbUsers) {
/// \brief Libération de toutes les mémoires allouées
/// \param[in] newBlockchain Blockchain contenant les blocs à libérer
/// \param[in] listofUsers Tableau contenant la chaîne de caractères de chaque utilisateur
/// \param[in] nbUsers Nombre d'utilisateurs
  for (int i=0; i<(*newBlockchain)->numberOfBlocks; i++) {
    for (int j=0; j<(*newBlockchain)->arrayofBlocks[i]->numberOfTransactions; j++)
      free((*newBlockchain)->arrayofBlocks[i]->listOfTransactions[j]);
    free((*newBlockchain)->arrayofBlocks[i]->currentBlockHash);
    free((*newBlockchain)->arrayofBlocks[i]->merkleRoot);
    free((*newBlockchain)->arrayofBlocks[i]);
  }
  free((*newBlockchain)->sentinelBlock);
  free(*newBlockchain);
  newBlockchain = NULL;

  free(listofUsers);
  listofUsers = NULL;
}

float satoToBnb(long int moneySupply) {
/// \brief Conversion satoBnb en Bnb
/// \param[in] moneySupply Masse monétaire en satoBnb
/// \returns Masse monétaire convertie
  float moneySupplyBnb = moneySupply/pow(10, 8);

  return moneySupplyBnb;
}

blockchain helicopereMoneyPhase(int nbUsers, BYTE **listofUsers, blockchain blockchain) {
/// \brief Phase helicopter money
/// \details Distribution de 50 Bnb par Creator à tous les utilisateurs
/// \param[in] nbUsers Nombre d'utilisateurs
/// \param[in] listofUsers Tableau contenant la chaîne de caractères de chaque utilisateur
/// \param[in] blockchain Blockchain où ajouter les blocs
/// \returns Blockchain avec les nouveaux blocs ajoutés
  int nbofTransactions, indUser;
  doubleLinkedBlock newBlock;
  doubleLinkedTransaction newTx;

  for (int i=1; i<=nbUsers; i++) {
    nbofTransactions = 1;
    indUser = i-1;

    newBlock = getBlock(blockchain->arrayofBlocks[i-1]->currentBlockHash, i, nbofTransactions);
    newTx = getTxHelicopter(listofUsers, indUser);

    pushBackTx(newBlock, newTx);

    newBlock->merkleRoot = getRoot(simplifyTx(newBlock->numberOfTransactions, newBlock->listOfTransactions), newBlock->numberOfTransactions);
    newBlock = mining(newBlock, blockchain->difficulty);

    pushBackBlock(blockchain, newBlock);
  }
  return blockchain;
}

blockchain inflationPhase(int nbofBlocks, blockchain blockchain, BYTE **listofUsers, int nbMaxofTx) {
/// \brief Phase de marché avec inflation
/// \details Les transactions sont entre les utilisateurs qui sont choisis au hasard
/// \param[in] nbofBlocks Nombre de blocs maximum à générer
/// \param[in] blockchain Blockchain où ajouter les blocs
/// \param[in] listofUsers Tableau contenant la chaîne de caractères de chaque utilisateur
/// \returns Blockchain avec les nouveaux blocs ajoutés
  int nbofTransactions;
  long int reward, moneySupply;
  doubleLinkedBlock newBlock;
  doubleLinkedTransaction newTx;

  reward = moneySupply = MAX_AMOUNT;

  while (blockchain->numberOfBlocks < nbofBlocks && reward > 0) {
    nbofTransactions = nbTransactionsRandom(nbMaxofTx);

    newBlock = getBlock(blockchain->arrayofBlocks[blockchain->numberOfBlocks-1]->currentBlockHash,
                        blockchain->numberOfBlocks,
                        nbofTransactions);

    for (int j=0; j<nbofTransactions; j++) {
      newTx = getTx(listofUsers, j);
      pushBackTx(newBlock, newTx);
    }

    newBlock->merkleRoot = getRoot(simplifyTx(newBlock->numberOfTransactions, newBlock->listOfTransactions), newBlock->numberOfTransactions);
    newBlock = mining(newBlock, blockchain->difficulty);

    pushBackBlock(blockchain, newBlock);

    /* Mise à jour de la récompense du mineur */
    if (blockchain->numberOfBlocks%20 == 0) {
      moneySupply += reward*blockchain->numberOfBlocks;
      reward /= 2;
      printf("Récompense : %ld satoBnb Masse monétaire : %ld (%f Bnb)\n", reward, moneySupply, satoToBnb(moneySupply));
    }
  }
  return blockchain;
}

blockchain noInflationPhase(int nbofBlocks, blockchain blockchain, BYTE **listofUsers, int nbMaxofTx) {
/// \brief Phase de marché sans inflation
/// \details Les transactions sont entre les utilisateurs qui sont choisis au hasard
/// \param[in] nbofBlocks Nombre de blocs maximum à générer choisi par l'utilisateur
/// \param[in] blockchain Blockchain où ajouter les blocs
/// \param[in] listofUsers Tableau contenant la chaîne de caractères de chaque utilisateur
/// \returns Blockchain avec les nouveaux blocs ajoutés
  int nbofTransactions;
  doubleLinkedBlock newBlock;
  doubleLinkedTransaction newTx;

  if (blockchain->numberOfBlocks < nbofBlocks) {
    printf("--- Phase de marché sans inflation ---\n");
    while (blockchain->numberOfBlocks < nbofBlocks) {
      nbofTransactions = nbTransactionsRandom(nbMaxofTx);

      newBlock = getBlock(blockchain->arrayofBlocks[blockchain->numberOfBlocks-1]->currentBlockHash,
                          blockchain->numberOfBlocks, nbofTransactions);

      for (int j=0; j<nbofTransactions; j++) {
        newTx = getTx(listofUsers, j);
        pushBackTx(newBlock, newTx);
      }

      newBlock->merkleRoot = getRoot(simplifyTx(newBlock->numberOfTransactions, newBlock->listOfTransactions), newBlock->numberOfTransactions);
      newBlock = mining(newBlock, blockchain->difficulty);

      pushBackBlock(blockchain, newBlock);
    }
  }
  return blockchain;
}

void blockchainWithNoCheaters(int difficulty, int nbofBlocks, int nbMaxofTx) {
/// \brief Blockchain sans utilisation de cheater
/// \details Passage par les différentes phases de la blockchain sans utilisation de cheater puis arrêt pour cause de cycle terminé ou arrêt brutal
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \param[in] nbofBlocks Nombre de blocs maximum à générer choisi par l'utilisateur
  int nbUsers=MAX_USERS;
  doubleLinkedBlock genesisBlock;
  blockchain newBlockchain;
  BYTE **listofUsers;

  printf("Difficulté : %d\nNombre de blocs : %d\nValeur initiale de récompense en SatoBnb : %ld\nMasse monétaire en satoBnb : %ld\n", difficulty,
                                                                                                                                      nbofBlocks,
                                                                                                                                      MAX_AMOUNT,
                                                                                                                                      MAX_AMOUNT);
  printf("\n--- Génésis ---\n");
  genesisBlock = initGenesisBlock(difficulty);

  newBlockchain = createBlockchain(genesisBlock, difficulty, nbofBlocks);

  listofUsers = initListofUsers(nbUsers);

  /* Phase d'helicoptere money : Distribution de 50 satoBnb à tous les utilisateurs */
  printf("--- Phase helicoptere money ---\n");
  newBlockchain = helicopereMoneyPhase(nbUsers, listofUsers, newBlockchain);

  /* Phase de marché avec inflation */
  printf("--- Phase de marché avec inflation ---\n");
  newBlockchain = inflationPhase(nbofBlocks, newBlockchain, listofUsers, nbMaxofTx);

  /* Potentielle phase de marché sans inflation si le nombre de blocs demandé n'est pas atteint */
  newBlockchain = noInflationPhase(nbofBlocks, newBlockchain, listofUsers, nbMaxofTx);

  printf("Il y a %d blocs dans la blockchain avec un total de %d transactions\n", newBlockchain->numberOfBlocks,
                                                                                  nbofTotalTx(newBlockchain)
                                                                                  );

  firstValidation(newBlockchain);
  secondValidation(newBlockchain);

  freesParty(&newBlockchain, listofUsers, nbUsers);
}

void blockchainWithCheaters(int difficulty, int nbofBlocks, int nbofBlockCheater, int nbMaxofTx, int nbofTxCheater) {
/// \brief Blockchain avec utilisation de(s) cheater(s)
/// \details Passage par les différentes phases de la blockchain avec utilisation de(s) cheater(s) puis arrêt pour cause de cycle terminé ou arrêt brutal
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \param[in] nbofBlocks Nombre de blocs maximum à générer choisi par l'utilisateur
  int nbUsers=MAX_USERS;
  doubleLinkedBlock genesisBlock;
  blockchain newBlockchain;
  BYTE **listofUsers;

  printf("Difficulté : %d\nNombre de blocs : %d\nValeur initiale de récompense en SatoBnb : %ld\nMasse monétaire en satoBnb : %ld\n", difficulty,
                                                                                                                                      nbofBlocks,
                                                                                                                                      MAX_AMOUNT,
                                                                                                                                      MAX_AMOUNT);
  printf("\n--- Génésis ---\n");
  genesisBlock = initGenesisBlock(difficulty);

  newBlockchain = createBlockchain(genesisBlock, difficulty, nbofBlocks);

  listofUsers = initListofUsers(nbUsers);

  /* Phase d'helicoptere money : Distribution de 50 satoBnb à tous les utilisateurs */
  printf("--- Phase helicoptere money ---\n");
  newBlockchain = helicopereMoneyPhase(nbUsers, listofUsers, newBlockchain);

  /* Phase de marché avec inflation */
  printf("--- Phase de marché avec inflation ---\n");
  newBlockchain = inflationPhase(nbofBlocks, newBlockchain, listofUsers, nbMaxofTx);

  /* Potentielle phase de marché sans inflation si le nombre de blocs demandé n'est pas atteint */
  newBlockchain = noInflationPhase(nbofBlocks, newBlockchain, listofUsers, nbMaxofTx);

  printf("Il y a %d blocs dans la blockchain avec un total de %d transactions\n", newBlockchain->numberOfBlocks,
                                                                                  nbofTotalTx(newBlockchain)
                                                                                  );

  if (nbofTxCheater)
    newBlockchain = cheaterofTx(newBlockchain, nbofBlockCheater, nbofTxCheater);
  newBlockchain = cheaterofBlock(newBlockchain, nbofBlockCheater);

  firstValidation(newBlockchain);
  secondValidation(newBlockchain);

  freesParty(&newBlockchain, listofUsers, nbUsers);
}

void helpMessage(void) {
/// \brief Message d'affichage si utilisation de la balise -h (-help)
  printf("Usage : bc DIFFICULTY BLOCKS NBMAXTX [CHEATERS]\n");
  printf("Generates blockchain with monetary creation.\n");
  printf("\n  -d difficulty\t\tHash difficulty to use\n");
  printf("  -b blocks\t\tBlocks number to generate\n");
  printf("  -t transactions\tMax transactions number for each bloc\n");
  printf("  -c cheaters\t\tActivate or not cheaters : first int to choose block number, optionnal second int to choose transaction number\n");
}

int main(int argc, char *argv[]) {
/// \brief Traitement des balises et des arguments donnés par l'utilisateur
/// \details Recherche de(s) balise(s) pointée(s) par des fonctions du header dh_bal.h
/// \param[in] argc Le nombre de paramètre(s) donné(s) à l'exécution du programme.
/// \param[in] argv Tableau contenant le(s) paramètre(s) donné(s) à l'exécution du programme.
/// \returns Code de retour 0 ou code erreur associé.
  srand(time(NULL));

  int nbofBlocks=0, difficulty=0, nbofBlockCheater=0, nbofTxCheater=0, nbMaxofTx=0;

  if (has_h_beacon(argc, argv))
    helpMessage();
  else {
    if (!has_c_beacon(argc, argv) && argc>1) {
      for (int i=1; i<argc; i++) {
        if (is_b_beacon(i, argv))
          nbofBlocks = atoi(argv[i+1]);
        else if (is_d_beacon(i, argv))
          difficulty = atoi(argv[i+1]);
        else if (is_t_beacon(i, argv))
          nbMaxofTx = atoi(argv[i+1]);
      }
      if (nbofBlocks < MAX_USERS) {
        fprintf(stderr, "Le nombre de bloc(s) demandé n'est pas suffisant\n");
        exit(ERR_NBOFBLOCKS);
      }
      else if (difficulty <= 0) {
        fprintf(stderr, "La difficulté choisie doit être strictement supérieure à 0\n");
        exit(ERR_DIFFICULTY);
      }
      else if (nbMaxofTx > MAX_NB_TRANSACTIONS || nbMaxofTx == 0) {
        fprintf(stderr, "Le nombre maximum de transactions dans un bloc est incorrect\n");
        exit(ERR_NBOFMAXTX);
      }
      blockchainWithNoCheaters(difficulty, nbofBlocks, nbMaxofTx);
    }
    else if (has_c_beacon(argc, argv)) {
      for (int i=1; i<argc; i++) {
        if (is_b_beacon(i, argv))
          nbofBlocks = atoi(argv[i+1]);
        else if (is_d_beacon(i, argv))
          difficulty = atoi(argv[i+1]);
        else if (is_c_beacon(i, argv)) {
          nbofBlockCheater = atoi(argv[i+1]);
          if (argv[i+2])
            nbofTxCheater = atoi(argv[i+2]);
        }
        else if (is_t_beacon(i, argv))
          nbMaxofTx = atoi(argv[i+1]);
      }
      if (nbofBlocks < MAX_USERS) {
        fprintf(stderr, "Le nombre de bloc(s) demandé n'est pas suffisant\n");
        exit(ERR_NBOFBLOCKS);
      }
      else if (difficulty <= 0) {
        fprintf(stderr, "La difficulté choisie doit être strictement supérieure à 0\n");
        exit(ERR_DIFFICULTY);
      }
      else if (nbofBlockCheater > nbofBlocks) {
        fprintf(stderr, "Le numéro du bloc voulant être supprimé est supérieur au nombre de blocs total demandé\n");
        exit(ERR_NBOFBLOCKCHEATER);
      }
      else if (nbMaxofTx > MAX_NB_TRANSACTIONS || nbMaxofTx == 0) {
        fprintf(stderr, "Le nombre maximum de transactions dans un bloc est incorrect\n");
        exit(ERR_NBOFMAXTX);
      }
      else if (nbofTxCheater) {
        if (nbofTxCheater > MAX_NB_TRANSACTIONS || nbofTxCheater > nbMaxofTx) {
          fprintf(stderr, "Le numéro de la transaction voulant être supprimée est supérieur au nombre de transactions maximum pour un bloc\n");
          exit(ERR_NBOFBLOCKCHEATER);
        }
      }
      blockchainWithCheaters(difficulty, nbofBlocks, nbofBlockCheater, nbMaxofTx, nbofTxCheater);
    }
    else {
      helpMessage();
    }
  }
  return OK;
}
