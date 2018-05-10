#include "Libreria.h"

void CaricaPartita ( )
{
	FILE *fp = NULL;
	int i = 0;
	int InputTastiera = 0;
	char Buffer = '0';
	char NomeFile[30] = "";
	int NumeroSalvataggi = 0;
	torneo TorneoInCorso;
	int FlagUscita = 0;
	giocatore *VincitoreTemporaneo;
	int NumeroPagine = 0;
	int PaginaCorrente = 1;
	int NumeroSalvataggiPaginaCorrente = 0;

	MostraCursore ( );
    system ( "cls" );
    StampaPattern ( 80 * 25 - 1 );
    i = CentraCursore ( 18 );
	PosizionaCursore ( i, 1 );
    printf ( "  CARICA PARTITA  " );
	i = 0;
    PosizionaCursore ( 1, 4 );
    printf ( " Elenco dei salvataggi: " );

	NumeroSalvataggi = IndicizzaSalvataggiValidi ( );

	fp = fopen ( "Indice.txt", "r" ); //L'indice di soli salvataggi validi
	if ( fp == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	Buffer = fgetc ( fp );

	if ( Buffer != EOF ) //Se c'è almeno un salvataggio (se non ci sono file *.tsg sarà creato un Indice.txt vuoto ed il suo primo carattere sarà EOF)
	{
		rewind ( fp ); //Recupero il carattere letto

		NumeroPagine = NumeroSalvataggi / 10;

		if ( NumeroSalvataggi % 10 != 0 )
			NumeroPagine++;

		if ( NumeroSalvataggi >= 10 )
			NumeroSalvataggiPaginaCorrente = 10;
		else
			NumeroSalvataggiPaginaCorrente = NumeroSalvataggi;

		//Stampo i nomi dei primi 10 salvataggi (se sono meno di 10, stampo solo quelli presenti)
		i = 0;
		ImpostaColore ( 0x6F );

		while ( !feof ( fp ) && i < 10 )
		{
			fscanf ( fp, "%s\n", NomeFile);
			PosizionaCursore ( 1, 6+i );
			printf ( " #%d: %s ", i, NomeFile );
			strcpy ( NomeFile, "" );
			i++;
		}

		rewind ( fp );

		if ( NumeroPagine > 1)
		{
			ImpostaColore ( 0x6F );
			PosizionaCursore ( 1, 20 );
			printf ( "Scorri i salvataggi con %c ", 18 ); //Carattere Freccia Su/Freccia Giù
		}

		ImpostaColore ( 0x3F );
		PosizionaCursore ( 1, 23 );
		printf ( " Digita il numero della partita da caricare:    " );
		PosizionaCursore ( 69, 24 );
		printf ( " Menu: ESC" );
		PosizionaCursore ( 46, 23 );

		FlagUscita = -1;

		do
		{
			InputTastiera = getch ( );
			if ( InputTastiera == 224 || InputTastiera == 0 )
				InputTastiera = getch ( );

			if ( InputTastiera >= 48 && InputTastiera - 48 < NumeroSalvataggiPaginaCorrente ) //Se ha premuto un tasto da 0 al numero di salvataggi della pagina
				FlagUscita = InputTastiera; //Uscirà dal do while (e non eseguirà niente dello switch)

			switch ( InputTastiera )
			{
				case 27: //ESC
					FlagUscita = 1;
				break;

				case 80: //Freccia GIU'
					if ( NumeroPagine > 1 && PaginaCorrente < NumeroPagine )
					{
						PaginaCorrente++;

						rewind ( fp );

						//Scorro il file fino ad arrivare al primo salvataggio da stampare
						i = 0;
						while ( i < PaginaCorrente * 10 - 10 )
						{
							Buffer = fgetc ( fp );
							if ( Buffer == '\n' ) //Ad ogni a capo corrisponde un salvataggio
								i++;
						}

						//Ripulisco lo spazio dove sono visualizzati i salvataggi, per evitare sovrapposizioni
						for ( i = 0; i < 10; i++ )
						{
							PosizionaCursore ( 1, 6 + i );
							StampaPattern ( 35 );
						}

						//Stampo i salvataggi successivi (al massimo 10)
						i = 0;
						NumeroSalvataggiPaginaCorrente = 0;
						while ( !feof ( fp ) && i < 10 )
						{
							fscanf ( fp, "%s\n", NomeFile );
							PosizionaCursore ( 1, 6 + i );
							ImpostaColore ( 0x6F );
							printf ( " #%d: %s ", i, NomeFile );
							strcpy ( NomeFile, "" );
							ImpostaColore ( 0x3F );
							i++;
							NumeroSalvataggiPaginaCorrente++;
						}
						PosizionaCursore ( 46, 23 );
					}
				break;

				case 72: //Freccia SU
					if ( NumeroPagine > 1 && PaginaCorrente > 1 )
					{
						PaginaCorrente--;

						rewind ( fp );

						//Scorro il file fino ad arrivare al primo salvataggio da stampare
						i = 0;
						while ( i < PaginaCorrente * 10 - 10 )
						{
							Buffer = fgetc ( fp );
							if ( Buffer == '\n' ) //Ad ogni a capo corrisponde un salvataggio
								i++;
						}

						//Ripulisco lo spazio dove sono visualizzati i salvataggi, per evitare sovrapposizioni
						for ( i = 0; i < 10; i++ )
						{
							PosizionaCursore ( 1, 6 + i );
							StampaPattern ( 35 );
						}

						//Stampo i salvataggi successivi (al massimo 10)
						i = 0;
						NumeroSalvataggiPaginaCorrente = 0;

						while ( !feof ( fp ) && i < 10 )
						{
							fscanf ( fp, "%s\n", NomeFile );
							PosizionaCursore ( 1, 6 + i );
							ImpostaColore ( 0x6F );
							printf( " #%d: %s ", i, NomeFile );
							strcpy ( NomeFile, "" );
							ImpostaColore ( 0x3F );
							i++;
							NumeroSalvataggiPaginaCorrente++;
						}
						PosizionaCursore ( 46, 23 );

					}
				break;
			}

		} while ( FlagUscita == -1 );


		if ( InputTastiera != 27) //Se non ha premuto ESC ( Deve aver premuto un numero da 0 a 9 necessariamente)
		{
			InputTastiera -= 48; //Converto l'ascii del numero premuto nell'intero corrispondente

			rewind ( fp );

			for ( i = 0; i < InputTastiera + PaginaCorrente * 10 - 10; i++ )
			{
				do
				{
					Buffer = fgetc ( fp );
				} while ( Buffer != '\n' ); //Leggo un numero di righe pari a InputTastiera: al termine il cursore del file si troverà al nome del file da aprire
			}

			fscanf ( fp, "%s", NomeFile );

			RicreaTorneo ( &TorneoInCorso, NomeFile );

			PlaySound ( NULL, 0, 0 ); //Sospendo l'esecuzione di musica (poteva essere stata attivata nel menu principale)

			FlagUscita = 0;
			if ( TorneoInCorso.FlagPartitaInCorso != -1 ) //Se il file non è corrotto
			{
				//Svolgimento del torneo
				for ( i = TorneoInCorso.Calendario.PartitaDaGiocare - 1 ; ( i < TorneoInCorso.Calendario.TotalePartite && FlagUscita == 0); i++ ) //Fin quando ci sono partite da giocare e il torneo non viene interrotto
				{
					VincitoreTemporaneo = AvviaPartita ( TorneoInCorso.Calendario.Match[i].Giocatore1, TorneoInCorso.Calendario.Match[i].Giocatore2, &TorneoInCorso );
					if ( VincitoreTemporaneo == NULL )
					{
						FlagUscita = 1;
					}
					else
					{
						TorneoInCorso.Calendario.Match[i].Vincitore = VincitoreTemporaneo;
						TorneoInCorso.Calendario.PartitaDaGiocare++;

						if ( i+1 != TorneoInCorso.Calendario.TotalePartite ) //Se non si è giocata l'ultima partita del torneo
						{
							//Inserisco il vincitore del match nella giusta posizione all'interno del tabellone per il tunro successivo - vedi nota in NuovaPartita
							if( i % 2 == 0 ) //Partita PARI
								TorneoInCorso.Calendario.Match[( i + ( TorneoInCorso.Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore1 = TorneoInCorso.Calendario.Match[i].Vincitore;

							if( i % 2 != 0 ) //Partita DISPARI
								TorneoInCorso.Calendario.Match[(i - 1 + ( TorneoInCorso.Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore2 = TorneoInCorso.Calendario.Match[i].Vincitore;

						}

						StampaCalendario ( &TorneoInCorso );

						if ( i+1 == TorneoInCorso.Calendario.TotalePartite ) //Se si è giocata l'ultima partita del torneo
						{
							CelebrazioneVincitoreTorneo ( TorneoInCorso.Calendario.Match[i].Vincitore, &TorneoInCorso.Impostazioni );
							
							//Deallocazione
							for ( i = 0; i < TorneoInCorso.Impostazioni.NumGiocatori; i++ )
							{
								 DeallocaLista ( TorneoInCorso.Partecipanti[i].GrigliaAttacco );
								 DeallocaLista ( TorneoInCorso.Partecipanti[i].GrigliaDifesa );
								 DeallocaLista ( TorneoInCorso.Partecipanti[i].MatricePosizionamento );
							}

							free ( TorneoInCorso.Partecipanti );
							free ( TorneoInCorso.Calendario.Match );
						}


					}

				}
			}
			else
			{
				system ( "cls" );
				PosizionaCursore ( 1, 1 );
				printf ( "Il file selezionato e' corrotto. Premi un tasto per tornare al menu..." );
				MostraCursore ( );
				getch ( );
				NascondiCursore ( );
			}

		}

	}
	else //Se non ci sono salvataggi
	{
		ImpostaColore ( 0x3F );
		PosizionaCursore ( 1, 23 );
		printf ( " Non sono presenti tornei da caricare. Premi ESC.  " );
		PosizionaCursore ( 69, 24 );
		printf ( " Menu: ESC" );
		NascondiCursore ( );

		do
		{
			InputTastiera = getch ( );
		} while ( InputTastiera != 27 ); //Finchè non viene premuto ESC
	}

	fclose ( fp );
	remove ( "Indice.txt" );

	if ( TorneoInCorso.Impostazioni.Audio == 0 )
		PlaySound ( "Audio\\MissionOnASleepwalk.WAV", NULL, SND_LOOP | SND_ASYNC );

}

void RicreaTorneo ( torneo* Torneo, char *NomeSalvataggio )
{
	FILE *fp = NULL;
	int i = 0;
	int j = 0;
	int FlagIntegrita = 0;
	int TempInt = 0;
	int Flag = 0;
	char TempNome[26] = "";


	fp = fopen ( NomeSalvataggio, "rb" );
	if ( fp == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	fscanf ( fp, "%d ", &FlagIntegrita );

	if ( FlagIntegrita == 123 ) //Numero-flag
	{

		fread ( &Torneo->Impostazioni, sizeof ( settaggi ), 1, fp );

		Torneo->Partecipanti = malloc ( Torneo->Impostazioni.NumGiocatori * sizeof ( giocatore ) );
		if ( Torneo->Partecipanti == NULL ) 
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}

		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{
			fscanf ( fp, "%s %d ", Torneo->Partecipanti[i].Nome, &Torneo->Partecipanti[i].Punteggio );

			InizializzaFlotta ( &Torneo->Partecipanti[i].Flotta, &Torneo->Impostazioni );

			for ( j = 0; j < Torneo->Impostazioni.NumSommergibili; j++ )
				fscanf ( fp, "%d %d ", &Torneo->Partecipanti[i].Flotta.Sommergibili[j].NonAffondato, &Torneo->Partecipanti[i].Flotta.Sommergibili[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumIncrociatori; j++ )
				fscanf ( fp, "%d %d ", &Torneo->Partecipanti[i].Flotta.Incrociatori[j].NonAffondato, &Torneo->Partecipanti[i].Flotta.Incrociatori[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumTorpediniere; j++ )
				fscanf ( fp, "%d %d ", &Torneo->Partecipanti[i].Flotta.Torpediniere[j].NonAffondato, &Torneo->Partecipanti[i].Flotta.Torpediniere[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumPortaerei; j++ )
				fscanf ( fp, "%d %d ", &Torneo->Partecipanti[i].Flotta.Portaerei[j].NonAffondato, &Torneo->Partecipanti[i].Flotta.Portaerei[j].Id );

			fscanf ( fp, "%d ", &Torneo->Partecipanti[i].Flotta.NaviNonAffondate );

			Torneo->Partecipanti[i].GrigliaAttacco = InizializzaListaInteri ( &Torneo->Impostazioni );

			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				fscanf ( fp, "%d ", &TempInt );
				ScriviElementoListaInteri ( Torneo->Partecipanti[i].GrigliaAttacco, j, TempInt );
			}

			Torneo->Partecipanti[i].GrigliaDifesa = InizializzaListaInteri ( &Torneo->Impostazioni );

			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				fscanf ( fp, "%d ", &TempInt );
				ScriviElementoListaInteri ( Torneo->Partecipanti[i].GrigliaDifesa, j, TempInt );
			}

			Torneo->Partecipanti[i].MatricePosizionamento = InizializzaListaInteri ( &Torneo->Impostazioni );

			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				fscanf ( fp, "%d ", &TempInt );
				ScriviElementoListaInteri ( Torneo->Partecipanti[i].MatricePosizionamento, j, TempInt );
			}

		}

		// Ricreo gli scontri
		CreaCalendario ( Torneo ); //Inizializza il primo turno e setta PartitaDaGiocare a 1

		fscanf ( fp, "%d ", &Torneo->Calendario.PartitaDaGiocare ); //Setta PartitaDaGiocare correttamente


		for ( i = 0; i < Torneo->Calendario.PartitaDaGiocare - 1; i++ )
		{
			//Reimposto il Giocatore1 di ogni partita giocata
			Flag = 0;
			fscanf ( fp, "%s ", TempNome );
			for ( j = 0; ( j < Torneo->Impostazioni.NumGiocatori && Flag == 0 ); j++ )
			{
				if ( strcmp ( TempNome, Torneo->Partecipanti[j].Nome ) == 0)
				{
					Torneo->Calendario.Match[i].Giocatore1 = &Torneo->Partecipanti[j];
					Flag = 1;
				}
			}
			strcpy ( TempNome, "" );

			//Reimposto il Giocatore2 di ogni partita giocata
			Flag = 0;
			fscanf ( fp, "%s ", TempNome );
			for ( j = 0; ( j < Torneo->Impostazioni.NumGiocatori && Flag == 0 ); j++ )
			{
				if ( strcmp ( TempNome, Torneo->Partecipanti[j].Nome ) == 0 )
				{
					Torneo->Calendario.Match[i].Giocatore2 = &Torneo->Partecipanti[j];
					Flag = 1;
				}
			}
			strcpy (TempNome, "" );

			//Reimposto il Vincitore di ogni partita giocata
			Flag = 0;
			fscanf ( fp, "%s ", TempNome );
			for ( j = 0; ( j < Torneo->Impostazioni.NumGiocatori && Flag == 0 ); j++ )
			{
				if ( strcmp ( TempNome, Torneo->Partecipanti[j].Nome) == 0 )
				{
					Torneo->Calendario.Match[i].Vincitore = &Torneo->Partecipanti[j];
					Flag = 1;
				}
			}
			strcpy ( TempNome, "" );
		}

/*
NB: NuovaPartita( ) inseriva il giocatore vincente di uno scontro nel campo Vincitore del medesimo scontro.
Inoltre, se non si era giocata l'ultima partita del torneo, copiava il giocatore vincente anche in una
determinata partita del turno successivo (perchè il vincitore deve continuare a giocare il torneo) o come Giocatore1 o come Giocatore2

La funzione SalvaPartita( ) scrive su file informazioni solo sulle partite già giocate.
Di conseguenza vengono perse tutte le informazioni sulle partite SUCCESSIVE a quelle giocate (che NuovaPartita ( ), invece, aveva gestito)
Il for seguente riprende tutti i vincitori delle partite già giocate e li reinserisce come Giocatore1 o Giocatore2
nei turni successivi. In questo modo il torneo caricato da file è perfettamente uguale a quello che si era scritto su file.
*/
		for ( i = 0; i < Torneo->Calendario.PartitaDaGiocare - 1 ; i++) //Itero sul numero di partite giocate: prendo il vincitore e lo copio nel turno successivo
		{
			//Inserisco il vincitore del match nella giusta posizione all'interno del tabellone per il tunro successivo
			if( i % 2 == 0 ) //Partita PARI
				Torneo->Calendario.Match[( i + ( Torneo->Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore1 = Torneo->Calendario.Match[i].Vincitore;

			if( i % 2 != 0 ) //Partita DISPARI
				Torneo->Calendario.Match[( i - 1 + ( Torneo->Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore2 = Torneo->Calendario.Match[i].Vincitore;

		}

		fscanf ( fp, "%d ", &TempInt );

		if( TempInt == 1 ) //Se una partita era in corso
		{
			Torneo->FlagPartitaInCorso = 1;

			fscanf ( fp, "%s ", TempNome ); //Acquisisco il nome del primo giocatore

			Flag = 0;
			for ( i = 0; ( i < Torneo->Impostazioni.NumGiocatori && Flag == 0 ); i++ )
			{
				if ( strcmp ( TempNome, Torneo->Partecipanti[i].Nome ) == 0 )
				{
					Torneo->PartitaInCorso.Giocatore1 = &Torneo->Partecipanti[i];
					Flag = 1;
				}
			}

			strcpy ( TempNome, "" );

			fscanf ( fp, "%s ", TempNome ); //Acquisisco il nome del secondo giocatore

			Flag = 0;
			for ( i = 0; ( i < Torneo->Impostazioni.NumGiocatori && Flag == 0 ); i++ )
			{
				if ( strcmp ( TempNome, Torneo->Partecipanti[i].Nome ) == 0 )
				{
					Torneo->PartitaInCorso.Giocatore2 = &Torneo->Partecipanti[i];
					Flag = 1;
				}
			}

			fscanf ( fp, "%d", &Torneo->PartitaInCorso.TurnoCorrente );
		}
		else
		{
			Torneo->FlagPartitaInCorso = 0;
		}

	}
	else
	{
		Torneo->FlagPartitaInCorso = -1; //-1 è un codice di errore. Se la variabile vale -1 il file è corrotto.
	}


	fclose ( fp );

}

int NomeSalvataggioValido ( char *Nome )
{
	int Flag = 1;
	int i = 0;

	if ( strlen ( Nome ) <= 28 )
	{
		for ( i = 0; i < (strlen(Nome) - 4); i++ ) //Escludo i controlli sull'estensione (i file indicizzati sono solo quelli *.tsg)
			if ( ( Nome[i] < 48 || Nome[i] > 57 ) && ( Nome[i] < 97 || Nome[i] > 122 ) && ( Nome[i] < 65 || Nome[i] > 90 ) ) //Se è un carattere non alfanumerico
				Flag = 0;
	}
	else
		Flag = 0;
	
	return Flag;
}

int IndicizzaSalvataggiValidi ( void )
{
	fpos_t PosizioneCorrente = 0;
	char *NomeMaxFile = NULL;
	char **VettoreNomi = NULL;
	int i = 0;
	int j = 0;
	FILE *fp = NULL;
	int FlagUscita = 0;
	char Buffer = '0';
	int NumeroSalvataggi = 0;

	//Se c'è, cancello il file Indice.txt
	if ( ControlloPresenzaFile ( "Indice.txt" ) )
		remove ( "Indice.txt" );

	//Scrivo nel file Indice.txt l'elenco di TUTTI i file con estensione .tsg
	system (" dir /B *.tsg > Indice.txt " ); //tsg = Tournament Savegame

	fp = fopen ( "Indice.txt", "r" );
	if ( fp == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	Buffer = fgetc ( fp );

	if ( Buffer == EOF ) //Se non ci sono file *.tsg sarà creato un Indice.txt vuoto (il suo primo carattere sarà EOF)
	{
		//Cancello la scritta "File non trovato" scritta da system(" dir /B *.tsg > Indice.txt ");
		ImpostaColore ( 0x3F );
		PosizionaCursore ( 25, 4 );
		StampaPattern ( 16 );
		NumeroSalvataggi = 0;
	}

	else
	{
		rewind ( fp ); //Recupero il carattere letto per verificare se il file fosse vuoto

		fgetpos ( fp, &PosizioneCorrente ); //Salvo la posizione corrente del file

		i = 0;

		//Controllo quali salvataggi, tra quelli presenti nella cartella (e dunque indicizzati nel file di testo), sono validi. Quelli validi li copio in VettoreNomi

		while ( FlagUscita == 0 ) //FlagUscita sarà settato a 1 solo se si incontrerà EOF
		{
			fsetpos ( fp, &PosizioneCorrente );

			do
			{
				Buffer = fgetc ( fp );

				//Alloco spazio per un nuovo carattere per NomeMaxFile
				if ( i == 0)
				{
					NomeMaxFile = malloc ( sizeof ( char ) );
					if ( NomeMaxFile == NULL )
					{
						system ( "cls" );
						PosizionaCursore ( 0, 0 );
						perror( "" );
						fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
						getch ( );
						exit ( -1 );
					}

				}
				if ( i > 0 ) 
				{
					NomeMaxFile = realloc ( NomeMaxFile, ( i + 1 ) * sizeof ( char ) );
					if ( NomeMaxFile == NULL )
					{
						system ( "cls" );
						PosizionaCursore ( 0, 0 );
						perror( "" );
						fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
						getch ( );
						exit ( -1 );
					}
				}

				NomeMaxFile[i] = Buffer;
				i++;
			} while ( Buffer != '\n' ); //Ad ogni a capo corrisponde un nuovo file

			NomeMaxFile[i-1] = '\0'; //Sostituisco \n con \0

			fgetpos ( fp, &PosizioneCorrente ); //Salvo la posizione corrente del file (leggerò un carattere per vedere se incontro EOF: se non è EOF quel carattere dovrà essere riletto dal do-while precedente)

			Buffer = fgetc ( fp );

			if ( Buffer == EOF ) //Controllo se è terminato il file
				FlagUscita = 1;

			i = 0;

			if ( NomeSalvataggioValido ( NomeMaxFile ) ) //Controllo se il file ha un nome di salvataggio valido (lunghezza massima di 24 caratteri e solo alfanumerico)
			{

				//Alloco spazio per un nuovo nome per VettoreNomi
				if ( NumeroSalvataggi == 0)
				{
					VettoreNomi = malloc ( sizeof (char * ) );
					if ( VettoreNomi == NULL )
					{
						system ( "cls" );
						PosizionaCursore ( 0, 0 );
						perror( "" );
						fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
						getch ( );
						exit ( -1 );

					}
				}

				if ( NumeroSalvataggi > 0)
				{
					VettoreNomi = realloc ( VettoreNomi, ( NumeroSalvataggi + 1 ) * sizeof ( char * ) );
					if ( VettoreNomi == NULL )
					{
						system ( "cls" );
						PosizionaCursore ( 0, 0 );
						perror( "" );
						fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
						getch ( );
						exit ( -1 );
					}
				}

				VettoreNomi[j] = malloc ( ( strlen ( NomeMaxFile ) + 1 ) * sizeof ( char ) );
				if ( VettoreNomi[j] == NULL )
				{
					system ( "cls" );
					PosizionaCursore ( 0, 0 );
					perror( "" );
					fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
					getch ( );
					exit ( -1 );
				}

				NumeroSalvataggi++;
				strcpy ( VettoreNomi[j], NomeMaxFile );
				j++;
				strcpy ( NomeMaxFile, "" );
			}
			
		}

		fclose ( fp );
		remove ( "Indice.txt" ); //Cancello l'indice di tutti i salvataggi e ne creo un altro contenente solo quelli validi

		fp = fopen ( "Indice.txt", "w" ); //Indice di salvataggi validi
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}

		for ( i = 0; i < NumeroSalvataggi; i++ )
			fprintf ( fp, "%s\n", VettoreNomi[i] );

		fclose ( fp );

	 }

	 return NumeroSalvataggi;
}

