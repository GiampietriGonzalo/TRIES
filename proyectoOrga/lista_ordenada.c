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
    TListaOrdenada l=malloc(sizeof(TListaOrdenada));
    TLista miLista=crear_lista();
    l->lista=miLista;
    l->cantidad_elementos=0;

    return l;
}

int lo_insertar(TListaOrdenada lista, TElemento elem){
    int res=0;
    int tr=LST_NO_INI;
    TCelda p=NULL;

    if(lista!=NULL){
        p=l_primera(lista->lista);
        if(p!=POS_NULA){ //La lista pasada tiene al menos un elemento
            res= comparador(p->elemento,elem);
            if(res!=1){
                if(p->proxima_celda!=POS_NULA){ /*La lista pasada tiene más de un elemento.
                    Recorro con p y comparo con su siguiente hasta hallar un elemento mayor a elem*/
                    res=comparador(p->elemento,elem);

                    while(p->proxima_celda!=POS_NULA && comparador(p->proxima_celda->elemento,elem)!=1){
                        p=p->proxima_celda;
                    }
                }
            }
        }
        tr=l_insertar(lista->lista,p,elem);
    }

    return  tr;
}

int lo_size(TListaOrdenada lista){return lista->cantidad_elementos;}
TPosicion lo_primera(TListaOrdenada lista){return l_primera(lista->lista);}
TPosicion lo_ultima(TListaOrdenada lista){return l_ultima(lista->lista);}
TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos){return l_siguiente(lista->lista,pos);}

