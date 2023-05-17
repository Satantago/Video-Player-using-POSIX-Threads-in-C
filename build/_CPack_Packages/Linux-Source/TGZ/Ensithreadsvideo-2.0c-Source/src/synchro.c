#include "ensitheora.h"
#include "synchro.h"
#include "ensivideo.h"

/* les variables pour la synchro, ici */

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mutexFenetre);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    fenetre_taille = true;
    pthread_cond_signal(&condTailleFenetre);
    pthread_mutex_unlock(&mutexFenetre);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&mutexFenetre);
    while (!fenetre_taille) {
        pthread_cond_wait(&condTailleFenetre, &mutexFenetre);
    }
    // fenetre_taille = false;
    pthread_mutex_unlock(&mutexFenetre);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mutexFenetreEtTexture);
    fenetre_texture_prete = true;
    pthread_cond_signal(&condFenetreEtTexture);
    pthread_mutex_unlock(&mutexFenetreEtTexture);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&mutexFenetreEtTexture);
    while (!fenetre_texture_prete) {
        pthread_cond_wait(&condFenetreEtTexture, &mutexFenetreEtTexture);
    }
    // fenetre_texture_prete = false;
    pthread_mutex_unlock(&mutexFenetreEtTexture);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&mutexTexture);
    while (nb_texture_to_display == 0) {
        pthread_cond_wait(&condTextureRead, &mutexTexture);
    }
    nb_texture_to_display--;
}

void finConsommerTexture() {
    pthread_cond_signal(&condTextureWrite);
    pthread_mutex_unlock(&mutexTexture);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&mutexTexture);
    while (nb_texture_to_display == NBTEX) {
        pthread_cond_wait(&condTextureWrite, &mutexTexture);
    }
    nb_texture_to_display++;
}

void finDeposerTexture() {
    pthread_cond_signal(&condTextureRead);
    pthread_mutex_unlock(&mutexTexture);
}
