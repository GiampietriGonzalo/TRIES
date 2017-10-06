#include <stdio.h>
#include <stdlib.h>
#include "lista_ordenada.h"

#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1

//int (*f)(void *,void *)

int f(TNodo* n1, TNodo* n2){
    char c1= n1->rotulo;
    char c2= n2->rotulo;
    int res=0;

    if(c1>c2)
        res=1;
    else
        res=-1;

    return res;
}


TTrie crear_trie(){

    TTrie tri= new malloc(sizeof(TTrie));
    TNodo node= new malloc(sizeof(TNodo));
    node->hijos=crear_lista_ordenada(f* funcion);
    node->padre=NULL;
    node->rotulo=NULL;
    node->contador=NULL;
    tri->raiz=node;
    tri->cantidad_elementos=0;

    return tri;
}


int tr_insertar(TTrie tr; char* str){

    int tr=TRI_NO_INI;




    return tr;
}

int tr_pertenece(TTrie tr, char* str){

    int res=TRI_NO_INI;
    int n=TRUE;
    char* caracter=str
    lista_ordenada lis_hijos;

    if(tr!=NULL)
        if(tr_size()!=0){
            //El trie pasado no es nulo y tiene al menos un nodo.
            lis_hijos=tr->raiz->hijos;

            while(n=TRUE && )


        }
    return res;
}

int tr_pertenece(TTrie tr, char* str){
    int res=TRI_NO_INI;
    lista_ordenada lis_hijos;
    TNodo hijo;
    char aux;

    if(tr!=NULL)
        if(tr_size()=1){
         //Trie con un solo nodo.
         lis_hijos=tr->raiz->hijos;
         aux=*lo_primera(lis_hijos)->elemento;
         if(aux==str && )

        }



}
