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


int (* f_comp)(void *, void *);

int f_comparador(void* v1, void* v2){

    TNodo* n1=v1;
    TNodo* n2=v2;
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



//FUNCIONES AUXILIARES



TNodo tr_recuperarHijo_auxiliar(TNodo padre, char* s){
    /*Buscar y retorna el nodo hijo de un nodo padre pasado por parámetro que tenga como rótulo el primer caracter de s.
    Si no lo encuentra, retorna NULL*/

    TListaOrdenada hijos;
    TPosicion hijoActual;
    TNodo nodoHijo=NULL;
    int salir=0;



    hijos=padre->hijos;
    /*if(padre->rotulo=='a' && lo_size(padre->hijos)>0){
        printf("tiene hijos: %d\n" , lo_size(padre->hijos));
        printf("Padre %c - Hijo: %c\n" , padre->rotulo, ((TNodo)(lo_primera(padre->hijos)->elemento))->rotulo);
     }
    */
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
    char* aux=s;

    if(padre->contador > 0 && longitud==1 && *s==padre->rotulo)
        resultado= TRUE;
    else{


        if(padre==tr->raiz){
            nodoHijo=tr_recuperarHijo_auxiliar(padre,aux);
                if(nodoHijo!=NULL) //Se encontro al nodo hijo que tiene el caracter s
                    resultado=tr_pertenece_auxiliar(nodoHijo,aux,longitud,tr);

        }

         if(*aux==padre->rotulo && longitud>0){
            aux++;
            //printf("Rotulo del padre: %c  - *aux: %c \n" , padre->rotulo , *aux);
            nodoHijo=tr_recuperarHijo_auxiliar(padre,aux);

                if(nodoHijo!=NULL){ //Se encontro al nodo hijo que tiene el caracter s{
                    //printf("*aux: %c \n" , *aux);

                    resultado=tr_pertenece_auxiliar(nodoHijo,aux,longitud-1,tr);

                }

            }
          }

    return resultado;
} //FIN metodo auxiliar recursivo de pertenece.

TNodo tr_buscar_auxiliar(TTrie tr,char* str){
    //Busca y retorna el nodo que contiene el ultimo caracter de str, si la palabra está en el trie.
    //Si no lo encuentra retorna NULL
    TNodo nodoActual=tr_recuperarHijo_auxiliar(tr->raiz,str);
    int longitud=strlen(str);
    int parar=0;
    char* aux=str;

    if(tr_pertenece(tr,aux)==TRUE && nodoActual!=NULL){

        while(parar==0){

            if(longitud==1 && *aux==nodoActual->rotulo && nodoActual->contador>0 )
                parar=1;
            else{

                if (longitud==1)
                    parar=2;
                else
                    if(longitud>1 && *aux==nodoActual->rotulo){
                        aux++;
                        longitud--;
                        nodoActual=tr_recuperarHijo_auxiliar(nodoActual,aux);

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
    node->hijos=crear_lista_ordenada(f_comp);
    tri->raiz=node;
    tri->cantidad_elementos=0;

    return tri;
}



int tr_pertenece(TTrie tr, char* str){

    int longitud= strlen(str);
    int resultado=FALSE;
    TNodo primerNodo=tr_recuperarHijo_auxiliar(tr->raiz,str);

    if(primerNodo!=NULL)
        resultado=tr_pertenece_auxiliar(tr->raiz,str,longitud,tr);

    return resultado;
}

int tr_recuperar(TTrie tr, char* str){

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

    if(longitud>0 && tr_pertenece(tr,str)==TRUE){
        //LA PALABRA YA SE ENCUENTRA EN EL TRIE
        nodo=tr_buscar_auxiliar(tr,str);
        if(nodo!=NULL){
            nodo->contador++;
            resultado=TRUE;
            tr->cantidad_elementos++;
        }
    }
    else
      if(longitud>0){
        nodo=tr->raiz;
        hijos=nodo->hijos;

        if(lo_size(hijos)>0){

            nodo=tr_recuperarHijo_auxiliar(nodo,aux);

            if (nodo!=NULL){
                hijos=nodo->hijos;
                if(nodo->contador>0) nodo->contador++;

                while(lo_size(hijos)!=0){
                    aux++;
                    longitud--;
                    nodo=tr_recuperarHijo_auxiliar(nodo,aux);
                    if (nodo!=NULL){
                        hijos=nodo->hijos;
                        if(nodo->contador>0) nodo->contador++;
                    }
                }//fin while
            }
        }

        if(nodo->contador>0){
            aux++;
            longitud--;
        }

        //Agrego los nodos faltantes de la palabra.
        while(longitud>0){

            nuevo=malloc(sizeof(TNodo));
            nuevaLista=crear_lista_ordenada(f_comp);
            nuevo->hijos=nuevaLista;
            nuevo->rotulo=*aux;
            nuevo->padre=nodo;
            if(longitud==1)
                nuevo->contador=1;
            else
                nuevo->contador=0;

            lo_insertar(hijos,nuevo);

            aux++;

            longitud--;
            hijos=nuevaLista;

        }

        tr->cantidad_elementos++;
        resultado=TRUE;
    }
    return resultado;
}

int tr_size(TTrie tr){return tr->cantidad_elementos;}


TPosicion recuperarPosAuxiliar(TListaOrdenada lista,TNodo nodo){

    TPosicion aux;
	aux=lista->lista->primer_celda;
    while(aux->elemento!=nodo)
        aux=aux->proxima_celda;
    return aux;
}


int tr_eliminar_auxiliar(TNodo nodito,char* str,int longitud){
	int elimine;
	/*Entero para indicar que tipo de eliminacion se realizo
		-1 no se realizo eliminacion
		0 se "elimino" la palabra pero no los nodos porque la palabra era prefijo de otra
		1 la palabra se elimino y tambien todos los nodos que la componian
	*/
	if(longitud==1){
		if(nodito->contador>0){
			if(lo_size(nodito->hijos)==0){
				TPosicion pos=recuperarPosAuxiliar(nodito->padre->hijos,nodito);
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
		TNodo hijo=tr_recuperarHijo_auxiliar(nodito,++str);
		if(hijo==NULL)
			elimine=-1; //La palabra no se encontró en el trie
		else{
			elimine=tr_eliminar_auxiliar(hijo,str+1,longitud--);
			if(elimine==1){
				if(lo_size(nodito->hijos)==0){
					TPosicion pos=recuperarPosAuxiliar(nodito->padre->hijos,nodito);
					free(nodito->hijos);
					lo_eliminar(nodito->padre->hijos,pos);
					free(nodito);
				}
				else{
					//el nodo actual es prefijo de otra palabra, no lo elimino
					elimine=0;
				}
			}
		}
	}
	return elimine;
}

int tr_eliminar(TTrie tr,char* str){

	int resultado=FALSE;
	int longitud= strlen(str);
	int elimine;
	TNodo primero=tr_recuperarHijo_auxiliar(tr->raiz,str);
	if(primero!=NULL){
		elimine=tr_eliminar_auxiliar(primero,str,longitud);
		if(elimine!=-1)
			resultado=TRUE;
	}
	return resultado;
}

int main(){

    int (* f_comp)(void *, void *);
    f_comp=f_comparador;
    char* p=malloc(sizeof(char));
    char str[200] = {"hola"};
    char str1[200] = {"holanda"};
    TNodo n1;

    printf("<<<<TESTER TRIE>>>>\n");
    printf("\n");
    printf("<TRIE VACIO>\n");
        TTrie trie= malloc(sizeof(TTrie));
        trie->cantidad_elementos=0;
        TNodo root= malloc(sizeof(TNodo));
        root->contador=0;
        TListaOrdenada lista_hijos=crear_lista_ordenada(f_comp);
        root->hijos=lista_hijos;
        root->padre=NULL;
        trie->raiz=root;

        printf("Size: %d \n", tr_size(trie));

        if(tr_pertenece(trie,str)==TRUE)  printf("Pertenece no funciona con trie vacio\n");
        else printf("Pertenece funciona con trie vacio\n");

        n1=tr_buscar_auxiliar(trie,str);
        if(n1!=NULL) printf("tr_buscar_auxiliar incorrecto. \n");
        else printf("tr_buscar_axuliar correcto. retorno NULL \n");

    printf("\n");
    printf("<TRIE CON 1 ELEMENTO>\n");

        if(tr_insertar(trie,p)==FALSE) printf("Insertar funciona con NULL\n");
        else printf("Insertar no funciona con NULL\n");

        if(tr_insertar(trie,str)==TRUE)  printf("Insertar funciona con hola\n");
        else printf("Insertar no funciona\n");
        printf("Size: %d \n" , tr_size(trie));


        if(tr_pertenece(trie,str)==TRUE)  printf("Pertenece funciona\n");
        else printf("Pertenece no funciona\n");

        n1=tr_buscar_auxiliar(trie,str);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto. retorno NULL \n");

        printf("String: %s \n" , str);
        printf("Recuperar: %d \n" , tr_recuperar(trie,str));
        printf("Resultado de eliminar: %d \n" , tr_eliminar(trie,str));

    printf("\n");
    printf("<TRIE CON 2 ELEMENTO, UNO PREFIJO DE OTRO>\n");

        if(tr_insertar(trie,str1)==TRUE)  printf("Insertar funciona con holanda\n");
        else printf("Insertar no funciona con holanda\n");
        printf("Size: %d \n" , tr_size(trie));

        printf("Recuperar de hola: %d \n" , tr_recuperar(trie,str));
        printf("Recuperar de holanda: %d \n" , tr_recuperar(trie,str1));

        if(tr_pertenece(trie,str1)==TRUE)  printf("Pertenece funciona con holanda\n");
        else printf("Pertenece no funciona\n");

        n1=tr_buscar_auxiliar(trie,str1);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto. retorno NULL \n");

    printf("\n");
    printf("\n");
    printf("TRIE: ");
    TNodo point=trie->raiz;
    while(lo_size(point->hijos)>0){
        printf(" [%c] " , ((TNodo)(lo_primera(point->hijos)->elemento))->rotulo);
        point= ((TNodo)(lo_primera(point->hijos)->elemento));
    }






    printf("\n");
    printf("\n");
    printf("\n");

    return 0;
}
