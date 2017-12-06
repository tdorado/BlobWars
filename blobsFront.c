#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blobsBack.h"

#define LIMPIAPANTALLA system("clear")

#define MINFILSYCOLS 5
#define MAXFILSYCOLS 30
#define MAXNOMBREARCHIVO 35

#define JUGADORUNO 1
#define JUGADORDOS 2

enum{JUGADORvsJUGADOR=1, JUGADORvsCOMPUTADORA, RECUPERAR, SALIR};

int menuJuego(tipoPartida *partida);
void cantFilsyCols(tipoPartida *partida);
int generaPartida(tipoPartida *partida);
int jugar(tipoPartida *partida);
void imprimeTablero(const tipoPartida *partida);
int leeIngresado(tipoPartida *partida);
int movimiento(tipoPartida *partida);
void imprimeGanador(const tipoPartida *partida);
void leeNumero(int *numero, int desdeN, int hastaN);
void leeNombre(tipoPartida *partida);
int siOno();

int main(void){
	tipoPartida partida;
	partida.s=malloc(MAXNOMBREARCHIVO);
	srand(time(NULL));
	LIMPIAPANTALLA;
	printf("\n¡¡Bienvenido a Blob Wars!!\n");
	while(menuJuego(&partida)==1);
	printf("\n¡Hasta luego!\n\n");
	free(partida.s);
}

int menuJuego(tipoPartida *partida){
	int opcion, flagDeExit=1, flagDeRecuperar=1, flagJuegaDeNuevo=0,\
	flagDeError=0;\

	printf("\n1. Juego de dos jugadores\n");
	printf("2. Juego contra computadora\n");
	printf("3. Recuperar un juego grabado\n");
	printf("4. Terminar\n\n");
	printf("Elegir opcion: ");

	leeNumero(&opcion, 1, 4);

	while(flagDeExit==1 && flagDeError==0){
		switch(opcion){
			case JUGADORvsJUGADOR:
				(*partida).modojuego=0;
				flagDeError=generaPartida(partida);
				if(flagDeError==1)
					printf("Error al intentar generar la partida.\n");
				else{
					flagDeExit=jugar(partida);
					if(flagDeExit==0){
						printf("¿Desea jugar denuevo?\n");
						flagJuegaDeNuevo=siOno();
						liberaTablero(partida);
					}
				}
			break;	
			case JUGADORvsCOMPUTADORA:
				(*partida).modojuego=1;
				flagDeError=generaPartida(partida);
				if(flagDeError==1)
					printf("Error al intentar generar la partida.\n");
				else{
					flagDeExit=jugar(partida);
					if(flagDeExit==0){
						printf("¿Desea jugar denuevo?\n");
						flagJuegaDeNuevo=siOno();
						liberaTablero(partida);
					}
				}
			break;
			case RECUPERAR:
				printf("Ingrese el nombre del archivo a recuperar");
				printf("(maximo 34 caracteres): ");

				leeNombre(partida);

				flagDeRecuperar=recuperarPartida(partida);
				if(flagDeRecuperar==1){
					printf("Partida recuperada con exito.\n");
					flagDeExit=jugar(partida);
				}
				else if(flagDeRecuperar==0){
					printf("Error al cargar");
					printf("(El archivo esta corrupto o no existe).\n");
					printf("¿Desea ingresar denuevo?\n");
					if(!siOno())
						opcion=SALIR;
				}
				if(flagDeExit==0){
					printf("¿Desea jugar denuevo?\n");
					flagJuegaDeNuevo=siOno();
					liberaTablero(partida);
				}
			break;
			case SALIR:
				flagDeExit=0;
				flagJuegaDeNuevo=0;
			break;
		}
	}
	return flagJuegaDeNuevo;
}

void cantFilsyCols(tipoPartida *partida){

	printf("Ingrese la cantidad de filas(Minimo 5 y Maximo 30): ");
	leeNumero(&(*partida).filas, MINFILSYCOLS, MAXFILSYCOLS);
	printf("Ingrese la cantidad de columnas(Minimo 5 y Maximo 30): ");
	leeNumero(&(*partida).columnas, MINFILSYCOLS, MAXFILSYCOLS);
	
	return;
}

int generaPartida(tipoPartida *partida){
	int flagDeError;

	(*partida).manchasA=2;
	(*partida).manchasZ=2;
	if((*partida).modojuego==0)
		turnoAleatorio(partida);
	else
		(*partida).turno=JUGADORUNO;
	cantFilsyCols(partida);
	flagDeError=creaTablero(partida);
	//retorna 1 si pudo encontrar memoria para el tablero, 0 si no
	return flagDeError;
}

int jugar(tipoPartida *partida){
	int flagTermina=1, flagTerminaAntes=LEE_Y_MUEVE;

	//JUGADOR VS JUGADOR
	if((*partida).modojuego==0){
		while(flagTermina==1 && (flagTerminaAntes==LEE_Y_MUEVE ||\
		flagTerminaAntes==GUARDA_PARTIDA))\
		{
			switch((*partida).turno){
				case JUGADORUNO:
					imprimeTablero(partida);
					flagTerminaAntes=movimiento(partida);
					(*partida).turno=JUGADORDOS;
					flagTermina=buscaLugar(partida);
					if(flagTermina!=1){
						llenaLugares(partida);
						imprimeTablero(partida);
						imprimeGanador(partida);
					}
				break;
				case JUGADORDOS:
					imprimeTablero(partida);
					flagTerminaAntes=movimiento(partida);
					(*partida).turno=JUGADORUNO;
					flagTermina=buscaLugar(partida);
					if(flagTermina!=1){
						llenaLugares(partida);
						imprimeTablero(partida);
						imprimeGanador(partida);
					}
				break;
			}
		}
	}

	//JUGADOR VS COMPUTADORA
	else if((*partida).modojuego==1){
		imprimeTablero(partida);
		while(flagTermina==1 && (flagTerminaAntes==LEE_Y_MUEVE ||\
		flagTerminaAntes==GUARDA_PARTIDA))\
		{
			switch((*partida).turno){
				case JUGADORUNO:
					flagTerminaAntes=movimiento(partida);
					flagTermina=buscaLugar(partida);
					(*partida).turno=JUGADORDOS;
					if(flagTermina!=1){
						llenaLugares(partida);
						imprimeTablero(partida);
						imprimeGanador(partida);
					}
				break;
				case JUGADORDOS:
					mueveComputadora(partida);
					(*partida).turno=JUGADORUNO;
					imprimeTablero(partida);
					printf("El movimiento anterior de la computadora ");
					printf("fue: [%d,%d][%d,%d]\n", (*partida).deFil,\
					(*partida).deCol, (*partida).aFil, (*partida).aCol);\
					flagTermina=buscaLugar(partida);
					if(flagTermina!=1){
						llenaLugares(partida);
						imprimeTablero(partida);
						printf("El movimiento anterior de la computadora ");
						printf("fue: [%d,%d][%d,%d]\n", (*partida).deFil,\
						(*partida).deCol, (*partida).aFil, (*partida).aCol);\
						imprimeGanador(partida);
					}
				break;
			}
		}
	}

	if(flagTerminaAntes==SALE_Y_GUARDA || flagTerminaAntes==SALE_SIN_GUARDAR)
		return flagTerminaAntes;
	else
		return flagTermina;
}

void imprimeTablero(const tipoPartida *partida){
	int i, j;

	LIMPIAPANTALLA;

	if((*partida).modojuego==0){
		printf("Puntaje del jugador 1(A): %d.\n", (*partida).manchasA);
		printf("Puntaje del jugador 2(Z): %d.\n\n", (*partida).manchasZ);
	}
	else if((*partida).modojuego==1){
		printf("Puntaje del jugador(A): %d.\n", (*partida).manchasA);
		printf("Puntaje de la computadora(Z): %d.\n\n", (*partida).manchasZ);
	}

	//imprimo los numeros de las columnas
	printf("       00");
	for(i=1;i<(*partida).columnas;i++)
		if(i>9)
			printf("  %d", i);
		else
			printf("  0%d", i);

	//imprimo parte superior del tablero
	printf("\n     ╔");
	for(i=1;i<(4*(*partida).columnas);i++)
		printf("%s", (i % 4)? "═" : "╦");
	printf("╗\n");


	for(i=0;i<(*partida).filas;i++){
		for(j=0;j<(*partida).columnas;j++){
			if ((*partida).tablero[i][j] == 0){
				if(j==0){
					//imprimo los numeros de las filas
					if(i>9)
						printf("  %d ║   ", i);
					else
						printf("  0%d ║   ", i);
				}
				else
					printf("║   ");
			}
			else{
				if(j==0){
					//imprimo los numeros de las filas
					if(i>9)
						printf("  %d ║ %c ", i, (*partida).tablero[i][j]);
					else
						printf("  0%d ║ %c ", i, (*partida).tablero[i][j]);
				}
				else
					printf("║ %c ", (*partida).tablero[i][j]);
			}
		}
		//imprimo lineas del tablero entre fila y fila
		printf("║\n    %s", (i!=(((*partida).filas)-1)) ? " ╠" : " ╚");
		for(j = 1; j <(4*(*partida).columnas); j++)
			if(i!=(*partida).filas-1)
				printf("%s", (j % 4)? "═" : "╬");
			else
				printf("%s", (j % 4)? "═" : "╩");
		printf("%s\n", (i!=(((*partida).filas)-1)) ? "╣" : "╝");
	}
	printf("\n");

	return;
}

int leeIngresado(tipoPartida *partida){
	int flagRepite, resultado=0;
	char c;

	do{
		if((flagRepite=scanf("[%d,%d][%d,%d", &(*partida).deFil,\
		 &(*partida).deCol, &(*partida).aFil, &(*partida).aCol)))\
		{
			//como el scanf no valida 
			//si el ultimo caracter es ']' hago lo siguiente
			c=getchar();
			if(c=='\n')
				flagRepite=0;
			else if(c!=']'){
				//limpia buffer y tira error si el ultimo caracter no es ']'
				do{
					c=getchar();
				}while(c!='\n');
				flagRepite=0;
			}
			else if(c==']'){
				if(getchar()!='\n'){
					//limpia buffer
					// y tira error si ingresa algo mas despues del ']'
					do{
						c=getchar();
					}while(c!='\n');
					flagRepite=0;
				}
			}
			if (flagRepite!=4)
				printf("Error, volver a ingresar: ");
			else
				resultado=LEE_Y_MUEVE;
		}
		else if((flagRepite=scanf("save %34s", (*partida).s))){
			if (getchar()!='\n'){
				do{
					c=getchar();
				}while(c!='\n');
				flagRepite=0;
			}
			if(flagRepite==0)
				printf("Error, volver a ingresar: ");
			else
				resultado=GUARDA_PARTIDA;
		}
		else if((flagRepite=scanf("%4s", (*partida).s))){
			if (getchar()!='\n'){
				//si se pasa de 4 caracteres limpia buffer y tira error
				do{
					c=getchar();
				}while(c!='\n');
				flagRepite=0;
			}
			if(flagRepite==0)
				printf("Error, volver a ingresar: ");
			else{
				if (!strcmp((*partida).s,"quit")){
					printf("¿Desea guardar su partida?\n");
					if(siOno()){
						printf("Ingrese el nombre del archivo");
						printf("(maximo 34 caracteres): ");
						leeNombre(partida);
						resultado=SALE_Y_GUARDA;
					}
					else
						resultado=SALE_SIN_GUARDAR;
				}
				else{
					printf("Error, volver a ingresar: ");
					flagRepite=0;
				}
			}
		}
	}while(flagRepite==0);
	return resultado;
}

int movimiento(tipoPartida *partida){
	int flagMovimiento=DISTINTOEXITO, flagGuardaPartida, resultado;

	printf("Turno jugador %d(%c).\n", (*partida).turno,\
	JUGADORALETRA((*partida).turno));\

	do{
		printf("Acciones: ");
		printf("[ff,cc][ff,cc] , ");
		printf("save filename(maximo 34 caracteres) ");
		printf("o quit\n");
		printf("Ingrese acción: ");
		resultado=leeIngresado(partida);
		if(resultado==LEE_Y_MUEVE){
			flagMovimiento=muevePosicion(partida);
			if(flagMovimiento==MOV_NO_PERMITIDO)
				printf("Error, movimiento no permitido.\n");
			else if(flagMovimiento==NO_EXISTE_POSICION)
				printf("Error, no existe la posicion.\n");
		}
		else if(resultado==GUARDA_PARTIDA){
			flagGuardaPartida=guardarPartida(partida);
			if(flagGuardaPartida==1)
				printf("Partida guardada con exito.\n");
			else if(flagGuardaPartida==0)
				printf("Error al guardar partida.\n");
		}
		else if(resultado==SALE_Y_GUARDA){
			flagGuardaPartida=guardarPartida(partida);
			if(flagGuardaPartida==1){
				printf("Partida guardada con exito.\n");
				flagMovimiento=EXITO;
			}
			else if(flagGuardaPartida==0){
				printf("Error al guardar partida.\n");
			}
		}
		else if(resultado==SALE_SIN_GUARDAR)
				flagMovimiento=EXITO;
	}while(flagMovimiento!=EXITO);

	return resultado;
}

void imprimeGanador(const tipoPartida *partida){

	if((*partida).modojuego==0){
		if((*partida).manchasA>(*partida).manchasZ){
			printf("¡Felicitaciones! el Jugador 1(A) ha ganado.\n");
		}
		else if((*partida).manchasZ>(*partida).manchasA){
			printf("¡Felicitaciones! el Jugador 2(Z) ha ganado.\n");
		}
		else if((*partida).manchasZ==(*partida).manchasA){
			printf("¡Felicitaciones la partida ha terminado en un empate!\n");
		}
	}
	else if((*partida).modojuego==1){
		if((*partida).manchasA>(*partida).manchasZ){
			printf("¡Felicitaciones! usted le ha ganado a la computadora.\n");
		}
		else if((*partida).manchasZ>(*partida).manchasA){
			printf("La computadora ha ganado.\n");
		}
		else if((*partida).manchasZ==(*partida).manchasA){
			printf("¡Felicitaciones la partida ha terminado en un empate!\n");
		}
	}

	return;
}

void leeNumero(int *numero, int desdeN, int hastaN){
	int flagCorta, resultado, auxnum;
	char aux2;

	//lee numero en intervalo que es valido lo ingresado es solamente numero
	do{
		do{
			flagCorta=scanf("%d", &auxnum);
			if(getchar()!='\n'){
				//limpia el buffer y corta apenas encuentra '\n'
				do{
					aux2=getchar();
				}while(aux2!='\n');
				flagCorta=0;
			}
			if(flagCorta!=1){
					printf("Error, volver a ingresar: ");
			}
		}while(flagCorta!=1);
		if(auxnum>=desdeN && auxnum<=hastaN){
			(*numero)=auxnum;
			resultado=1;
		}
		else{
			printf("Error, volver a ingresar: ");
			resultado=0;
		}
	}while(resultado!=1);

	return;
}

void leeNombre(tipoPartida *partida){
	int flag;
	char aux;

	do{
		flag=scanf("%34s", (*partida).s);
		if(getchar()!='\n'){
		//limpia el buffer y corta apenas encuentra '\n'
		//sirve solamente si se pasa de los 34 caracteres
			do{
				aux=getchar();
			}while(aux!='\n');
			flag=0;
		}
		if(flag!=1)
			printf("Error, volver a ingresar: ");
	}while(flag!=1);

	return;
}

int siOno(){
	int flag, resultado;
	char s[4], aux;

	do{
		do{
			printf("Ingrese si o no: ");
			flag=scanf("%3s", s);
			if(getchar()!='\n'){
			//limpia el buffer y corta apenas encuentra '\n'
				do{
					aux=getchar();
				}while(aux!='\n');
				flag=0;
			}
			if(flag!=1){
				printf("Error en la respuesta ingresada, ");
			}
		}while(flag!=1);
		if(!strcmp(s, "si"))
			resultado=1;
		else if(!strcmp(s, "no")){
			resultado=0;
		}
		else{
			printf("Error en la respuesta ingresada, ");
			resultado=2;
		}
	}while(resultado==2);

	//retorna 1 si escribe "si", 0 si escribe "no"
	return resultado;
}