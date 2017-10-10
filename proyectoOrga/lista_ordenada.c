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

            nuevo=malloc((sizeof(TCelda)));
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

int main(){



    int (*funcion_c)(void*,void*);

    int f_comparador(void* v1, void* v2){
        int* n1=v1;
        int* n2=v2;
        int res=0;

        if((*n1)>(*n2))
            res=1;
        else
            if((*n1)<*(n2))
                res=-1;

        return res;
    }

    funcion_c=f_comparador;



    printf("<<<<TESTER ListaOrdenada>>>>\n");
    printf("\n");

    TListaOrdenada lo;
    int* n1=malloc(sizeof(int));
    int* n2=malloc(sizeof(int));
    int* n3=malloc(sizeof(int));
    int* n4=malloc(sizeof(int));
    int* n5=malloc(sizeof(int));

    TPosicion primera,ultima,siguiente,auxiliar;

        printf("<Crear_Lista_Ordenada>\n");
            lo=crear_lista_ordenada(funcion_c);

        printf("\n");

            printf("<LISTA VACIA>\n");
                printf("Size: %d\n",lo_size(lo));
                if(lo_primera(lo)==NULL) printf("lo_Primera correcto\n");
                else printf("lo_Primera incorrecto \n");
                if(lo_ultima(lo)==NULL) printf("lo_Ultima correcto\n");
                else printf("lo_Ultima incorrecto \n");
                printf("Resultado de eliminar: %d (si es 3, es correcto) \n", lo_eliminar(lo,lo_primera(lo)));
                if(lo_siguiente(lo,lo_primera(lo)) == NULL) printf("lo_Siguiente correcto \n");
                else printf("lo_siguiente incorrecto \n");

            printf("\n");

            printf("<UN SOLO ELEMENTO>\n");
                *n1=3;
                printf("Resultado de insertar el número 3: %d \n",lo_insertar(lo,n1));
                printf("Size: %d \n", lo_size(lo));
                primera=lo_primera(lo);
                ultima=lo_ultima(lo);
                printf("Primer: %d - ultimo: %d \n" , *((int*)primera->elemento),*((int*)ultima->elemento));
                primera=lo_siguiente(lo,primera);
                if(primera==NULL) printf("Siguiente: Correcto \n");
                else printf("Siguiente: Incorrecto y elem: %d\n", *((int*)primera->elemento));
                printf("Resultado de eliminar: %d\n" , lo_eliminar(lo,ultima));


            printf("\n");

            //LA LISTA ESTA VACIA EN ESTE PUNTO.
            printf("<CORROBORACION DE LISA VACIA>\n");
                printf("Size: %d\n",lo_size(lo));
                if(lo_primera(lo)==NULL) printf("lo_Primera correcto\n");
                else printf("lo_Primera incorrecto \n");
                if(lo_ultima(lo)==NULL) printf("lo_Ultima correcto\n");
                else printf("lo_Ultima incorrecto \n");
                printf("Resultado de eliminar: %d (si es 3, es correcto) \n", lo_eliminar(lo,lo_primera(lo)));
                if(lo_siguiente(lo,lo_primera(lo)) == NULL) printf("lo_Siguiente correcto \n");
                else printf("lo_siguiente incorrecto \n");

            printf("\n");

            printf("<2 ELEMENTOS>\n");
                n1=malloc(sizeof(int));
                n2=malloc(sizeof(int));
                *n1=4;
                *n2=3;

                printf("Resultado de insertar 4: %d\n" , lo_insertar(lo,n1));
                printf("Resultado de insertar 3: %d\n" , lo_insertar(lo,n2));
                printf("Size: %d \n", lo_size(lo));
                primera=lo_primera(lo);
                ultima=lo_ultima(lo);
                printf("Primer: %d - ultimo: %d \n" , *((int*)primera->elemento),*((int*)ultima->elemento));
                siguiente=lo_siguiente(lo,primera);
                if (siguiente!=NULL) printf("siguiente de primera: %d\n" , *((int*)siguiente->elemento));
                else printf("siguiente incorrecto en primer elemento \n");
                siguiente=lo_siguiente(lo,ultima);
                if (siguiente!=NULL) printf("siguiente incorrecto en último elemento. Elemento: %d\n" , *((int*)siguiente->elemento));
                else printf("siguiente Correcto en último elemento \n");
                printf("Resultado de eliminar primera: %d\n", lo_eliminar(lo,primera));
                printf("Resultado de eliminar ultima: %d\n", lo_eliminar(lo,ultima));

            printf("\n");

            printf("<CORROBORACION DE LISA VACIA>\n");
                printf("Size: %d\n",lo_size(lo));
                if(lo_primera(lo)==NULL) printf("lo_Primera correcto\n");
                else printf("lo_Primera incorrecto \n");
                if(lo_ultima(lo)==NULL) printf("lo_Ultima correcto\n");
                else printf("lo_Ultima incorrecto \n");
                printf("Resultado de eliminar: %d (si es 3, es correcto) \n", lo_eliminar(lo,lo_primera(lo)));
                if(lo_siguiente(lo,lo_primera(lo)) == NULL) printf("lo_Siguiente correcto \n");
                else printf("lo_siguiente incorrecto \n");

            printf("\n");

            printf("<MAS DE 2 ELEMENTOS>\n");
                n1=malloc(sizeof(int));
                n2=malloc(sizeof(int));
                *n1=4;
                *n2=3;
                *n3=1;
                *n4=7;
                *n5=2;

                printf("Resultado de insertar 4: %d\n" , lo_insertar(lo,n1));
                printf("Resultado de insertar 3: %d\n" , lo_insertar(lo,n2));
                printf("Resultado de insertar 1: %d\n" , lo_insertar(lo,n3));
                printf("Resultado de insertar 7: %d\n" , lo_insertar(lo,n4));
                printf("Resultado de insertar 2: %d\n" , lo_insertar(lo,n5));
                printf("Size: %d \n", lo_size(lo));

                primera=lo_primera(lo);
                ultima=lo_ultima(lo);
                printf("Primer: %d - ultimo: %d \n" , *((int*)primera->elemento),*((int*)ultima->elemento));
                siguiente=lo_siguiente(lo,primera);
                if (siguiente!=NULL) printf("siguiente de primera: %d\n" , *((int*)siguiente->elemento));
                else printf("siguiente incorrecto en primer elemento \n");

                printf("\n");
                printf("Elementos de la listaOrdenada:  ");
                auxiliar=lo_primera(lo);

                while(auxiliar!=NULL){
                    printf(" [%d] " , *((int*)auxiliar->elemento));
                    auxiliar=lo_siguiente(lo,auxiliar);
                }

                printf("\n");

                siguiente=lo_siguiente(lo,ultima);
                if (siguiente!=NULL) printf("siguiente incorrecto en último elemento. Elemento: %d\n" , *((int*)siguiente->elemento));
                else printf("siguiente Correcto en último elemento \n");
                printf("Resultado de eliminar primera: %d\n", lo_eliminar(lo,primera));
                printf("Resultado de eliminar ultima: %d\n", lo_eliminar(lo,ultima));

                auxiliar=lo_primera(lo);
                while(lo_size(lo)>0){
                    printf("Resultado de eliminar el resto de los elementos: %d (si es 1, es correcto) \n", lo_eliminar(lo,auxiliar));
                    auxiliar=lo_primera(lo);
                }

            printf("\n");

            printf("<CORROBORACION DE LISA VACIA>\n");
                printf("Size: %d\n",lo_size(lo));
                if(lo_primera(lo)==NULL) printf("lo_Primera correcto\n");
                else printf("lo_Primera incorrecto \n");
                if(lo_ultima(lo)==NULL) printf("lo_Ultima correcto\n");
                else printf("lo_Ultima incorrecto \n");
                printf("Resultado de eliminar: %d (si es 3, es correcto) \n", lo_eliminar(lo,lo_primera(lo)));
                if(lo_siguiente(lo,lo_primera(lo)) == NULL) printf("lo_Siguiente correcto \n");
                else printf("lo_siguiente incorrecto \n");

            printf("\n");


    return 0;
}

