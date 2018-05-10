#include "Libreria.h"

int CentraCursore ( int Lunghezza )
{
	int OrdinataCentrata = 0;

	if ( Lunghezza <= 0 )
		OrdinataCentrata = 0;
	else
		OrdinataCentrata = ( 80 - Lunghezza ) / 2;

	return OrdinataCentrata;
}

void StampaPattern ( int Contatore )
{
	int i = 0;

	for ( i = 0; i < Contatore; i++ )
		printf ( "%c", 177); //177 corrisponde al carattere scelto come pattern della finestra (pixel riempito di puntini fittamente)

 }

void ImpostaDimensioniFinestra ( )
{
	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
	COORD Dimensioni = { 80, 25 }; //80 colonne e 25 righe

	SetConsoleScreenBufferSize ( Handle, Dimensioni );
}

void CreaImpostazioniDefault ( )
{
	FILE *fp = NULL;
	settaggi Default = { 2, 10, 10, 1, 2, 1, 1, 1, 1 };
	/* Inizializzo la struttura in default:

	2 Giocatori;
	10 Righe, 10 Colonne;
	1 Sommergibile, 2 Incrociatori, 1 Torpediniera, 1 Portaerei;
	Audio: No;
	Primo giocatore di turno: Giocatore 1 */

	fp = fopen ( "DefaultSettings.ini", "wb" ); //Creo il file di default
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
		fwrite ( &Default, sizeof ( settaggi ), 1, fp ); //Scrivo la struttura nel file
		fclose ( fp );
	}
}

void CambiaNomeFinestra ( )
{
   SetConsoleTitle ( "Battaglia Navale" );
}

void ImpostaColore ( int Colore )
{
	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );

	SetConsoleTextAttribute ( Handle, Colore );
}

void PosizionaCursore ( int Colonna, int Riga )
{
	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
	COORD PosizioneCursore = { Colonna, Riga };

	SetConsoleCursorPosition ( Handle, PosizioneCursore );
}

void NascondiCursore ( )
{
	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
	CONSOLE_CURSOR_INFO ProprietaCursore = { 10, FALSE }; //Setto le proprietà del cursore: 10 = il cursore occuperà il 10% di un pixel; FALSE = non sarà visibile

	SetConsoleCursorInfo ( Handle, &ProprietaCursore );
}

void MostraCursore ( )
{
	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
	CONSOLE_CURSOR_INFO ProprietaCursore = { 10, TRUE }; //Setto le proprietà del cursore: 10 = il cursore occuperà il 10% di un pixel; TRUE = sarà visibile

	SetConsoleCursorInfo ( Handle, &ProprietaCursore );
}

void FrecciaMenu ( int *FlagRistampaMenu )
{
	int InputTastiera = 0;
	int RigaCorrente = 8;
	int OrdinataCorrente = 32;
	int FlagStopFreccia = 0;
	int FlagAudio = 1;
	FILE * fp = NULL;
	settaggi Settaggi = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //Inizializzo la struttura
	int Flag = 0;

	//N.B.: Il carattere 26 corrisponde alla freccia ->

	PosizionaCursore ( OrdinataCorrente, RigaCorrente );
	printf ( "%c", 26 );

	do
	{
		InputTastiera = getch ( );

		switch( InputTastiera )
		{
			case 72: //Freccia SU
				if( RigaCorrente > 8 && RigaCorrente < 13)
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( " " );
					RigaCorrente--;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( "%c", 26 );
				}
				else if( RigaCorrente == 13 )
				{
					PosizionaCursore ( 73, 24 );
					printf ( " " );
					RigaCorrente--;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( "%c", 26 );
				}
				else if ( RigaCorrente == 8 )
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( " " );
					RigaCorrente = 13;
					PosizionaCursore ( 73, 24 );
					printf ( "%c", 26 );
				}
			break;

			case 80: //Freccia GIU
				if( RigaCorrente == 12 )
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( " " );
					PosizionaCursore ( 73, 24 );
					printf ( "%c", 26 );
					RigaCorrente++;
				}
				else if ( RigaCorrente > 7 && RigaCorrente < 12 )
				{
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf( " " );
					RigaCorrente++;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( "%c", 26 );
				}
				else if ( RigaCorrente == 13 )
				{
					PosizionaCursore ( 73, 24 );
					printf( " " );
					RigaCorrente = 8;
					PosizionaCursore ( OrdinataCorrente, RigaCorrente );
					printf ( "%c", 26 );
				}
			break;

			case 13: //INVIO
				switch ( RigaCorrente )
				{
					case 8:
						NuovaPartita ( );
						FlagStopFreccia = 1;
					break;

					case 9:
						CaricaPartita ( );
						FlagStopFreccia = 1;
					break;

					case 10:
						PlaySound ( NULL, 0, 0 ); //Fermo la riproduzione di musica (le impostazioni hanno una traccia di sottofondo diversa)
						Impostazioni ( );
						FlagStopFreccia = 1;
						*FlagRistampaMenu = 1;
						//Controllo se devo riprodurre della musica
						if ( ControlloPresenzaFile ( "NuoveImpostazioni.ini" ) )
						{
							fp = fopen ( "NuoveImpostazioni.ini", "rb" ); //Se esiste un file NuoveImpostazioni.ini, cioè se l'utente è entrato almeno una volta nella voce Impostazioni del menu (ed ha potuto attivare l'audio)
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
								fread ( &Settaggi, sizeof ( settaggi ), 1, fp);
								FlagAudio = Settaggi.Audio;
								fclose ( fp );
								if ( FlagAudio == 0 ) //Se è stato attivato l'audio nelle impostazioni
									PlaySound ( "Audio\\MissionOnASleepwalk.WAV", NULL, SND_LOOP | SND_ASYNC ); //Riproduci in LOOP la traccia audio
							}
						}
					break;

					case 11:
						TopTen ( );
						FlagStopFreccia = 1;
					break;

					case 12:
						Istruzioni ( );
						FlagStopFreccia = 1;
					break;

					case 13: //Esci
						Flag = MessageBox ( NULL, "Uscendo perderai la configurazione delle impostazioni attuali. Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL );
						if ( Flag == 6 ) //Ha premuto Sì nel popup
						{
							Esci ( );
							FlagStopFreccia = 1;
							*FlagRistampaMenu = 0;
						}
					break;
				}
			break;
		}

	} while ( FlagStopFreccia != 1 );
}

void StampaSfondoMenu ( )
{
	StampaPattern ( 80 * 25 - 1 );
	PosizionaCursore ( CentraCursore ( 20 ), 1 );
	printf ( "  BATTAGLIA NAVALE  " );
	PosizionaCursore ( 31, 8 );
	printf ( "   NUOVA PARTITA  " );
	PosizionaCursore ( 31, 9 );
	printf ( "   CARICA PARTITA " );
	PosizionaCursore ( 31, 10 );
	printf ( "   IMPOSTAZIONI   " );
	PosizionaCursore ( 31, 11 );
	printf ( "   TOP TEN        " );
	PosizionaCursore ( 31, 12 );
	printf ( "   ISTRUZIONI     " );
	PosizionaCursore ( 72, 24 );
	printf ( "   ESCI" );
	ImpostaColore ( 0x6F );
	PosizionaCursore ( 2, 20 );
	printf ( " Scorri le voci con %c e %c ", 24, 25 );
	PosizionaCursore ( 2, 21 );
	printf ( " Premi INVIO per selezionarne una. " );
}

void AcquisisciNome ( char *Nome, int Colonna, int Riga )
{
	int i = 0;
	int InputTastiera = 0;

	do
	{
		PosizionaCursore ( Colonna + i, Riga );
		printf ( " " );
		PosizionaCursore ( Colonna + i, Riga );

		InputTastiera = getch ( );
		if ( InputTastiera == 224 || InputTastiera == 0 )
		{
			InputTastiera = getch ( );
		}
		else
		{
			if ( ( ( InputTastiera >= 48 && InputTastiera <= 57) || ( InputTastiera >= 97 && InputTastiera <= 122 ) || ( InputTastiera >= 65 && InputTastiera <= 90 ) ) && i < 24 ) //Se ha inserito una lettera maiuscola o minuscola o un numero ( e il contatore dei caratteri è minore di 25 )
			{
				PosizionaCursore ( Colonna + i, Riga );
				printf ( "%c", InputTastiera );
				Nome[i] = InputTastiera;
				i++;
				Nome[i] = '\0';
				printf ( " " );
				PosizionaCursore ( Colonna + i, Riga );
			}
			else
			{
				if ( InputTastiera == 8 && i > 0 ) //Se viene premuto Backspace
				{
					i--;
					PosizionaCursore ( Colonna + i, Riga );
					StampaPattern ( 2 ); //Cancella l'ultimo carattere immesso
					PosizionaCursore ( Colonna + i, Riga );
					Nome[i] = '\0';
				}
		   }
        }
    } while ( InputTastiera != 13 || i == 0 ); //Fin quando non viene premuto INVIO o non sono stati inseriti caratteri

}

void CancellaImpostazioniTemporanee ( )
{
	if ( ControlloPresenzaFile ( "NuoveImpostazioni.ini" ) )
		remove ( "NuoveImpostazioni.ini" );
}

int ControlloPresenzaFile ( char *Nome )
{
	int Flag = 0;
	FILE *fp = NULL;

	fp = fopen ( Nome, "r" );
	if ( fp == NULL )
		Flag = 0;
	else
	{
		Flag = 1;
		fclose ( fp );
	}
	return Flag;
}
