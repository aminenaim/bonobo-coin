/// \file beacons.c
/// \author Axel Delas, Amine Naim, Mamadou Djibo
/// \date février 2021
/// \brief Partie traitement consacré aux balises et arguments

#include "annexe.h"

bool has_h_beacon(int argc, char **argv) {
/// \brief Vérification si la balise -h (-help) figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "-h"))
      return true;
    i++;
  }
  return false;
}

bool has_c_beacon(int argc, char **argv) {
/// \brief Vérification si la balise -c (-cheaters) figure.
/// \param[in] argc Le nombre de paramètres donnés à l'exécution du programme.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  int i=0;
  while (i<argc) {
    if (!strcmp(argv[i], "-c"))
      return true;
    i++;
  }
  return false;
}

bool is_b_beacon(int i, char **argv) {
/// \brief Pointage de la balise -b (-blocks)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-b"))
    return true;
  return false;
}

bool is_d_beacon(int i, char **argv) {
/// \brief Pointage de la balise -d (-difficulty)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-d"))
    return true;
  return false;
}

bool is_c_beacon(int i, char **argv) {
/// \brief Pointage de la balise -c (-cheaters)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-c"))
    return true;
  return false;
}

bool is_t_beacon(int i, char **argv) {
/// \brief Pointage de la balise -t (-transactions)
/// \param[in] i L'indice du paramètre à pointer.
/// \param[in] argv Tableau contenant les paramètres donnés à l'exécution du programme.
/// \returns Vrai ou Faux
  if (!strcmp(argv[i], "-t"))
    return true;
  return false;
}
