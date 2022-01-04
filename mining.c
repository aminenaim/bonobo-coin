/// \file mining.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date janvier 2021
/// \brief Partie traitement consacré au minage

#include "annexe.h"

bool hashValidation(BYTE *hashAttempt, int difficulty) {
/// \brief Vérification si le hash est valide
/// \details Si le hash contient un 0 de l'indice 0 à difficulty-1 alors le hash est correct, sinon il ne l'est pas
/// \param[in] hashAttempt Hash à vérifier
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \returns Booléen True ou False
  for (int i=0; i<difficulty; i++)
    if (memcmp(&hashAttempt[i], "0", 1) != 0)
      return false;

  return true;
}

doubleLinkedBlock mining(doubleLinkedBlock block, int difficulty) {
/// \brief Minage du hash et récupération des infos associées au bloc
/// \param[in] block Bloc à miner
/// \param[in] difficulty Difficulté choisie par l'utilisateur
/// \returns Bloc miné
  BYTE *blockHeader = malloc(SHA256_BLOCK_SIZE*5 + 1);
  BYTE *hash = malloc(SHA256_BLOCK_SIZE*2 + 1);
  int nonce = 0;
  BYTE *timestamp;

  if (!block->timestamp)
    timestamp = malloc(SIZE_TIMESTAMP * sizeof(char) +1);
  else
    timestamp = block->timestamp;

  do {
    if (!block->timestamp)
      memcpy(timestamp, (BYTE*)getTimeStamp(), SIZE_TIMESTAMP);

    sprintf((char*)blockHeader, "%d%s%s%s%d", block->index,
                                              timestamp,
                                              block->merkleRoot,
                                              block->previousBlockHash,
                                              nonce);
    sha256ofString(blockHeader, hash);
    nonce++;
  } while (!hashValidation(hash, difficulty));

  block->timestamp = timestamp;
  block->currentBlockHash = hash;
  block->nonce = nonce-1;

  printf ("\033[37m  [✓] %s | Block n°%3d | Nonce : %6d\33[00m\n", hash, block->index, nonce-1);

  return block;
}
