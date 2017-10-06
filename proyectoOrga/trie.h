#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED
#include "lista_ordenada.h"


typedef struct nodo{
    char rotulo;
    unsigned int contador;
    struct nodo * padre;
    TListaOrdenada hijos;
} * TNodo;

typedef struct trie{
    unsigned int cantidad_elementos;
    TNodo raiz;
}* TTrie;




TTrie crear_trie();
int tr_insertar(TTrie tr, char* str);
int tr_pertenece(TTrie tr, char* str);
int tr_recuperar(TTrie tr, char* str);
int tr_size(TTrie tr);
int tr_eliminar(TTrie tr, char* str);

#endif // TRIE_H_INCLUDED
