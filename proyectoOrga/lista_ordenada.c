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


int (*comparador)(void *, void *);

TListaOrdenada crear_lista_ordenada(int (*f)(void *,void *)){
    /*
			Crea y retorna una lista ordenda vacía. El orden de los elementos en la 
			lista se especifica al momento de la creación, a través de una función de 
			comparación int (*f) (void *, void *). La función f devuelve -1 si el orden 
			del primer elemento es menor al orden del segundo, 0 si el orden es el mismo,
			y 1 si el orden del primer argumento es mayor que el orden del segundo.
		*/	
	
		comparador=f;
    TListaOrdenada l=malloc(sizeof(struct lista_ordenada));
    TLista miLista=crear_lista();
    l->lista=miLista;
    l->cantidad_elementos=0;

    return l;
}

int lo_insertar(TListaOrdenada lista, TElemento elem){
		/*	
		Agrega un elemento elem a la lista l, de modo que quede siempre ordenada de 
		forma ascendente. Retorna verdadero si procede con éxito, falso en caso contrario.
		*/
		
	if(lista==NULL)
				exit(LST_NO_INI);
	
    int res=FALSE;
    int tr=LST_NO_INI;
    TCelda p=POS_NULA;
		TCelda nuevo;


    if(lista!=NULL){
        p=lo_primera(lista);
        if(p!=POS_NULA){ //La lista pasada tiene al menos un elemento
				
            res= comparador(p->elemento,elem);
					
            if(res!=1){ //p->elemento <= elem
                if(p->proxima_celda!=POS_NULA){ 
									/*La lista pasada tiene más de un elemento.
                    Recorro con p y comparo con su siguiente hasta hallar un elemento mayor a elem*/
                    p=p->proxima_celda;

                    while(p->proxima_celda!=POS_NULA && comparador(p->proxima_celda->elemento,elem)!=1)
                        p=p->proxima_celda;
                }
            }
        }

        if(p!=POS_NULA && p->proxima_celda==POS_NULA && comparador(p->elemento,elem)==-1){
						//El elemento a insertar es mayor a todos los elementos de la lista ordenada. Se procede a insertar al final de la lista.
            
						nuevo=malloc((sizeof(struct celda)));
            nuevo->elemento=elem;
            nuevo->proxima_celda=NULL;
            l_ultima(lista->lista)->proxima_celda=nuevo;
						lista->lista->cantidad_elementos++;
  
        }
        else{
            tr=l_insertar(lista->lista,p,elem);
						//lista->cantidad_elementos++;
				}
			
    		lista->cantidad_elementos++;
		}

    return  tr;
}

int lo_eliminar(TListaOrdenada lista,TPosicion pos){
	/*
	Elimina el elemento en la posición pos de la lista. 
	Retorna verdadero si procede con éxito, falso en caso contrario.
	*/
	
	int resultado=FALSE;
	
	if(lista==NULL)
				exit(LST_NO_INI);
	
	resultado=l_eliminar(lista->lista,pos);
	
	if(resultado==TRUE)
		lista->cantidad_elementos--;
	
	return resultado;
}

int lo_size(TListaOrdenada lista){
	/*Retorna la cantidad de elemento de la lista l.*/
	
	if(lista==NULL)
				exit(LST_NO_INI);
	
	return lista->cantidad_elementos;
}

TPosicion lo_primera(TListaOrdenada lista){
	/*Retorna la primera posición de la lista l.*/
	
	if(lista==NULL)
				exit(LST_NO_INI);
	
	return l_primera(lista->lista);
}

TPosicion lo_ultima(TListaOrdenada lista){
	/*Retorna la última posición  de la lista l. */
	
	if(lista==NULL)
				exit(LST_NO_INI);
	
	return l_ultima(lista->lista);
}

TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos){
	/*
	Retorna la posición siguiente a pos en la lista.
	*/
	if(lista==NULL)
				exit(LST_NO_INI);
	
	return l_siguiente(lista->lista,pos);
}

