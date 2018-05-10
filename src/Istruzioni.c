#include "Libreria.h"

void Istruzioni ( )
{
	system ( "cls" );
	StampaPattern ( 80 * 25 - 1 );
	StampaSfondoIstruzioni ( );
	NascondiCursore ( );
	ImpostaColore ( 0x3F ); 
	FrecciaIstruzioni ( ); 
}

void FrecciaIstruzioni ( )
{
	
	int InputTastiera = 0;
	int RigaCorrente = 8;
	int ColonnaCorrente = 28;
	int FlagUscita = 0; 

	//N.B.: 26 è il carattere di ->

	PosizionaCursore ( ColonnaCorrente, RigaCorrente );
	printf ( "%c", 26 );

	do
	{
		InputTastiera = getch ( );
		
		switch ( InputTastiera )
		{
			case 27: //Tasto ESC
				FlagUscita = 1; //Imposta il flag di uscita a vero per poter uscire dal ciclo e tornare al menu principale
			break;
			
			case 72: //Freccia su
				if ( RigaCorrente > 8 && RigaCorrente < 11)
				{
					PosizionaCursore ( ColonnaCorrente, RigaCorrente );
					printf ( " " ); //Sovrascrive il carattere di freccia con lo spazio
					RigaCorrente--; //Decrementa il numero della riga per poter spostare la freccia su
					PosizionaCursore ( ColonnaCorrente, RigaCorrente );
					printf ( "%c", 26 ); //Stampa la freccia nella riga precedente
				}
			break;
			
			case 80: //Freccia giù
				if ( RigaCorrente > 7 && RigaCorrente < 10)
				{
					PosizionaCursore ( ColonnaCorrente, RigaCorrente );
					printf ( " " ); //Sovrascrive il carattere di freccia con lo spazio
					RigaCorrente++; //Incrementa il numero della riga per poter spostare la freccia giù
					PosizionaCursore ( ColonnaCorrente, RigaCorrente );
					printf ( "%c", 26 ); //Stampa la freccia nella riga successiva
				}
			break;
			
			case 13: //Tasto INVIO
				switch ( RigaCorrente )
				{
					case 8: //Se il carattere freccetta si trova sulla riga delle regole
						RegoleGioco ( ); //Stampa a video le regole del gioco
						system ( "cls" );
						StampaPattern (80 * 25 - 1 );
						StampaSfondoIstruzioni ( );
						PosizionaCursore ( 28, 8 );
						printf ( "%c", 26 );
					break;

					case 9: //Se il carattere freccetta si trova sulla riga dei parametri delle impostazioni
						ParametriImpostazioni ( ); //Stampa a video i parametri delle impostazioni
						system ( "cls" );
						StampaPattern (80 * 25 - 1 );
						StampaSfondoIstruzioni ( );
						PosizionaCursore ( 28, 9 );
						printf ( "%c", 26 );
					break;
					
					case 10: //Se il carattere freccetta si trova sulla riga dei comandi del gioco
						ComandiGioco ( ); //Stampa a video i comandi del gioco
						system ( "cls" );
						StampaPattern ( 80 * 25 - 1 );
						StampaSfondoIstruzioni ( );
						PosizionaCursore ( 28, 10 );
						printf ( "%c", 26 );
					break;
				}
			break;
		}

	} while ( FlagUscita != 1 );
	system ( "cls" );
}

void StampaSfondoIstruzioni ( )
{
	PosizionaCursore ( CentraCursore ( 14 ), 1 ); //14 è la lunghezza della stringa al rigo successivo
	printf ( "  ISTRUZIONI  " );
	PosizionaCursore ( 27, 8 );
	printf ( "   REGOLE DEL GIOCO       " );
	PosizionaCursore ( 27, 9 );
	printf ( "   PARAMETRI IMPOSTAZIONI " );
	PosizionaCursore ( 27, 10 );
	printf ( "   COMANDI DEL GIOCO      " );
	PosizionaCursore ( 69, 24 );
	printf ( " Menu: ESC" );
	ImpostaColore ( 0x6F );
	PosizionaCursore ( 2, 20 );
	printf ( " Scorri le voci con %c e %c ", 24, 25 );
	PosizionaCursore ( 2, 21 );
	printf ( " Premi INVIO per selezionarne una. " );
	PosizionaCursore ( 2, 23 );
	ImpostaColore ( 0x2F );
	printf ( " Sviluppatori: Francesco Scuccimarri - Gianvito Taneburgo " );
	ImpostaColore ( 0x3F );

}

void RegoleGioco ( )
{
	FILE *fp = NULL;
	int Allineamento = 0;
	char Buffer = '0';
	int i = 0;
	int j = 0;
	int InputTastiera = 0;
	int EsciDaRegole = 0;
	int PaginaCorrente = 1;
	int Contatore = 0;

	system ( "cls" );
	StampaPattern ( 80 * 25 - 1 );
	NascondiCursore ( );

	if ( ControlloPresenzaFile ( "Regole.txt" ) )
	{
		fp = fopen ( "Regole.txt", "r" );
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}

		Allineamento = CentraCursore ( 20 );
		PosizionaCursore ( Allineamento, 1 );
		printf ( "  REGOLE DEL GIOCO  " );
		PosizionaCursore ( 65, 24 );
		printf ( " Indietro: ESC" );

		PosizionaCursore ( 3, 24 );
		printf ( " Scorri le pagine con %c e %c ", 25, 24 );
		PosizionaCursore ( 41, 24 );
		printf ( " Pagina 1 di 3 " );
	
		for ( i = 0; i < 20; i++ ) //Stampa la prima pagina delle regole del gioco
		{
			PosizionaCursore ( 3, 3 + i );
			for ( j = 0; j < 74 && Buffer != '\n'; j++ ) //Legge da file e stampa a video carattere per carattere tutta la prima pagina delle regole del gioco
			{
				Buffer = fgetc ( fp );
				printf ( "%c", Buffer );

			}
			Buffer = '0';
		}


		do
		{
			InputTastiera = getch ( );
			
			switch ( InputTastiera )
			{
				case 27: //Tasto ESC
					EsciDaRegole = 1; //Imposta questo flag a vero per poter uscire dal ciclo e tornare al menu Istruzioni
				break;
				
				case 72: //Freccia su
					switch ( PaginaCorrente )
					{
						case 2: //Se la pagina corrente è la seconda
							PaginaCorrente = 1; //Impostala a 1
							StampaPattern ( 80 * 25 - 1 );
							PosizionaCursore ( Allineamento, 1 );
							printf ( "  REGOLE DEL GIOCO  " );
							PosizionaCursore ( 3, 24 );
							printf ( " Scorri le pagine con %c e %c ", 25, 24 );
							PosizionaCursore ( 41, 24 );
							printf ( " Pagina 1 di 3 " );
							PosizionaCursore ( 65, 24 );
							printf ( " Indietro: ESC" );
							rewind ( fp ); //Posizionaa il cursore del file all'inizio (prima pagina)
							for ( i = 0; i < 20; i++ ) //Legge da file e stampa a video carattere per carattere tutta la prima pagina delle regole del gioco
							{
								PosizionaCursore ( 3, 3 + i );
								for ( j = 0; j < 74 && Buffer != '\n'; j++ )
								{
									Buffer = fgetc ( fp );
									printf ( "%c", Buffer );
								}
								Buffer = '0';
							}
						break;

						case 3: //Se la pagina corrente è la terza
							PaginaCorrente = 2; //Impostala a 2
							StampaPattern ( 80 * 25 - 1);
							PosizionaCursore ( Allineamento, 1 );
							printf ( "  REGOLE DEL GIOCO  " );
							rewind ( fp );
							for ( i = 0; i < 1041; i++ ) //Posiziona il cursore del file all'inizio della seconda pagina
								Buffer = fgetc ( fp );
							for ( i = 0; i < 20; i++ ) //Leggi da file e stampa a video carattere per carattere tutta la seconda pagina delle regole del gioco
							{
								PosizionaCursore ( 3, 3 + i );
								for ( j = 0; j < 74 && Buffer != '\n'; j++ )
								{
									Buffer = fgetc ( fp );
									printf ( "%c", Buffer );
								}
								Buffer = '0';
							}
							
							PosizionaCursore ( 0, 24 );
							StampaPattern ( 79 );
							PosizionaCursore ( 3, 24 );
							printf ( " Scorri le pagine con %c e %c ", 25, 24 );
							PosizionaCursore ( 41, 24 );
							printf ( " Pagina 2 di 3 " );
							PosizionaCursore ( 65, 24 );
							printf ( " Indietro: ESC" );
						break;

					}
				break;

				case 80: //Freccia giù
					switch ( PaginaCorrente )
					{
						case 1: //Se la pagina corrente è la prima
							PaginaCorrente = 2; //Impostala a 2
							StampaPattern ( 80 * 25 - 1 );
							PosizionaCursore ( Allineamento, 1 );
							printf ( "  REGOLE DEL GIOCO  " );
							PosizionaCursore ( 3, 24 );
							printf ( " Scorri le pagine con %c e %c ", 25, 24 );
							PosizionaCursore ( 41, 24 );
							printf ( " Pagina 2 di 3 " );
							PosizionaCursore ( 65, 24 );
							printf ( " Indietro: ESC" );
							rewind ( fp );
							for ( i = 0; i < 1041; i++ ) //Posiziona il cursore del file all'inizio della seconda pagina
								Buffer = fgetc ( fp );
							for ( i = 0; i < 20; i++ ) //Leggi da file e stampa a video carattere per carattere tutta la seconda pagina delle regole del gioco
							{
								PosizionaCursore ( 3, 3 + i );
								for ( j = 0; j < 74 && Buffer != '\n'; j++ )
								{
									Buffer = fgetc ( fp );
									printf ( "%c", Buffer );
								}
								Buffer = '0';
							}
						break;

						case 2: //Se la pagina corrente è la seconda
							PaginaCorrente = 3; //Impostala a 3
							StampaPattern ( 80 * 25 - 1 );
							PosizionaCursore ( Allineamento, 1 );
							printf ( "  REGOLE DEL GIOCO  " );
							PosizionaCursore ( 3, 24 );
							printf ( " Scorri le pagine con %c e %c ", 25, 24 );
							PosizionaCursore ( 41, 24 );
							printf ( " Pagina 3 di 3 " );
							PosizionaCursore ( 65, 24 );
							printf ( " Indietro: ESC" );
							rewind ( fp );
							for ( i = 0; i < 1041; i++ ) //Posiziona il cursore all'inizio della seconda pagina
								Buffer = fgetc ( fp );
							for ( i = 0; i < 1106; i++ ) //Posiziona il cursore all'inizio della terza pagina
								Buffer = fgetc ( fp );
							for ( i = 0; i < 20; i++ ) //Leggi da file e stampa a video carattere per carattere tutta la terza pagina delle regole del gioco
							{
								PosizionaCursore ( 3, 2 + i );
								for ( j = 0; j < 74 && Buffer != '\n' && Contatore < 447; j++ ) //447 sono i caratteri della terza pagina
								{
									Buffer = fgetc ( fp );
									printf ( "%c", Buffer );
									Contatore++;
								}
								Buffer = '0';
							}
							Contatore = 0;
						break;
					}
				break;
			}

		} while ( EsciDaRegole != 1 );

		fclose ( fp );

	}
	else //Se il file Regole.txt non è presente
	{
		PosizionaCursore ( 14, 24 );
		printf ( "Impossibile trovare il file. Premi un tasto per tornare indietro " );
		MostraCursore ( );
		getch ( );
		NascondiCursore ( );
	}
}

void ParametriImpostazioni ( )
{

	int InputTastiera = 0;
	int i = 0;
	FILE *fp = NULL;
	char Buffer = '0';
	
	system ( "cls" );
	StampaPattern ( 80 * 25 - 1 );
	NascondiCursore ( );

	if ( ControlloPresenzaFile ( "Regole.txt" ) )
	{
		fp = fopen ( "Regole.txt", "r" );
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}

		for ( i = 0; i < 2612; i++ )
			Buffer = fgetc ( fp ); //Posiziona il cursore del file alla voce Parametri Impostazioni del file Regole.txt
											
		i = CentraCursore ( 32 );
		PosizionaCursore ( i, 1 );
		printf ( "  PARAMETRI DELLE IMPOSTAZIONI  " );
		i = 0;
		PosizionaCursore ( 65, 24 );
		printf ( " Indietro: ESC" );
		ImpostaColore ( 0x3F );
		PosizionaCursore ( 3, 4) ;
		do 
		{
			Buffer = fgetc ( fp ); //Legge da file e stampa a video carattere per carattere la 1° riga dei parametri delle impostazioni
			printf ( "%c", Buffer );
		} while ( Buffer != '\n' );

		ImpostaColore ( 0x6F );
		for ( i = 0; i < 4; i++ ) //Legge da file e stampa a video carattere per carattere le 3 righe successive dei parametri delle impostazioni
		{
			PosizionaCursore ( 3, 6 + i );
			do 
			{	
				Buffer = fgetc ( fp ); 
				printf ( "%c", Buffer );
			} while ( Buffer != '\n' );
		}

		ImpostaColore ( 0x3F );
		PosizionaCursore ( 3, 13 );
		do 
		{
			Buffer = fgetc ( fp ); //Legge da file e stampa a video carattere per carattere la riga successiva dei parametri delle impostazioni
			printf ( "%c", Buffer );
		} while ( Buffer != '\n' );

		ImpostaColore ( 0x6F );
		for ( i = 0; i < 4; i++ ) //Legge da file e stampa a video carattere per carattere la ultime righe dei parametri delle impostazioni
		{
			PosizionaCursore ( 3, 15 + i );
			do 
			{	
				Buffer = fgetc ( fp );
				printf ( "%c", Buffer );
			} while ( Buffer != '\n' );
		}

		ImpostaColore ( 0x3F );

		do
		{
			InputTastiera = getch ( );
		} while ( InputTastiera != 27 ); //Finchè non viene premuto il tasto ESC
		
		fclose ( fp );
	}
	else //Se il file Regole.txt non è presente
	{
		PosizionaCursore ( 14, 24 );
		printf ( "Impossibile trovare il file. Premi un tasto per tornare indietro " );
		MostraCursore ( );
		getch ( );
		NascondiCursore ( );
	}
}

void ComandiGioco ( )
{
	int InputTastiera = 0;
	int i = 0;
	FILE *fp = NULL;
	char Buffer = '0';
	int Buffer2 = '0';
	int Contatore = 0;

	system ( "cls" );
	StampaPattern ( 80 * 25 - 1 );
	NascondiCursore ( );
	
	if ( ControlloPresenzaFile ( "Regole.txt" ) )
	{
		fp = fopen ( "Regole.txt", "r" );
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}
		for ( i = 0; i < 3267; i++ )
			Buffer = fgetc ( fp ); //Posiziona il cursore del file alla voce Comandi del file Regole.txt

		i = CentraCursore ( 21 );
		PosizionaCursore ( i, 1 );
		printf ( "  COMANDI DEL GIOCO  " );
		i = 0;
		PosizionaCursore ( 65, 24 );
		printf ( " Indietro: ESC" );
		ImpostaColore ( 0x6F );

		PosizionaCursore ( 20, 4 ); //Legge il primo comando (ha un formato diverso dagli altri)
		do 
		{	
			fscanf ( fp, "%d", &Buffer2 );
			printf ( " %c ", Buffer2 );
			Buffer = fgetc ( fp );
			Contatore++;
		} while ( Contatore < 4 );

		do 
		{	
			Buffer = fgetc ( fp ); //Scorre il file fino al carattere '\n'
		} while ( Buffer != '\n');

		PosizionaCursore ( 40, 4 );
		
		do 
		{	
			Buffer = fgetc ( fp );
			printf ( "%c", Buffer );
		} while ( Buffer != '\n'); //Legge da file e stampa a video carattere per carattere


		for ( i = 1; i < 9; i++ ) //Leggi i successivi 8 comandi
		{
			PosizionaCursore ( 20, 4 + 2 * i );
			do 
			{	
				Buffer = fgetc ( fp );
				printf ( "%c", Buffer );
			} while ( Buffer != '\n' ); //Legge da file e stampa a video carattere per carattere

			PosizionaCursore ( 40, 4 + 2 * i );
			do 
			{	
				Buffer = fgetc ( fp );
				printf ( "%c", Buffer );
			} while ( Buffer != '\n' ); //Legge da file e stampa a video carattere per carattere

		}
	

		ImpostaColore ( 0x3F );
		
		do
		{
			InputTastiera = getch ( );

		} while ( InputTastiera != 27 ); //Finchè non viene premuto il tasto ESC
		
		fclose ( fp );
	}
	else //Se il file Regole.txt non è presente
	{
		PosizionaCursore ( 14, 24 );
		printf ( "Impossibile trovare il file. Premi un tasto per tornare indietro " );
		MostraCursore ( );
		getch ( );
		NascondiCursore ( );
	}
}