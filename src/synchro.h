#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <stdbool.h>
#include <pthread.h>

extern bool fini;
extern bool fenetre_texture_prete;
extern bool fenetre_taille;

/* Les extern des variables pour la synchro ici */
extern pthread_mutex_t mutexFenetre;
extern pthread_cond_t condTailleFenetre;
extern pthread_mutex_t mutexFenetreEtTexture;
extern pthread_cond_t condFenetreEtTexture;
/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
