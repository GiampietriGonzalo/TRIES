#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "lista_ordenada.h"
#include "lista_ordenada.c"

#define FALSE 0
#define TRUE 1


//IMPLEMENTACIÓN INCOMPLETA :( - LAS POCAS FUNCIONES IMPLEMENTADAS NO ESTAN TESTEADAS

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




