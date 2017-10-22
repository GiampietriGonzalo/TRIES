#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "lista_ordenada.h"
#include "lista.c"

#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1


int (*comparador)(void *, void *);

TListaOrdenada crear_lista_ordenada(int (*f)(void *,void *)){
    comparador=f;
    TListaOrdenada l=malloc(sizeof(struct lista_ordenada));
    TLista miLista=crear_lista();
    l->lista=miLista;
    l->cantidad_elementos=0;


    return l;
}

int lo_insertar(TListaOrdenada lista, TElemento elem){

    int res=FALSE;
    int tr=LST_NO_INI;
    TCelda p=POS_NULA;
    TCelda nuevo;


    if(lista!=NULL){
        p=lo_primera(lista);
        if(p!=POS_NULA){ //La lista pasada tiene al menos un elemento
				
            res= comparador(p->elemento,elem);
					
            if(res!=1){ //p->elemento <= elem
                if(p->proxima_celda!=POS_NULA){ /*La lista pasada tiene más de un elemento.
                    Recorro con p y comparo con su siguiente hasta hallar un elemento mayor a elem*/
                    p=p->proxima_celda;

                    while(p->proxima_celda!=POS_NULA && comparador(p->proxima_celda->elemento,elem)!=1)
                        p=p->proxima_celda;
                }
            }
        }

        if(p!=POS_NULA && p->proxima_celda==POS_NULA && comparador(p->elemento,elem)==-1){

            nuevo=malloc((sizeof(struct celda)));
            nuevo->elemento=elem;
            nuevo->proxima_celda=NULL;
            l_ultima(lista->lista)->proxima_celda=nuevo;
            lista->cantidad_elementos++;
            lista->lista->cantidad_elementos++;

        }
        else
            tr=l_insertar(lista->lista,p,elem);
    }

    return  tr;
}

int lo_eliminar(TListaOrdenada lista,TPosicion pos){
	return l_eliminar(lista->lista,pos);
}

int lo_size(TListaOrdenada lista){
	return lista->lista->cantidad_elementos;
}

TPosicion lo_primera(TListaOrdenada lista){
	return l_primera(lista->lista);
}

TPosicion lo_ultima(TListaOrdenada lista){
	return l_ultima(lista->lista);
	}

TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos){
	return l_siguiente(lista->lista,pos);
}

