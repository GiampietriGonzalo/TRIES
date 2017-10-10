#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "lista_ordenada.h"
#include "lista_ordenada.c"

#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1



int f(TNodo* n1, TNodo* n2){
    char c1= (*n1)->rotulo;
    char c2= (*n2)->rotulo;
    int res=0;

    if(c1>c2)
        res=1;
    else
        if(c1<c2)
			res=-1;

    return res;
}

int (*funcion)(TNodo *, TNodo *)=f;


//FUNCIONES AUXILIARES



TNodo tr_recuperarHijo_auxiliar(TNodo padre, char* s,TTrie tr){
    /*Buscar y retorna el nodo hijo de un nodo padre pasado por parámetro que tenga como rótulo el primer caracter de s.
    Si no lo encuentra, retorna NULL*/

    TListaOrdenada hijos;
    TPosicion hijoActual;
    TNodo nodoHijo=NULL;
    int salir=0;


    hijos=padre->hijos;

    if(lo_size(hijos)>0){

        hijoActual=lo_primera(hijos);
        nodoHijo=hijoActual->elemento;

        while(salir==0 && *s!=nodoHijo->rotulo){
            //Recorro la lista de hijos hasta hallar el nodo que contiene al caracter apuntado por s
            hijoActual=lo_siguiente(hijos,hijoActual);

            if(hijoActual==NULL)
                salir=1;
            else
                nodoHijo=hijoActual->elemento;
        }

        if(salir==1) nodoHijo=NULL; //No se encontró al nodo hijo que tiene el caracter s
    }
    return nodoHijo;
}

int tr_pertenece_auxiliar(TNodo padre, char* s,int longitud, TTrie tr){
    //función auxiliar y recusivo para la función tr_pertenece.

    TNodo nodoHijo;
    int resultado=FALSE;

    if(padre->contador > 0 && longitud==1 && *s==padre->rotulo)
        resultado= TRUE;
    else{

        if(!(*s!=padre->rotulo || (longitud==1 && padre->contador==0) || (*s==padre->rotulo && longitud>1))){

            nodoHijo=tr_recuperarHijo_auxiliar(padre,s,tr);
                if(nodoHijo!=NULL){ //Se encontro al nodo hijo que tiene el caracter s
                    s++;
                    resultado=tr_pertenece_auxiliar(nodoHijo,s,longitud-1,tr);
                }

            } else resultado=FALSE;
          }

    return resultado;
} //FIN metodo auxiliar recursivo de pertenece.

TNodo tr_buscar_auxiliar(TTrie tr,char* str){

    TNodo nodoActual=tr_recuperarHijo_auxiliar(tr->raiz,str,tr);
    int longitud=strlen(str);
    int parar=0;

    if(tr_pertenece(tr,str)==TRUE && nodoActual!=NULL){

        while(parar==0){

            if(longitud==1 && *str==nodoActual->rotulo && nodoActual->contador>0 )
                parar=1;
            else{

                if (longitud==1)
                    parar=2;
                else
                    if(longitud>1 && *str==nodoActual->rotulo){
                        str++;
                        longitud--;
                        nodoActual=tr_recuperarHijo_auxiliar(nodoActual,str,tr);

                        if(nodoActual==NULL)
                            parar=2;
                    }
            }
        }//fin WHile


        if(parar==2) //No Se encontro el nodo.
            nodoActual=NULL;
    }

    return nodoActual;
}



//FUNCIONES DE trie.h

TTrie crear_trie(){

    TTrie tri= malloc(sizeof(TTrie));
    TNodo node= malloc(sizeof(TNodo));
    node->hijos=crear_lista_ordenada(funcion);
    tri->raiz=node;
    tri->cantidad_elementos=0;

    return tri;
}



int tr_pertenece(TTrie tr, char* str){

    int longitud= strlen(str);
    int resultado;

    //Busco al hijo de la raiz que contenga el primer caracter de la palabra pasada.
    TNodo primerNodo=tr_recuperarHijo_auxiliar(tr->raiz,str,tr);

    if(primerNodo!=NULL)
        resultado=tr_pertenece_auxiliar(tr->raiz,str,longitud,tr);


    return resultado;
}

int recuperar(TTrie tr, char* str){

    int resultado= STR_NO_PER;
    TNodo nodo=tr_buscar_auxiliar(tr,str);
    if (nodo!=NULL)
        resultado=nodo->contador;

    return resultado;
}

int tr_insertar(TTrie tr, char* str){

    int resultado=FALSE;
    TNodo nodo,nuevo;
    TListaOrdenada hijos;
    char* aux=str;
    int longitud=strlen(str);
    TListaOrdenada nuevaLista;

    if(tr_pertenece(tr,str)){

        nodo=tr_buscar_auxiliar(tr,str);
        if(nodo!=NULL){
            nodo->contador+=1;
            resultado=TRUE;
            tr->cantidad_elementos++;
        }
    }
    else{
        nodo=tr->raiz;
        hijos=nodo->hijos;

        if(lo_size(hijos)!=0){

            nodo=tr_recuperarHijo_auxiliar(nodo,aux,tr);
            hijos=nodo->hijos;
            if (nodo!=NULL){
                if(nodo->contador>0) nodo->contador++;

                while(lo_size(hijos)!=0){
                    aux++;
                    longitud--;
                    nodo=tr_recuperarHijo_auxiliar(nodo,aux,tr);
                    if (nodo!=NULL){
                        hijos=nodo->hijos;
                        if(nodo->contador>0) nodo->contador++;
                    }
                }//fin while
            }
        }

        //Agrego los nodos faltantes de la palabra.
        while(longitud>0){

            nuevo=malloc(sizeof(TNodo));
            nuevo->contador=0;
            nuevaLista=crear_lista_ordenada(funcion());
            nuevo->hijos=nuevaLista;
            nuevo->rotulo=*aux;
            nuevo->padre=nodo;

            lo_insertar(hijos,nuevo);

            aux++;
            tr->cantidad_elementos++;
            longitud--;
            hijos=nuevaLista;
        }
        resultado=TRUE;
    }
    return resultado;
}

int tr_size(TTrie tr){return tr->cantidad_elementos;}

int tr_eliminar(TTrie tr,char* str){
	
	int resultado=FALSE;
	int longitud(strlen(str);
	int elimine;
	TNodo primero=tr_recuperarHijo_auxiliar(tr->raiz,str,tr);
	if(primero!=NULL){
		elimine=eliminarAuxiliar(primero,str,longitud);
		if(elimine!=-1)
			resultado=TRUE;
	}
	return resultado;
}

int eliminarAuxiliar(TNodo nodito,char* str,int longitud){
	int elimine;
	/*Entero para indicar que tipo de eliminacion se realizo
		-1 no se realizo eliminacion
		0 se "elimino" la palabra pero no los nodos porque la palabra era prefijo de otra
		1 la palabra se elimino y tambien todos los nodos que la componian
	*/
	if(longitud==1){
		if(nodito->contador>0){
			if(nodito->hijos->cantidad_elementos==0){
				TPosicion pos=lo_recuperar(nodito->padre->hijos,nodito);
				free(nodito->hijos);
				lo_eliminar(nodito->padre->hijos,pos);
				free(nodito);
				elimine=1;
				//Se eliminan los nodos
			}
			else{
				nodito->contador--;
				elimine=0;
				//No se eliminan los nodos porque la palabra es prefijo de otra
			}
		}
		else{
			elimine=-1;
			//la palabra se encontro pero no pertenece al trie, no se puede eliminar
		}
	}
	else{
		TNodo hijo=tr_recuperarHijo_auxiliar(nodito,str+1,tr);
		if(hijo==NULL)
			elimine=-1; //La palabra no se encontró en el trie
		else{
			elimine=eliminarAuxiliar(hijo,str+1,longitud--);
			if(elimine==1){
				if(nodito->hijos->cantidad_elementos==0){
					TPosicion pos=lo_recuperar(nodito->padre->hijos,nodito);
					free(nodito->hijos);
					lo_eliminar(nodito->padre->hijos,pos);
					free(nodito);
				}
				else{
					//el nodo actual es prefijo de otra palabra, no lo elimino
					eliminar=0;
				}
			}
		}
	}
	return elimine;
}