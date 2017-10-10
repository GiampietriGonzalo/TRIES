#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "lista_ordenada.h"
#include "lista_ordenada.c"

#define FALSE 0
#define TRUE 1

void mostrarPalabras(TTrie tr){
	
	
	
}

int consultar(TTrie tr,char* str){
	int i=tr_pertenece(tr,str);
	if(i==1){
		int cant=tr_recuperar(tr,str);
		printf("La palabra pertenece al archivo y aparece %i veces",cant);
	}
	else
		printf("La palabra no pertenece al archivo");
	
	return i;
}


int comienzaConAux(TNodo nodito,char c){
	int cont=0;
	if(lo_size(nodito->hijos)==0){
		cont=nodito->contador;
	}
	else{
		if(nodito->contador>0) cont+=nodito->contador;
		else{
			TPosicion aux=lo_primera(nodito->hijos);
			while(aux!=NULL){
				cont+=comienzaConAux(aux->elemento,c);
				aux=aux->proxima_celda;
			}
		}
	}
	return cont;
}

int comienzaCon(TTrie tr,char c){
	
	int i=0;
	char* ch=malloc(sizeof(char));
	*ch=c;
	TNodo aux=tr_recuperarHijo_auxiliar(tr->raiz,ch);
	if(aux==NULL) {
		i=0;
	}
	else {
		i=comienzaConAux(aux,c);
	}
	free(ch);
	return i;
}

int esPrefijo(TNodo nodo,char* str,int longitud){
	int i=0;
	if(nodo->rotulo==*str){	
		if(longitud==1 && nodo->contador>0) i=1;
	
		else{
			TPosicion aux=lo_primera(nodo->hijos);
			while(i==0 && aux!=NULL){
				i=esPrefijo(aux->elemento,str+1,longitud-1);
				aux=aux->proxima_celda;
			}	
		}
	}
	return i;
}


int porcentajePrefijo(TTrie tr,char* str){
	
}



int main(char *argv[]) {
	
	FILE* archivo=fopen("a.txt","rt");
	
	TTrie tr;
	printf("Se abrio el archivo\n");
	int opcion=0;
		
	do {
		printf("\n1.Mostrar palabras\n");
		printf("2.Consultar\n");
		printf("3.Comienza con\n");
		printf("4.Es prefijo\n");
		printf("5.Porcentaje prefijo\n");
		printf("6.Salir\n");
		scanf("%d",&opcion);
		
		switch(opcion) {
		case 1:
			
			break;
		case 2:
			printf("Ingrese la palabra a consultar: ");
			char* str;
			scanf("%s",str);
			int cant=consultar(tr,str);
			if(cant>0)
				printf("La palabra pertenece al archivo y aparece %i veces",cant);
			else
				printf("La palabra no pertenece al archivo");
			
			break;
		case 3:
			printf("Ingrese una letra: ");
			char c;
			scanf("%c",&c);
			int i;
			i=comienzaCon(tr,c);
			printf("Hay %i palabras que comienzan con la letra %c ",i,c);
		
			break;
		case 4:
			printf("Ingrese la palabra a consultar: ");
			char *s;
			scanf("%s",s);
			int j=esPrefijo(tr->raiz,s,strlen(s));
			if(j==1) printf("La palabra es prefijo de alguna palabra en el archivo");
			else printf("La palabra no es prefijo de ninguna plabra, o no se encuentra en el archivo");
			break;
		case 5:
			
			break;
		case 6:
			
			break;
		}
	
	} while(opcion!=6);
	
	return 0;
}
