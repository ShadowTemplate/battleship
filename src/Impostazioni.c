#include "Libreria.h"

void Impostazioni ( )
{
	system ( "cls" );
    StampaPattern (80 * 25 - 1 ); 
	StampaSfondoImpostazioni ( );
    FrecciaImpostazioni ( );
}

void FrecciaImpostazioni ( )
{
	int InputTastiera = 0;
	int TotaleCaselle = 0;
	int NumTotaleNavi = 0;
	int RigaCorrente = 4;
	int OrdinataCorrente = 3;
	int FlagUscita = 0;
	int FlagAudio = 1;
	FILE *fp = NULL;
	settaggi NuoveImpostazioni;
	settaggi Default;
	settaggi ImpostazioniTemporanee;

	//N.B.: Il carattere 26 corrisponde a ->

	//N.B.: Vedere il pdf con le impostazioni per capire quali controlli sono stati fatti sui valori delle impostazioni

	PosizionaCursore ( OrdinataCorrente, RigaCorrente );
	printf ( "%c", 26 );

	//Controllo se deve essere riprodotta la musica
	if ( ControlloPresenzaFile ( "NuoveImpostazioni.ini" ) )
	{
		fp = fopen ( "NuoveImpostazioni.ini", "rb" );
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}
		else
		{
			fread ( &ImpostazioniTemporanee, sizeof ( settaggi ), 1, fp ); //Aggiorna le ImpostazioniTemporanee con le impostazioni del file NuoveImpostazioni.ini esistente
			FlagAudio = ImpostazioniTemporanee.Audio;
			fclose ( fp );
			if ( FlagAudio == 0 )
				PlaySound ( "Audio\\PowerOfTheGods.WAV", NULL, SND_LOOP | SND_ASYNC );
		}


	}

	OttieniImpostazioniIniziali ( &NuoveImpostazioni );

	do
	{
		InputTastiera = getch ( );
		switch ( InputTastiera )
		{
			case 27: //ESC
				FlagUscita = 1;
			break;
			
			case 72: //Freccia su
				if ( RigaCorrente > 4 && RigaCorrente < 21 )
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( " " );
					RigaCorrente--;
					RigaCorrente--;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf( "%c", 26 );
				}
			break;
			
			case 80: //Freccia giù
				if ( RigaCorrente > 3 && RigaCorrente < 20)
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( " " );
					RigaCorrente++;
					RigaCorrente++;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( "%c", 26 );
				}
			break;
			
			case 13: //INVIO
				switch ( RigaCorrente )
				{
					case 4:
						NuoveImpostazioni.NumGiocatori = ModificaImpostazione ( NuoveImpostazioni.NumGiocatori, 23, RigaCorrente, 2 );
					break;

					case 6:
						NuoveImpostazioni.NumRighe = ModificaImpostazione ( NuoveImpostazioni.NumRighe, 33, RigaCorrente, 10); 
						//Controllo se, dopo aver modificato l'impostazione, il numero delle navi sia compatibile
						//Altrimenti potrebbe: ingrandire il campo, aumentare il numero delle navi, diminuire il campo (il numero delle navi diventa incompatibile)
						TotaleCaselle = NuoveImpostazioni.NumRighe * NuoveImpostazioni.NumColonne;
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( sqrt ( TotaleCaselle ) < NumTotaleNavi ) 
						{
							//Setta il numero delle navi a default
							fp = fopen ( "DefaultSettings.ini", "rb" );
							if ( fp == NULL )
							{
								system ( "cls" );
								PosizionaCursore ( 0, 0 );
								perror ( "" );
								fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
								getch ( );
								exit ( -1 );
							}
							fread ( &Default, sizeof ( settaggi ), 1, fp );
							fclose ( fp );
							NuoveImpostazioni.NumSommergibili = Default.NumSommergibili;
							NuoveImpostazioni.NumIncrociatori = Default.NumIncrociatori;
							NuoveImpostazioni.NumTorpediniere = Default.NumTorpediniere;
							NuoveImpostazioni.NumPortaerei = Default.NumPortaerei;
							StampaImpostazione ( NuoveImpostazioni.NumSommergibili, 1, 33, 10 );
							StampaImpostazione ( NuoveImpostazioni.NumIncrociatori, 1,33, 12);
							StampaImpostazione ( NuoveImpostazioni.NumTorpediniere, 1, 33, 14 );
							StampaImpostazione ( NuoveImpostazioni.NumPortaerei, 1, 33, 16 );
						}
					break;

					case 8:
						NuoveImpostazioni.NumColonne = ModificaImpostazione ( NuoveImpostazioni.NumColonne, 35, RigaCorrente, 10 );
						//Controllo se, dopo aver modificato l'impostazione, il numero delle navi sia compatibile
						//Altrimenti potrebbe: ingrandire il campo, aumentare il numero delle navi, diminuire il campo (il numero delle navi diventa incompatibile)
						TotaleCaselle = NuoveImpostazioni.NumRighe * NuoveImpostazioni.NumColonne;
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( sqrt ( TotaleCaselle ) < NumTotaleNavi )
						{
							//Setta il numero delle navi a default
							fp = fopen ( "DefaultSettings.ini", "rb" );
							if ( fp == NULL )
							{
								system ( "cls" );
								PosizionaCursore ( 0, 0 );
								perror ( "" );
								fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
								getch ( );
								exit ( -1 );
							}
							fread ( &Default, sizeof ( settaggi ), 1, fp );
							fclose ( fp );
							NuoveImpostazioni.NumSommergibili = Default.NumSommergibili;
							NuoveImpostazioni.NumIncrociatori = Default.NumIncrociatori;
							NuoveImpostazioni.NumTorpediniere = Default.NumTorpediniere;
							NuoveImpostazioni.NumPortaerei = Default.NumPortaerei;
							StampaImpostazione ( NuoveImpostazioni.NumSommergibili, 1, 33, 10 );
							StampaImpostazione ( NuoveImpostazioni.NumIncrociatori, 1,33, 12);
							StampaImpostazione ( NuoveImpostazioni.NumTorpediniere, 1, 33, 14 );
							StampaImpostazione ( NuoveImpostazioni.NumPortaerei, 1, 33, 16 );
						}
					break;

					case 10:
						NuoveImpostazioni.NumSommergibili = ModificaImpostazione ( NuoveImpostazioni.NumSommergibili, 33, RigaCorrente, 1 );
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( NumTotaleNavi == 0 ) //Se il totale delle Navi è 0
						{
							NuoveImpostazioni.NumSommergibili = 1; //Imposta il numero dei sommergibili a 1 per evitare che l'utente possa iniziare il gioco con 0 navi
							StampaImpostazione ( NuoveImpostazioni.NumSommergibili, 1, 33, RigaCorrente);
						}
					break;

					case 12:
						NuoveImpostazioni.NumIncrociatori = ModificaImpostazione ( NuoveImpostazioni.NumIncrociatori, 33, RigaCorrente, 1 );
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( NumTotaleNavi == 0 ) //Se il totale delle Navi è 0
						{
							NuoveImpostazioni.NumIncrociatori = 1; //Imposta il numero degli incrociatori a 1 per evitare che l'utente possa iniziare il gioco con 0 navi
							StampaImpostazione ( NuoveImpostazioni.NumIncrociatori, 1, 33, RigaCorrente );
						}
					break;

					case 14:
						NuoveImpostazioni.NumTorpediniere = ModificaImpostazione ( NuoveImpostazioni.NumTorpediniere, 33, RigaCorrente, 1 );
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( NumTotaleNavi == 0 ) //Se il totale delle Navi è 0
						{
							NuoveImpostazioni.NumTorpediniere = 1; //Imposta il numero delle torpediniere a 1 per evitare che l'utente possa iniziare il gioco con 0 navi
							StampaImpostazione ( NuoveImpostazioni.NumTorpediniere, 1, 33, RigaCorrente );
						}
					break;
					
					case 16:
						NuoveImpostazioni.NumPortaerei = ModificaImpostazione ( NuoveImpostazioni.NumPortaerei, 33, RigaCorrente, 1 );
						NumTotaleNavi = NuoveImpostazioni.NumSommergibili + NuoveImpostazioni.NumIncrociatori + NuoveImpostazioni.NumTorpediniere + NuoveImpostazioni.NumPortaerei;
						if ( NumTotaleNavi == 0 ) //Se il totale delle Navi è 0
						{
							NuoveImpostazioni.NumPortaerei = 1; //Imposta il numero delle torpediniere a 1 per evitare che l'utente possa iniziare il gioco con 0 navi
							StampaImpostazione ( NuoveImpostazioni.NumPortaerei, 1, 33, RigaCorrente );
						}
					break;
					
					case 18:
						NuoveImpostazioni.Audio = ModificaImpostazione ( NuoveImpostazioni.Audio, 14, RigaCorrente, -1 );
						if ( NuoveImpostazioni.Audio == 1 )
							PlaySound ( NULL, 0, 0 );
						if ( NuoveImpostazioni.Audio == 0 )
							PlaySound ( "Audio\\PowerOfTheGods.WAV", NULL, SND_LOOP | SND_ASYNC );
					break;
					
					case 20:
						NuoveImpostazioni.PrimoGiocatore = ModificaImpostazione ( NuoveImpostazioni.PrimoGiocatore, 31, RigaCorrente, -2 );
					break;

				}
				//Aggiorna il file
				fp = fopen ( "NuoveImpostazioni.ini", "wb" );
				if ( fp == NULL )
				{
					system ( "cls" );
					PosizionaCursore ( 0, 0 );
					perror ( "" );
					fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
					getch ( );
					exit ( -1 );
				}
				fwrite ( &NuoveImpostazioni, sizeof ( settaggi ), 1, fp );
				fclose ( fp );
			break;
		}
	} while ( FlagUscita != 1 );
	
	PlaySound ( NULL, 0, 0 );

}

void StampaImpostazione ( int ValoreAttuale, int ValoreIncremento, int OrdinataCorrente, int RigaCorrente )
{
	PosizionaCursore ( OrdinataCorrente, RigaCorrente );

	switch ( ValoreIncremento )
	{
 		case -1: //Identificatore dell'Audio
			if ( ValoreAttuale == 0 )
				printf ( "[Si]" );
			else
				printf ( "[No]" );
		break;
		
		case 1: //Identificatore per ogni tipo di nave
			printf ( "[%d] ", ValoreAttuale );
		break;
		
		case 2: //Identificatore per il numero dei giocatori
			printf ( "[%d] ", ValoreAttuale );
		break;
		
		case 10: //Identificatore per il numero di righe e di colonne
			printf ( "[%d] ", ValoreAttuale );
		break;
		
		case -2: //Identificatore per il turno del primo giocatore
			switch ( ValoreAttuale )
			{
				case 1:
					printf ( "[Giocatore1] " );
				break;
				
				case 2:
					printf ( "[Giocatore2] " );
				break;
				
				case 3:
					printf ( "[Manuale] " );
					ImpostaColore ( 0x3F );
					PosizionaCursore ( 41, 20 );
					StampaPattern ( 39 );
				break;
				
				case 0:
					printf ( "[Random] " );
					ImpostaColore ( 0x3F );
					PosizionaCursore ( 40, 20 );
					StampaPattern ( 40 );
				break;
			
			}
		break;
	}

}

int ModificaImpostazione ( int ValoreAttuale, int OrdinataCorrente, int RigaCorrente, int ValoreIncremento )
{
	int InputTastiera = 0;
	int Max = 0;
	int TotCaselle = 0;
	int ControlloTotale = 0;
	settaggi Settaggi;
	FILE *fp = NULL;

	ImpostaColore ( 0x3a );
	StampaImpostazione ( ValoreAttuale, ValoreIncremento, OrdinataCorrente, RigaCorrente ); //Stampa il valore attuale in verde per procedere al cambiamento dell'impostazione selezionata

	do
	{
		InputTastiera = getch ( );
		
		switch ( InputTastiera )
		{
			case 75: //Freccia sinistra
				
				switch ( ValoreIncremento )
				{
					case 10: //Identificatore per il numero di righe e di colonne
						if ( ValoreAttuale != 10 ) 
							ValoreAttuale = ValoreAttuale - ValoreIncremento; //Decrementa di 10 il numero di righe o colonne
					break;
					
					case 2: //Identificatore per il numero di giocatori
						if ( ValoreAttuale != 2 ) 
							ValoreAttuale = ValoreAttuale / ValoreIncremento; //Dividi il numero dei giocatori per 2 per passare alla potenza di 2 precedente
					break;

					case -2: //Identificatore per il turno del  primo giocatore
						if ( ValoreAttuale != 1 )  //Se il turno del primo giocatore non è impostato a "Giocatore1"
						{
							if ( ValoreAttuale == 0 ) //Se il turno del primo giocatore è impostato a "Random"
								ValoreAttuale = 3;  //Passa a "Manuale"
							else
								ValoreAttuale = ValoreAttuale - 1; //Passa all'impostazione precedente
						}
					break;

					case 1: //Identificatore per ogni tipo di nave
						if ( ValoreAttuale != 0 ) 
							ValoreAttuale = ValoreAttuale - 1; //Decrementa il numero della navi di quel tipo
					break;

					case -1: //Identificatore per l'audio
						if ( ValoreAttuale == 0 ) //Se l'audio è impostato a "No"
							ValoreAttuale = 1; //Passa a "Si"
					break;
				}
			break;

			case 77: //Freccia destra

				switch ( ValoreIncremento )
				{
					case 10: //Identificatore per il numero di righe e di colonne
						if ( ValoreAttuale != 990 )
							ValoreAttuale = ValoreAttuale + ValoreIncremento; //Incrementa di 10 il numero di righe o colonne
					break;
					
					case 2: //Identificatore per il numero dei giocatori
						if ( ValoreAttuale != 16)
							ValoreAttuale = ValoreAttuale * ValoreIncremento; //Moltiplica il numero dei giocatori per 2 per passare alla potenza di 2 successiva
					break;

					case -1: //Identificatore per l'audio
						if ( ValoreAttuale == 1 ) //Se l'audio è impostato a "Si"
							ValoreAttuale = 0; //Passa a "No"
					break;

					case -2: //Identificatore per il turno del primo giocatore
						if ( ValoreAttuale == 3 ) //Se il turno del primo giocatore è impostato a "Manuale"
							ValoreAttuale = 0; //Passa a "Random"
						else if ( ValoreAttuale != 0 )
							ValoreAttuale = ValoreAttuale + 1;
					break;
					
					case 1: //Identificatore per ogni tipo di nave
						fp = fopen ( "NuoveImpostazioni.ini", "rb" );
						if ( fp == NULL )
						{
							system ( "cls" );
							PosizionaCursore ( 0, 0 );
							perror ( "" );
							fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
							getch ( );
							exit ( -1 );
						}
						fread ( &Settaggi, sizeof ( settaggi ), 1, fp );
						fclose ( fp );
						TotCaselle = Settaggi.NumRighe * Settaggi.NumColonne;
						//Per ogni tipo di nave calcola la percentuale del numero di navi massimo 
						switch ( RigaCorrente )
						{
							case 10:
								Max = TotCaselle / 100 * 5;
								Settaggi.NumSommergibili = ValoreAttuale;
							break;

							case 12:
								Max = TotCaselle / 100 * 4;
								Settaggi.NumIncrociatori = ValoreAttuale;
							break;

							case 14:
								Max = TotCaselle / 100 * 3;
								Settaggi.NumTorpediniere = ValoreAttuale;
							break;

							case 16:
								Max = TotCaselle / 100 * 2;
								Settaggi.NumPortaerei = ValoreAttuale;
							break;
						}
						
						ControlloTotale = sqrt ( TotCaselle ) - Settaggi.NumSommergibili - Settaggi.NumIncrociatori - Settaggi.NumTorpediniere - Settaggi.NumPortaerei;
						if ( ControlloTotale > 0 && ValoreAttuale < Max ) // Se è maggiore del ControlloTotale ed è minore del numero massimo di navi di quel tipo che possono essere posizionate
							ValoreAttuale = ValoreAttuale + 1; //Incrementa il numero di quel tipo di nave
					break;
				}
			break;

		}
		
		ImpostaColore ( 0x3a );
		StampaImpostazione ( ValoreAttuale, ValoreIncremento, OrdinataCorrente, RigaCorrente ); //Stampa l'impostazione sselezionata in verde

	} while ( InputTastiera != 13 );
	ImpostaColore ( 0x3f );
	StampaImpostazione ( ValoreAttuale, ValoreIncremento, OrdinataCorrente, RigaCorrente ); //Stampa l'impostazione selezionata in bianco non appena viene premuto il tasto Invio
	 

	return ValoreAttuale;
}

void OttieniImpostazioniIniziali ( settaggi *ImpostazioniEffettive )
{
	FILE *fp = NULL;

	if ( ControlloPresenzaFile ( "NuoveImpostazioni.ini" ) )
	{
		fp = fopen ( "NuoveImpostazioni.ini", "rb" );
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}
		fread ( ImpostazioniEffettive, sizeof ( settaggi ), 1, fp ); //Lettura dal file "NuoveImpostazioni.ini"
		fclose ( fp );
	}
	else //Se il file "NuoveImpostazioni.ini" non esiste
	{
		if ( !ControlloPresenzaFile ( "DefaultSettings.ini" ) ) //Se non esiste il file "DefaultSettings.ini"
			CreaImpostazioniDefault ( ); //Crea il file "DefaultSettings.ini"

		fp = fopen ( "DefaultSettings.ini", "rb" );
		if ( fp == NULL ) 
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}
		
		fread ( ImpostazioniEffettive, sizeof ( settaggi ), 1, fp ); //Lettura dal file "DefaultSettings.ini"
		fclose ( fp );

		fp = fopen ( "NuoveImpostazioni.ini", "wb" ); //Apertura file Impostazioni temporanee in scrittura
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}
		fwrite ( ImpostazioniEffettive, sizeof ( settaggi ), 1, fp ); 
		fclose ( fp );


	}


}

void StampaSfondoImpostazioni ( )
{ 
	settaggi NuoveImpostazioni;
	/* N.B.:
	Il carattere 14 corrisponde alla nota musicale;
	Il carattere 254 corrisponde ad un rettangolo, usato per stilizzare una cella della nave;
	*/
	
	OttieniImpostazioniIniziali ( &NuoveImpostazioni );
	PosizionaCursore ( CentraCursore ( 16 ), 1 ); //16 è la lunghezza della stringa al rigo successivo
    printf ( "  IMPOSTAZIONI  " );
	PosizionaCursore ( 2, 4 );
    printf ( "   Numero giocatori: [%d] ", NuoveImpostazioni.NumGiocatori );
	PosizionaCursore ( 2, 6 );
    printf ( "   Numero righe della tabella: [%d] ", NuoveImpostazioni.NumRighe );
	PosizionaCursore ( 2, 8 );
    printf ( "   Numero colonne della tabella: [%d] ", NuoveImpostazioni.NumColonne );
	PosizionaCursore ( 2, 10 );
    printf ( "   Numero sommergibili:  %c%c    [%d] ", 254, 254, NuoveImpostazioni.NumSommergibili );
	PosizionaCursore ( 2, 12 );
    printf ( "   Numero incrociatori:  %c%c%c   [%d] ", 254, 254, 254, NuoveImpostazioni.NumIncrociatori );
	PosizionaCursore ( 2, 14 );
    printf ( "   Numero torpediniere:  %c%c%c%c  [%d] ", 254, 254, 254, 254, NuoveImpostazioni.NumTorpediniere );
	PosizionaCursore ( 2, 16 );
    printf ( "   Numero portaerei:     %c%c%c%c%c [%d] ", 254, 254, 254, 254, 254, NuoveImpostazioni.NumPortaerei );
	PosizionaCursore ( 2, 18 );
	if ( NuoveImpostazioni.Audio == 0 )
		printf ( "   Audio %c: [Si] ", 14 );
	else
		printf ( "   Audio %c: [No] ", 14 );
	PosizionaCursore ( 2, 20 );
	switch ( NuoveImpostazioni.PrimoGiocatore )
	{
		case 0:
			printf ( "   Primo giocatore di turno: [Random] " );
		break;
		
		case 1:
			printf ( "   Primo giocatore di turno: [Giocatore1] " );
		break;

		case 2:
			printf ( "   Primo giocatore di turno: [Giocatore2] " );
		break;

		case 3:
			printf ( "   Primo giocatore di turno: [Manuale] " );
	}
	PosizionaCursore ( 69, 24 );
	printf ( " Menu: ESC" );
	ImpostaColore ( 0x6F );
	PosizionaCursore ( 2, 23 );
    printf ( " Scorri le voci con %c e %c. Premi INVIO per selezionarne una. ", 24, 25 );
	PosizionaCursore ( 2, 24 );
	printf ( " Dopo, scorri con %c e %c per modificarne il valore. ", 27, 26 );
	ImpostaColore ( 0x3F );

}
