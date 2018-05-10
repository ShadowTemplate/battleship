#include "Libreria.h"

void Esci ( )
{
     system ( "cls" );
     StampaPattern ( 80 * 25 - 1 );
     PosizionaCursore ( 23, 9 );
     printf ( "                                 " );
     PosizionaCursore ( 23, 10 );
     printf ( "     GRAZIE PER AVER GIOCATO!    " );
     PosizionaCursore ( 23, 11 );
     printf ( "                                 " );
     PosizionaCursore ( 23, 12 );
     printf ( "   TORNA PER UN'ALTRA PARTITA!   " );
     PosizionaCursore ( 23, 13 );
     printf ( "                                 " );
     PosizionaCursore ( 50, 24);
	 printf ( " Premi un tasto per uscire..." );
	 getch ( );

}
