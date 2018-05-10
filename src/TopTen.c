#include "Libreria.h"

void TopTen ( )
{
	int SceltaInput = 0;

	system ( "cls" );
	StampaPattern ( 80 * 25 - 1 );
	StampaSfondoTopTen ( );
	StampaPunteggi ( );
	
	do
	{
		SceltaInput = getch ( ); 
	} while ( SceltaInput != 27 ); //Ripeti fichè non preme ESC

}

void StampaSfondoTopTen ( )
{
	int i = 0;

	PosizionaCursore ( CentraCursore ( 11 ), 1 ); //11 è la lunghezza della stringa al rigo successivo
    printf ( "  TOP TEN  " );
    PosizionaCursore ( 11, 3);
    printf ( " # " );
    PosizionaCursore ( 33, 3 );
    printf ( " Nome " );
    PosizionaCursore ( 57, 3 );
    printf ( " Score " );
	PosizionaCursore ( 69, 24 );
	printf ( " Menu: ESC" );
	ImpostaColore ( 0x6F );
	PosizionaCursore ( 10, 5 );
	for ( i = 0; i < 9; i++ )
	{
		PosizionaCursore ( 10, 5 + 2 * i );
		printf ( "  %d  ", i + 1 );
		PosizionaCursore ( 24, 5 + 2 * i );
		printf ( "                          " );
		PosizionaCursore ( 55, 5 + 2 * i );
		printf ( "           " );
	}
	PosizionaCursore ( 10, 23 );
	printf ( "  10 " );
    PosizionaCursore ( 24, 23 );
    printf ( "                          " );
	PosizionaCursore ( 55, 23 );
	printf ( "           " );
}

void StampaPunteggi ( )
{
	FILE * fp = NULL;
	char Nome[25] = ""; //Lunghezza massima nome
	int Punteggio = 0;
	int i = 0;

	if ( ControlloPresenzaFile ( "TopTen.txt" ) ) //Se il file TopTen.txt esiste
	{
		fp = fopen ( "TopTen.txt", "r" );
		if ( fp == NULL ) //Se il file non esiste
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
			getch ( );
			exit ( -1 ); //Termina il gioco
		}
	}

	if ( !ControlloPresenzaFile ( "TopTen.txt" ) ) //Se il file TopTen.txt non esiste
	{
		
		CreaTopTenDefault ( ); //Crea il file con la Top Ten di default
		fp = fopen ( "TopTen.txt", "r" );
		if ( fp == NULL ) //Se il file non esiste
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
			getch ( );
			exit ( -1 ); //Termina il gioco
		}
	}
	
	for ( i = 0; i < 10; i++ ) //Stampa a video la Top Ten presente su file
	{
		fscanf ( fp, "%s ", Nome );
		PosizionaCursore ( 25, 5 + 2 * i );
		printf ( "%s", Nome );
		fscanf ( fp, "%d", &Punteggio );
		PosizionaCursore ( 59, 5 + 2 * i );
		printf ( "%d", Punteggio );
		strcpy ( Nome, "" );
	}

	fclose ( fp );

}

int NuovoRecord ( int Punteggio )
{
	
	FILE *fp = NULL;
	char Nome[25] = "";
	int i = 0;
	int Minimo = 0;
	int Flag = 0;
	
	
	if ( ControlloPresenzaFile ( "TopTen.txt" ) ) //Se il file esiste
	{
		fp = fopen ( "TopTen.txt", "r" );
		if ( fp == NULL ) //Se il file non esiste
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
			getch ( );
			exit ( -1 ); //Termina il gioco
		}
	}
		
	if ( !ControlloPresenzaFile ( "TopTen.txt" ) ) //Se il file non esiste lo creo
	{
		CreaTopTenDefault ( );
		fp = fopen ( "TopTen.txt", "r" );
		if ( fp == NULL ) //Se il file non esiste
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
			getch ( );
			exit ( -1 ); //Termina il gioco
		}
	}
	
	for ( i = 0; i < 9; i++)
		fscanf ( fp, "%s %d\n", Nome, &Minimo ); //Scorre le prime 9 voci della topten
        	
	fscanf ( fp, "%s %d", Nome, &Minimo ); //Legge la decima riga (non ha '\n' finale)
        	        	
	if ( Punteggio > Minimo ) //Se il Punteggio del giocatore è maggiore del punteggio del giocatore ultimo in classifica
		Flag = 1; //Imposta il flag a vero
	else
		Flag = 0; //Imposta il flag a falso
        	
	fclose ( fp );

	return Flag;
}

void CreaTopTenDefault ( )
{
	FILE* fp = NULL;

	fp = fopen ( "TopTen.txt", "w" );
	if ( fp == NULL ) //Se il file non esiste
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
		getch ( );
		exit ( -1 ); //Termina il gioco
	}
	else
	{
		//Stampa su file la Top Ten di default
		fprintf(fp, "Surya 150\n");
		fprintf(fp, "Claudio 140\n");
		fprintf(fp, "Ugo 130\n");
		fprintf(fp, "Cecilia 120\n");
		fprintf(fp, "Carlo 110\n");
		fprintf(fp, "Iris 100\n");
		fprintf(fp, "Ettore 95\n");
		fprintf(fp, "Giovanni 90\n");
		fprintf(fp, "Alessandro 85\n");
		fprintf(fp, "Ylenia 80"); //L'ultima (non ha '\n' finale)
	}

	fclose ( fp );
}

void AggiornaTopTen ( giocatore *Giocatore )
{
     
	int Posizione = 0;
	int i = 0;
	int j = 0;
	int Contatore = 0;
	typedef char Nome[25];
	Nome VettoreNomi[10];
	int VettorePunteggi[10];
	FILE *fp = NULL;

	//Copia la classifica dei giocatori dal file ai vettori
    fp = fopen ( "TopTen.txt", "r" );
	if ( fp == NULL ) //Se il file non esiste
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
		getch ( );
		exit ( -1 ); //Termina il gioco
	}
	
	for ( i = 0; i < 9; i++ )
		fscanf ( fp, "%s %d\n", VettoreNomi[i], &VettorePunteggi[i] ); //Memorizza le prime 9 voci

	fscanf ( fp, "%s %d", VettoreNomi[9], &VettorePunteggi[9] ); //Memorizza l'ultima (non ha '\n' finale)
	
	fclose ( fp );

	i = 0;

	while ( Posizione == 0 && i < 10 ) //Trova la posizione in cui deve essere collocato il nuovo punteggio
	{
		if ( VettorePunteggi[i] < Giocatore->Punteggio )
			Posizione = 1; //Indica che bisogna stampare il punteggio del giocatore sopra il punteggio individuato
		else
		{
			if ( VettorePunteggi[i] == Giocatore->Punteggio )
			{
				Posizione = 2; //Indica che bisogna stampare il punteggio del giocatore sotto il punteggio individuato
				for ( j = 0; j < 10; j++ )
					if ( VettorePunteggi[j] == Giocatore->Punteggio ) //Se il punteggio del giocatore da inserire in To Ten è uguale al j-esimo punteggio del giocatore in Top Ten
						Contatore++; //Incrementa il contatore del numero dei punteggi nella TopTen che sono uguale al punteggio del giocatore ricevuto in input
			}
		}
		i++;
     }
     
	if ( Posizione == 1 )
		i--; //Decrementa l'indice che indica dove posizionare il nome e il punteggio del giocatore
	else if ( Posizione == 2 )
		i = i + Contatore - 1; //Incrementa l'indice con il contatore così da inserirlo dopo tutti gli altri giocatori con lo stesso punteggio

	//Aggiorna il file con il nuovo punteggio
	fp = fopen ( "TopTen.txt", "w" );
	if ( fp == NULL ) //Se il file non esiste
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." ); //Comunica un messaggio di errore irreversibile
		getch ( );
		exit ( -1 ); //Termina il gioco
	}
	for ( j = 0; j < i; j++ )
		fprintf ( fp, "%s %d\n", VettoreNomi[j], VettorePunteggi[j] ); //Stampa su file i nomi e i punteggi dei giocatori con punteggio maggiore di quello del giocatore ricevuto in input
	
	if( j != 9 ) //Se non è l'ultima posizione
	{
		fprintf ( fp, "%s %d\n", Giocatore->Nome, Giocatore->Punteggio ); //Stampa su file il nome e il punteggio del giocatore da inserire in Top Ten
		for ( j = i; j < 8; j++ )
			fprintf ( fp, "%s %d\n", VettoreNomi[j], VettorePunteggi[j] ); //Stampa su file i nomi e i punteggi dei giocatori con punteggio minore di quello del giocatore appena inserito nella Top Ten
         
		fprintf ( fp, "%s %d", VettoreNomi[j], VettorePunteggi[j] ); //Stampa su file il nome e il punteggio dell'ultimo giocatore (senza '\n')
             
	}
	else
		fprintf ( fp, "%s %d", Giocatore->Nome, Giocatore->Punteggio ); //Stampa su file il nome e il punteggio dell'ultimo giocatore (senza '\n')

	fclose ( fp );
}