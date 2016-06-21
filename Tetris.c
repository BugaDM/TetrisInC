#include <stdio.h>
#include <Windows.h> //system("cls"); 
#include <conio.h> //getche();
#include <stdlib.h> //srand(); rand();
#include <time.h> //time();

//Declaración de constantes
#define i 21
#define j 12
#define NUEVO_JUEGO '1'
#define JUEGO_GUARDADO '2'
#define GUARDAR_JUEGO '3'
#define SALIR '4'
#define ENTER 13
#define FLECHA1 224
#define FLECHA -32
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80

//Prototipos de funciones
void ImprimirCabecera ();
void Cargando ();
int SeleccionIdioma ();
void CrearSelector ( char [19][24], int );
void MostrarSelector ( char [19][24] );
void CrearCaja ( char [i][j] );
void ImprimirMatriz ( char [i][j] );
void ImprimirMenu ( int );
void Jugar ( char [i][j], int [4][2], int*, int, int*, int*, int );
int CrearPieza ( char [i][j], int [4][2], int*, int* );
void ImprimirMarcador ( int );
void ImprimirMenuJugar ( int );
void JuegoTerminado ( int );
int MoverPieza ( char [i][j], int [4][2], int, int*, char);
int RotarPieza ( char [i][j], int [4][2], int, int*);
void RangoMaximo ( int [4][2], int*, int*, int*);
int PerimetroPieza ( char [i][j], int [4][2], int, int, int, int, int, int, int, int );
int SiguientePosicion( char [i][j], int [4][2], int, int );
void Puntuacion ( char [i][j], int* );
void GuardarJuego( char [i][j], int [4][2], int, int, int );
int CargarJuego( char [i][j], int [4][2], int*, int*, int* );

//____________________MAIN____________________
int main(){

	char caja[i][j];
	char opt;
	int puntos, partida_cargada, contador, numero_pieza, variable_rotacion, idioma, jugaste = 0, fallo_cargar;
	int coordenadas_pieza[4][2];

	srand ( time(NULL) ); //Genera una semilla en función de la hora a la que se ejecuta el programa

	Cargando ();
	idioma = SeleccionIdioma ();

	do{
		system ( "cls" );
		CrearCaja ( caja );
		ImprimirMatriz ( caja );
		ImprimirMenu ( idioma );
		scanf ( "%c", &opt );
	}
	while ( opt<'1' && opt>'4' );

	while ( opt != SALIR ){
		switch ( opt ){
		case NUEVO_JUEGO:
			jugaste = 1;
			puntos = 0;
			partida_cargada = 0; //No has cargado partida, has iniciado una nueva
			CrearCaja ( caja );
			Jugar ( caja, coordenadas_pieza, &numero_pieza, partida_cargada, &variable_rotacion, &puntos, idioma );
			break;
		case JUEGO_GUARDADO:
			partida_cargada = 1; //Has cargado una partida
			fallo_cargar = CargarJuego ( caja, coordenadas_pieza, &numero_pieza, &puntos, &variable_rotacion);
			if ( fallo_cargar ) break;
			Jugar ( caja, coordenadas_pieza, &numero_pieza, partida_cargada, &variable_rotacion, &puntos, idioma );
			break;
		case GUARDAR_JUEGO:
			//Si no se ha iniciado una nueva partida ni se ha cargado otra existente no habrá datos para guardar
			if ( jugaste == 0 && fallo_cargar != 0){
				system ("cls");
				switch (idioma){
				case 1: printf("\n There is no data to save.");
					break;
				case 2: printf ("\n Non hai datos para gardar.");
					break;
				case 3:
					printf ("\n No hay datos para guardar.");
					break;
				}
				Sleep (3000);
				break;
			}
			GuardarJuego ( caja, coordenadas_pieza, numero_pieza, puntos, variable_rotacion ); 
			break;
		}//Cierra el switch
		getchar();
		system( "cls" );
		ImprimirMatriz ( caja );
		ImprimirMenu ( idioma );
		scanf( "%c", &opt);
	}

	return 0;
}

//____________________FUNCIONES____________________

//____________________ImprimirCabecera____________________
//Esta función imprime la cabecera del tetris
void ImprimirCabecera(void){

	printf("\n\n\n                                                           @@@@@@    ");     
    printf("\n        @@@@  @@@@@@  @@@@  @@@@@@@  @@  @@@@@@            @@@@@@        ");
    printf("\n       @@@@@@ @@@@@@ @@@@@@ @@@@@@@  @@  @@@@@@            @@@@@@        ");
    printf("\n         @@   @@@      @@   @@@ @@@  @@  @@@               @@@@@@        ");
    printf("\n         @@   @@@@@@   @@   @@@ @@@  @@  @@@@@@                          ");
    printf("\n         @@   @@@@@@   @@   @@@@@@@  @@  @@@@@@    @@@@@@  @@@@@@  @@@@@@");
    printf("\n         @@   @@@      @@   @@@@@@   @@     @@@    @@@@@@  @@@@@@  @@@@@@");
    printf("\n         @@   @@@@@@   @@   @@  @@@  @@  @@@@@@    @@@@@@  @@@@@@  @@@@@@");
    printf("\n         @@   @@@@@@   @@   @@   @@@ @@  @@@@@@    @@@@@@  @@@@@@  @@@@@@");
    printf("\n\n");
    printf("\n           @@@@@@  @@@@     @@@     @@@@@@                 @@@@@@  @@@@@@");
    printf("\n           @@@@@@  @@@@@    @@@     @@@@@@                 @@@@@@  @@@@@@");
    printf("\n           @@@     @@@@@@   @@@     @@@                    @@@@@@  @@@@@@");
    printf("\n           @@@@@@  @@@ @@@  @@@     @@@                    @@@@@@  @@@@@@");
    printf("\n           @@@@@@  @@@  @@@ @@@     @@@");
    printf("\n           @@@     @@@   @@@@@@     @@@            @@@@@@  @@@@@@ ");
    printf("\n           @@@@@@  @@@    @@@@@     @@@@@@         @@@@@@  @@@@@@");
    printf("\n           @@@@@@  @@@     @@@@     @@@@@@         @@@@@@  @@@@@@");
    printf("\n                                                   @@@@@@  @@@@@@\n\n");     
	printf("             ALL RIGHTS RESERVED, COPYRIGHT © 2013 DN4M45T3R. \n\n");

	return;
}

//____________________Cargando_____________________
//esta funcion genera una animacion que simula la carga del juego

void Cargando (){

    ImprimirCabecera();
    printf("Cargando");
    Sleep(0500);
    system("cls");
    ImprimirCabecera();
    printf("Cargando.");
    Sleep(0500);
    system("cls");
    ImprimirCabecera();
    printf("Cargando..");
    Sleep(0500);
    system("cls");
    ImprimirCabecera ();
    printf("Cargando...");
    Sleep(0500);
    system("cls");
    ImprimirCabecera ();
    printf("Cargando....");
    Sleep(0500);
    system("cls");
    ImprimirCabecera ();
    printf("Cargando....Listo!\n");
    Sleep (2000);
             
    return;
}


//____________________SeleccionIdioma____________________
//Esta función te permite elegir el idioma en el que se mostrará el juego. 
int SeleccionIdioma (){
	int f, c, idioma = 3, tecla;
	char selector_idioma[19][24];
	
	system("cls");
	CrearSelector ( selector_idioma, idioma );
	MostrarSelector ( selector_idioma );
	
	tecla = getche();

	while ( tecla != ENTER ){

		while( tecla != FLECHA1 && tecla != ENTER){
			printf ( "\a" );
			system ( "cls" );
			MostrarSelector ( selector_idioma );
			tecla = getche();
		}

		if ( tecla == ENTER ) return idioma;
		if ( tecla == FLECHA1 ) tecla = getche();

		switch (tecla){
		case KEY_UP:
		case KEY_LEFT:
			idioma--;
			if ( idioma < 1 ) idioma = 3;
			CrearSelector ( selector_idioma, idioma );
			break;
		case KEY_DOWN:
		case KEY_RIGHT: 
			idioma++;
			if ( idioma > 3 ) idioma = 1;
			CrearSelector ( selector_idioma, idioma );
			break;
		}

		system( "cls" );
		MostrarSelector ( selector_idioma );
		tecla = getche();
	}

	return idioma;
}

//____________________CrearSelector____________________
//Esta función crea el selector de idioma
void CrearSelector ( char selector_idioma[19][24], int idioma ){
	int f, c;
	char titulo[] = "  Select your lenguaje  ";
	char idioma1[] = "        English         ";
	char idioma2[] = "        Galego          ";
	char idioma3[] = "        Espanol         ";

	for ( f=0; f<19; f++){
		for ( c=0; c<24; c++){
			selector_idioma[f][c] = ' ';
		}
	}

	for ( c=0; c<24; c++){
		selector_idioma[2][c] = titulo[c];
		selector_idioma[7][c] = idioma1[c];
		selector_idioma[11][c] = idioma2[c];
		selector_idioma[15][c] = idioma3[c];
	}
	
	for ( f=0; f<19; f++){
		for ( c=0; c<24; c++){
			if ( f==0 || f==4 || f==18 || c==0 || c==23) selector_idioma[f][c] = 'H';
		}
	}

	switch (idioma){
	case 1: 
		for ( f=5; f<10; f++){
			for ( c=1; c<23; c++){
				if ( c==1 || c==22) selector_idioma[f][c] = '*';
				if ( f==5 || f==9 ) selector_idioma[f][c] = '*';
			}
		}
		break;
	case 2: 
		for ( f=9; f<14; f++){
			for ( c=1; c<23; c++){
				if ( c==1 || c==22) selector_idioma[f][c] = '*';
				if ( f==9 || f==13 ) selector_idioma[f][c] = '*';
			}
		}
		break;
	case 3: 
		for ( f=13; f<18; f++){
			for ( c=1; c<23; c++){
				if ( c==1 || c==22) selector_idioma[f][c] = '*';
				if ( f==13 || f==17 ) selector_idioma[f][c] = '*';
			}
		}
		break;
	}

	return;
}

//____________________MostrarSelector____________________
//Esta función recibe el selector de idioma y lo muestra por pantalla
void MostrarSelector (char selector_idioma[19][24]){
	int f,c;
	printf( "\n\n\n\n\n\n\n\n\n\n\n\n\n" );
	for ( f=0; f<19; f++){
		printf( "\t\t\t     " );
		for ( c=0; c<24; c++){
			printf("%c", selector_idioma[f][c]);
		}
		printf("\n");
	}
	return;
}

//____________________CrearCaja____________________
//Esta función crea la caja "bidimensional" 
void CrearCaja ( char m[i][j] ){
	int f,c;
	for( f=0; f<i; f++ ){
		for( c=0; c<j; c++ ){
			m[f][c] = ' ';
			if( c==0 || c==11 ) m[f][c] = '|';
			if( f==20 ) m[f][c]= '=';
		}
	}
	return;
}

//____________________ImprimirMatriz____________________
//Esta función recibe una matriz de dimensiones ixj como parámetro y la muestra por pantalla
void ImprimirMatriz ( char m[i][j] ){
	int f,c;
	printf( "\n\n\n" );//Tabula verticalmente la caja
	for( f=0; f<i; f++ ){
		printf( "\t\t\t\t " );//Tabula horizontalmente la caja
		for( c=0; c<j; c++ ){
			printf( "%c", m[f][c] );
		}
		printf( "\n" );
	}
	return;
}

//____________________ImprimirMenu____________________
//Esta función muestra en la pantalla el menú principal del juego
void ImprimirMenu ( int idioma ){
	switch(idioma){
	case 1:
		printf( "\n\n\n\t\t  Tetris:" );
		printf( "\n\n\t\t\t  1. Start new game." );
		printf( "\n\t\t\t  2. Load game." );
		printf( "\n\t\t\t  3. Save game." );
		printf( "\n\t\t\t  4. Exit.\n" );
		printf( "\n\t\t  Please, choose your option and press ENTER: " );
		break;
	case 2:
		printf( "\n\n\n\t\t  Tetris:" );
		printf( "\n\n\t\t\t  1. Comezar novo xogo." );
		printf( "\n\t\t\t  2. Recuperar xogo gardado." );
		printf( "\n\t\t\t  3. Gardar xogo." );
		printf( "\n\t\t\t  4. Sair do xogo.\n" );
		printf( "\n\t\t  Por favor, introduza a opcion desexada e prema ENTER: " );
		break;
	case 3:
		printf( "\n\n\n\t\t  Juego Tetris:" );
		printf( "\n\n\t\t\t  1. Iniciar nuevo juego." );
		printf( "\n\t\t\t  2. Recuperar juego guardado." );
		printf( "\n\t\t\t  3. Guardar juego." );
		printf( "\n\t\t\t  4. Salir del juego.\n" );
		printf( "\n\t\t  Por favor, introduzca la opcion deseada y presione ENTER: " );
		break;
	}
	return;
}

//____________________Jugar____________________
/*Esta función se encarga de llamar y coordinar a todas las funciones necesarias para que se pueda llevar a cabo una
partida.*/
void Jugar ( char caja[i][j], int coordenadas[4][2], int *ptr_numero_pieza, int has_cargado, int *ptr_variable_rotacion, int *ptr_puntos, int idioma){

	int pieza_nueva, caja_saturada = 0; // nueva_pieza indica si hay que crear una pieza nueva(1) ó no(0), y caja_saturada si se ha llenado la caja(1) ó no (0).
	int numero_pieza = *ptr_numero_pieza, variable_rotacion = *ptr_variable_rotacion, puntos = *ptr_puntos;
	char opcion;
	time_t t1, t2;
	float t;
	int saltar = 0;

	pieza_nueva = has_cargado ? 0:1; //Si no hemos cargado una partida habrá que crear una pieza nueva.

	if ( pieza_nueva == 1 ) numero_pieza = CrearPieza ( caja, coordenadas, &caja_saturada, &variable_rotacion );

	system ( "cls" );
	ImprimirMatriz ( caja );
	ImprimirMarcador ( puntos );
	ImprimirMenuJugar ( idioma );

	if ( caja_saturada == 1 ){
		JuegoTerminado ( idioma );
		return;
	}

	opcion = getche (); //Esta función recoge la tecla pulsada sin necesidad de pulsar ENTER

	while( opcion != 'm' && opcion != 'M'){
		//Pedir que introduzca una opción mientras esta no sea válida
		while( opcion != FLECHA && opcion != 'm' && opcion != 'M'){
			printf ( "\a" );
			system( "cls" );
			ImprimirMatriz ( caja );
			ImprimirMarcador ( puntos );
			ImprimirMenuJugar ( idioma );
			opcion = getche ();
		}

		if( opcion == 'm' || opcion == 'M') return;
		if( opcion == FLECHA ) opcion = getche();

		if ( opcion == KEY_DOWN ){
			time ( &t1 );
			time ( &t2 );
			t = (float) t2 - t1;

			while ( t < 0.1 ){
				if ( kbhit() ){
					opcion = getche ();
					if ( opcion == KEY_DOWN ){
						while ( pieza_nueva != 1){
							pieza_nueva = MoverPieza ( caja, coordenadas, numero_pieza, &variable_rotacion, opcion);
							saltar = 1;
						}
					}
				}
				time ( &t2 );
				t = (int) t2-t1;
			}
			
		}
		
		if (saltar == 0) pieza_nueva = MoverPieza ( caja, coordenadas, numero_pieza, &variable_rotacion, opcion);

		if( pieza_nueva == 1 ){ 
			Puntuacion( caja, &puntos );
			numero_pieza = CrearPieza ( caja, coordenadas, &caja_saturada, &variable_rotacion );
		}

		if ( caja_saturada == 1 ){
			JuegoTerminado ( idioma );
			return;
		}

		system( "cls" );
		ImprimirMatriz ( caja );
		ImprimirMarcador ( puntos );
		ImprimirMenuJugar ( idioma );
		opcion=getche();//Esta función no necesita que se pulse ENTER para reconocer la tecla que pulsamos
		saltar = 0;
	}

	//Actualizamos el contenido de las posiciones señaladas por los punteros
	*ptr_numero_pieza = numero_pieza;
	*ptr_variable_rotacion = variable_rotacion;
	*ptr_puntos = puntos;
	return;
}

//____________________CrearPieza____________________
/* Esta función comprueba si es posible crear una pieza nueva, de ser así, la crea y devuelve el número
asignado a esta (que se utiliza para identificar que tipo de pieza se está moviendo). En caso contrario
devuelve a través de caja_saturada un 1, indicando de este modo que la caja está llena y no es posible
introducir más piezas. Además, se encarga de inicializar la variable de rotación a 1.*/
int CrearPieza ( char caja[i][j], int coordenadas[4][2], int *ptr_caja_saturada, int *ptr_variable_rotacion ){

	int numero_pieza;

	*ptr_variable_rotacion = 1;

	numero_pieza = rand() % (3) + 1; //Elegimos un número de pieza aleatoriamente rand() % (N-M+1)+M -> número aleatorio entre M y N

	switch ( numero_pieza ){
	case 1:  
		if( caja[0][4] != ' ' || caja[0][5] != ' ' || caja[0][6] != ' ' || caja[0][7] != ' ' ){
			*ptr_caja_saturada = 1;
			return numero_pieza;
		}
		else{
			caja[0][4] = '%'; caja[0][5] = '%'; caja[0][6] = '%'; caja[0][7] = '%';
			coordenadas[0][0] = 0; coordenadas[0][1] = 4; 
			coordenadas[1][0] = 0; coordenadas[1][1] = 5;
			coordenadas[2][0] = 0; coordenadas[2][1] = 6;
			coordenadas[3][0] = 0; coordenadas[3][1] = 7;
		}
		break;
	case 2: 
		if(caja[0][4] != ' ' || caja[0][5] != ' ' || caja[0][6] != ' ' || caja[1][4] != ' '){
			*ptr_caja_saturada = 1;
			return numero_pieza;
		}
		else{
			caja[0][4] = '#'; caja[0][5] = '#'; caja[0][6] = '#'; caja[1][4] = '#';
			coordenadas[0][0] = 0; coordenadas[0][1] = 4; 
			coordenadas[1][0] = 0; coordenadas[1][1] = 5;
			coordenadas[2][0] = 0; coordenadas[2][1] = 6;
			coordenadas[3][0] = 1; coordenadas[3][1] = 4;
		}
		break;
	case 3: 
		if(caja[0][4] != ' ' || caja[0][5] != ' ' || caja[1][4] != ' ' || caja[1][5] != ' '){
			*ptr_caja_saturada = 1;
			return numero_pieza;
		}
		else{
			caja[0][4] = 'O'; caja[0][5] = 'O'; caja[1][4] = 'O'; caja[1][5] = 'O';
			coordenadas[0][0] = 0; coordenadas[0][1] = 4; 
			coordenadas[1][0] = 0; coordenadas[1][1] = 5;
			coordenadas[2][0] = 1; coordenadas[2][1] = 4;
			coordenadas[3][0] = 1; coordenadas[3][1] = 5;
		}
		break;
	}

	return numero_pieza;
}

//____________________ImprimirMarcador____________________
//Esta función muestra por pantalla un marcador con la puntuación del jugador
void ImprimirMarcador ( int puntos ){

	int num;
	printf( "\n\n" ); //Tabula verticalmente el marcador
	printf( "\t\t\t\t    " ); //Tabula horizontalmente el marcador
    for(num=0; num<(puntos/10)+5;num++) printf("*");
	printf("\n\t\t\t\t    *");for(num=0; num<(puntos/10)+3;num++) printf(" "); printf("*");
    printf("\n\t\t\t\t    * %d *\n", puntos);
	printf("\t\t\t\t    *");for(num=0; num<(puntos/10)+3;num++) printf(" "); printf("*\n");
	printf( "\t\t\t\t    " ); //Tabula horizontalmente el marcador
    for(num=0; num<(puntos/10)+5;num++) printf("*");
	printf( "\n\n" ); //Tabula verticalmente el marcador

	return;
}

//____________________ImprimirMenuJugar____________________
//Esta función muestra por pantalla las diferentes opciones a elegir dentro de una partida
void ImprimirMenuJugar( int idioma ){
	switch ( idioma ){
	case 1:
		printf ( "\n\t\t\tTO PLAY PRESS AN <arrow>: \n" );
		printf ( "\n\t\t\t   Move to the RIGH: RIGHT key" );
		printf ( "\n\t\t\t   Move to the LEFT: LEFT key" );
		printf ( "\n\t\t\t   Move DOWN: DOWN key" );
		printf ( "\n\t\t\t   Turn AROUND: UP key\n" );
		printf ( "\n\t\t\tTO RETURN TO THE MENU, PRESS m o M " );
		break;
	case 2: 
		printf ( "\n\t\t\tPARA XOGAR PREMA UNHA <frecha>: \n" );
		printf ( "\n\t\t\t   Desprazar DEREITA: frecha DEREITA" );
		printf ( "\n\t\t\t   Desprazar ESQUERDA: frecha ESQUERDA" );
		printf ( "\n\t\t\t   Desprazar ABAIXO: frecha ABAIXO" );
		printf ( "\n\t\t\t   Virar: frecha ARRIBA\n" );
		printf ( "\n\t\t\tPARA TORNAR AO MENU, PREMER m o M " );
		break;
	case 3:
		printf ( "\n\t\t\tPARA JUGAR PULSAR UNA <flecha>: \n" );
		printf ( "\n\t\t\t   Desplazar DCHA: flecha DCHA" );
		printf ( "\n\t\t\t   Desplazar IZDA: flecha IZDA" );
		printf ( "\n\t\t\t   Desplazar ABAJO: flecha ABAJO" );
		printf ( "\n\t\t\t   Girar: flecha ARRIBA\n" );
		printf ( "\n\t\t\tPARA VOLVER AL MENU, PULSAR m o M " );
		break;
	}
	return;
}

//____________________JuegoTerminado____________________
//Esta función muestra por pantalla un mensaje que indica que la partida ha terminado
void JuegoTerminado ( int idioma ){
	system("cls");
	switch(idioma){
	case 1:
		printf("\n");
		printf("\n");     
		printf("\t@@@@@@@@@@  @@@@@@@@@  @@@@@@@@@@@@@  @@@@@@@@@@                    \n");
		printf("\t@@@@@@@@@@  @@@@@@@@@  @@@@@@@@@@@@@  @@@@@@@@@@                    \n");
		printf("\t@@@         @@@   @@@  @@@  @@@  @@@  @@@             @@@    @@@    \n");
		printf("\t@@@   @@@@  @@@   @@@  @@@  @@@  @@@  @@@@@@          @@@    @@@    \n");
		printf("\t@@@   @@@@  @@@@@@@@@  @@@  @@@  @@@  @@@@@@          @@@    @@@    \n");
		printf("\t@@@    @@@  @@@@@@@@@  @@@       @@@  @@@             @@@    @@@    \n");
		printf("\t@@@@@@@@@@  @@@   @@@  @@@       @@@  @@@@@@@@@@      @@@    @@@    \n");
		printf("\t@@@@@@@@@@  @@@   @@@  @@@       @@@  @@@@@@@@@@      @@@    @@@    \n");
		printf("\n");
		printf("\t@@@@@@@@@@ @@@     @@@ @@@@@@@@@@ @@@@@@@@@            @@@@@@@@     \n");
		printf("\t@@@@@@@@@@ @@@     @@@ @@@@@@@@@@ @@@@@@@@@@          @@@@@@@@@@    \n");
		printf("\t@@@    @@@  @@@   @@@  @@@        @@@    @@@         @@@      @@@   \n");
		printf("\t@@@    @@@  @@@   @@@  @@@@@@     @@@    @@@        @@@        @@@  \n");
		printf("\t@@@    @@@   @@@ @@@   @@@@@@     @@@@@@@@@@       @@@          @@@ \n");
		printf("\t@@@    @@@   @@@ @@@   @@@        @@@@@@@@@       @@@            @@@\n");
		printf("\t@@@@@@@@@@    @@@@@    @@@@@@@@@@ @@@   @@@                         \n");
		printf("\t@@@@@@@@@@    @@@@@    @@@@@@@@@@ @@@    @@@                        \n");
		printf("\n\n\n\n");
		break;
	case 2:
		printf("\n");
		printf("\n");    
		printf(" @@@@@@@@  @@@@@@@@ @@@@@@@  @@@@@@@  @@@ @@@@@@@@ @@@   @@@ @@@@@@@@ @@@@@@@@ \n");
		printf(" @@@@@@@@@ @@@@@@@@ @@@@@@@@ @@@@@@@@ @@@ @@@@@@@@ @@@   @@@ @@@@@@@@ @@@@@@@@ \n");
		printf(" @@@   @@@ @@@      @@@  @@@ @@@  @@@ @@@ @@@      @@@   @@@ @@@      @@@      \n");
		printf(" @@@   @@@ @@@@@@   @@@  @@@ @@@  @@@ @@@ @@@      @@@@@@@@@ @@@@@@   @@@@@@@@ \n");
		printf(" @@@@@@@@@ @@@@@@   @@@@@@@@ @@@  @@@ @@@ @@@      @@@@@@@@@ @@@@@@   @@@@@@@@ \n");
		printf(" @@@@@@@@  @@@      @@@@@@@  @@@  @@@ @@@ @@@      @@@   @@@ @@@           @@@ \n");
		printf(" @@@       @@@@@@@@ @@@ @@@  @@@@@@@@ @@@ @@@@@@@@ @@@   @@@ @@@@@@@@ @@@@@@@@ \n");
		printf(" @@@       @@@@@@@@ @@@  @@@ @@@@@@@  @@@ @@@@@@@@ @@@   @@@ @@@@@@@@ @@@@@@@@ \n");
		printf("\n");
		printf("                                         @@@@@\n");
		printf("                          @@@@@@@@     @@@@@@@\n");
		printf("                          @@@@@@@@   @@@@\n");
		printf("                                     @@\n");
		printf("                          @@@@@@@@   @@@@\n");
		printf("                          @@@@@@@@     @@@@@@@\n");
		printf("                                         @@@@@\n");
		printf("\n");
		printf("\n\n\n\n");
		break;
	case 3:
		printf("\n");
		printf("\n");    
		printf("       @@@    @@@  @@@@@@@@@  @@@@@@@@@@           \n");
		printf("       @@@    @@@  @@@@@@@@@  @@@@@@@@@@                    @@@@@\n");
		printf("       @@@    @@@  @@@   @@@  @@@            @@@@@@@@     @@@@@@@\n");
		printf("       @@@@@@@@@@  @@@   @@@  @@@@@@@@@@     @@@@@@@@   @@@@\n");
		printf("       @@@@@@@@@@  @@@@@@@@@  @@@@@@@@@@                @@\n");
		printf("       @@@    @@@  @@@@@@@@@         @@@     @@@@@@@@   @@@@\n");
		printf("       @@@    @@@  @@@   @@@  @@@@@@@@@@     @@@@@@@@     @@@@@@@\n");
		printf("       @@@    @@@  @@@   @@@  @@@@@@@@@@                    @@@@@\n");
		printf("\n");
		printf("       @@@@@@@@@  @@@@@@@@@@ @@@@@@@@@  @@@@@@@@@  @@@ @@@@@@@@@  @@@@@@@@@\n");
		printf("       @@@@@@@@@@ @@@@@@@@@@ @@@@@@@@@@ @@@@@@@@@@ @@@ @@@@@@@@@@ @@@@@@@@@\n");
		printf("       @@@    @@@ @@@        @@@    @@@ @@@    @@@ @@@ @@@    @@@ @@@   @@@\n");
		printf("       @@@    @@@ @@@@@@     @@@    @@@ @@@    @@@ @@@ @@@    @@@ @@@   @@@\n");
		printf("       @@@@@@@@@@ @@@@@@     @@@@@@@@@@ @@@    @@@ @@@ @@@    @@@ @@@   @@@\n");
		printf("       @@@@@@@@@  @@@        @@@@@@@@@  @@@    @@@ @@@ @@@    @@@ @@@   @@@\n");
		printf("       @@@        @@@@@@@@@@ @@@   @@@  @@@@@@@@@@ @@@ @@@@@@@@@@ @@@@@@@@@\n");
		printf("       @@@        @@@@@@@@@@ @@@    @@@ @@@@@@@@@  @@@ @@@@@@@@@  @@@@@@@@@\n");
		printf("\n\n\n\n");
		break;
	}
	Sleep(3000);
	return;
}

//____________________MoverPieza____________________
/*Esta función se encarga del desplazamiento de las piezas por la caja "bidimensional". En el momento que no es 
posible continuar moviendo, ya sea por llegar al final de la caja o por colisionar contra otra pieza, la función devuelve
un 1 para que se cree una nueva pieza.*/
int MoverPieza ( char caja[i][j], int coordenadas[4][2], int numero_pieza, int *ptr_variable_rotacion, char movimiento){
	
	int variable_rotacion = *ptr_variable_rotacion;
	int has_rotado = 0, mov_posible;

	//Creamos pares de puntos (x,y) que almacenan las coordenadas de los elementos de la figura que vamos a mover
	int x1 = coordenadas[0][0], x2 = coordenadas[1][0], x3 = coordenadas[2][0], x4 = coordenadas[3][0];
	int y1 = coordenadas[0][1], y2 = coordenadas[1][1], y3 = coordenadas[2][1], y4 = coordenadas[3][1];

	//Hacemos una copia de los pares de puntos (x,y)
	int xo1 = x1, xo2 = x2, xo3 = x3, xo4 = x4, yo1 = y1, yo2 = y2, yo3 = y3, yo4 = y4;

	//Reajustamos las posiciones
	switch ( movimiento ){
	case KEY_DOWN: x1++; x2++; x3++; x4++; break;
	case KEY_LEFT: y1--; y2--; y3--; y4--; break;
	case KEY_RIGHT: y1++; y2++; y3++; y4++; break;
	case KEY_UP: has_rotado = RotarPieza ( caja, coordenadas, numero_pieza, &variable_rotacion); break;	
	}

	if ( has_rotado == 1){
		//Creamos pares de puntos (x,y) que almacenan las coordenadas de la figura que acabamos de rotar
		x1 = coordenadas[0][0]; x2 = coordenadas[1][0]; x3 = coordenadas[2][0]; x4 = coordenadas[3][0];
		y1 = coordenadas[0][1]; y2 = coordenadas[1][1]; y3 = coordenadas[2][1]; y4 = coordenadas[3][1];
	}

	//Comprobamos que la nueva posición no es una pared
	if(caja[x1][y1] == '|' || caja[x2][y2] == '|' || caja[x3][y3] == '|' || caja[x4][y4] == '|'){
		printf("\a"); 
		return 0;
	}

	//Si en la nueva posición hay otra pieza la función devuelve un 1 para que se cree una pieza nueva
	if ( PerimetroPieza (caja, coordenadas, x1, y1, x2, y2, x3, y3, x4, y4) == 1) return 1;

	//Limpiamos las posiciones viejas
	caja[xo1][yo1] = ' '; caja[xo2][yo2] = ' '; caja[xo3][yo3] = ' '; caja[xo4][yo4] = ' ';

	//Actualizamos la caja
	switch(numero_pieza){
	case 1: caja[x1][y1] = '%'; caja[x2][y2] = '%'; caja[x3][y3] = '%'; caja[x4][y4] = '%'; break;
	case 2: caja[x1][y1] = '#'; caja[x2][y2] = '#'; caja[x3][y3] = '#'; caja[x4][y4] = '#'; break;
	case 3: caja[x1][y1] = 'O'; caja[x2][y2] = 'O'; caja[x3][y3] = 'O'; caja[x4][y4] = 'O'; break;
	}

	//Actualizamos el contenido de la posicion señalada por el puntero ptr_variable_rotacion
	*ptr_variable_rotacion = variable_rotacion;

	//Guardamos las nuevas coordenadas
	coordenadas[0][0] = x1; coordenadas[1][0] = x2; coordenadas[2][0] = x3; coordenadas[3][0] = x4;
	coordenadas[0][1] = y1; coordenadas[1][1] = y2; coordenadas[2][1] = y3; coordenadas[3][1] = y4;

	//Si llegamos al final la función devuelve un 1 para que se cree una nueva pieza
	if( caja[x1+1][y1] == '=' || caja[x2+1][y2] == '=' || caja[x3+1][y3] == '=' || caja[x4+1][y4] == '=') return 1; 
}

//____________________RotarPieza____________________
/*Esta función es la encargada de rotar las figuras. En caso de que se haya podido rotar la figura la función 
devuelve un 1, y en caso contrario devuelve un 0.*/
int RotarPieza ( char caja[i][j], int coordenadas[4][2], int numero_pieza, int *ptr_variable_rotacion){

	int f, c, n, fmin, cmin, faux, caux, cont = 0, variable_rotacion = *ptr_variable_rotacion;
	char **reticula, **matriz_auxiliar; 

	RangoMaximo ( coordenadas, &n, &fmin, &cmin); //Calcula el tamaño de la retícula de giro

	//"Dimensionamos" dos "matrices" (arreglos de arreglos) de tamaño nxn dinámicamente
	reticula = ( char** ) malloc ( n * sizeof ( char ) );
	matriz_auxiliar = ( char** ) malloc ( n * sizeof ( char ) );

	for ( f=0; f<n ; f++ ){
		reticula[f] = ( char* ) malloc ( n * sizeof ( char ) );
		matriz_auxiliar[f] = ( char* ) malloc ( n * sizeof ( char ) );
	}

	//Creamos la retícula de giro y contamos cuantas de sus posiciones están "ocupadas"
	//NOTA: Más de cuatro posiciones ocupadas implica más de una pieza en la retícula por lo que no se puede rotar
	for ( f=0; f<n ;f++ ){
		for ( c=0; c<n; c++ ){
			reticula[f][c] = caja[f+fmin][c+cmin];
			if( reticula[f][c] != ' ' ) cont++;
			if( cont > 4 ){
				printf("\a"); 
				return 0;
			}
		}
	}

	//Copiamos la retícula de giro en una matriz auxiliar
	for ( f=0; f<n; f++ ){
		for ( c=0; c<n; c++ ){
			matriz_auxiliar[f][c] = reticula[f][c];
		}
	}

	switch(n){
	case 2: 
		return 1;
		break;
	case 3:
		faux=2;
		caux=0;

		if ( variable_rotacion == 5 ) variable_rotacion = 1;

		//Rotamos la retícula 
		for ( f=0; f<n; f++){
			for ( c=0; c<n; c++){
				reticula[f][c] = matriz_auxiliar[faux][caux];
				faux--;
			}
			caux++;
			faux = 2;
		}

		if ( variable_rotacion == 1 || variable_rotacion == 3 ){
			for ( f=0; f<n; f++){
				for( c=0; c<n; c++){
					reticula[f][c] = reticula[f][c+1];
				}
			}
			for ( f=0; f<n; f++) reticula[f][2]=' ';
		}

		variable_rotacion++;
		break;
	case 4: 
		if ( variable_rotacion == 1 ){
			for ( f=0; f<n; f++){
				reticula[f][0] = '%';
				reticula[f][1] = ' ';
				reticula[f][2] = ' ';
				reticula[f][3] = ' ';
			}
			variable_rotacion++;
		}
		else{
			for ( c=0; c<n; c++){
				reticula[0][c] = '%';
				reticula[1][c] = ' ';
				reticula[2][c] = ' ';
				reticula[3][c] = ' ';
			}
			variable_rotacion--;
		}

		break;
	}//Cierra el switch()

	//Actualizamos las posiciones de la figura
	faux=0; caux=0;
	for(f=0;f<n;f++){
		for(c=0;c<n;c++){
			if( reticula[f][c] != ' ' ){
				coordenadas[faux][caux]=f+fmin;
				coordenadas[faux][caux+1]=c+cmin;
				faux++;
				caux=0;
			}
		}
	}

	*ptr_variable_rotacion = variable_rotacion;
	return 1;
}

//____________________RangoMaximo____________________
/*Esta función es la encargada de calcular el tamaño de la retícula de giro. Además, devuelve las coordenadas
del primer elemento de la figura, a partir del cual se creará la retícula.*/
void RangoMaximo ( int coordenadas[4][2], int *ptr_n, int *ptr_fmin, int *ptr_cmin ){
	//Creamos pares de puntos (x,y) que almacenan las coordenadas de los elementos de la figura que vamos a rotar
	int x1=coordenadas[0][0], x2=coordenadas[1][0], x3=coordenadas[2][0], x4=coordenadas[3][0];
	int y1=coordenadas[0][1], y2=coordenadas[1][1], y3=coordenadas[2][1], y4=coordenadas[3][1];

	int f, c, n, fmax=0, fmin, cmax=0, cmin;

	//Calculamos el rango máximo de rotación de la figura
	for(f=0;f<4;f++){
		if(coordenadas[f][0]>fmax) fmax=coordenadas[f][0];
		if(coordenadas[f][1]>cmax) cmax=coordenadas[f][1];
	}
	fmin = fmax; cmin = cmax;

	for(f=0;f<4;f++){
		if(coordenadas[f][0]<fmin) fmin=coordenadas[f][0];
		if(coordenadas[f][1]<cmin) cmin=coordenadas[f][1];
	}

	n=(fmax-fmin)>(cmax-cmin)?(fmax-fmin):(cmax-cmin); n++; // nxn --> tamaño de la retícula

	//Actualizamos el contenido de las posiciones señaladas por los punteros
	*ptr_n = n; *ptr_fmin = fmin; *ptr_cmin = cmin;
	return;
}

//____________________PerimetroPieza____________________
/*Esta función devuelve un 0 si no hay ninguna pieza en la zona a la que queremos movernos o un 1
en caso contrario.*/
int PerimetroPieza(char caja[i][j], int coordenadas[4][2], int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
	//Comprobamos que en la nueva zona no hay otra pieza
	if ( SiguientePosicion ( caja, coordenadas, x1, y1) ||
		 SiguientePosicion ( caja, coordenadas, x2, y2) ||
		 SiguientePosicion ( caja, coordenadas, x3, y3) ||
		 SiguientePosicion ( caja, coordenadas, x4, y4) ){
			 return 1; 
	}
	return 0;
}

//____________________SiguientePosicion____________________
/*Esta función analiza si en la posición a donde vas a desplazar uno de los cuatro elementos del tetramino hay un elemento 
de otro tetramino. En caso de haberlo devuelve un 1, y en caso contrario un 0.*/
int SiguientePosicion(char caja[i][j], int coordenadas[4][2], int x, int y){
	//Creamos pares de puntos (x,y) que almacenan las coordenadas de los elementos de la figura que estamos moviendo
	int xo1=coordenadas[0][0], xo2=coordenadas[1][0], xo3=coordenadas[2][0], xo4=coordenadas[3][0];
	int yo1=coordenadas[0][1], yo2=coordenadas[1][1], yo3=coordenadas[2][1], yo4=coordenadas[3][1];

	/*Comprueba si el nuevo punto ( x, y ) cae en un punto de la figuara ( xo, yo ) o en uno fuera de la figura.
	Si cae fuera, comprueba si pertenece o no a otra figura. En caso de que pertenezca a otra figura devuelve un 1,
	en caso contrario devuelve un 0.*/
	if( ! ( ( x == xo1 && y == yo1 ) || (x == xo2 && y == yo2) || (x == xo3 && y == yo3) || (x == xo4 && y == yo4) ) ){
			if(caja[x][y] == '%' || caja[x][y] == 'O' || caja[x][y] == '#') return 1;
	}
	return 0;
}

//____________________Puntuacion____________________
/*Esta función actualiza los puntos, borra las líneas completadas, y reajusta el contenido de la matriz tras borrar
las líneas.*/
void Puntuacion( char caja[i][j], int *ptr_puntos){
	int f, c, cont, cont2 = 0, f2;

	*ptr_puntos;

	for ( f=0; f<i-1; f++){
		cont = 0;
		for ( c=0; c<j; c++){
			if ( caja[f][c] != ' ' ) cont++;
		}
		if( cont == 12 ){
			*ptr_puntos = *ptr_puntos + 1;
			cont2++; //Contamos el número de filas que vamos a tener que actualizar 

			//Quitamos las filas llenas
			for( f2 = f; 0<=f2; f2-- ){
				for( c=0; c<j; c++){
					if( c>0 && c<11) caja[f2][c] = caja[f2-1][c];
				}
			}

		}
	}

	for ( f=0; f<cont2; f++){
		for ( c=1; c<j-1; c++){
			caja[f][c] = ' ';
		}
	}
	return;
}

//____________________GuardarJuego____________________
//Esta función se encarga de guardar la partida
void GuardarJuego ( char caja[i][j], int coordenadas[4][2], int numero_pieza, int puntos, int variable_rotacion ){
	int f,c;

	FILE *ptr_partida_guardada;
	ptr_partida_guardada = fopen ( "save.txt", "w" );

	if ( ptr_partida_guardada == NULL ){
		system ( "cls" );
		printf("\a\n  ERROR 302\n");
		Sleep(2500); // Se hace un retardo de 2,5 segundos (expresados en milisegundos).
		return;
	}
	else{
		//Guardamos la caja
		for ( f=0; f<i; f++){
			for ( c=0; c<j; c++){
				fprintf ( ptr_partida_guardada, "%c", caja[f][c]);
			}
		}

		//Guardamos las posiciones de la figura que estamos moviendo
		for ( f=0; f<4; f++){
			for ( c=0; c<2; c++){
				fprintf ( ptr_partida_guardada, "%d ", coordenadas[f][c]);
			}
		}
	
		//Guardamos el tipo de figura que estamos moviendo
		fprintf ( ptr_partida_guardada, "%d ", numero_pieza);
	
		//Guardamos la variable de rotación
		fprintf ( ptr_partida_guardada, "%d ", variable_rotacion);

		//Guardamos los puntos
		fprintf ( ptr_partida_guardada, "%d ", puntos);
	}
	fclose(ptr_partida_guardada);
	return;
}

//____________________CargarJuego____________________
/*Esta función se encarga de cargar una partida guardada. En caso de que se produzca algún tipo de error
al cargar la partida, la función devuelve un 1 (0 en otro caso).*/
int CargarJuego(  char caja[i][j], int coordenadas[4][2], int *ptr_numero_pieza, int *ptr_puntos, int *ptr_variable_rotacion ){
	int f,c;

	FILE *ptr_partida_guardada;
	ptr_partida_guardada = fopen ( "save.txt", "r" );

	if ( ptr_partida_guardada == NULL ){
		system ( "cls" );
		printf("\a\n  ERROR 303\n");
		Sleep(2500); // Se hace un retardo de 2,5 segundos (expresados en milisegundos).
		return 1;
	}
	else{
		//Cargamos la caja
		for ( f=0; f<i; f++){
			for ( c=0; c<j; c++){
				fscanf ( ptr_partida_guardada, "%c", &caja[f][c]);
			}
		}

		//Cargamos las posiciones de la figura que estamos moviendo
		for ( f=0; f<4; f++){
			for ( c=0; c<2; c++){
				fscanf ( ptr_partida_guardada, "%d ", &coordenadas[f][c]);
			}
		}
	
		//Cargamos el tipo de figura que estamos moviendo
		fscanf ( ptr_partida_guardada, "%d ", ptr_numero_pieza);

		//Cargamos la variable de rotación
		fscanf ( ptr_partida_guardada, "%d ", ptr_variable_rotacion);

		//Cargamos los puntos
		fscanf ( ptr_partida_guardada, "%d ", ptr_puntos);
	}
	fclose(ptr_partida_guardada);
	return 0;
}
