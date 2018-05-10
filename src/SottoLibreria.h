#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> //Necessaria per getch ( )
#include <string.h> //Necessaria per le operazioni sulle stringhe
#include <math.h> //Necessaria per le operazioni artimetiche, come sqrt ( )
#include <time.h> //Necessaria per srand ( )

/*Vedere il file con le specifiche per informazioni sulle strutture*/

typedef struct el
{
	int Info;
	struct el *Prox;
} elemlista;

typedef elemlista *listaint;

typedef struct
{
	int NonAffondato;
	int Id;
} sommergibile;

typedef struct
{
	int NonAffondato;
	int Id;
} incrociatore;

typedef struct
{
	int NonAffondato;
	int Id;
} torpediniera;

typedef struct
{
	int NonAffondato;
	int Id;
} portaereo;

typedef struct
{
	sommergibile *Sommergibili;
	int NumSommergibili;
	incrociatore *Incrociatori;
	int NumIncrociatori;
	torpediniera *Torpediniere;
	int NumTorpediniere;
	portaereo *Portaerei;
	int NumPortaerei;
	int NaviNonAffondate;
} flotta;

typedef struct
{
	int NumGiocatori;
	int NumRighe;
	int NumColonne;
	int NumSommergibili;
	int NumIncrociatori;
	int NumTorpediniere;
	int NumPortaerei;
	int Audio; // 0: Audio attivo; 1: Audio disattivo
	int PrimoGiocatore; //1: Primo giocatore, 2: Secondo Giocatore, 3: Manuale, 0: Random
} settaggi;

typedef struct
{
	char Nome[25];
	int Punteggio;
	flotta Flotta;
	listaint GrigliaAttacco; //Lista che tiene traccia degli attacchi che il giocatore sta eseguendo
	listaint GrigliaDifesa; //Lista che tiene traccia degli attacchi che il giocatore sta subendo
	listaint MatricePosizionamento; //Lista che tiene traccia della posizione delle navi della griglia di difesa (gli Id delle navi vengono salvati in questa lista)
} giocatore;

typedef struct
{
	giocatore *Giocatore1;
	giocatore *Giocatore2;
	giocatore *Vincitore;
} scontro;

typedef struct
{
	int PartitaDaGiocare;
	int TotalePartite;
	scontro *Match;
} calendario;

typedef struct
{
	giocatore *Giocatore1;
	giocatore *Giocatore2;
	int TurnoCorrente;
} salvataggio;

typedef struct
{
	settaggi Impostazioni;
	giocatore *Partecipanti;
	calendario Calendario;
	int FlagPartitaInCorso; //Vale 0 (nessuna partita in corso) o 1 (una partita deve essere completata).
	salvataggio PartitaInCorso;
} torneo;
/*N.B.: FlagPartitaInCorso può valere -1 se c'è un errore nell'apertura di un salvataggio o
se viene premuto ESC durante il posizionamento delle navi*/
