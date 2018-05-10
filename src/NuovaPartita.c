#include "Libreria.h"

void NuovaPartita ( )
{

	int InputTastiera = 0;
	int i = 0;
	FILE *fp = NULL;
	torneo NuovoTorneo;
	giocatore *VincitoreTemporaneo = NULL;
	int FlagUscita = 0;
	HANDLE Handle;
	COORD Coordinate;
	DWORD CaratteriScritti;
	int Pattern = 177;


	system ( "cls" );
    StampaPattern ( 80 * 25 - 1 );
	MostraCursore ( );
	
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
		fread ( &NuovoTorneo.Impostazioni, sizeof ( settaggi ), 1, fp );
		fclose ( fp );

	}
	else //Se non esiste un file NuoveImpostazioni.ini, cioè se l'utente non è mai entrato nella voce Impostazioni del menu
	{
		if ( ControlloPresenzaFile ( "DefaultSettings.ini" ) == 0 )
			CreaImpostazioniDefault ( );

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
		fread ( &NuovoTorneo.Impostazioni, sizeof ( settaggi ), 1, fp );
		fclose ( fp );
	}


	NuovoTorneo.Partecipanti = malloc ( NuovoTorneo.Impostazioni.NumGiocatori * sizeof ( giocatore ) ); //Alloco tanti giocatori quanti sono salvati nelle impostazioni
	if ( NuovoTorneo.Partecipanti == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	for ( i = 0; i < NuovoTorneo.Impostazioni.NumGiocatori; i++ ) //Inizializzo tutti i campi di ogni giocatore
	{
		//Il nome lo si richiede dopo
		NuovoTorneo.Partecipanti[i].Punteggio = 0;
		InizializzaFlotta ( &NuovoTorneo.Partecipanti[i].Flotta, &NuovoTorneo.Impostazioni );
		NuovoTorneo.Partecipanti[i].GrigliaAttacco = InizializzaListaInteri ( &NuovoTorneo.Impostazioni );
		NuovoTorneo.Partecipanti[i].GrigliaDifesa = InizializzaListaInteri ( &NuovoTorneo.Impostazioni );
		NuovoTorneo.Partecipanti[i].MatricePosizionamento = InizializzaListaInteri( &NuovoTorneo.Impostazioni );
	}

	CreaCalendario ( &NuovoTorneo );
	NuovoTorneo.FlagPartitaInCorso = 0;

	PosizionaCursore ( CentraCursore(17) ,1 ); //17 = Lunghezza della scritta della riga successiva
	printf ( "  NUOVA PARTITA  " );
	PosizionaCursore ( 69, 24 );
	printf ( " Menu: ESC" );
	PosizionaCursore ( 2, 4 );
    printf ( " Si sfideranno %d giocatori. ", NuovoTorneo.Impostazioni.NumGiocatori );
	PosizionaCursore ( 2, 6 );
	printf ( " Premi 1 per inserire manualmente i loro nomi. " );
	PosizionaCursore ( 2, 7 );
    printf ( " Premi 2 per assegnarli automaticamente (Giocatore1, Giocatore2, ecc):   " );
	PosizionaCursore ( 73, 7 );

	do
	{
		InputTastiera = getch ( );
	} while ( InputTastiera != 27 && InputTastiera != 49 && InputTastiera != 50 ); //Ripete fin quando non viene premuto 1 o 2 o ESC

	if ( InputTastiera != 27 ) //Se non ha premuto ESC
	{

		if ( InputTastiera == 50 ) //Ha inserito 2: assegnamento automatico dei nomi
		{

			PosizionaCursore ( 0, 4 );
			StampaPattern ( 80 * 5 ); //Pulisce 5 righe di schermo

			for ( i = 0; i < NuovoTorneo.Impostazioni.NumGiocatori; i++ ) //Inizializza i nomi di tutti i giocatori
				sprintf ( NuovoTorneo.Partecipanti[i].Nome, "Giocatore%d", i + 1 ); //Scrivo "Giocatore" seguito dal suo numero
			
			PosizionaCursore ( 2, 4 );
			printf ( " I nomi dei giocatori sono stati inseriti automaticamente. " );
			PosizionaCursore ( 2, 6 );
			printf ( " Premi INVIO per continuare [ ] " );
			PosizionaCursore ( 31, 6 );
		}

		if ( InputTastiera == 49 ) //Ha inserito 1: assegnamento manuale dei nomi
		{

			PosizionaCursore ( 0, 4 );
			StampaPattern ( 80 * 5 ); //Pulisce 5 righe di schermo

			PosizionaCursore ( 2, 4 );
			printf ( " Inserisci manualmente i nomi dei giocatori. " );
			PosizionaCursore ( 2, 5 );
			printf ( " Sono ammessi nomi alfanumerici di massimo 24 caratteri. " );
			PosizionaCursore ( 2, 24 );
			printf ( " N.B.: Puoi inserire soltanto nomi tutti diversi tra loro. " );

			//Nascondo la scritta Menu: ESC
			PosizionaCursore ( 69, 24 );
			StampaPattern ( 10 );
			
			Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
			Coordinate.X = 79;
			Coordinate.Y = 24;
			FillConsoleOutputCharacter ( Handle, Pattern, 1, Coordinate, &CaratteriScritti ); //Stampa il pattern nel pixel in basso a destra

			for ( i = 0; i < NuovoTorneo.Impostazioni.NumGiocatori; i++ ) //Inizializza tutti i nomi dei giocatore
			{
				do
				{
					strcpy ( NuovoTorneo.Partecipanti[i].Nome, "" );
					PosizionaCursore ( 2, 7 + i );
					StampaPattern ( 50 );
					PosizionaCursore ( 2, 7 + i );
					printf ( " Nome Giocatore%d: ", i + 1 );
					PosizionaCursore ( 23, 7 + i );
					AcquisisciNome ( NuovoTorneo.Partecipanti[i].Nome, 23, 7 + i );
				} while ( NuovoNome ( &NuovoTorneo, i ) != 1 ); //Fin quando non inserisce un nome nuovo

			}

			PosizionaCursore ( 69, 24 );
			printf ( " Menu: ESC" );
			Pattern = 32; //Spazio
			FillConsoleOutputCharacter ( Handle, Pattern, 1, Coordinate, &CaratteriScritti ); //Stampa lo spazio nel pixel in basso a destra

			//Ripulisce alcune scritte della finestra
			PosizionaCursore ( 2, 24 );
			StampaPattern ( 66 );

			PosizionaCursore ( 2, 8 + i );
			printf ( " Premi INVIO per continuare [ ] " );
			PosizionaCursore ( 31, 8 + i );


		}


		do
		{
			InputTastiera = getch ( );
		} while ( InputTastiera != 13 && InputTastiera != 27 ); //Finchè è diverso da INVIO e ESC

		if ( InputTastiera == 13 ) //Se ha premuto INVIO
		{

			PlaySound ( NULL, 0, 0 ); //Sospende l'esecuzione di musica (poteva essere stata attivata nel menu iniziale)

			StampaCalendario ( &NuovoTorneo );

			//Svolgimento delle partite del torneo
			for ( i = 0; ( i < NuovoTorneo.Calendario.TotalePartite && FlagUscita == 0 ); i++ ) //Fin quando ci sono partite da giocare e il torneo non viene interrotto
			{
				VincitoreTemporaneo = AvviaPartita ( NuovoTorneo.Calendario.Match[i].Giocatore1, NuovoTorneo.Calendario.Match[i].Giocatore2, &NuovoTorneo );
				if ( VincitoreTemporaneo == NULL )
				{
					FlagUscita = 1;
				}
				else
				{
					NuovoTorneo.Calendario.Match[i].Vincitore = VincitoreTemporaneo;
					NuovoTorneo.Calendario.PartitaDaGiocare++;
					if ( i + 1 != NuovoTorneo.Calendario.TotalePartite ) //Se non si è giocata l'ultima partita del torneo
					{
						/*Inserisco il vincitore della partita nel match che dovrà giocare nel turno successivo.
						Le formule usate per calcolare gli indici permettono di determinare, in base alla partita
						che si è appena giocata ed in base al numero di giocatori del torneo, quale sarà il prossimo 
						match che il vincitore dovrà disputare (e se sarà il primo giocatore o il secondo di quel match)*/
						
						if ( i % 2 == 0 ) //Partita PARI
							NuovoTorneo.Calendario.Match[( i + ( NuovoTorneo.Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore1 = NuovoTorneo.Calendario.Match[i].Vincitore;

						if ( i % 2 != 0 ) //Partita DISPARI
							NuovoTorneo.Calendario.Match[( i - 1 + ( NuovoTorneo.Impostazioni.NumGiocatori / 2 ) - ( i / 2 ) )].Giocatore2 = NuovoTorneo.Calendario.Match[i].Vincitore;

					}

					StampaCalendario ( &NuovoTorneo );

					if ( i + 1 == NuovoTorneo.Calendario.TotalePartite ) //Se si è giocata l'ultima partita del torneo
					{
						CelebrazioneVincitoreTorneo ( NuovoTorneo.Calendario.Match[i].Vincitore, &NuovoTorneo.Impostazioni );

						//Dealloco liste e vettori
						for ( i = 0; i < NuovoTorneo.Impostazioni.NumGiocatori; i++ )
						{
							 DeallocaLista ( NuovoTorneo.Partecipanti[i].GrigliaAttacco );
							 DeallocaLista ( NuovoTorneo.Partecipanti[i].GrigliaDifesa );
							 DeallocaLista ( NuovoTorneo.Partecipanti[i].MatricePosizionamento );
						}

						free ( NuovoTorneo.Partecipanti );
						free ( NuovoTorneo.Calendario.Match );
					}
				}

			}

		}

	}

	if ( NuovoTorneo.Impostazioni.Audio == 0 )
		PlaySound ( "Audio\\MissionOnASleepwalk.WAV", NULL, SND_LOOP | SND_ASYNC );
}

giocatore* AvviaPartita ( giocatore *GiocatoreA, giocatore *GiocatoreB, torneo *Torneo )
{

	int FinePartita = 0;
	int TurnoCorrente = 0;
	int QuadranteOrizzontaleAttacco_A = 1;
	int QuadranteVerticaleAttacco_A = 1;
	int QuadranteOrizzontaleDifesa_A = 1;
	int QuadranteVerticaleDifesa_A = 1;
	int QuadranteOrizzontaleAttacco_B = 1;
	int QuadranteVerticaleAttacco_B = 1;
	int QuadranteOrizzontaleDifesa_B = 1;
	int QuadranteVerticaleDifesa_B = 1;
	int InputTastiera = 0;
	int AscissaMirino = 3;
	int OrdinataMirino = 6;
	int AscissaCella = 0;
	int OrdinataCella = 0;
	int FlagAcqua = 0;
	int i = 0;
	char Carattere = 27; //Freccia <--
	char StringaPunteggi[250] = "";
	int IndiceLista = 0;
	int TempInt = 0; //Intero Temporaneo
	int Flag = 0;
	int FlagUscita = 0;
	giocatore* Vincitore = NULL;
	int FlagSaltaPartita = 0;

	char MessaggioHelp[]= "F1, F2, F3, F4, F5, F6, F7, F8 (o il mirino) scorrono i quadranti:\nF1 mostra il quadrante a sinistra in attacco\nF2 mostra il quadrante a destra in attacco\nF3 mostra il quadrante in alto in attacco\nF4 mostra il quadrante in basso in attacco\nF5 mostra il quadrante a sinistra in difesa\nF6 mostra il quadrante a destra in difesa\nF7 mostra il quadrante in alto in difesa\nF8 mostra il quadrante in basso in difesa\n\nMuovi il mirino con le frecce direzionali\nINVIO Attaccare\n  Mostra punteggi\nESC Esci\nF9 Salva\nTAB Mostra legenda colori";
	char StringaLegendaColori[] = "Celeste: Cella non chiamata\nBlu: Acqua\nRosso: Nave colpita\nNero: Nave affondata\nBianco: Nave non colpita";

	Carattere = 92; // \: Slash
	MessaggioHelp[468] = Carattere; //Inserisco il carattere \ all'interno della stringa (non si può farlo al momento della dichiarazione)

	PlaySound ( NULL, 0, 0 ); //Sospende l'esecuzione di eventuale musica

	system ( "cls" );
	PosizionaCursore ( 1, 1 );
	printf ( "MATCH %d: %s vs %s", Torneo->Calendario.PartitaDaGiocare, GiocatoreA->Nome, GiocatoreB->Nome );

	if ( Torneo->FlagPartitaInCorso == 0 ) //Se non è in corso nesssuna partita
	{
		PosizionaCursore ( 1, 3 );
		printf ( "%s deve posizionare le sue navi:", GiocatoreA->Nome );
		PosizionaCursore ( 1, 4 );
		printf ( "%s e' invitato a voltarsi.", GiocatoreB->Nome );
		PosizionaCursore ( 1, 7 );
		printf ( "%s premi un tasto per cominciare a disporre le navi ", GiocatoreA->Nome );
		InputTastiera = getch ( );
		if ( InputTastiera == 224 || InputTastiera == 0 )
			InputTastiera = getch ( );
		ProceduraPosizionamento ( GiocatoreA, Torneo );

		if ( Torneo->FlagPartitaInCorso == -1 ) //Se il posizionamento non è stato completato
			FlagSaltaPartita = 1;

		if ( FlagSaltaPartita != 1 )
		{
			system ( "cls" );
			PosizionaCursore ( 1, 1 );
			printf ( "MATCH %d: %s vs %s", Torneo->Calendario.PartitaDaGiocare, GiocatoreA->Nome, GiocatoreB->Nome );
			PosizionaCursore ( 1, 3 );
			printf ( "%s deve posizionare le sue navi:", GiocatoreB->Nome );
			PosizionaCursore ( 1, 4 );
			printf ( "%s e' invitato a voltarsi.", GiocatoreA->Nome );
			PosizionaCursore ( 1, 7 );
			printf ( "%s premi un tasto per cominciare a disporre le navi ", GiocatoreB->Nome );
			MostraCursore ( );
			InputTastiera = getch ( );
			if ( InputTastiera == 224 || InputTastiera == 0 )
				InputTastiera = getch ( );
			ProceduraPosizionamento ( GiocatoreB, Torneo );

			if ( Torneo->FlagPartitaInCorso == -1 ) //Se il posizionamento non è stato completato
				FlagSaltaPartita = 1;
		}

	}
	else
	{
		PosizionaCursore ( 1, 5 );
		printf ( "Premi un tasto per continuare " );
		MostraCursore ( );
		getch ( );
		NascondiCursore ( );

	}

	if( FlagSaltaPartita != 1 ) //Se ha terminato il posizionamento delle navi di entrambi i giocatori
	{

		if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
			PlaySound ( "Audio\\GameStarted.WAV", NULL, SND_ASYNC );

		//Imposto il primo giocatore di turno, in base alle opzioni del torneo o del salvataggio
		if ( Torneo->Impostazioni.PrimoGiocatore == 1 )
			TurnoCorrente = 1;
		if ( Torneo->Impostazioni.PrimoGiocatore == 2 )
			TurnoCorrente = 2;
		if ( Torneo->Impostazioni.PrimoGiocatore == 0 )
		{
			srand ( time ( NULL ) );
			TurnoCorrente = 1 + rand ( ) % 2;
		}
		if ( Torneo->Impostazioni.PrimoGiocatore == 3 )
		{
			system ( "cls" );
			PosizionaCursore ( 1, 1 );
			printf ( "Hai scelto di inserire manualmente il primo giocatore di turno." );
			PosizionaCursore ( 1, 3 );
			printf ( "Premi 1 per %s o 2 per %s: ", GiocatoreA->Nome, GiocatoreB->Nome );
			MostraCursore ( );
			do
			{
				TurnoCorrente = getch ( );
			} while ( TurnoCorrente != 49 && TurnoCorrente != 50 ); //Ripete fin quando non viene premuto 1 o 2
			TurnoCorrente -= 48;
			printf ( "%d", TurnoCorrente ); //Simulo l'echo da tastiera

			PosizionaCursore ( 1, 5 );
			if ( TurnoCorrente == 1 )
				printf ( "Il primo a giocare sara' %s.", GiocatoreA->Nome );
			if ( TurnoCorrente == 2 )
				printf ( "Il primo a giocare sara' %s.", GiocatoreB->Nome );

			PosizionaCursore ( 1, 7 );
			printf ( "Premi un tasto per continuare " );
			getch ( );

		}

		if ( Torneo->FlagPartitaInCorso == 1 )
			TurnoCorrente = Torneo->PartitaInCorso.TurnoCorrente;

		//Comincia la partita: setto il flag
		Torneo->FlagPartitaInCorso = 1;

		do{
			if ( TurnoCorrente == 1 )
			{

				StampaSchermataAttesa ( GiocatoreA, GiocatoreB );
				StampaSfondoCampo ( );
				StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
				StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
				StampaMirino ( OrdinataMirino, AscissaMirino );
				PosizionaCursore ( 1, 24 );
				printf ( "Turno: %s", GiocatoreA->Nome );

				do{

					InputTastiera = getch ( );

					switch ( InputTastiera )
					{
						case 27: //ESC: Esci dal gioco
							Flag = MessageBox ( NULL, "Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL );
							if ( Flag == 6 ) //Ha premuto Sì nel popup
							{
								Flag = MessageBox ( NULL, "Vuoi salvare i progressi di gioco?", "Salvataggio", MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL );

								if ( Flag == 6 ) //Ha premuto Sì nel secondo popup
									SalvaPartita ( Torneo, GiocatoreA, GiocatoreB, TurnoCorrente, Torneo->FlagPartitaInCorso );

								FlagUscita = 1;
							}
						break;

						case 59: //F1: Spostati nel quadrante orizzontale a sinistra nella griglia di attacco
							if ( QuadranteOrizzontaleAttacco_A > 1 )
							{
								QuadranteOrizzontaleAttacco_A = QuadranteOrizzontaleAttacco_A - 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 60: //F2: Spostati nel quadrante orizzontale a destra nella griglia di attacco
							if ( QuadranteOrizzontaleAttacco_A < (Torneo->Impostazioni.NumColonne / 10 ) )
							{
								QuadranteOrizzontaleAttacco_A = QuadranteOrizzontaleAttacco_A + 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 61: //F3: Spostati nel quadrante verticale inferiore nella griglia di attacco
							if ( QuadranteVerticaleAttacco_A > 1 )
							{
								QuadranteVerticaleAttacco_A = QuadranteVerticaleAttacco_A - 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 62: //F4: Spostati nel quadrante verticale superiore nella griglia di attacco
							if ( QuadranteVerticaleAttacco_A < (Torneo->Impostazioni.NumRighe / 10 ) )
							{
								QuadranteVerticaleAttacco_A = QuadranteVerticaleAttacco_A + 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 63: //F5: Spostati nel quadrante orizzontale a sinistra nella griglia di difesa
							if ( QuadranteOrizzontaleDifesa_A > 1 )
							{
								QuadranteOrizzontaleDifesa_A = QuadranteOrizzontaleDifesa_A - 1;
								StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 64: //F6: Spostati nel quadrante orizzontale a destra nella griglia di difesa
							if ( QuadranteOrizzontaleDifesa_A < (Torneo->Impostazioni.NumColonne / 10 ) )
							{
								QuadranteOrizzontaleDifesa_A = QuadranteOrizzontaleDifesa_A + 1;
								StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 65: //F7: Spostati nel quadrante verticale inferiore nella griglia di difesa
							if ( QuadranteVerticaleDifesa_A > 1 )
							{
								QuadranteVerticaleDifesa_A = QuadranteVerticaleDifesa_A - 1;
								StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 66: //F8: Spostati nel quadrante verticale superiore nella griglia di difesa
							if ( QuadranteVerticaleDifesa_A < (Torneo->Impostazioni.NumRighe / 10 ) )
							{
								QuadranteVerticaleDifesa_A = QuadranteVerticaleDifesa_A + 1;
								StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 72: //Freccia SU: Sposta il mirino su
							if ( AscissaMirino == 3 && QuadranteVerticaleAttacco_A > 1 ) //Il mirino si trova sulla prima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 21; //Imposto il mirino sull'ultima riga
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticaleAttacco_A = QuadranteVerticaleAttacco_A - 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino > 3 && AscissaMirino <= 21 ) //Il mirino si trova su una qualunque riga tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino - 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 80: //Freccia GIU': Sposta il mirino giù
							if ( AscissaMirino == 21 && QuadranteVerticaleAttacco_A < (Torneo->Impostazioni.NumRighe / 10 ) ) //Il mirino si trova sull'ultima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 3; //Imposto il mirino alla prima riga
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticaleAttacco_A = QuadranteVerticaleAttacco_A + 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino >= 3 && AscissaMirino < 21 ) //Il mirino si trova su una qualunque riga tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino + 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 75: //Freccia SINISTRA: Sposta il mirino a sinistra
							if ( OrdinataMirino == 6 && QuadranteOrizzontaleAttacco_A > 1 ) //Il mirino si trova sulla prima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 42; //Imposta il mirino all'ultima colonna
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontaleAttacco_A = QuadranteOrizzontaleAttacco_A - 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino > 6 && OrdinataMirino <= 42 ) //Il mirino si trova su una qualunque colonna tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino - 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 77: //Freccia DESTRA: Sposta il mirino a destra
							if ( OrdinataMirino == 42 && QuadranteOrizzontaleAttacco_A < ( Torneo->Impostazioni.NumColonne / 10 ) ) //Il mirino si trova sull'ultima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 6; //Imposto il mirino alla prima colonna
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontaleAttacco_A = QuadranteOrizzontaleAttacco_A + 1;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino >= 6 && OrdinataMirino < 42) //Il mirino si trova su una qualunque colonna tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino + 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 67: //F9: Salva
							SalvaPartita ( Torneo, GiocatoreA, GiocatoreB, TurnoCorrente, 1 );
							StampaSfondoCampo ( );
							StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
							StampaQuadranteDifesa ( GiocatoreA->GrigliaDifesa, QuadranteOrizzontaleDifesa_A, QuadranteVerticaleDifesa_A, Torneo->Impostazioni.NumColonne );
							StampaMirino ( OrdinataMirino, AscissaMirino );
							PosizionaCursore ( 1, 24 );
							printf ( "Turno: %s", GiocatoreA->Nome );
							NascondiCursore ( );
						break;

						case 68: //F10: Help
							MessageBox ( NULL, MessaggioHelp, "Comandi del gioco", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 92: // \: Punteggi
							sprintf ( StringaPunteggi, "%s: %d\n%s: %d\n\n\nNota Bene:\n\nAcqua: -3 pt\nColpito: +5 pt\nSommergibile affondato: +15 pt\nIncrociatore affondato: +11 pt\nTorpedieniera affondata: +8 pt\nPortaerei affondata: +5 pt\nMatch vinto: +20 pt", GiocatoreA->Nome, GiocatoreA->Punteggio, GiocatoreB->Nome, GiocatoreB->Punteggio );
							MessageBox ( NULL, StringaPunteggi, "Punteggi", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 9: // TAB: Legenda Colori
							MessageBox ( NULL, StringaLegendaColori, "Legenda colori", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 13: // INVIO: Attacca
							//Converto le coordinate del mirino nell'elemento della lista corrispondente
							AscissaCella = ( OrdinataMirino - 6 ) / 4 + ( QuadranteOrizzontaleAttacco_A - 1 ) * 10;
							OrdinataCella = ( AscissaMirino - 3 ) / 2 + ( QuadranteVerticaleAttacco_A - 1 ) * 10;
							IndiceLista = OrdinataCella * Torneo->Impostazioni.NumColonne + AscissaCella;

							TempInt = LeggiElementoListaInteri ( GiocatoreB->GrigliaDifesa, IndiceLista );

							if ( TempInt >= 1 && TempInt <= 3 ) //Cella già chiamata
							{
								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "Cella chiamata. Scegli un'altra." );

							}
							else if ( TempInt == 0 ) //Acqua
							{

								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\MissShip.WAV", NULL, SND_ASYNC );

								GiocatoreA->Punteggio -= 3;
								ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, IndiceLista, 3 );
								ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, IndiceLista, 3 );
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );
								FlagAcqua = 1; //Finisce il turno del giocatore
								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "ACQUA! Premi un tasto." );
								InputTastiera = getch ( );
								if ( InputTastiera == 224 || InputTastiera == 0 )
									InputTastiera = getch ( );
							}
							else if ( TempInt == 4 ) //Colpito
							{

								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\ShipHit.WAV", NULL, SND_ASYNC );

								GiocatoreA->Punteggio += 5;
								ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, IndiceLista, 1 );
								ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, IndiceLista, 1 );
								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "COLPITO! Continua ad attaccare." );

								// Rintraccio la nave colpita in base all'Id. Scrollo tutte le navi.
								TempInt = LeggiElementoListaInteri ( GiocatoreB->MatricePosizionamento, IndiceLista );
								Flag = 0;

								for ( i = 0; ( i < Torneo->Impostazioni.NumSommergibili && Flag == 0 ); i++ )
								{
									if ( GiocatoreB->Flotta.Sommergibili[i].Id == TempInt )
									{
										GiocatoreB->Flotta.Sommergibili[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreB->Flotta.Sommergibili[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreB->Flotta.NaviNonAffondate--;

											GiocatoreA->Punteggio += 15;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreB->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumIncrociatori && Flag == 0 ); i++ )
								{
									if ( GiocatoreB->Flotta.Incrociatori[i].Id == TempInt )
									{
										GiocatoreB->Flotta.Incrociatori[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreB->Flotta.Incrociatori[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreB->Flotta.NaviNonAffondate--;

											GiocatoreA->Punteggio += 11;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreB->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumTorpediniere && Flag == 0 ); i++ )
								{
									if ( GiocatoreB->Flotta.Torpediniere[i].Id == TempInt )
									{
										GiocatoreB->Flotta.Torpediniere[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreB->Flotta.Torpediniere[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreB->Flotta.NaviNonAffondate--;

											GiocatoreA->Punteggio += 8;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreB->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumPortaerei && Flag == 0 ); i++ )
								{
									if ( GiocatoreB->Flotta.Portaerei[i].Id == TempInt )
									{
										GiocatoreB->Flotta.Portaerei[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreB->Flotta.Portaerei[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreB->Flotta.NaviNonAffondate--;

											GiocatoreA->Punteggio += 5;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreB->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreB->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreA->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								Flag = 0;
								StampaQuadranteAttacco ( GiocatoreA->GrigliaAttacco, QuadranteOrizzontaleAttacco_A, QuadranteVerticaleAttacco_A, Torneo->Impostazioni.NumColonne );


								if ( GiocatoreB->Flotta.NaviNonAffondate == 0 )
								{
									Vincitore = GiocatoreA;
									GiocatoreA->Punteggio += 20;
									FlagAcqua = 1; //Finisce il turno del giocatore
									FinePartita = 1;
									if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
										PlaySound ( "Audio\\GameWon.WAV", NULL, SND_ASYNC );
								}
							}

						break;

					}//Fine switch

				} while ( FlagAcqua == 0 && FlagUscita == 0 );

				TurnoCorrente = 2;
				AscissaMirino = 3;
				OrdinataMirino = 6;
				FlagAcqua = 0;
			}


			else if ( TurnoCorrente == 2 )
			{

				StampaSchermataAttesa ( GiocatoreB, GiocatoreA );
				StampaSfondoCampo ( );
				StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
				StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
				StampaMirino ( OrdinataMirino, AscissaMirino );
				PosizionaCursore ( 1, 24 );
				printf ( "Turno: %s", GiocatoreB->Nome );

				do{

					InputTastiera = getch ( );

					switch ( InputTastiera )
					{
						case 27: //ESC: Esci dal gioco
							Flag = MessageBox ( NULL, "Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL );
							if ( Flag == 6 ) //Ha premuto Sì nel popup
							{
								Flag = MessageBox ( NULL, "Vuoi salvare i progressi di gioco?", "Salvataggio", MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL );

								if ( Flag == 6 ) //Ha premuto Sì nel secondo popup
									SalvaPartita ( Torneo, GiocatoreA, GiocatoreB, TurnoCorrente, Torneo->FlagPartitaInCorso );

								FlagUscita = 1;
							}
						break;

						case 59: //F1: Spostati nel quadrante orizzontale a sinistra nella griglia di attacco
							if ( QuadranteOrizzontaleAttacco_B > 1 )
							{
								QuadranteOrizzontaleAttacco_B = QuadranteOrizzontaleAttacco_B - 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 60: //F2: Spostati nel quadrante orizzontale a destra nella griglia di attacco
							if ( QuadranteOrizzontaleAttacco_B < ( Torneo->Impostazioni.NumColonne / 10 ) )
							{
								QuadranteOrizzontaleAttacco_B = QuadranteOrizzontaleAttacco_B + 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 61: //F3: Spostati nel quadrante verticale inferiore nella griglia di attacco
							if ( QuadranteVerticaleAttacco_B > 1 )
							{
								QuadranteVerticaleAttacco_B = QuadranteVerticaleAttacco_B - 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 62: //F4: Spostati nel quadrante verticale superiore nella griglia di attacco
							if ( QuadranteVerticaleAttacco_B < (Torneo->Impostazioni.NumRighe / 10 ) )
							{
								QuadranteVerticaleAttacco_B = QuadranteVerticaleAttacco_B + 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 63: //F5: Spostati nel quadrante orizzontale a sinistra nella griglia di difesa
							if ( QuadranteOrizzontaleDifesa_B > 1 )
							{
								QuadranteOrizzontaleDifesa_B = QuadranteOrizzontaleDifesa_B - 1;
								StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 64: //F6: Spostati nel quadrante orizzontale a destra nella griglia di difesa
							if ( QuadranteOrizzontaleDifesa_B < (Torneo->Impostazioni.NumColonne / 10 ) )
							{
								QuadranteOrizzontaleDifesa_B = QuadranteOrizzontaleDifesa_B + 1;
								StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 65: //F7: Spostati nel quadrante verticale inferiore nella griglia di difesa
							if ( QuadranteVerticaleDifesa_B > 1 )
							{
								QuadranteVerticaleDifesa_B = QuadranteVerticaleDifesa_B - 1;
								StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 66: //F8: Spostati nel quadrante verticale superiore nella griglia di difesa
							if ( QuadranteVerticaleDifesa_B < (Torneo->Impostazioni.NumRighe / 10 ) )
							{
								QuadranteVerticaleDifesa_B = QuadranteVerticaleDifesa_B + 1;
								StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 72: //Freccia SU: Sposta il mirino su
							if ( AscissaMirino == 3 && QuadranteVerticaleAttacco_B > 1 ) //Il mirino si trova sulla prima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 21; //Imposto il mirino sull'ultima riga
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticaleAttacco_B = QuadranteVerticaleAttacco_B - 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino > 3 && AscissaMirino <= 21 ) //Il mirino si trova su una qualunque riga tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino - 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 80: //Freccia GIU': Sposta il mirino giù
							if ( AscissaMirino == 21 && QuadranteVerticaleAttacco_B < ( Torneo->Impostazioni.NumRighe / 10 ) ) //Il mirino si trova sull'ultima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 3; //Imposto il mirino alla prima riga
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticaleAttacco_B = QuadranteVerticaleAttacco_B + 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino >= 3 && AscissaMirino < 21 ) //Il mirino si trova su una qualunque riga tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino + 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 75: //Freccia SINISTRA: Sposta il mirino a sinistra
							if (OrdinataMirino == 6 && QuadranteOrizzontaleAttacco_B > 1 ) //Il mirino si trova sulla prima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 42; //Imposta il mirino all'ultima colonna
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontaleAttacco_B = QuadranteOrizzontaleAttacco_B - 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino > 6 && OrdinataMirino <= 42 ) //Il mirino si trova su una qualunque colonna tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino - 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 77: //Freccia DESTRA: Sposta il mirino a destra
							if ( OrdinataMirino == 42 && QuadranteOrizzontaleAttacco_B < ( Torneo->Impostazioni.NumColonne / 10 ) ) //Il mirino si trova sull'ultima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 6; //Imposta il mirino alla prima colonna
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontaleAttacco_B = QuadranteOrizzontaleAttacco_B + 1;
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino >= 6 && OrdinataMirino < 42) //Il mirino si trova su una qualunque colonna tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino + 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 67: //F9: Salva
							SalvaPartita ( Torneo, GiocatoreA, GiocatoreB, TurnoCorrente, 1 );
							StampaSfondoCampo ( );
							StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
							StampaQuadranteDifesa ( GiocatoreB->GrigliaDifesa, QuadranteOrizzontaleDifesa_B, QuadranteVerticaleDifesa_B, Torneo->Impostazioni.NumColonne );
							StampaMirino ( OrdinataMirino, AscissaMirino );
							PosizionaCursore ( 1, 24 );
							printf ( "Turno: %s", GiocatoreB->Nome );
							NascondiCursore ( );
						break;

						case 68: //F10: Help
							MessageBox ( NULL, MessaggioHelp, "Comandi del gioco", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 92: // \: Punteggi
							sprintf ( StringaPunteggi, "%s: %d\n%s: %d\n\n\nNota Bene:\n\nAcqua: -3 pt\nColpito: +5 pt\nSommergibile affondato: +15 pt\nIncrociatore affondato: +11 pt\nTorpedieniera affondata: +8 pt\nPortaerei affondata: +5 pt\nMatch vinto: +20 pt", GiocatoreA->Nome, GiocatoreA->Punteggio, GiocatoreB->Nome, GiocatoreB->Punteggio );
							MessageBox ( NULL, StringaPunteggi, "Punteggi", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 9: // TAB: Legenda Colori
							MessageBox ( NULL, StringaLegendaColori, "Legenda colori", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 13: // INVIO: Attacca
							//Converto le coordinate del mirino nell'elemento della lista corrispondente
							AscissaCella = ( OrdinataMirino - 6 ) / 4 + ( QuadranteOrizzontaleAttacco_B - 1 ) * 10;
							OrdinataCella = ( AscissaMirino - 3 ) / 2 + ( QuadranteVerticaleAttacco_B - 1) * 10;
							IndiceLista = OrdinataCella * Torneo->Impostazioni.NumColonne + AscissaCella;

							TempInt = LeggiElementoListaInteri ( GiocatoreA->GrigliaDifesa, IndiceLista );

							if ( TempInt >= 1 && TempInt <= 3 ) //Cella già chiamata
							{
								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "Cella chiamata. Scegli un'altra." );

							}
							else if ( TempInt == 0 ) //Acqua
							{

								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\MissShip.WAV", NULL, SND_ASYNC );

								GiocatoreB->Punteggio -= 3;
								ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, IndiceLista, 3 );
								ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, IndiceLista, 3 );
								StampaQuadranteAttacco ( GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );
								FlagAcqua = 1; //Finisce il turno del giocatore
								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "ACQUA! Premi un tasto." );
								InputTastiera = getch ( );
								if ( InputTastiera == 224 || InputTastiera == 0)
									InputTastiera = getch ( );
							}
							else if ( TempInt == 4 ) //Colpito
							{
								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\ShipHit.WAV", NULL, SND_ASYNC );

								GiocatoreB->Punteggio += 5;
								ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, IndiceLista, 1 );
								ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, IndiceLista, 1 );
								PosizionaCursore ( 36, 24 );
								printf ( "                                " );
								PosizionaCursore ( 36, 24 );
								printf ( "COLPITO! Continua ad attaccare." );

								// Rintraccio la nave colpita in base all'Id. Scrollo tutte le navi.
								TempInt = LeggiElementoListaInteri ( GiocatoreA->MatricePosizionamento, IndiceLista );
								Flag = 0;

								for ( i = 0; ( i < Torneo->Impostazioni.NumSommergibili && Flag == 0 ); i++ )
								{
									if ( GiocatoreA->Flotta.Sommergibili[i].Id == TempInt )
									{
										GiocatoreA->Flotta.Sommergibili[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreA->Flotta.Sommergibili[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreA->Flotta.NaviNonAffondate--;

											GiocatoreB->Punteggio += 15;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreA->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumIncrociatori && Flag == 0 ); i++ )
								{
									if ( GiocatoreA->Flotta.Incrociatori[i].Id == TempInt )
									{
										GiocatoreA->Flotta.Incrociatori[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreA->Flotta.Incrociatori[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreA->Flotta.NaviNonAffondate--;

											GiocatoreB->Punteggio += 11;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreA->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumTorpediniere && Flag == 0 ); i++ )
								{
									if ( GiocatoreA->Flotta.Torpediniere[i].Id == TempInt )
									{
										GiocatoreA->Flotta.Torpediniere[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreA->Flotta.Torpediniere[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreA->Flotta.NaviNonAffondate--;

											GiocatoreB->Punteggio += 8;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri ( GiocatoreA->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumPortaerei && Flag == 0 ); i++ )
								{
									if ( GiocatoreA->Flotta.Portaerei[i].Id == TempInt )
									{
										GiocatoreA->Flotta.Portaerei[i].NonAffondato--;
										Flag = 1;

										if ( GiocatoreA->Flotta.Portaerei[i].NonAffondato == 0 ) //Se ha affondato la nave
										{
											GiocatoreA->Flotta.NaviNonAffondate--;

											GiocatoreB->Punteggio += 5;

											if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
												PlaySound ( "Audio\\ShipDestroyed.WAV", NULL, SND_ASYNC );

											PosizionaCursore ( 36, 24 );
											printf ( "                                " );
											PosizionaCursore ( 36, 24 );
											printf ( "AFFONDATO! Continua ad attacare." );

											//Imposto le celle della nave a colpite ed affondate
											for ( i = 0; i < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; i++ )
											{
												if ( LeggiElementoListaInteri( GiocatoreA->MatricePosizionamento, i ) == TempInt )
												{
													ScriviElementoListaInteri ( GiocatoreA->GrigliaDifesa, i, 2 );
													ScriviElementoListaInteri ( GiocatoreB->GrigliaAttacco, i, 2 );
												}
											}
										}
									}
								}

								Flag = 0;
								StampaQuadranteAttacco  (GiocatoreB->GrigliaAttacco, QuadranteOrizzontaleAttacco_B, QuadranteVerticaleAttacco_B, Torneo->Impostazioni.NumColonne );

								if ( GiocatoreA->Flotta.NaviNonAffondate == 0 )
								{
									Vincitore = GiocatoreB;
									GiocatoreB->Punteggio += 20;
									FlagAcqua = 1; //Finisce il turno del giocatore
									FinePartita = 1;
									if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
										PlaySound ( "Audio\\GameWon.WAV", NULL, SND_ASYNC );
								}

							}
						break;

					}//Fine switch

				} while ( FlagAcqua == 0 && FlagUscita == 0 );

				TurnoCorrente = 1;
				AscissaMirino = 3;
				OrdinataMirino = 6;
				FlagAcqua = 0;
			}

		} while ( FinePartita == 0 && FlagUscita == 0 );

	} //Se FlagSaltaPartita != 1

	if ( FlagUscita == 1 || FlagSaltaPartita == 1 )
	{
		Vincitore = NULL;
	}
	else
	{
		TempInt = 0;

		//Controllo se i punteggi devono essere scritti in TopTen
		if ( NuovoRecord ( GiocatoreA->Punteggio ) )
		{
			AggiornaTopTen ( GiocatoreA );
			TempInt = 1;
			if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
				PlaySound ( "Audio\\Yeah.WAV", NULL, SND_ASYNC );
		}

		if ( NuovoRecord ( GiocatoreB->Punteggio ) )
		{
			AggiornaTopTen ( GiocatoreB );
			TempInt = 2;
			if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
				PlaySound ( "Audio\\Yeah.WAV", NULL, SND_ASYNC );
		}

		strcpy( MessaggioHelp, ""); //Per non dichiarare altre stringhe, riutilizzo MessaggioHelp
		sprintf ( MessaggioHelp, "%s: %d\n%s: %d\n\n", GiocatoreA->Nome, GiocatoreA->Punteggio, GiocatoreB->Nome, GiocatoreB->Punteggio );

		if ( TempInt == 0 )
			strcat ( MessaggioHelp, "Nessun nuovo record." );

		if ( TempInt == 1 )
		{
			strcat ( MessaggioHelp, "Top 10 aggiornata! Record di " );
			strcat ( MessaggioHelp, GiocatoreA->Nome );
		}

		if ( TempInt == 2 )
		{
			strcat ( MessaggioHelp, "Top 10 aggiornata! Record di " );
			strcat ( MessaggioHelp, GiocatoreB->Nome );
		}

		MessageBox ( NULL, MessaggioHelp, "Punteggi finali", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );


		//Azzero tutte le liste del vincitore, perchè dovrà disputare altri incontri
		Vincitore->GrigliaAttacco = InizializzaListaInteri ( &Torneo->Impostazioni );
		Vincitore->GrigliaDifesa = InizializzaListaInteri ( &Torneo->Impostazioni );
		Vincitore->MatricePosizionamento = InizializzaListaInteri ( &Torneo->Impostazioni );
		RipristinaFlotta ( &Vincitore->Flotta );

		CelebrazioneVincitorePartita ( Vincitore, &Torneo->Impostazioni );
	}

	Torneo->FlagPartitaInCorso = 0;
	GiocatoreA->Punteggio = 0;
	GiocatoreB->Punteggio = 0;


	return Vincitore;
}

void StampaMirino ( int Ordinata, int Ascissa )
{
	//I caratteri stampati sono delle frecce
	ImpostaColore ( 0x3c );
	PosizionaCursore ( Ordinata - 1, Ascissa );
	printf ( "%c", 16 );
	PosizionaCursore ( Ordinata + 1, Ascissa );
	printf ( "%c", 17 );
	PosizionaCursore ( Ordinata, Ascissa - 1 );
	printf ( "%c", 31 );
	PosizionaCursore ( Ordinata, Ascissa + 1 );
	printf ( "%c", 30 );
	ImpostaColore ( 0x3f );
}

void CancellaMirino ( int Ordinata, int Ascissa )
{
	//Stampando gli spazi vengono sovrascritti i caratteri preesistenti
	ImpostaColore ( 0x34 );
	PosizionaCursore ( Ordinata - 1, Ascissa );
	printf ( " " );
	PosizionaCursore ( Ordinata + 1, Ascissa );
	printf ( " " );
	PosizionaCursore ( Ordinata, Ascissa - 1 );
	printf ( " " );
	PosizionaCursore ( Ordinata, Ascissa + 1 );
	printf ( " " );
}

void PulisciLogBox ( )
{
	int i = 0;

	for ( i = 1; i < 24; i++ )
	{
		PosizionaCursore ( 46, i );
		printf ( "                                  " );
	}
}

void ProceduraPosizionamento ( giocatore *Giocatore, torneo *Torneo )
{
	int i = 0;
	int QuadranteOrizzontale = 1;
	int QuadranteVerticale = 1;
	int InputTastiera = 0;
	int OrdinataMirino = 6;
	int AscissaMirino = 3;
	int RigaLog = 2;
	int Ascissa = 0;
	int Ordinata = 0;
	int Id = 0;
	int Temp = 0;
	int Flag = 0;
	int FlagFinePosizionamento = 0;
	int SpostamentoInCorso = 1;
	char MessaggioHelp[]= "Premi ESC per uscire.\n\nUsa F5, F6, F7 ed F8 (o il mirino) per scorrere i quadranti:\nF5: mostra il quadrante a sinistra\nF6: mostra il quadrante a destra\nF7: mostra il quadrante in alto\nF8: mostra il quadrante in basso\nL'indice, in alto a sinistra, indica il quadrante corrente.\n\nMuovi il mirino con le frecce direzionali.\n\nPer posizionare una nave premi INVIO su una cella.\nPoi, premi INVIO su una cella adiacente a quella scelta per continuare a posizionarla. Continua fin quando l'intera nave non e' posizionata.\n\nSommergibile = 2 celle adiacenti\nIncrociatore = 3 celle adiacenti\nTorpediniera = 4 celle adiacenti\nPortaerei = 5 celle adiacenti\n\nLe navi NON possono toccarsi in nessun modo!";

	NascondiCursore ( );
	StampaSfondoCampoPosizionamento ( );
	StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
	PosizionaCursore ( 1, 24 );
	printf ( "Posizionamento - Turno di %s", Giocatore->Nome );
	StampaMirino ( OrdinataMirino, AscissaMirino );

	for ( i = 0; ( i < Torneo->Impostazioni.NumPortaerei && Torneo->FlagPartitaInCorso != - 1 ); i++ ) //FlagPartitaInCorso varrà - 1 se il posizionamento della nave non viene completato correttamente
	{
		if ( RigaLog > 23 ) //Se il logbox è pieno
		{
			PulisciLogBox ( );
			RigaLog = 2;
			PosizionaCursore (46, RigaLog );
			printf ( "Posiziona il portaerei #%d (*)\n", i + 1 );
			RigaLog += 2;
		}
		else
		{
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona il portaerei #%d (*)\n", i + 1 );
			RigaLog += 2;
		}

		PosizionaCursore ( 56, 24 );
		printf ( "*: 5 celle" );

		PosizionaNave ( 5, &QuadranteOrizzontale, &QuadranteVerticale, Giocatore, Torneo, &AscissaMirino, &OrdinataMirino, &RigaLog, Giocatore->Flotta.Portaerei[i].Id );
	}


	for ( i = 0; ( i < Torneo->Impostazioni.NumTorpediniere && Torneo->FlagPartitaInCorso != - 1 ); i++ ) //FlagPartitaInCorso varrà - 1 se il posizionamento della nave non viene completato correttamente
	{
		if ( RigaLog > 23 )
		{
			PulisciLogBox ( );
			RigaLog = 2;
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona la torpediniera #%d (*)\n", i + 1 );
			RigaLog += 2;
		}
		else
		{
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona la torpediniera #%d (*)\n", i + 1 );
			RigaLog += 2;
		}

		PosizionaCursore ( 56, 24 );
		printf ( "*: 4 celle" );

		PosizionaNave ( 4, &QuadranteOrizzontale, &QuadranteVerticale, Giocatore, Torneo, &AscissaMirino, &OrdinataMirino, &RigaLog, Giocatore->Flotta.Torpediniere[i].Id );
	}

	for ( i = 0; ( i < Torneo->Impostazioni.NumIncrociatori && Torneo->FlagPartitaInCorso != - 1 ) ; i++ ) //FlagPartitaInCorso varrà - 1 se il posizionamento della nave non viene completato correttamente
	{
		if ( RigaLog > 23 )
		{
			PulisciLogBox ( );
			RigaLog = 2;
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona l'incrociatore #%d (*)\n", i + 1 );
			RigaLog += 2;
		}
		else
		{
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona l'incrociatore #%d (*)\n", i + 1 );
			RigaLog += 2;
		}

		PosizionaCursore ( 56, 24 );
		printf ( "*: 3 celle" );

		PosizionaNave ( 3, &QuadranteOrizzontale, &QuadranteVerticale, Giocatore, Torneo, &AscissaMirino, &OrdinataMirino, &RigaLog, Giocatore->Flotta.Incrociatori[i].Id );
	}

	for ( i = 0; ( i < Torneo->Impostazioni.NumSommergibili && Torneo->FlagPartitaInCorso != -1 ); i++ ) //FlagPartitaInCorso varrà - 1 se il posizionamento della nave non viene completato correttamente
	{
		if ( RigaLog > 23 )
		{
			PulisciLogBox ( );
			RigaLog = 2;
			PosizionaCursore (46, RigaLog );
			printf ( "Posiziona il sommergibile #%d (*)\n", i + 1 );
			RigaLog += 2;
		}
		else
		{
			PosizionaCursore ( 46, RigaLog );
			printf ( "Posiziona il sommergibile #%d (*)\n", i + 1 );
			RigaLog += 2;
		}

		PosizionaCursore ( 56, 24 );
		printf ( "*: 2 celle" );

		PosizionaNave ( 2, &QuadranteOrizzontale, &QuadranteVerticale, Giocatore, Torneo, &AscissaMirino, &OrdinataMirino, &RigaLog, Giocatore->Flotta.Sommergibili[i].Id );
	}

	PosizionaCursore ( 56, 24 );
	printf ( "          " ); //Cancello *: # celle

	if ( Torneo->FlagPartitaInCorso != -1 ) //Se ha completato correttamente il posizionamento di TUTTE le navi (FlagPartitaInCorso viene settato a -1 se durante il posizionamento di una nave il giocatore preme ESC)
	{

		do
		{
			PulisciLogBox ( );
			RigaLog = 2;
			PosizionaCursore ( 46, RigaLog );
			printf ( "ATTENZIONE:" );
			RigaLog++;
			PosizionaCursore (46, RigaLog );
			printf ( "Hai posizionato tutte le navi" );
			RigaLog += 2;
			PosizionaCursore ( 46, RigaLog );
			printf ( "Premi 0 per proseguire col gioco o" );
			RigaLog++;
			PosizionaCursore (46, RigaLog );
			printf ( "premi 1 per riposizionare una nave" );
			RigaLog += 2;
			PosizionaCursore (46, RigaLog );
			printf ( "Scegli: " );
			RigaLog += 2;
			MostraCursore ( );

			do
			{
				FlagFinePosizionamento = getch ( );
			} while ( FlagFinePosizionamento != 48 && FlagFinePosizionamento != 49 ); //Finchè non preme 0 o 1

			NascondiCursore ( );

			if ( FlagFinePosizionamento == 49 ) //Se ha premuto 1: sposta una nave
			{

				printf ( "1" ); //Simulo l'eco del tasto premuto
				SpostamentoInCorso = 1;
				PosizionaCursore ( 46, RigaLog );
				printf ( "Premi INVIO su una cella per" );
				RigaLog++;
				PosizionaCursore ( 46, RigaLog );
				printf ( "cancellare la nave che la occupa" );
				RigaLog += 2;

				do
				{
					InputTastiera = getch ( );

					switch ( InputTastiera )
					{
						case 27: //ESC
							i = MessageBox ( NULL, "Uscendo perderai tutte le navi posizionate. Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL );
							if (i == 6 ) //Ha premuto Sì nel popup
								Torneo->FlagPartitaInCorso = -1;
						break;

						case 63: //F5: Spostati nel quadrante orizzontale a sinistra
							if ( QuadranteOrizzontale > 1 )
							{
								QuadranteOrizzontale--;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 64: //F6: Spostati nel quadrante orizzontale a destra
							if ( QuadranteOrizzontale < (Torneo->Impostazioni.NumColonne / 10) )
							{
								QuadranteOrizzontale++;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 65: //F7: Spostati nel quadrante verticale inferiore
							if ( QuadranteVerticale > 1 )
							{
								QuadranteVerticale--;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 66: //F8: Spostati nel quadrante verticale superiore
							if ( QuadranteVerticale < (Torneo->Impostazioni.NumRighe / 10) )
							{
								QuadranteVerticale++;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
						break;

						case 72: //Freccia SU: Sposta il mirino su
							if ( AscissaMirino == 3 && QuadranteVerticale > 1 ) //Il mirino si trova sulla prima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 21;
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticale = QuadranteVerticale - 1;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino > 3 && AscissaMirino <= 21) //Il mirino si trova su una qualunque riga tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino - 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 80: //Freccia GIU': Sposta il mirino giù
							if ( AscissaMirino == 21 && QuadranteVerticale < (Torneo->Impostazioni.NumRighe / 10) ) //Il mirino si trova sull'ultima riga
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = 3;
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteVerticale = QuadranteVerticale + 1;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
							else if ( AscissaMirino >= 3 && AscissaMirino < 21) //Il mirino si trova su una qualunque riga tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								AscissaMirino = AscissaMirino + 2;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 75: //Freccia SINISTRA: Sposta il mirino a sinistra
							if ( OrdinataMirino == 6 && QuadranteOrizzontale > 1 ) //Il mirino si trova sulla prima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 42;
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontale = QuadranteOrizzontale - 1;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino > 6 && OrdinataMirino <= 42) //Il mirino si trova su una qualunque colonna tranne la prima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino - 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 77: //Freccia DESTRA: Sposta il mirino a destra
							if ( OrdinataMirino == 42 && QuadranteOrizzontale < ( Torneo->Impostazioni.NumColonne / 10 ) ) //Il mirino si trova sull'ultima colonna
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = 6;
								StampaMirino ( OrdinataMirino, AscissaMirino );
								QuadranteOrizzontale = QuadranteOrizzontale + 1;
								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );
							}
							else if ( OrdinataMirino >= 6 && OrdinataMirino < 42) //Il mirino si trova su una qualunque colonna tranne l'ultima
							{
								CancellaMirino ( OrdinataMirino, AscissaMirino );
								OrdinataMirino = OrdinataMirino + 4;
								StampaMirino ( OrdinataMirino, AscissaMirino );
							}
						break;

						case 68: //F10: Help
							MessageBox ( NULL, MessaggioHelp, "HELP", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
						break;

						case 13: //INVIO: Inizia a posizionare la nave

							//Converto le coordinate del mirino nelle coordinate della cella nella lista
							Ascissa = ( OrdinataMirino - 6 ) / 4 + ( QuadranteOrizzontale - 1 ) * 10;
							Ordinata = ( AscissaMirino - 3 ) / 2 + ( QuadranteVerticale - 1 ) * 10;
							i = Ordinata * Torneo->Impostazioni.NumColonne + Ascissa;

							Id = LeggiElementoListaInteri ( Giocatore->MatricePosizionamento, i );

							if ( Id == 0 ) //Se la cella non è occupata da nessuna nave (non sta cancellando niente)
							{
								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

								if ( RigaLog > 22 )
								{
									PulisciLogBox ( );
									RigaLog = 2;
									PosizionaCursore ( 46, RigaLog );
									printf ( "Cella non occupata" );
									RigaLog += 2;
								}
								else
								{
									PosizionaCursore ( 46, RigaLog );
									printf ( "Cella non occupata" );
									RigaLog += 2;
								}

							}
							else
							{
								if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
									PlaySound ( "Audio\\ShipReplace.WAV", NULL, SND_ASYNC );

								//Cancello la nave dalla MatricePosizionamento e da GrigliaDifesa

								i = 0;
								do
								{
									Temp = LeggiElementoListaInteri ( Giocatore->MatricePosizionamento, i );
									if ( Temp == Id )
									{
										ScriviElementoListaInteri ( Giocatore->MatricePosizionamento, i, 0 );
										ScriviElementoListaInteri ( Giocatore->GrigliaDifesa, i, 0 );
									}
									i++;

								} while ( i < Torneo->Impostazioni.NumRighe * Torneo->Impostazioni.NumColonne );

								StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, QuadranteOrizzontale, QuadranteVerticale, Torneo->Impostazioni.NumColonne );

								// Ricavo la lunghezza della nave in base all'Id della nave e la memorizzo in Temp. Scrollo tutte le navi.

								for ( i = 0; ( i < Torneo->Impostazioni.NumSommergibili && Flag == 0 ); i++ )
								{
									if ( Giocatore->Flotta.Sommergibili[i].Id == Id )
									{
										Temp = 2;
										Flag = 1;
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumIncrociatori && Flag == 0 ); i++ )
								{
									if ( Giocatore->Flotta.Incrociatori[i].Id == Id )
									{
										Temp = 3;
										Flag = 1;
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumTorpediniere && Flag == 0 ); i++ )
								{
									if ( Giocatore->Flotta.Torpediniere[i].Id == Id )
									{
										Temp = 4;
										Flag = 1;
									}
								}

								for ( i = 0; ( i < Torneo->Impostazioni.NumPortaerei && Flag == 0 ); i++ )
								{
									if ( Giocatore->Flotta.Portaerei[i].Id == Id )
									{
										Temp = 5;
										Flag = 1;
									}
								}
								Flag = 0;

								if ( RigaLog > 21 )
								{
									PulisciLogBox ( );
									RigaLog = 2;
									PosizionaCursore ( 46, RigaLog );
									printf ( "Nave cancellata" );
									RigaLog++;
									PosizionaCursore ( 46, RigaLog );
									printf ( "Scegli %d celle adiacenti", Temp );
									RigaLog += 2;
								}
								else
								{
									PosizionaCursore ( 46, RigaLog );
									printf ( "Nave cancellata" );
									RigaLog++;
									PosizionaCursore ( 46, RigaLog );
									printf ( "Scegli %d celle adiacenti", Temp );
									RigaLog += 2;
								}

								PosizionaNave ( Temp, &QuadranteOrizzontale, &QuadranteVerticale, Giocatore, Torneo, &AscissaMirino, &OrdinataMirino, &RigaLog, Id );

								SpostamentoInCorso = 0;
							}

						break;

					}// Fine switch principale

				} while ( SpostamentoInCorso && Torneo->FlagPartitaInCorso != -1 );

			}

			if ( FlagFinePosizionamento == 48 ) //Se ha premuto 0: termina posizionamento
			{
				printf ( "0" ); //Simulo l'eco del tasto premuto
			}

		} while ( FlagFinePosizionamento - 48 && Torneo->FlagPartitaInCorso != -1 ); //Ripete conferma prima di uscire finchè non viene premuto 0

		if ( Torneo->FlagPartitaInCorso != -1 )
		{
			PosizionaCursore (46, RigaLog );
			printf ( "Premi un tasto per continuare " );
			MostraCursore ( );
			getch ( );
			NascondiCursore ( );
		}

	}

}

void InizializzaFlotta ( flotta *Flotta, settaggi *Impostazioni )
{
	int i = 0;
	int ContatoreId = 1; //L'Id 0 significa cella vuota. Il primo Id disponibile è 1.

	Flotta->Sommergibili = malloc ( sizeof ( sommergibile ) * Impostazioni->NumSommergibili );
	if ( Flotta->Sommergibili == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}
	Flotta->NumSommergibili = Impostazioni->NumSommergibili;
	for ( i = 0; i < Flotta->NumSommergibili; i++ )
	{
		Flotta->Sommergibili[i].NonAffondato = 2;
		Flotta->Sommergibili[i].Id = ContatoreId;
		ContatoreId++;
	}


	Flotta->Incrociatori = malloc ( sizeof ( incrociatore ) * Impostazioni->NumIncrociatori );
	if ( Flotta->Incrociatori == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}
	Flotta->NumIncrociatori = Impostazioni->NumIncrociatori;
	for ( i = 0; i < Flotta->NumIncrociatori; i++ )
	{
		Flotta->Incrociatori[i].NonAffondato = 3;
		Flotta->Incrociatori[i].Id = ContatoreId;
		ContatoreId++;
	}


	Flotta->Torpediniere = malloc ( sizeof ( torpediniera ) * Impostazioni->NumTorpediniere );
	if ( Flotta->Torpediniere == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}
	Flotta->NumTorpediniere = Impostazioni->NumTorpediniere;
	for ( i = 0; i < Flotta->NumTorpediniere; i++ )
	{
		Flotta->Torpediniere[i].NonAffondato = 4;
		Flotta->Torpediniere[i].Id = ContatoreId;
		ContatoreId++;
	}


	Flotta->Portaerei = malloc ( sizeof ( portaereo ) * Impostazioni->NumPortaerei );
	if ( Flotta->Portaerei == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}
	Flotta->NumPortaerei = Impostazioni->NumPortaerei;
	for ( i = 0; i < Flotta->NumPortaerei; i++ )
	{
		Flotta->Portaerei[i].NonAffondato = 5;
		Flotta->Portaerei[i].Id = ContatoreId;
		ContatoreId++;
	}

	Flotta->NaviNonAffondate = Flotta->NumSommergibili + Flotta->NumIncrociatori + Flotta->NumTorpediniere + Flotta->NumPortaerei;

}

void CreaCalendario ( torneo *Torneo)
{
	int i = 0;
	int Contatore = 0;

	Torneo->Calendario.PartitaDaGiocare = 1;
	Torneo->Calendario.TotalePartite = (Torneo->Impostazioni.NumGiocatori - 1 ); //Con n giocatori (n è una potenza di 2) il torneo si concluderà in (n-1) partite

	Torneo->Calendario.Match = malloc ( sizeof ( scontro ) * ( Torneo->Impostazioni.NumGiocatori - 1 ) ); 
	if ( Torneo->Calendario.Match == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	for ( i = 0; i < ( Torneo->Impostazioni.NumGiocatori / 2 ); i++ ) //Organizzo la prima fase del gioco (primo turno)
	{
		Torneo->Calendario.Match[i].Giocatore1 = &(Torneo->Partecipanti[Contatore]);
		Contatore = Contatore + 2;
	}

	Contatore = 1;

	for ( i = 0; i < ( Torneo->Impostazioni.NumGiocatori / 2 ); i++ ) //Organizzo la prima fase del gioco (primo turno)
	{
		Torneo->Calendario.Match[i].Giocatore2 = &(Torneo->Partecipanti[Contatore]);
		Contatore = Contatore + 2;
	}


}

void StampaSfondoCampo ( )
{

	int i = 0;
	char Lettera = 'A';

	//I caratteri stampati sono linee verticali o orizzontali
	system ( "cls" );
	ImpostaColore ( 0x3f );
	printf ( "%c   %c%c0   1   2   3   4   5   6   7   8   9\n", 27, 26, 179 );
	printf ( "%c   %c%c^   ^   ^   ^   ^   ^   ^   ^   ^   ^\n", 24, 25, 179 );
	printf ( "%c%c%c%c%c%c                                    \n", 196, 196, 196, 196, 196, 217 );
	printf ( " A <\n\n B <\n\n C <\n\n D <\n\n E <\n\n F <\n\n G <\n\n H <\n\n I <\n\n L <\n\n" );
	for ( i = 3; i < 23; i++ )
	{
		PosizionaCursore ( 45, i );
		printf ( "%c", 179 );
	}
	PosizionaCursore ( 43, 0 );
	printf ( "%c", 179 );
	PosizionaCursore ( 43, 1 );
	printf ( "%c", 179 );
	PosizionaCursore ( 43, 2 );
	printf ( "%c%c%c%c%c%c%c", 192, 196, 194, 196, 196, 196, 217 );
	PosizionaCursore ( 49, 0 );
	printf ( "%c", 179 );
	PosizionaCursore ( 49, 1 );
	printf ( "%c", 179 );
	PosizionaCursore ( 44, 0 );
	printf ( "%c   %c", 27, 26 );
	PosizionaCursore ( 44, 1 );
	printf ( "%c   %c", 24, 25 );
	PosizionaCursore ( 50, 0 );
	printf ( "0  1  2  3  4  5  6  7  8  9" );
	PosizionaCursore ( 50, 1 );
	printf ( "^  ^  ^  ^  ^  ^  ^  ^  ^  ^" );

	for ( i = 3; i < 20; i++ ) //Stampo le lettere dalla A alla I
	{
		PosizionaCursore ( 47, i );
		printf ( "%c<", Lettera );
		i++;
		Lettera++;
	}

	Lettera = 'L';
	PosizionaCursore ( 47, i ); //Stampo la L (se avessi fatto un ciclo in più nel for avrebbe stampato la J invece che la L)
	printf ( "%c<", Lettera );

	PosizionaCursore ( 47, 23 );
	printf ( "Schema Personale" );
	PosizionaCursore ( 1, 23 );
	printf ( "Schema Avversario" );
	PosizionaCursore ( 70, 24 );
	printf ( "Help: F10" );

}

void StampaSfondoCampoPosizionamento ( )
{

	int i = 0;

	//I caratteri stampati sono linee verticali o orizzontali

	system ( "cls" );
	ImpostaColore ( 0x3f );
	printf ( "%c   %c%c0   1   2   3   4   5   6   7   8   9\n", 27, 26, 179 );
	printf ( "%c   %c%c^   ^   ^   ^   ^   ^   ^   ^   ^   ^\n", 24, 25, 179 );
	printf ( "%c%c%c%c%c%c                                    \n", 196, 196, 196, 196, 196, 217 );
	printf ( " A <\n\n B <\n\n C <\n\n D <\n\n E <\n\n F <\n\n G <\n\n H <\n\n I <\n\n L <\n\n" );
	for ( i = 0; i < 23; i++ )
	{
		PosizionaCursore ( 45, i );
		printf ( "%c", 179 );
	}
	PosizionaCursore ( 70, 24 );
	printf ( "Help: F10" );
	PosizionaCursore ( 55, 0 );
	printf ( "Cronologia Azioni" );

}

char OttieniCarattere ( listaint Griglia, int Ascissa, int Ordinata, int NumColonne )
{

	char Carattere = '0';
	int Elemento = 0;

	Elemento = LeggiElementoListaInteri ( Griglia, Ordinata * NumColonne + Ascissa );

	switch ( Elemento )
	{
		case 0:
			ImpostaColore ( 0x3f );
			Carattere = ' ';
		break;

		case 1:
			ImpostaColore ( 0x3c );
			Carattere = 219;
		break;

		case 2:
			ImpostaColore ( 0x30 );
			Carattere = 219;
		break;

		case 3:
			ImpostaColore ( 0x31 );
			Carattere = 177;
		break;

		case 4:
			ImpostaColore ( 0x3f );
			Carattere = 219;
		break;

	}

	return Carattere;
}

void StampaQuadranteAttacco ( listaint Griglia, int QuadranteOrizzontale, int QuadranteVerticale, int NumColonne )
{

	int i = 0;
	int j = 0;
	char Carattere = '0';
	int Riga = 0;
	int Colonna = 0;

	//"Pulisce" il box riservato agli indici dei quadranti
	PosizionaCursore ( 2, 0 );
	printf ( "  " );
	PosizionaCursore ( 2, 1 );
	printf ( "  " );

	//Stampa il corretto numero dei quadranti
	PosizionaCursore ( 2, 0 );
	printf ( "%d", QuadranteOrizzontale );
	PosizionaCursore ( 2, 1 );
	printf ( "%d", QuadranteVerticale );

	for ( i = 0; i < 10; i++ )
	{
		Riga = i * 2 + 3;
		for ( j = 0; j < 10; j++ )
		{
			Colonna = j * 4 + 6;
			PosizionaCursore ( Colonna, Riga );
			Carattere = OttieniCarattere ( Griglia, j + ( QuadranteOrizzontale - 1 ) * 10, i + ( QuadranteVerticale - 1 ) * 10, NumColonne );
			printf ( "%c", Carattere );
		}
	}

	ImpostaColore ( 0x3f );

}

void StampaQuadranteDifesa ( listaint Griglia, int QuadranteOrizzontale, int QuadranteVerticale, int NumColonne )
{
	int i = 0;
	int j = 0;
	char Carattere = '0';
	int Riga = 0;
	int Colonna = 0;

	PosizionaCursore ( 46, 0 );
	printf ( "%d", QuadranteOrizzontale );
	PosizionaCursore ( 46, 1 );
	printf ( "%d", QuadranteVerticale );

	for ( i = 0; i < 10; i++ )
	{
		Riga = i * 2 + 3;
		for ( j = 0; j < 10; j++ )
		{
			Colonna = j * 3 + 50;
			PosizionaCursore ( Colonna, Riga );
			Carattere = OttieniCarattere ( Griglia, j + ( QuadranteOrizzontale - 1 ) * 10, i + ( QuadranteVerticale - 1 ) * 10, NumColonne );
			printf ( "%c", Carattere );
		}
	}

	ImpostaColore ( 0x3f );

}

void ScriviElementoListaInteri ( listaint Lista, int Posizione, int Numero )
{
	int i = 0;

	for ( i = 0; i < Posizione; i++ )
		Lista = Lista->Prox; //Scorre la lista

	Lista->Info = Numero;

}

int LeggiElementoListaInteri ( listaint Lista, int Posizione )
{
	int i = 0;

	for ( i = 0; i < Posizione; i++ )
		Lista = Lista->Prox; //Scorre la lista

	return Lista->Info;
}

listaint InizializzaListaInteri ( settaggi *Impostazioni )
{

	int i = 0;
	listaint Lista;
	int Nodi = 0;

	Lista = InizializzaLista ( );

	Nodi = Impostazioni->NumRighe * Impostazioni->NumColonne;

	for ( i = 0; i < Nodi; i++ )
		Lista = InserisciInTesta ( Lista, 0 );

	return Lista;
}

listaint InserisciInTesta ( listaint Lista, int Elemento )
{
	listaint Cursore = NULL;

	Cursore = malloc ( sizeof ( elemlista ) );
	if ( Cursore == NULL )
	{
		system ( "cls" );
		PosizionaCursore ( 0, 0 );
		perror ( "" );
		fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
		getch ( );
		exit ( -1 );
	}

	Cursore->Info = Elemento;
	Cursore->Prox = Lista;

	return Cursore;

}

listaint InizializzaLista ( )
{
	return NULL;
}

int CellaIsolata ( int Ascissa, int Ordinata, listaint MatricePosizionamento, int NumRighe, int NumColonne )
{
	int t = 0;
	int Flag = 0;

	t = Ordinata * NumColonne + Ascissa; //Calcola la posizione cella

	if (Ascissa == 0 && Ordinata == 0 ) //Se la cella è nell'angolo in alto a sinistra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 //cella a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 //cella in basso a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) // cella in basso
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata == 0 ) //Se la cella è nell'angolo in alto a destra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 //cella a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 //cella in basso a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) // cella in basso
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa == 0 && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a sinistra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0  //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - (NumColonne - 1 ) ) == 0 //cella in alto a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //cella a destra
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a destra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0  //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 //cella in alto a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //cella a sinistra
			Flag = 1;
		else
			Flag = 0;
        }

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata  == 0 ) //Se la cella è una tra le celle del lato superiore
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0  //cella a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 //cella a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 //cella in basso
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 //cella in basso a sinistra
            && LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //cella in basso a destra
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata  == ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato inferiore
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0  //cella a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 //cella a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 //cella in alto a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - (NumColonne + 1 ) ) == 0 ) //cella in alto a sinistra
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa == 0 && Ordinata  > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato a sinistra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0  //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 //cella in alto a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 //cella a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 //cella in basso
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + (NumColonne + 1 ) ) == 0 ) //cella in basso a destra
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato a destra
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0  //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 //cella in alto a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 //cella a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 //cella in basso
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + (NumColonne - 1 ) ) == 0 ) //cella in basso a sinistra
			Flag = 1;
		else
			Flag = 0;
	}

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata  > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle situate al centro
	{ //Controlla che le celle circostanti siano vuote
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0  //cella in alto
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 //cella in alto a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 //cella in alto a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 //cella a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 //cella a destra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 //cella in basso
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 //cella in basso a sinistra
			&& LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //cella in basso a destra
			Flag = 1;
		else
			Flag = 0;
	}

	return Flag;
}

int CellaValidaPosizionamento ( int Ascissa, int Ordinata, int LunghezzaNave, giocatore *Utente, torneo *Torneo )
{
	int Flag = 0;
	int ID = 0;
	int CelleDisponibiliOrizzontalmente = 1;
	int CelleDisponibiliVerticalmente = 1;
	int CellaLibera = 0;
	int i = 0;

	/*
	N.B.: la funzione restituisce 0 se la nave non potrà essere posizonata in nessun modo
	1 se solo orizzontalmente, 2 se solo verticalmente, 3 se sia orizzontalmente che verticalmente
	*/
	
	ID = LeggiElementoListaInteri ( Utente->MatricePosizionamento, Ordinata * Torneo->Impostazioni.NumColonne + Ascissa );  //i*c+j

	if ( ID == 0 )
	{
		//Misuro lo spazio libero a destra
		if ( Ascissa != ( Torneo->Impostazioni.NumColonne - 1 ) ) //Se si trova al bordo destro non può vedere quante celle libere ha a destra
		{
			CellaLibera = 1;
			for ( i = 1; ( Ascissa + i < Torneo->Impostazioni.NumColonne && CellaLibera == 1 ); i++ ) //Fin quando non arriva al bordo destro e fin quando ha continuato a trovare celle isolate
			{
				if ( CellaIsolata ( Ascissa + i, Ordinata, Utente->MatricePosizionamento, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne ) )
				{
					CelleDisponibiliOrizzontalmente++;
					CellaLibera = 1;
				}
				else CellaLibera = 0;
			}

		}

		//Misuro lo spazio libero a sinistra
		if ( Ascissa > 0 ) //Se si trova al bordo sinistro non può vedere quante celle libere ha a sinistra
		{
			CellaLibera = 1;
			for ( i = 1; ( Ascissa - i >= 0 && CellaLibera == 1 ); i++ )
			{
				if ( CellaIsolata ( Ascissa - i, Ordinata, Utente->MatricePosizionamento, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne ) )
				{
					CelleDisponibiliOrizzontalmente++;
					CellaLibera = 1;
				}
				else CellaLibera = 0;
			}

		}

		//Misuro lo spazio libero sopra
		if ( Ordinata > 0 ) //Se si trova al bordo superiore non può vedere quante celle libere ha sopra
		{
			CellaLibera = 1;
			for ( i = 1; ( Ordinata - i >= 0 && CellaLibera == 1 ); i++ )
			{
				if ( CellaIsolata ( Ascissa, Ordinata - i, Utente->MatricePosizionamento, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne ) )
				{
					CelleDisponibiliVerticalmente++;
					CellaLibera = 1;
				}
				else CellaLibera = 0;
			}

		}

		//Misuro lo spazio libero sotto
		if ( Ordinata != ( Torneo->Impostazioni.NumRighe - 1 ) ) //Se si trova al bordo inferiore non può vedere quante celle libere ha sotto
		{
			CellaLibera = 1;
			for ( i = 1; ( Ordinata + i < Torneo->Impostazioni.NumRighe && CellaLibera == 1 ); i++ )
			{
				if ( CellaIsolata ( Ascissa, Ordinata + i, Utente->MatricePosizionamento, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne ) )
				{
					CelleDisponibiliVerticalmente++;
					CellaLibera = 1;
				}
				else CellaLibera = 0;
			}

		}

		if ( CelleDisponibiliOrizzontalmente >= LunghezzaNave ) //Se la nave può essere posizionata orizzontamente
			Flag = 1; //Imposta il flag da restituire con il valore 1 che indica l'orientamenteo della nave

		if ( CelleDisponibiliVerticalmente >= LunghezzaNave ) //Se la nave può essere posizionata verticalmente
			Flag = 2; //Imposta il flag da restituire con il valore 2 che indica l'orientamenteo della nave

		if ( CelleDisponibiliOrizzontalmente >= LunghezzaNave && CelleDisponibiliVerticalmente >= LunghezzaNave ) //Se la nave può essere posizionata sia orizzontalmente che verticalmente
			Flag = 3; //Imposta il flag da restituire con il valore 3 che indica l'orientamenteo della nave

	}

	return Flag;
}

void PosizionaNave ( int CelleDaPosizionare, int *QuadranteOrizzontale, int *QuadranteVerticale, giocatore *Giocatore, torneo *Torneo, int *AscissaMirino, int *OrdinataMirino, int *RigaLog, int Id )
{

	int InputTastiera = 0;
	int Ascissa = 0;
	int Ordinata = 0;
	int IndiceLista = 0;
	int Flag = 0;
	int Orientamento = 0;
	char MessaggioHelp[]= "Premi ESC per uscire.\n\nUsa F5, F6, F7 ed F8 (o il mirino) per scorrere i quadranti:\nF5: mostra il quadrante a sinistra\nF6: mostra il quadrante a destra\nF7: mostra il quadrante in alto\nF8: mostra il quadrante in basso\nL'indice, in alto a sinistra, indica il quadrante corrente.\n\nMuovi il mirino con le frecce direzionali.\n\nPer posizionare una nave premi INVIO su una cella.\nPoi, premi INVIO su una cella adiacente a quella scelta per continuare a posizionarla. Continua fin quando l'intera nave non e' posizionata.\n\nSommergibile = 2 celle adiacenti\nIncrociatore = 3 celle adiacenti\nTorpediniera = 4 celle adiacenti\nPortaerei = 5 celle adiacenti\n\nLe navi NON possono toccarsi in nessun modo!";

	do
	{
		InputTastiera = getch ( );

		switch ( InputTastiera )
		{
			case 27: //ESC
				InputTastiera = MessageBox ( NULL, "Uscendo perderai tutte le navi posizionate. Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL );

				if ( InputTastiera == 6 ) //Ha premuto Sì nel popup
					Torneo->FlagPartitaInCorso = -1; //Flag di uscita
			break;

			case 63: //F5: Spostati nel quadrante orizzontale a sinistra
				if ( *QuadranteOrizzontale > 1 )
				{
					*QuadranteOrizzontale = *QuadranteOrizzontale - 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
			break;

			case 64: //F6: Spostati nel quadrante orizzontale a destra
				if ( *QuadranteOrizzontale < ( Torneo->Impostazioni.NumColonne / 10) )
				{
					*QuadranteOrizzontale = *QuadranteOrizzontale + 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
			break;

			case 65: //F7: Spostati nel quadrante verticale inferiore
				if ( *QuadranteVerticale > 1 )
				{
					*QuadranteVerticale = *QuadranteVerticale - 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
			break;

			case 66: //F8: Spostati nel quadrante verticale superiore
				if ( *QuadranteVerticale < ( Torneo->Impostazioni.NumRighe / 10 ) )
				{
					*QuadranteVerticale = *QuadranteVerticale + 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
			break;

			case 72: //Freccia SU: Sposta il mirino su
				if ( *AscissaMirino == 3 && *QuadranteVerticale > 1 ) //Il mirino si trova sulla prima riga
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*AscissaMirino = 21;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
					*QuadranteVerticale = *QuadranteVerticale - 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
				else if ( *AscissaMirino > 3 && *AscissaMirino <= 21) //Il mirino si trova su una qualunque riga tranne la prima
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*AscissaMirino = *AscissaMirino - 2;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
				}
			break;

			case 80: //Freccia GIU': Sposta il mirino giù
				if ( *AscissaMirino == 21 && *QuadranteVerticale < ( Torneo->Impostazioni.NumRighe / 10 ) ) //Il mirino si trova sull'ultima riga
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*AscissaMirino = 3;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
					*QuadranteVerticale = *QuadranteVerticale + 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
				else if ( *AscissaMirino >= 3 && *AscissaMirino < 21) //Il mirino si trova su una qualunque riga tranne l'ultima
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*AscissaMirino = *AscissaMirino + 2;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
				}
			break;

			case 75: //Freccia SINISTRA: Sposta il mirino a sinistra
				if ( *OrdinataMirino == 6 && *QuadranteOrizzontale > 1 ) //Il mirino si trova sulla prima colonna
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*OrdinataMirino = 42;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
					*QuadranteOrizzontale = *QuadranteOrizzontale - 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
				else if ( *OrdinataMirino > 6 && *OrdinataMirino <= 42) //Il mirino si trova su una qualunque colonna tranne la prima
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*OrdinataMirino = *OrdinataMirino - 4;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
				}
			break;

			case 77: //Freccia DESTRA: Sposta il mirino a destra
				if ( *OrdinataMirino == 42 && *QuadranteOrizzontale < ( Torneo->Impostazioni.NumColonne / 10 ) ) //Il mirino si trova sull'ultima colonna
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*OrdinataMirino = 6;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
					*QuadranteOrizzontale = *QuadranteOrizzontale + 1;
					StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
				}
				else if ( *OrdinataMirino >= 6 && *OrdinataMirino < 42) //Il mirino si trova su una qualunque colonna tranne l'ultima
				{
					CancellaMirino ( *OrdinataMirino, *AscissaMirino );
					*OrdinataMirino = *OrdinataMirino + 4;
					StampaMirino ( *OrdinataMirino, *AscissaMirino );
				}
			break;

			case 68: //F10: Help
				MessageBox ( NULL, MessaggioHelp, "HELP", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
			break;

			case 13: //INVIO: Inizia a posizionare la nave

				//Converto le coordinate del mirino nelle coordinate della cella nella lista
				Ascissa = ( *OrdinataMirino - 6 ) / 4 + ( *QuadranteOrizzontale - 1 ) * 10;
				Ordinata = ( *AscissaMirino - 3 ) / 2 + ( *QuadranteVerticale - 1 ) * 10;
				IndiceLista = Ordinata * Torneo->Impostazioni.NumColonne + Ascissa;

				/*Controllo che la cella scelta non sia già occupata da un'altra nave
				E' inutile controllare se LA CELLA SCELTA è libera (basta controllare quelle A LEI ADIACENTI): 
				se la celle fosse occupata da una nave, infatti, lo sarebbe necessariamente anche una delle 
				celle a lei adiacente, perchè la lunghezza minima delle navi è 2*/
				Flag = CellaIsolata ( Ascissa, Ordinata, Giocatore->MatricePosizionamento, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne );
				if ( Flag == 0 ) //Se la cella è occupata da un'altra nave non puoi piazzare la nave
				{
					if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
						PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

					if ( *RigaLog > 22 )
					{
						PulisciLogBox ( );
						*RigaLog = 2;
						PosizionaCursore ( 46, *RigaLog );
						printf ( "Cella non valida, scegli un'altra" );
						*RigaLog = *RigaLog + 2;
					}
					else
					{
						PosizionaCursore ( 46, *RigaLog );
						printf ( "Cella non valida, scegli un'altra" );
						*RigaLog = *RigaLog + 2;
					}
				}
				else //Se è isolata controllo che la nave possa entrare
				{
					Orientamento = CellaValidaPosizionamento ( Ascissa, Ordinata, CelleDaPosizionare, Giocatore, Torneo );
					if ( Orientamento == 0 ) //Se la nave non può essere posizionata in alcun modo
					{
						if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
							PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

						if ( *RigaLog > 22 )
						{
							PulisciLogBox ( );
							*RigaLog = 2;
							PosizionaCursore ( 46, *RigaLog );
							printf ( "Spazio insufficiente per la nave" );
							*RigaLog = *RigaLog + 2;
						}
						else
						{
							PosizionaCursore ( 46, *RigaLog );
							printf ( "Spazio insufficiente per la nave" );
							*RigaLog = *RigaLog + 2;
						}

					}
					else //Se la nave è posizionabile
					{

						if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
							PlaySound ( "Audio\\CellSet.WAV", NULL, SND_ASYNC );

						ScriviElementoListaInteri ( Giocatore->MatricePosizionamento, IndiceLista, Id ); //Scrivo l'ID della nave nella MatricePosizionamento
						ScriviElementoListaInteri ( Giocatore->GrigliaDifesa, IndiceLista, 4 ); //Nella GrigliaDifesa setto la cella a non chiamata e contenente una nave

						CelleDaPosizionare--; //Decremento il numero delle celle da posizionare della nave
						StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );

						if ( *RigaLog > 21 )
						{
							PulisciLogBox ( );
							*RigaLog = 2;
							PosizionaCursore ( 46, *RigaLog );
							printf ( "Cella posizionata con successo" );
							*RigaLog = *RigaLog + 1;
							PosizionaCursore ( 46, *RigaLog );
							if ( CelleDaPosizionare > 1)
								printf ( "Ancora %d celle da posizionare", CelleDaPosizionare ); //Plurale
							if ( CelleDaPosizionare == 1 )
								printf ( "Ancora %d cella da posizionare", CelleDaPosizionare ); //Singolare
							*RigaLog = *RigaLog + 2;
						}
						else
						{
							PosizionaCursore (46, *RigaLog );
							printf ( "Cella posizionata con successo" );
							*RigaLog = *RigaLog + 1;
							PosizionaCursore ( 46, *RigaLog );
							if ( CelleDaPosizionare > 1)
								printf ( "Ancora %d celle da posizionare", CelleDaPosizionare ); //Plurale
							if ( CelleDaPosizionare == 1 )
								printf ( "Ancora %d cella da posizionare", CelleDaPosizionare ); //Singolare
							*RigaLog = *RigaLog + 2;
						}

						do //Deve posizionare ora le restanti caselle
						{
							InputTastiera = getch ( );

							switch ( InputTastiera )
							{
								case 27: //ESC
									InputTastiera = MessageBox ( NULL, "Uscendo perderai tutte le navi posizionate. Sei sicuro di voler uscire?", "Uscita", MB_YESNO | MB_ICONWARNING | MB_SYSTEMMODAL );
									if ( InputTastiera == 6 ) //Ha premuto Sì nel popup
										Torneo->FlagPartitaInCorso = -1; //Flag di uscita
								break;

								case 63: //F5: Spostati nel quadrante orizzontale a sinistra
									if ( *QuadranteOrizzontale > 1 )
									{
										*QuadranteOrizzontale = *QuadranteOrizzontale - 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
								break;

								case 64: //F6: Spostati nel quadrante orizzontale a destra
									if ( *QuadranteOrizzontale < (Torneo->Impostazioni.NumColonne / 10 ) )
									{
										*QuadranteOrizzontale = *QuadranteOrizzontale + 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
								break;

								case 65: //F7: Spostati nel quadrante verticale inferiore
									if ( *QuadranteVerticale > 1 )
									{
										*QuadranteVerticale = *QuadranteVerticale - 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
								break;

								case 66: //F8: Spostati nel quadrante verticale superiore
									if ( *QuadranteVerticale < (Torneo->Impostazioni.NumRighe / 10 ) )
									{
										*QuadranteVerticale = *QuadranteVerticale + 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
								break;

								case 72: //Freccia SU: Sposta il mirino su
									if ( *AscissaMirino == 3 && *QuadranteVerticale > 1 ) //Il mirino si trova sulla prima riga
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*AscissaMirino = 21;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
										*QuadranteVerticale = *QuadranteVerticale - 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
									else if ( *AscissaMirino > 3 && *AscissaMirino <= 21) //Il mirino si trova su una qualunque riga tranne la prima
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*AscissaMirino = *AscissaMirino - 2;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
									}
								break;

								case 80: //Freccia GIU': Sposta il mirino giù
									if ( *AscissaMirino == 21 && *QuadranteVerticale < ( Torneo->Impostazioni.NumRighe / 10 ) ) //Il mirino si trova sull'ultima riga
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*AscissaMirino = 3;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
										*QuadranteVerticale = *QuadranteVerticale + 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
									else if ( *AscissaMirino >= 3 && *AscissaMirino < 21 ) //Il mirino si trova su una qualunque riga tranne l'ultima
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*AscissaMirino = *AscissaMirino + 2;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
									}
								break;

								case 75: //Freccia SINISTRA: Sposta il mirino a sinistra
									if ( *OrdinataMirino == 6 && *QuadranteOrizzontale > 1 ) //Il mirino si trova sulla prima colonna
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*OrdinataMirino = 42;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
										*QuadranteOrizzontale = *QuadranteOrizzontale - 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
									else if ( *OrdinataMirino > 6 && *OrdinataMirino <= 42) //Il mirino si trova su una qualunque colonna tranne la prima
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*OrdinataMirino = *OrdinataMirino - 4;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
									}
								break;

								case 77: //Freccia DESTRA: Sposta il mirino a destra
									if ( *OrdinataMirino == 42 && *QuadranteOrizzontale < ( Torneo->Impostazioni.NumColonne / 10 ) ) //Il mirino si trova sull'ultima colonna
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*OrdinataMirino = 6;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
										*QuadranteOrizzontale = *QuadranteOrizzontale + 1;
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );
									}
									else if ( *OrdinataMirino >= 6 && *OrdinataMirino < 42) //Il mirino si trova su una qualunque colonna tranne l'ultima
									{
										CancellaMirino ( *OrdinataMirino, *AscissaMirino );
										*OrdinataMirino = *OrdinataMirino + 4;
										StampaMirino ( *OrdinataMirino, *AscissaMirino );
									}
								break;

								case 68: //F10: Help
									MessageBox ( NULL, MessaggioHelp, "HELP", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL );
								break;

								case 13: //INVIO: Continua a posizionare le altre celle della nave

									//Converto le coordinate del mirino nelle coordinate della cella nella lista
									Ascissa = ( *OrdinataMirino - 6 ) / 4 + ( *QuadranteOrizzontale - 1 ) * 10;
									Ordinata = ( *AscissaMirino - 3 ) / 2 + ( *QuadranteVerticale - 1 ) * 10;
									IndiceLista = Ordinata * Torneo->Impostazioni.NumColonne + Ascissa;

									Flag = CellaAdiacente ( Ascissa, Ordinata, Torneo->Impostazioni.NumRighe, Torneo->Impostazioni.NumColonne, Giocatore->MatricePosizionamento, &Orientamento, Id );

									if ( Flag == 0 || LeggiElementoListaInteri ( Giocatore->MatricePosizionamento, IndiceLista ) != 0 ) //Se non è adiacente alla nave che si sta posizionando o se la cella non è vuota
									{
										if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
											PlaySound ( "Audio\\Beep.WAV", NULL, SND_ASYNC );

										if ( *RigaLog > 22 )
										{
											PulisciLogBox ( );
											*RigaLog = 2;
											PosizionaCursore ( 46, *RigaLog );
											printf ( "Cella non valida, scegli un'altra" );
											*RigaLog = *RigaLog + 2;
										}
										else
										{
											PosizionaCursore ( 46, *RigaLog );
											printf ( "Cella non valida, scegli un'altra" );
											*RigaLog = *RigaLog + 2;
										}
									}
									else
									{

										if ( Torneo->Impostazioni.Audio == 0 ) //Se l'audio è stato attivato
											PlaySound ( "Audio\\CellSet.WAV", NULL, SND_ASYNC );

										ScriviElementoListaInteri ( Giocatore->MatricePosizionamento, IndiceLista, Id ); //Scrivo l'ID della nave nella MatricePosizionamento
										ScriviElementoListaInteri ( Giocatore->GrigliaDifesa, IndiceLista, 4 ); //Nella GrigliaDifesa setto la cella a non chiamata e contenente una nave

										CelleDaPosizionare--; //Decremento il numero delle celle da posizionare della nave
										StampaQuadranteAttacco ( Giocatore->GrigliaDifesa, *QuadranteOrizzontale, *QuadranteVerticale, Torneo->Impostazioni.NumColonne );

										if ( *RigaLog > 21 )
										{
											PulisciLogBox ( );
											*RigaLog = 2;
											PosizionaCursore ( 46, *RigaLog );
											printf ( "Cella posizionata con successo" );
											if ( CelleDaPosizionare == 1 )
											{
												*RigaLog = *RigaLog + 1;
												PosizionaCursore ( 46, *RigaLog );
												printf ( "Ancora %d cella da posizionare", CelleDaPosizionare );
											}
											if ( CelleDaPosizionare > 1 )
											{
												*RigaLog = *RigaLog + 1;
												PosizionaCursore ( 46, *RigaLog );
												printf ( "Ancora %d celle da posizionare", CelleDaPosizionare );
											}
											*RigaLog = *RigaLog + 2;
										}
										else
										{
											PosizionaCursore ( 46, *RigaLog );
											printf ( "Cella posizionata con successo" );
											if ( CelleDaPosizionare == 1 )
											{
												*RigaLog = *RigaLog + 1;
												PosizionaCursore (46, *RigaLog );
												printf ( "Ancora %d cella da posizionare", CelleDaPosizionare );
											}
											if ( CelleDaPosizionare > 1 )
											{
												*RigaLog = *RigaLog + 1;
												PosizionaCursore ( 46, *RigaLog );
												printf ( "Ancora %d celle da posizionare", CelleDaPosizionare );
											}
											*RigaLog = *RigaLog + 2;

											if ( CelleDaPosizionare == 0 && *RigaLog > 21 ) //Non ci sarebbe spazio per stampare l'avviso di posizionare la nave successiva
											{
												PulisciLogBox ( );
												*RigaLog = 2;
											}
										}

									}

								break;


							} //Fine switch annidato

						} while ( CelleDaPosizionare && Torneo->FlagPartitaInCorso != -1 ); //Ripeti fin quando ci sono celle da posizionare e non è stato premuto ESC

						if ( Torneo->Impostazioni.Audio == 0 && CelleDaPosizionare == 0 ) //Se l'audio è stato attivato e una nave è stata interamente posizionata
							PlaySound ( "Audio\\ShipSet.WAV", NULL, SND_ASYNC );
					}
				}
			break;


		} //Fine switch principale

	} while ( CelleDaPosizionare && Torneo->FlagPartitaInCorso != -1 ); //Ripeti fin quando ci sono celle da posizionare e non è stato premuto ESC

}

int CellaAdiacente ( int Ascissa, int Ordinata, int NumRighe, int NumColonne, listaint MatricePosizionamento, int *Orientamento, int Id )
{

	int Flag = 0;
	int t = 0;

    t = Ordinata * NumColonne + Ascissa; //Calcola la posizione cella

	if ( ContaCelleVuote ( Ascissa, Ordinata, NumRighe, NumColonne, MatricePosizionamento ) ) //La cella deve essere isolata, tranne che in una cella di quelle che la circondano (perchè lì c'è la nave a cui si attacca)
	{

		if ( *Orientamento == 3 ) //Se la nave può essere posizionata sia orizzontalmente che verticalmente
		{
			if ( Ascissa == 0 && Ordinata == 0 ) //Se la cella è nell'angolo in alto a sinistra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a destra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in basso è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else Flag = 0;
			}

			if ( Ascissa == 0 && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a sinistra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id ) //Se nella cella in alto è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else if ( LeggiElementoListaInteri (MatricePosizionamento, t + 1) == Id ) //Se nella cella a destra è presente una cella della nave con id ricevuto input
				{
					 Flag = 1;
					 *Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else Flag = 0;
			}

			if ( Ascissa == ( NumColonne - 1 ) && Ordinata == 0 ) //Se la cella è nell'angolo in alto a destra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id )//Se nella cella a sinistra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in basso è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else Flag = 0;
			}

			if ( Ascissa == ( NumColonne -1 ) && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a destra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id ) //Se nella cella in alto è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id ) //Se nella cella a sinistra è presente una cella della nave con id ricevuto input
				{
					 Flag = 1;
					 *Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else Flag = 0;
			}

			if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata == 0 ) //Se la cella è una tra le celle del lato superiore (esclusi gli angoli)
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in basso è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a destra o a sinistra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else Flag = 0;

			}

			if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata == ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato inferiore (esclusi gli angoli)
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id ) //Se nella cella in alto è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //cella a destra o sinistra
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else Flag = 0;

			}

			if ( Ascissa == 0 && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato sinistro (esclusi gli angoli)
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a destra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) // Cella sopra e sotto
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else Flag = 0;

			}

			if ( Ascissa == ( NumColonne - 1 ) && Ordinata > 0 && Ordinata < ( NumRighe - 1) ) //Se la cella è una tra le celle del lato destro (esclusi gli angoli)
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id ) //Se nella cella a sinistra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in alto o in basso è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else Flag = 0;

			}

			if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle centrali
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in alto o in basso è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 2; //Imposta l'orientamento a verticale
				}
				else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id || LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a destra o a sinistra è presente una cella della nave con id ricevuto input
				{
					Flag = 1;
					*Orientamento = 1; //Imposta l'orientamento ad orizzontale
				}
				else Flag = 0;

			}
		}

		else if ( *Orientamento == 2 ) //Se la nave è posizionata verticalmente
		{
			if ( Ordinata == 0 ) //Se la cella è quella più in alto
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id ) //Se nella cella in basso è presente una cella della nave con id ricevuto input
					Flag = 1;
				else
					Flag = 0;
			}
			else if ( Ordinata == ( NumRighe - 1 ) ) //Se la cella è quella più in basso
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id ) //Se nella cella in alto è presente una cella della nave con id ricevuto input
					Flag = 1;
				else
					Flag = 0;
			}
			else if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == Id 
					|| LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == Id ) //Se nella cella in basso o in alto è presente una cella della nave con id ricevuto input
				Flag = 1;
			else
				Flag = 0;
		}

		else if ( *Orientamento == 1 ) //Se la nave è posizionata orizzontale
		{
			if ( Ascissa == 0 ) //Se la cella è quella più a sinistra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a destra è presente una cella della nave con id ricevuto input
					Flag = 1;
				else
					Flag = 0;
			}
			else if ( Ascissa == ( NumColonne - 1 ) ) //Se la cella è quella più a destra
			{
				if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id ) //Se nella cella a sinistra è presente una cella della nave con id ricevuto input
					Flag = 1;
				else
					Flag = 0;
			}
			else if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == Id 
					|| LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == Id ) //Se nella cella a sinistra o a destraè presente una cella della nave con id ricevuto input
				Flag = 1;
			else
				Flag = 0;
		}

	}

	else Flag = 0;

	return Flag;
}

int ContaCelleVuote ( int Ascissa, int Ordinata, int NumRighe, int NumColonne, listaint MatricePosizionamento )
{
	int Contatore = 0;
	int Flag = 0;
	int t = 0;

	t = Ordinata * NumColonne + Ascissa; //Calcola la posizione della cella

	if ( Ascissa == 0 && Ordinata == 0 ) //Se la cella è nell'angolo in alto a sinistra
    {
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //Se la cella  in basso a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella  in basso è vuota
			 Contatore++;

		if ( Contatore == 2 ) //Se le celle vuote circostanti sono due
			Flag = 1; 
		else Flag = 0;

	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata == 0 ) //Se la cella è nell'angolo in alto a destra
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //Se la cella a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 ) //Se la cella in basso a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella in basso è vuota
			Contatore++;

		if ( Contatore == 2 ) //Se le celle vuote circostanti sono due
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa == 0 && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a sinistra
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 )  //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 ) //Se la cella in alto a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;

		if ( Contatore == 2 ) //Se le celle vuote circostanti sono due
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata == ( NumRighe - 1 ) ) //Se la cella è nell'angolo in basso a destra
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 )  //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 ) //Se la cella in alto a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //Se la cella a sinistra è vuota
			Contatore++;

		if ( Contatore == 2 ) //Se le celle vuote circostanti sono due
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata  == 0 ) //Se la cella è una tra le celle del lato superiore
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //Se la cella a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella in basso è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 ) //Se la cella in basso a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //Se la cella in basso a destra è vuota
			Contatore++;

		if ( Contatore == 4 ) //Se le celle vuote circostanti sono quattro
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata  == ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato inferiore
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 )  //Se la cella a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 ) //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 ) //Se la cella in alto a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 ) //Se la cella in alto a sinistra è vuota
			Contatore++;

		if ( Contatore == 4 ) //Se le celle vuote circostanti sono quattro
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa == 0 && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato sinistro
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 )  //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 ) //Se la cella in alto a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella in basso è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //Se la cella in basso a destra è vuota
			Contatore++;

		if ( Contatore == 4 ) //Se le celle vuote circostanti sono quattro
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa == ( NumColonne - 1 ) && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle del lato destro
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 )  //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 ) //Se la cella in alto a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //Se la cella a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella in basso è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 ) //Se la cella in basso a sinistra è vuota
			Contatore++;

		if ( Contatore == 4 ) //Se le celle vuote circostanti sono quattro
			Flag = 1;
		else Flag = 0;
	}

	if ( Ascissa > 0 && Ascissa < ( NumColonne - 1 ) && Ordinata > 0 && Ordinata < ( NumRighe - 1 ) ) //Se la cella è una tra le celle centrali
	{
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - NumColonne ) == 0 ) //Se la cella in alto è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne + 1 ) ) == 0 ) //Se la cella in alto a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - ( NumColonne - 1 ) ) == 0 ) //Se la cella in alto a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t - 1 ) == 0 ) //Se la cella a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + 1 ) == 0 ) //Se la cella a destra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + NumColonne ) == 0 ) //Se la cella in basso è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne - 1 ) ) == 0 ) //Se la cella in basso a sinistra è vuota
			Contatore++;
		if ( LeggiElementoListaInteri ( MatricePosizionamento, t + ( NumColonne + 1 ) ) == 0 ) //Se la cella in basso a destra è vuota
			Contatore++;

		if ( Contatore == 7 ) //Se le celle vuote circostanti sono sette
			Flag = 1;
		else Flag = 0;

	}

	return Flag;
}

void StampaSchermataAttesa ( giocatore *Giocante, giocatore *Attendente )
{
	char Linea1[60]; //Caratteri sufficienti anche nel caso di nome di lunghezza massima
	char Linea2[50]; //Caratteri sufficienti anche nel caso di nome di lunghezza massima
	int InputTastiera = 0;

	sprintf ( Linea1, "Sta per cominciare il turno di %s", Giocante->Nome );
	sprintf ( Linea2, "%s e' pregato di voltarsi", Attendente->Nome );

	system ( "cls" );

	PosizionaCursore ( CentraCursore ( strlen ( Linea1 ) ), 8 );
	printf ( "%s", Linea1 );
	PosizionaCursore ( CentraCursore ( strlen ( Linea2 ) ), 11 );
	printf ( "%s", Linea2 );
	PosizionaCursore ( 5, 15 );
	MostraCursore ( );
	printf ( "Premi due tasti qualunque per continuare " ); //Uno solo potrebbe essere premuto per errore dall'ultimo giocatore di turno...
	InputTastiera = getch ( );
	if ( InputTastiera == 224 || InputTastiera == 0 )
		InputTastiera = getch ( );
	PosizionaCursore ( 5, 17 );
	printf ( "Premi un altro tasto per cominciare il turno " );
	InputTastiera = getch ( );
	if ( InputTastiera == 224 || InputTastiera == 0 )
		InputTastiera = getch ( );
	NascondiCursore ( );

}

void CelebrazioneVincitoreTorneo ( giocatore * Vincitore, settaggi * Opzioni )
{
	char Linea1[40]; //Caratteri sufficienti anche nel caso di nome di lunghezza massima
	char Linea2[25];
	int InputTastiera = 0;

	sprintf ( Linea1, "COMPLIMENTI %s", Vincitore->Nome );
	strcpy ( Linea2, "HAI VINTO IL TORNEO!" );

	system ( "cls" );

	if ( Opzioni->Audio == 0 )
		PlaySound ( "Audio\\GameWon.WAV", NULL, SND_ASYNC );

	PosizionaCursore ( CentraCursore ( strlen ( Linea1 ) ), 9 );
	printf ( "%s", Linea1 );
	PosizionaCursore ( CentraCursore ( 20 ), 11 ); //20 = lunghezza di Linea2
	printf ( "%s", Linea2 );

	PosizionaCursore ( 5, 15 );
	printf ( "Premi un tasto per terminare il torneo " );
	MostraCursore ( );
	InputTastiera = getch ( );
	if ( InputTastiera == 224 || InputTastiera == 0 )
		InputTastiera = getch ( );

}

void CelebrazioneVincitorePartita ( giocatore * Vincitore, settaggi * Opzioni )
{
	char Linea1[40]; //Caratteri sufficienti anche nel caso di nome di lunghezza massima
	char Linea2[23];
	int InputTastiera = 0;

	sprintf ( Linea1, "COMPLIMENTI %s", Vincitore->Nome );
	strcpy ( Linea2, "HAI VINTO LA PARTITA!" );

	system ( "cls" );

	if ( Opzioni->Audio == 0 )
		PlaySound ( "Audio\\GameWon.WAV", NULL, SND_ASYNC );

	PosizionaCursore ( CentraCursore ( strlen ( Linea1 ) ), 9 );
	printf ( "%s", Linea1 );
	PosizionaCursore ( CentraCursore ( 21 ), 11 ); //21 = lunghezza di Linea2
	printf ( "%s", Linea2 );

	PosizionaCursore ( 5, 15 );
	printf ( "Premi un tasto per continuare " );
	MostraCursore ( );
	InputTastiera = getch ( );
	if ( InputTastiera == 224 || InputTastiera == 0 )
		InputTastiera = getch ( );

}

void RipristinaFlotta ( flotta* Flotta )
{
	int i = 0;

	for ( i = 0; i < Flotta->NumSommergibili; i++ )
		Flotta->Sommergibili[i].NonAffondato = 2; //Reimposta il campo NonAffondato di ogni sommergibile della flotta

	for ( i = 0; i < Flotta->NumIncrociatori; i++ )
		Flotta->Incrociatori[i].NonAffondato = 3; //Reimposta il campo NonAffondato di ogni incrociatore della flotta

	for ( i = 0; i < Flotta->NumTorpediniere; i++ )
		Flotta->Torpediniere[i].NonAffondato = 4; //Reimposta il campo NonAffondato di ogni torpediniera della flotta

	for ( i = 0; i < Flotta->NumIncrociatori; i++ )
		Flotta->Portaerei[i].NonAffondato = 5; //Reimposta il campo NonAffondato di ogni portaereo della flotta

	Flotta->NaviNonAffondate = Flotta->NumSommergibili + Flotta->NumIncrociatori + Flotta->NumTorpediniere + Flotta->NumPortaerei; //Reimposta il campo NaviNonAffondate della flotta

}

void StampaCalendario ( torneo* Torneo )
{
	int i = 0;
	int j =0;
	int Flag = 0;
	giocatore Temp;
	giocatore Temp2;
	int InputTastiera = 0;

	//I caratteri stampati sono trattini verticali o orrizontali

	system ( "cls" );
	PosizionaCursore ( CentraCursore(25), 1 ); //25 = Lunghezza della scritta della riga successiva
	printf ( "  CALENDARIO DEL TORNEO  " );

	PosizionaCursore ( 1, 3 );
	printf ( "Sono evidenziati i giocatori che disputeranno il prossimo match." );

	//Disegno i trattini del primo turno
	for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
	{
		PosizionaCursore (1, 5 + i );
		for ( j = 0; j < 27; j++ )
			printf ( "%c", 196 );
		printf ( "%c", 191 );
		i++;
		PosizionaCursore  (1, 5 + i );
		for ( j = 0; j < 27; j++ )
			printf ( "%c", 196 );
		printf ( "%c", 217 );

	}

	//Disegno i trattini della che fanno incrociare i giocatori
	switch ( Torneo->Impostazioni.NumGiocatori )
	{
		case 2: //2 Giocatori
			PosizionaCursore ( 29, 5 );
			printf ( "____" );
		break;

		case 4: //4 Giocatori
			PosizionaCursore ( 29, 5 );
			printf ( "____" );
			PosizionaCursore ( 33, 6 );
			printf ( "%c____", 179 );
			PosizionaCursore ( 29, 7 );
			printf ( "____%c", 179 );
		break;

		case 8: //8 Giocatori
			for ( i = 0; i < 2; i++ )
			{
				PosizionaCursore ( 29, 5 + 4 * i );
				printf ( "____" );
				PosizionaCursore ( 33, 6 + 4 * i );
				printf ( "%c____", 179 );
				PosizionaCursore ( 29, 7 + 4 * i );
				printf ( "____%c", 179 );
			}
			PosizionaCursore ( 38, 7 );
			printf ( "%c", 179 );
			PosizionaCursore ( 38, 8 );
			printf ( "%c____", 179 );
			PosizionaCursore( 38, 9 );
			printf("%c", 179);
			PosizionaCursore( 38, 10 );
			printf("%c", 179);
		break;

		case 16: //16 Giocatori
			for( i=0; i< 4; i++ )
			{
				PosizionaCursore( 29, 5+4*i);
				printf("____");
				PosizionaCursore( 33, 6+4*i );
				printf("%c____", 179 );
				PosizionaCursore ( 29, 7 + 4 * i );
				printf ( "____%c", 179 );
			}
			for ( i = 0; i < 2; i++ )
			{
				PosizionaCursore ( 38, 7 + 8 * i );
				printf ( "%c", 179 );
				PosizionaCursore ( 38, 8 + 8 * i );
				printf ( "%c____", 179 );
				PosizionaCursore ( 38, 9 + 8 * i );
				printf ( "%c", 179 );
				PosizionaCursore ( 38, 10 + 8 * i );
				printf ( "%c", 179 );
			}
			for ( i = 0; i < 8; i++ )
			{
				PosizionaCursore ( 43, 9 + i );
				printf ( "%c", 179 );
			}
			PosizionaCursore ( 44, 12 );
			printf ( "____" );
		break;

	}



	//Evidenzio in rosso i vincitori del primo turno e traccio le linee successivi
	ImpostaColore ( 0x3C );
	if ( ( Torneo->Calendario.PartitaDaGiocare <= ( Torneo->Impostazioni.NumGiocatori / 2 ) ) && ( Torneo->Calendario.PartitaDaGiocare != 1 ) )
	{

		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{

			for ( j = 0; j < Torneo->Calendario.PartitaDaGiocare - 1; j++ )
			{
				Temp = *Torneo->Calendario.Match[j].Vincitore;
				if ( strcmp ( Torneo->Partecipanti[i].Nome, Temp.Nome ) == 0 )
					Flag = 1;
			}

			if ( Flag == 1 ) //Se il giocatore ha vinto almeno un match disegno la linea rossa
			{
				PosizionaCursore (1, 5 + i );
				if ( i % 2 == 0 )
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c____", 191 );
				}
				else
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c", 217 );
					PosizionaCursore ( 29, 4 + i );
					printf ( "____" );
				}

			}
			Flag = 0;

		}
	}

	else if ( ( Torneo->Calendario.PartitaDaGiocare > ( Torneo->Impostazioni.NumGiocatori / 2 ) ) && ( Torneo->Calendario.PartitaDaGiocare != 1 ) )
	{
		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{

			for ( j = 0; j < ( Torneo->Impostazioni.NumGiocatori / 2 ); j++ )
			{
				Temp = *Torneo->Calendario.Match[j].Vincitore;
				if ( strcmp ( Torneo->Partecipanti[i].Nome, Temp.Nome ) == 0 )
					Flag = 1;
			}

			if ( Flag == 1 ) //Se il giocatore ha vinto almeno un match disegno la linea rossa
			{
				PosizionaCursore ( 1, 5 + i );
				if ( i % 2 == 0)
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c____", 191 );
				}
				else
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c", 217 );
					PosizionaCursore ( 29, 4 + i );
					printf ( "____" );
				}

			}
			Flag = 0;

		}
	}
	else if ( ( Torneo->Impostazioni.NumGiocatori == 2 ) && ( Torneo->Calendario.PartitaDaGiocare == 2 ) ) //Si è completato un torneo a 2 giocatori
	{
		for ( i = 0; i < 2; i++ )
		{

			Temp = *Torneo->Calendario.Match[0].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[i].Nome, Temp.Nome ) == 0 )
				Flag = 1;

			if ( Flag == 1 ) //Se il giocatore ha vinto il primo ed unico match, disegno la linea rossa
			{
				PosizionaCursore ( 1, 5 + i );
				if ( i % 2 == 0 )
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c____", 191 );
				}
				else
				{
					for ( j = 0; j < 27; j++ )
						printf ( "%c", 196 );
					printf ( "%c", 217 );
					PosizionaCursore ( 29, 4 + i );
					printf ( "____" );
				}

			}
			Flag = 0;

		}
	}
	ImpostaColore ( 0x3f );


	//Evidenzio in rosso i vincitori del secondo turno e traccio le linee successive per torneo a 4 giocatori
	ImpostaColore ( 0x3C );
	if ( ( Torneo->Impostazioni.NumGiocatori == 4 ) && ( Torneo->Calendario.PartitaDaGiocare == 4 ) ) //Si è completato un torneo a 4 giocatori
	{

		Temp = *Torneo->Calendario.Match[2].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 1 o dal 2
		{
			PosizionaCursore ( 33, 6 );
			printf ( "%c____", 179 );
		}
		else //Se l'incontro è stato vinto dal giocatore 3 o dal 4
		{
			PosizionaCursore ( 34, 6 );
			printf ( "____" );
			PosizionaCursore ( 33, 7 );
			printf ( "%c", 179 );
		}
	}
	ImpostaColore ( 0x3f );


	//Evidenzio in rosso i vincitori del secondo turno e traccio le linee successive per torneo a 8 giocatori
	ImpostaColore ( 0x3C );
	if ( ( Torneo->Impostazioni.NumGiocatori == 8 ) && ( Torneo->Calendario.PartitaDaGiocare >= 6 ) ) //Si è giocata almeno una partita del secondo turno del torneo
	{
		//Gestitisco la quinta partita del torneo
		Temp = *Torneo->Calendario.Match[4].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 ) //Se il sesto incontro è stato vinto dal giocatore 1 o dal 2
		{
			PosizionaCursore ( 33, 6 );
			printf ( "%c____", 179 );
		}
		else //Se l'incontro è stato vinto dal giocatore 3 o dal 4
		{
			PosizionaCursore ( 34, 6 );
			printf ( "____" );
			PosizionaCursore ( 33, 7 );
			printf ( "%c", 179 );
		}

		if ( Torneo->Calendario.PartitaDaGiocare >= 7 ) //Se si è giocata anche la sesta partita del torneo
		{
			//Gestitisco la sesta partita del torneo
			Temp = *Torneo->Calendario.Match[5].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[4].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[5].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 5 o dal 6
			{
				PosizionaCursore ( 33, 10 );
				printf ( "%c____", 179 );
			}
			else //Se l'incontro è stato vinto dal giocatore 7 o dal 8
			{
				PosizionaCursore ( 34, 10 );
				printf ( "____" );
				PosizionaCursore ( 33, 11 );
				printf ( "%c", 179 );
			}
		}
	}
	ImpostaColore ( 0x3f );

	//Evidenzio in rosso i vincitori del secondo turno e traccio le linee successive per torneo a 16 giocatori
	ImpostaColore ( 0x3C );
	if ( ( Torneo->Impostazioni.NumGiocatori == 16 ) && ( Torneo->Calendario.PartitaDaGiocare >= 10 ) ) //Si è giocata almeno una partita del secondo turno del torneo
	{
		//Gestitisco la nona partita del torneo
		Temp = *Torneo->Calendario.Match[8].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 ) //Se il sesto incontro è stato vinto dal giocatore 1 o dal 2
		{
			PosizionaCursore ( 33, 6 );
			printf ( "%c____", 179 );
		}
		else //Se l'incontro è stato vinto dal giocatore 3 o dal 4
		{
			PosizionaCursore ( 34, 6 );
			printf ( "____" );
			PosizionaCursore ( 33, 7 );
			printf ( "%c", 179 );
		}

		if ( Torneo->Calendario.PartitaDaGiocare >= 11 ) //Se si è giocata anche la decima partita del torneo
		{
			//Gestitisco la decima partita del torneo
			Temp = *Torneo->Calendario.Match[9].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[4].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[5].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 5 o dal 6
			{
				PosizionaCursore ( 33, 10 );
				printf ( "%c____", 179 );
			}
			else //Se l'incontro è stato vinto dal giocatore 7 o dal 8
			{
				PosizionaCursore ( 34, 10 );
				printf ( "____" );
				PosizionaCursore ( 33, 11 );
				printf ( "%c", 179 );
			}
		}

		if ( Torneo->Calendario.PartitaDaGiocare >= 12 ) //Se si è giocata anche l'undicesima partita del torneo
		{
			//Gestitisco l'undicesima partita del torneo
			Temp = *Torneo->Calendario.Match[10].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[8].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[9].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 9 o dal 10
			{
				PosizionaCursore ( 33, 14 );
				printf ( "%c____", 179 );
			}
			else //Se l'incontro è stato vinto dal giocatore 11 o dal 12
			{
				PosizionaCursore ( 34, 14 );
				printf ( "____" );
				PosizionaCursore ( 33, 15 );
				printf ( "%c", 179 );
			}
		}

		if ( Torneo->Calendario.PartitaDaGiocare >= 13 ) //Se si è giocata anche la dodicesima partita del torneo
		{
			//Gestitisco la dodicesima partita del torneo
			Temp = *Torneo->Calendario.Match[11].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[12].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[13].Nome, Temp.Nome ) == 0 ) //Se l'incontro è stato vinto dal giocatore 13 o dal 14
			{
				PosizionaCursore ( 33, 18 );
				printf ( "%c____", 179 );
			}
			else //Se l'incontro è stato vinto dal giocatore 15 o dal 16
			{
				PosizionaCursore ( 34, 18 );
				printf ( "____" );
				PosizionaCursore ( 33, 19 );
				printf ( "%c", 179 );
			}
		}
	}
	ImpostaColore ( 0x3f );


	//Evidenzio in rosso i vincitori del terzo turno e traccio le linee successive per torneo a 16 giocatori
	ImpostaColore ( 0x3C );
	if ( ( Torneo->Impostazioni.NumGiocatori == 16 ) && ( Torneo->Calendario.PartitaDaGiocare >= 14 ) ) //Si è giocata almeno una partita del terzo turno del torneo
	{
		Temp = *Torneo->Calendario.Match[12].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[2].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[3].Nome, Temp.Nome ) == 0 ) //Se l'incontro è stato vinto dal giocatore 1, 2, 3 o 4
		{
			PosizionaCursore ( 38, 7 );
			printf ( "%c", 179 );
			PosizionaCursore ( 38, 8 );
			printf ( "%c____", 179 );
		}
		else //Se l'incontro è stato vinto dal giocatore 5, 6, 7 o 8
		{
			PosizionaCursore ( 39, 8 );
			printf ( "____" );
			PosizionaCursore ( 38, 9 );
			printf ( "%c", 179 );
			PosizionaCursore ( 38, 10 );
			printf ( "%c", 179 );
		}

		if ( Torneo->Calendario.PartitaDaGiocare >= 15 ) //Se si è giocata anche l'ultima partita del terzo turno
		{
			//Gestitisco la quattordicesima partita del torneo
			Temp = *Torneo->Calendario.Match[13].Vincitore;
			if ( strcmp ( Torneo->Partecipanti[8].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[9].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[10].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[11].Nome, Temp.Nome ) == 0 ) //Se l'incontro è stato vinto dal giocatore 9, 10, 11 o 12
			{
				PosizionaCursore ( 38, 15 );
				printf ( "%c", 179 );
				PosizionaCursore ( 38, 16 );
				printf ( "%c____", 179 );
			}
			else //Se l'incontro è stato vinto dal giocatore 13, 14, 15 o 16
			{
				PosizionaCursore ( 39, 16 );
				printf ( "____" );
				PosizionaCursore ( 38, 17 );
				printf ( "%c", 179 );
				PosizionaCursore ( 38, 18 );
				printf ( "%c", 179 );
			}
		}
	}

	ImpostaColore ( 0x3f );



	//Scrivo i nomi dei giocatori (quelli che stanno per giocare li scrivo in giallo)
	if ( Torneo->Calendario.PartitaDaGiocare <= Torneo->Calendario.TotalePartite ) //Se il torneo non si è concluso
	{
		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{
			PosizionaCursore ( 1, 5 + i );

			Temp = *Torneo->Calendario.Match[ Torneo->Calendario.PartitaDaGiocare - 1 ].Giocatore1;
			Temp2 = *Torneo->Calendario.Match[ Torneo->Calendario.PartitaDaGiocare - 1 ].Giocatore2;

			if ( ( strcmp ( Torneo->Partecipanti[i].Nome, Temp.Nome ) == 0 ) || ( strcmp ( Torneo->Partecipanti[i].Nome, Temp2.Nome ) == 0 ) ) //Se il giocatore deve giocare la prossima partita
			{
				ImpostaColore ( 0x3E );
				printf ( "%s ", Torneo->Partecipanti[i].Nome );
				ImpostaColore ( 0x3f );

			}

			else
			{
				printf ( "%s ", Torneo->Partecipanti[i].Nome );
			}
		}
	}
	else //Se il torneo si è concluso non posso evidenziare in giallo il prossimo match (si sono già disputati tutti)
	{
		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{
			PosizionaCursore ( 1, 5 + i );
			printf ( "%s ", Torneo->Partecipanti[i].Nome );
		}
	}

	//Scrivo i nomi dei vincitori se si è concluso il torneo
	ImpostaColore ( 0x3E );
	if ( ( Torneo->Impostazioni.NumGiocatori == 2 ) && ( Torneo->Calendario.PartitaDaGiocare == 2 ) )
	{
		Temp = *Torneo->Calendario.Match[0].Vincitore;
		PosizionaCursore ( 34, 5 );
		printf ( "%s", Temp.Nome );
	}

	if ( ( Torneo->Impostazioni.NumGiocatori == 4 ) && ( Torneo->Calendario.PartitaDaGiocare == 4 ) )
	{
		Temp = *Torneo->Calendario.Match[2].Vincitore;
		PosizionaCursore ( 39, 6 );
		printf ( "%s", Temp.Nome );
	}

	if ( ( Torneo->Impostazioni.NumGiocatori == 8 ) && ( Torneo->Calendario.PartitaDaGiocare == 8 ) )
	{
		ImpostaColore ( 0x3C );
		//Coloro l'ultimo segmento in base al vincitore dell'ultimo match
		Temp = *Torneo->Calendario.Match[6].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[2].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[3].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 1, 2, 3 o 4
		{
			PosizionaCursore ( 38, 7 );
			printf ( "%c", 179 );
			PosizionaCursore ( 38, 8 );
			printf ( "%c____", 179 );
		}
		else //Se l'ultimo incontro è stato vinto dal giocatore 5, 6, 7 o 8
		{
			PosizionaCursore ( 39, 8 );
			printf ( "____" );
			PosizionaCursore ( 38, 9 );
			printf ( "%c", 179 );
			PosizionaCursore ( 38, 10 );
			printf ( "%c", 179 );
		}
		ImpostaColore ( 0x3E );
		PosizionaCursore ( 44, 8 );
		printf ( "%s", Temp.Nome );
	}

	if ( ( Torneo->Impostazioni.NumGiocatori == 16 ) && ( Torneo->Calendario.PartitaDaGiocare == 16 ) )
	{
		ImpostaColore ( 0x3C );
		//Coloro l'ultimo segmento in base al vincitore dell'ultimo match
		Temp = *Torneo->Calendario.Match[14].Vincitore;
		if ( strcmp ( Torneo->Partecipanti[0].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[1].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[2].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[3].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[4].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[5].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[6].Nome, Temp.Nome ) == 0 || strcmp ( Torneo->Partecipanti[7].Nome, Temp.Nome ) == 0 ) //Se l'ultimo incontro è stato vinto dal giocatore 1, 2, 3, 4, 5, 6, 7 o 8
		{
			PosizionaCursore ( 43, 9 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 10 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 11 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 12 );
			printf ( "%c____", 179);
		}
		else //Se l'ultimo incontro è stato vinto dal giocatore 9, 10, 11, 12, 13, 14, 15 o 16
		{
			PosizionaCursore ( 44, 12 );
			printf ( "____" );
			PosizionaCursore ( 43, 13 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 14 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 15 );
			printf ( "%c", 179 );
			PosizionaCursore ( 43, 16 );
			printf ( "%c", 179 );
		}

		ImpostaColore ( 0x3E );
		PosizionaCursore ( 49, 12 );
		printf ( "%s", Temp.Nome );
	}


	ImpostaColore ( 0x3f );

	PosizionaCursore ( 56, 23 );
	printf ( "Salva e continua: TAB " );

	PosizionaCursore ( 1, 23 );
	printf ( "Premi un tasto per continuare " );

	InputTastiera = getch ( );
	if ( InputTastiera == 224 || InputTastiera == 0 )
		InputTastiera = getch ( );

	if ( InputTastiera == 9 ) //TAB: Salva
		SalvaPartita ( Torneo, NULL, NULL, 0, 0 );
}

void SalvaPartita (torneo * Torneo, giocatore * GiocatoreA, giocatore * GiocatoreB, int TurnoCorrente, int PartitaInCorso )
{
	int InputTastiera = 0;
	char NomeSalvataggio[30] = "";
	FILE *fp = NULL;
	int i = 0;
	int j = 0;
	int Temporaneo = 0;
	int Pattern = 177;
	char Buffer = '0';
	int NumeroPagine = 0;
	int PaginaCorrente = 1;
	int NumeroSalvataggiPaginaCorrente = 0;
	int NumeroSalvataggi = 0;
	int FlagUscita = 0;
	int Flag = 0;

	HANDLE Handle = GetStdHandle ( STD_OUTPUT_HANDLE );
	COORD Coordinate;
	DWORD CaratteriScritti;


	system ( "cls" );
	Coordinate.X = 79;
	Coordinate.Y = 24;

	StampaPattern ( 80 * 25 - 1 );
	FillConsoleOutputCharacter ( Handle, Pattern, 1, Coordinate, &CaratteriScritti ); //Stampa il pattern nel pixel in basso a destra

	PosizionaCursore ( CentraCursore(21), 1 ); //21 è la lunghezza della stringa al rigo successivo
	printf ( " SALVATAGGIO PARTITA " );

	PosizionaCursore ( 1, 3 );
	printf ( "Premi 1 per sovrascrivere un salvataggio o 2 per crearne uno nuovo:   " );
	PosizionaCursore ( 69, 3 );
	MostraCursore ( );
	do
	{
		InputTastiera = getch ( );
	} while ( InputTastiera != 49 && InputTastiera != 50 ); //Ripete fin quando non viene premuto 1 o 2

	if ( InputTastiera == 49 ) //Ha premuto 1
	{

		printf ( "1" ); //Simulo l'echo dell'input da tastiera
		PosizionaCursore ( 1, 5 );
		printf ( "Elenco dei salvataggi: " );

		NumeroSalvataggi = IndicizzaSalvataggiValidi ( );

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

		if ( Buffer != EOF ) //Se c'è almeno un file .tsg (se non ci fossero salvataggi il file Indice.txt sarebbe vuoto ed il primo carattere sarebbe EOF)
		{
			rewind ( fp ); //Recupero il carattere letto

			NumeroPagine = NumeroSalvataggi / 10;

			if ( NumeroSalvataggi % 10 != 0 )
				NumeroPagine++;

			if ( NumeroSalvataggi >= 10 )
				NumeroSalvataggiPaginaCorrente = 10;
			else
				NumeroSalvataggiPaginaCorrente = NumeroSalvataggi;

			rewind ( fp );

			//Stampo i nomi dei primi 10 salvataggi (se sono meno di 10, stampo solo quelli presenti)
			ImpostaColore ( 0x6F );

			while ( !feof ( fp ) && i < 10 )
			{
				fscanf ( fp, "%s\n", NomeSalvataggio );
				PosizionaCursore ( 1, 7 + i );
				printf ( " #%d: %s ", i, NomeSalvataggio );
				strcpy ( NomeSalvataggio, "" );
				i++;
			}

			rewind ( fp );

			if ( NumeroPagine > 1 )
			{
				ImpostaColore ( 0x6F );
				PosizionaCursore ( 1, 20 );
				printf ( "Scorri i salvataggi con %c ", 18 );
			}

			ImpostaColore ( 0x3F );
			PosizionaCursore ( 1, 23 );
			printf ( " Digita il numero della partita da sovrascrivere:   " );
			PosizionaCursore ( 51, 23 );

			FlagUscita = -1;

			do
			{
				InputTastiera = getch ( );
				if ( InputTastiera == 224 )
					InputTastiera = getch ( );

				if ( InputTastiera >= 48 && InputTastiera - 48 < NumeroSalvataggiPaginaCorrente ) //Se ha premuto un tasto da 0 al numero di salvataggi
					FlagUscita = InputTastiera; //Uscirà dal do while

				switch ( InputTastiera )
				{
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
								PosizionaCursore ( 1, 7 + i );
								StampaPattern ( 35 );
							}

							//Stampo i salvataggi successivi (al massimo 10)
							i = 0;
							NumeroSalvataggiPaginaCorrente = 0;
							while ( !feof ( fp ) && i < 10 )
							{
								fscanf ( fp, "%s\n", NomeSalvataggio );
								PosizionaCursore ( 1, 7 + i );
								ImpostaColore ( 0x6F );
								printf ( " #%d: %s ", i, NomeSalvataggio );
								strcpy ( NomeSalvataggio, "" );
								ImpostaColore ( 0x3F );
								i++;
								NumeroSalvataggiPaginaCorrente++;
							}
							PosizionaCursore ( 46, 23 );
						}
					break;

					case 72: //Freccia SU
						if ( NumeroPagine > 1 && PaginaCorrente > 1)
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
								PosizionaCursore ( 1, 7 + i );
								StampaPattern ( 35 );
							}

							//Stampo i salvataggi successivi (al massimo 10)
							i = 0;
							NumeroSalvataggiPaginaCorrente = 0;

							while ( !feof ( fp ) && i < 10 )
							{
								fscanf ( fp, "%s\n", NomeSalvataggio );
								PosizionaCursore ( 1, 7 + i );
								ImpostaColore ( 0x6F );
								printf ( " #%d: %s ", i, NomeSalvataggio );
								strcpy ( NomeSalvataggio, "" );
								ImpostaColore ( 0x3F );
								i++;
								NumeroSalvataggiPaginaCorrente++;
							}
							PosizionaCursore ( 46, 23 );

						}
					break;
				}

			} while ( FlagUscita == -1 );


			InputTastiera -= 48; //Converto l'ascii del numero premuto nell'intero corrispondente

			rewind ( fp );
			for ( i = 0; i < InputTastiera + PaginaCorrente * 10 - 10; i++ )
			{
				do
				{
					Buffer = fgetc(fp);
				} while ( Buffer != '\n' ); //Leggo un numero di righe pari a InputTastiera: al termine il cursore del file si troverà al nome del file da aprire
			}

			fscanf ( fp, "%s", NomeSalvataggio );
			InputTastiera = 50; //In questo modo entrerà nella procedura per creare un nuovo salvataggio
			Flag = 2; //Il flag farà saltare la procedura che acquisisce il nome del salvataggio del file da tastiera


		}
		else //Se non ci sono salvataggi
		{
			ImpostaColore ( 0x3F );
			PosizionaCursore ( 1, 4 );
			StampaPattern ( 25 );

			PosizionaCursore ( 1, 5 );
			printf ( "Non sono presenti tornei da sovrascrivere. Salverai su un nuovo file." );
			PosizionaCursore ( 1, 7 );
			printf ( "Premi un tasto per continuare   " );
			MostraCursore ( );
			PosizionaCursore ( 31, 7 );
			Temporaneo = getch ( );
			if ( Temporaneo == 224 || Temporaneo == 0 )
				Temporaneo = getch ( );
			Temporaneo = 0;
			system ( "cls" );

			NascondiCursore ( );

			InputTastiera = 50;
			Flag = 1;
		}

		fclose ( fp ); //Chiude il file Indice.txt
		remove ( "Indice.txt" );

	}

	if ( InputTastiera == 50 ) //Ha premuto 2
	{
		if ( Flag == 1 ) //Se mancavano salvataggi da sovrascrivere è stata effettuata system("cls"): è necessario ristampare alcune scritte
		{

			StampaPattern ( 80 * 25 - 1 );
			FillConsoleOutputCharacter ( Handle, Pattern, 1, Coordinate, &CaratteriScritti); //Stampa il pattern nel pixel in basso a destra
			PosizionaCursore ( CentraCursore(21), 1 ); //21 è la lunghezza della stringa al rigo successivo
			printf ( " SALVATAGGIO PARTITA " );

			PosizionaCursore ( 1, 3 );
			printf ( "Premi 1 per sovrascrivere un salvataggio o 2 per crearne uno nuovo:   " );
			PosizionaCursore ( 69, 3 );
			MostraCursore ( );
		}

		if ( Flag != 2 ) //Flag vale 2 solo se l'utente ha selezionato un salvataggio da sovrascrivere: non è necessario acquisire il nome del file
		{
			printf ( "2" ); //Simulo l'echo dell'input da tastiera
			PosizionaCursore ( 63, 24 ); //Elimina un errore grafico
			StampaPattern ( 16 );
			PosizionaCursore ( 1, 5 );
			printf ( "Inserisci il nome del salvataggio (max 24 caratteri): " );

			do
			{
				PosizionaCursore ( 1, 7 );
				StampaPattern ( 35 ); //Pulisce la riga
				PosizionaCursore ( 1, 7 );
				printf ( "Nome:  " );
				AcquisisciNome ( NomeSalvataggio, 8, 7 );
				strcat ( NomeSalvataggio, ".tsg" ); //Aggiungo l'estensione al nome del file

				fp = fopen (NomeSalvataggio, "rb" );

				if ( fp != NULL )
				{
					PosizionaCursore ( 1, 9 );
					printf ( "Esiste gia' un file con quel nome: reinserisci il nome nello spazio sopra." );
					fclose ( fp );
				}

			} while ( fp != NULL ); //Ripete la richiesta di inserimento di un nome per il file fin quando non viene immesso il nome di un file non presente

			PosizionaCursore ( 1, 9 );
			StampaPattern ( 78 );
		}

		system ( "cls" );

		//Inizio procedura salvataggio
		fp = fopen( NomeSalvataggio, "wb");
		if ( fp == NULL )
		{
			system ( "cls" );
			PosizionaCursore ( 0, 0 );
			perror ( "" );
			fprintf ( stderr, "\n\nErrore irreversibile. Premi un tasto per uscire..." );
			getch ( );
			exit ( -1 );
		}

		fprintf ( fp, "123 " ); //123 è un numero-flag che garantisce l'integrità del file

		fwrite ( &Torneo->Impostazioni, sizeof ( settaggi ), 1, fp );

		for ( i = 0; i < Torneo->Impostazioni.NumGiocatori; i++ )
		{
			fprintf ( fp, "%s %d ", Torneo->Partecipanti[i].Nome, Torneo->Partecipanti[i].Punteggio );

			for ( j = 0; j < Torneo->Impostazioni.NumSommergibili; j++ )
				fprintf ( fp, "%d %d ", Torneo->Partecipanti[i].Flotta.Sommergibili[j].NonAffondato, Torneo->Partecipanti[i].Flotta.Sommergibili[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumIncrociatori; j++ )
				fprintf ( fp, "%d %d ", Torneo->Partecipanti[i].Flotta.Incrociatori[j].NonAffondato, Torneo->Partecipanti[i].Flotta.Incrociatori[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumTorpediniere; j++ )
				fprintf ( fp, "%d %d ", Torneo->Partecipanti[i].Flotta.Torpediniere[j].NonAffondato, Torneo->Partecipanti[i].Flotta.Torpediniere[j].Id );

			for ( j = 0; j < Torneo->Impostazioni.NumPortaerei; j++ )
				fprintf ( fp, "%d %d ", Torneo->Partecipanti[i].Flotta.Portaerei[j].NonAffondato, Torneo->Partecipanti[i].Flotta.Portaerei[j].Id );

			fprintf ( fp, "%d ", Torneo->Partecipanti[i].Flotta.NaviNonAffondate );


			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				Temporaneo = LeggiElementoListaInteri ( Torneo->Partecipanti[i].GrigliaAttacco, j );
				fprintf ( fp, "%d ", Temporaneo );
			}

			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				Temporaneo = LeggiElementoListaInteri ( Torneo->Partecipanti[i].GrigliaDifesa, j );
				fprintf ( fp, "%d ", Temporaneo );
			}

			for ( j = 0; j < Torneo->Impostazioni.NumColonne * Torneo->Impostazioni.NumRighe; j++ )
			{
				Temporaneo = LeggiElementoListaInteri ( Torneo->Partecipanti[i].MatricePosizionamento, j );
				fprintf ( fp, "%d ", Temporaneo );
			}

		}

		fprintf ( fp, "%d ", Torneo->Calendario.PartitaDaGiocare );

		//Per ogni partita giocata scrivo: Giocatore1, Giocatore2, Vincitore
		for ( i = 0; i < Torneo->Calendario.PartitaDaGiocare - 1; i++ )
		{
			fprintf ( fp, "%s ", Torneo->Calendario.Match[i].Giocatore1->Nome );
			fprintf ( fp, "%s ", Torneo->Calendario.Match[i].Giocatore2->Nome );
			fprintf ( fp, "%s ", Torneo->Calendario.Match[i].Vincitore->Nome );
		}

		fprintf ( fp, "%d ", PartitaInCorso );

		if ( PartitaInCorso == 1 )
		{
			fprintf ( fp, "%s %s ", GiocatoreA->Nome, GiocatoreB->Nome );
			fprintf ( fp, "%d ", TurnoCorrente );
		}
		//Fine procedura salvataggio

		fclose ( fp ); //Chiude il file del salvataggio

		MessageBox ( NULL, "Torneo salvato correttamente", "Avviso", MB_ICONINFORMATION | MB_SYSTEMMODAL );

	}
}

int NuovoNome ( torneo* Torneo, int Posizione )
{
    int Controllo = 1;
	int i = 0;

	if ( Posizione > 0 && Posizione < Torneo->Impostazioni.NumGiocatori )
    {
		for ( i = 0; ( i < Posizione && Controllo == 1 ); i++ )
			if ( strcmp ( Torneo->Partecipanti[i].Nome, Torneo->Partecipanti[Posizione].Nome ) == 0 )
				Controllo = 0;
	}

    return Controllo;
}

void DeallocaLista ( listaint Lista )
{
	listaint CursoreTemp = NULL;
	listaint CursoreCorrente = Lista;

	while ( CursoreCorrente != NULL )
	{
		//Salvo il nodo da eliminare (corrispondente al nodo corrente)
		CursoreTemp = CursoreCorrente;
		// Mi posiziono sul nodo successivo per tener traccia della lista
		CursoreCorrente = CursoreCorrente->Prox;
		// Rimuovo il nodo corrente
		free ( CursoreTemp );
	}
}
