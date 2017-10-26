#ifndef LISTA_ORDENADA_H_INCLUDED
#define LISTA_ORDENADA_H_INCLUDED
#include "lista.c" //??


typedef struct lista_ordenada {
    unsigned int cantidad_elementos;
    TLista lista;
} * TListaOrdenada;

TListaOrdenada crear_lista_ordenada(int (*f)(void *,void *));

int lo_insertar(TListaOrdenada lista, TElemento elem);

int lo_eliminar(TListaOrdenada lista, TPosicion pos);

int lo_size(TListaOrdenada lista);

TPosicion lo_primera(TListaOrdenada lista);

TPosicion lo_ultima(TListaOrdenada lista);

TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos);

#endif
