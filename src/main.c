#include "Libreria.h"

/*************************************************************
********************** BATTAGLIA NAVALE **********************
**************************************************************
** Sviluppatori: Gianvito Taneburgo & Francesco Scuccimarri **
**************************************************************
**************************************************************/

/*************************************************************
>>>>>>>>>>>>>>>>>>>> Commenti generali <<<<<<<<<<<<<<<<<<<<<<<
**************************************************************
* 1) Nel corso del programma viene utilizzata spesso la funzione
* StampaPattern, specialmente per inserire il pattern come sfondo
* in tutta la finestra. In tal caso viene si effettua la chiamata
* StampaPattern(80*25-1) e non StampaPattern(80*25), come sarebbe
* invece più logico pensare (avendo il campo dimensioni fissate a 80*25).
* Il motivo è che stampando il pattern nell'ultimo pixel della 
* finestra il cursore si sposta nella riga successiva, generando
* un problema di visualizzazione: diventa quindi necessario NON
* stampare alcun carattere nell'ultimo pixel della finestra (a
* tal fine si usa il -1). Laddove diventa imprescindibile riempire
* l'ultimo pixel della finestra viene usata la funzione FillConsoleOutputCharacter()
* che, dopo aver stampato un carattere, non sposta il cursore nella
* riga successiva (e non genera il problema di StampaPattern).
* In generale, durante quasi tutta l'esecuzione del gioco l'ultimo
* pixel non viene mai stampato.
* 
* 2) Ogni giocatore ha un proprio nome. Nel corso di uno stesso torneo
* non ci possono essere giocatori con lo stesso nome.
* 
* 3) I nomi dei giocatori, così come i nomi dei salvataggi, sono
* necessariamente alfanumerici, di massimo 24 caratteri.
* 
* 4) Ogni nave ha un proprio Id. Due navi non possono avere lo 
* stesso Id. L'Id 0 corrisponde ad una cella non occupata da navi.
* 
* 5) Per l'acquisizione di input da tastiera viene utilizzata
* solitamente getch(). Alcuni tasti, come F1, F2, F3, ecc... restituiscono
* due valori: un primo valore (0 o 224) per indicare che è
* stato premuto un tasto "speciale" e poi il loro effettivo
* valore. Durante il gioco spesso è stato necessario gestire questo
* "doppio ritorno", acquisendo un carattere in più nel caso fosse
* premuto uno di quei tasti (come se si svuotasse il buffer di lettura).
* A tal fine, infatti, spesso compare il controllo:
* 
*   if ( InputTastiera == 224 || InputTastiera == 0 )
* 
**************************************************************/

int main ( )
{
	int FlagRistampaMenu = 1;
	
	CambiaNomeFinestra ( );
	ImpostaDimensioniFinestra ( );
	CancellaImpostazioniTemporanee ( );
	/*Il file contenente le impostazioni temporanee potrebbe essere presente nella cartella del gioco nel caso 
	in cui il gioco, durante la sua esecuzione precedente, non sia stato chiuso utilizzando la voce ESCI del menu 
	(l'utente aveva chiuso la finestra, ad esempio, premendo la X). E' necessario rimuovere il file, se presente*/
	CreaImpostazioniDefault ( );
	
	do
	{
		ImpostaColore ( 0x3F );
		system ( "cls" );
		StampaSfondoMenu ( );
		NascondiCursore ( );
		ImpostaColore ( 0x3F );
		FrecciaMenu( &FlagRistampaMenu );
	} while ( FlagRistampaMenu != 0 ); //Soltanto Esci() imposterà la variabile a 0 causando la fine del gioco

	CancellaImpostazioniTemporanee ( );
	
	return 0;
}
