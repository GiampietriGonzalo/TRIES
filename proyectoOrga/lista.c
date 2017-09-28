#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1





TLista crear_lista(){
    TLista l=malloc(sizeof(struct lista));
    l->cantidad_elementos=0;
    l->primer_celda=NULL;
    return l;
}

int l_insertar(TLista lista, TPosicion pos, TElemento elem){

    TCelda nueva=malloc(sizeof(struct celda));
    nueva->elemento=elem;
    nueva->proxima_celda=NULL;
    TCelda aux=lista->primer_celda;
    int r=FALSE;

    if(pos==POS_NULA){
        if(lista->primer_celda!=POS_NULA) /*LISTA NO VACIA*/
            nueva->proxima_celda = lista->primer_celda;
            lista->primer_celda=nueva;
            lista->cantidad_elementos++;
            r=1;
    }
    else{

        if(lista->primer_celda==pos){/*Pos es el primer elemento de la lista.*/
            nueva->proxima_celda=lista->primer_celda;
            lista->primer_celda=nueva;
            lista->cantidad_elementos++;
            r=TRUE;
        }
        else{ /*Pos no es el primer elemento de la lista, se debe buscar.*/
          while(aux->proxima_celda!=pos && aux->proxima_celda!=POS_NULA)
              aux=aux->proxima_celda;

          if (aux->proxima_celda==pos){/*Lo encontro
              colocar la nueva celda entre aux y pos*/
              nueva->proxima_celda=pos;
              aux->proxima_celda=nueva;
              lista->cantidad_elementos++;
              r=TRUE;
          }
        }
    }


    return r;
}

int l_eliminar(TLista lista, TPosicion pos){

    int r=FALSE;
    TCelda aux=lista->primer_celda;

    if(pos!=POS_NULA){
        if(aux==pos)
            if (lista->cantidad_elementos==1){/*ES LA UNICA CELDA EN LA LISTA.*/
                lista->primer_celda=POS_NULA;
            }
            else{/*ES LA PRIMER CELDA DE LA LISTA, PERO NO LA UNICA.*/
                lista->primer_celda=aux->proxima_celda;
            }
        else{
          /*Pos no es la primer celda*/
          while(aux->proxima_celda!=pos && aux->proxima_celda!=POS_NULA)
                aux=aux->proxima_celda;

          if (aux->proxima_celda==pos){/*Lo encontro, elimina celda*/
              aux->proxima_celda=pos->proxima_celda;
          }
        }
        r=TRUE;

        free(pos->elemento);
        free(pos);
        lista->cantidad_elementos--;
    }
    else
        r=LST_POS_INI;

    return r;
}

TPosicion l_primera(TLista lista){
    TPosicion r=POS_NULA;
    if(lista->cantidad_elementos!=0)
        r=lista->primer_celda;
    return r;
}

TPosicion l_ultima(TLista lista){
    TPosicion r=POS_NULA;
    if(lista->cantidad_elementos!=0){
        TCelda aux=lista->primer_celda;
        while(aux->proxima_celda!=POS_NULA){
            aux=aux->proxima_celda;
        }
        r=aux;
    }
    return r;
}

int l_size(TLista lista){return lista->cantidad_elementos;}

TPosicion l_anterior(TLista lista, TPosicion pos){

    TPosicion aux=POS_NULA;
    if(lista->primer_celda!=pos && pos!=POS_NULA){
        aux=lista->primer_celda;
        while(aux->proxima_celda!=POS_NULA && aux->proxima_celda!=pos)
            aux=aux->proxima_celda;
    }
    return aux;
}

TPosicion l_siguiente(TLista lista,TPosicion pos){

    TPosicion aux=POS_NULA;

    if(pos!=POS_NULA && lista!=NULL){
        aux=lista->primer_celda;
        while(aux->proxima_celda!=POS_NULA && aux!=pos)
            aux=aux->proxima_celda;
    }

    return aux;
}

TElemento l_recuperar(TLista lista, TPosicion pos){

    TElemento tr=ELE_NULO;
    TCelda aux;

    if(pos!=POS_NULA && lista!=NULL){ /*Lo busco en la lista.*/
        aux=lista->primer_celda;
        while(aux!=pos && aux->proxima_celda!=POS_NULA)
            aux=aux->proxima_celda;

        if(aux==pos) /*Se encontró pos en la lista.*/
            tr=aux->elemento;
    }
    return tr;
}




int main(){

    printf("<<< TESTER TDALista >>>\n");
    printf("\n");


    int r=0,i=9;
    TLista lista= malloc(sizeof(TLista));
    lista=crear_lista();
    int* n=malloc(sizeof(int));
    TElemento elem=malloc(sizeof(int));
    *n=77;
    elem=n;
    TPosicion pos;
    TCelda aux;

    TElemento n1= malloc(sizeof(int));
    TElemento n2= malloc(sizeof(int));
    TElemento n3= malloc(sizeof(int));
    TElemento n4= malloc(sizeof(int));
    TElemento n5= malloc(sizeof(int));
    TElemento n6= malloc(sizeof(int));
    TElemento n7= malloc(sizeof(int));
    int* i1=malloc(sizeof(int));
    int* i2=malloc(sizeof(int));
    int* i3=malloc(sizeof(int));
    int* i4=malloc(sizeof(int));
    int* i5=malloc(sizeof(int));
    int* i6=malloc(sizeof(int));
    int* i7=malloc(sizeof(int));
    *i1=6; *i2=7; *i3=8; *i4=9;
    n1=i1; n2=i2; n3=i3; n4=i4;

    *i5=2; *i6=88; *i7=77;
    n5=i5; n6=i6; n7=i7;


    printf("---Insertar---\n");
    r=l_insertar(lista,NULL,n5);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" , *(((int*)lista->primer_celda->elemento)));
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    printf("\n");

    printf("Inserción de otro elemento\n");

    r=l_insertar(lista,lista->primer_celda,n6);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" , *(((int*)lista->primer_celda->elemento)));
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    printf("\n");

    printf("Inserción de otro elemento\n");

    r=l_insertar(lista,lista->primer_celda->proxima_celda,n7);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" ,  *((int*)(lista->primer_celda->proxima_celda->elemento)));
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    //LISTA HASTA AHORA 2-88-77
    printf("\n");

    printf("---Eliminar---\n");
    pos=lista->primer_celda->proxima_celda->proxima_celda;
    printf("Elemento eliminado: %d \n",*((int*)pos->elemento));
    r=l_eliminar(lista,pos);
    printf("Resultado de eliminar con un elemento: %d \n" , r);

    //LISTA HASTA AHORA 2-88

    printf("\n");
    l_eliminar(lista,l_primera(lista));



    l_insertar(lista,NULL,n1);
    l_insertar(lista,NULL,n2);
    l_insertar(lista,NULL,n3);
    l_insertar(lista,NULL,n4);
    //LISTA HASTA AHORA 6-7-8-9-88

    printf("Lista: ");
    aux=l_primera(lista);
    while(aux!=NULL){
        printf("[%d] ", *((int*)(aux->elemento)));
        aux=aux->proxima_celda;
    }


    printf("\n");
    printf("\n");

    printf("---Primera---\n");
    printf("Elemento de la primera posicion de la lista: %d \n" , *((int*)l_primera(lista)->elemento));

    printf("\n");

    printf("---Ultima---\n");
    printf("Elemento de la última posicion de la lista: %d \n" , *((int*)l_ultima(lista)->elemento));

    printf("\n");

    printf("---Anterior---\n");
    r=l_anterior(lista,l_primera(lista));
    printf("Resultado del Anterior del primer elemento: %d \n" , r);
    printf("Anterior del último elemento: %d \n" , *((int*)l_anterior(lista,l_ultima(lista))->elemento));

    printf("\n");

    printf("---Recuperar---\n");
    r=l_recuperar(lista,NULL);
    printf("Resultado del recuperar el elemento de una posición nula: %d \n" , r);
    printf("Elemento recuperado de la primer posicion: %d \n" , *((int*)l_recuperar(lista,l_primera(lista))));

    aux=lista->primer_celda->proxima_celda->proxima_celda;

    printf("\n");

    printf("---Recuperar elemento que no es el primero ni ultmo---\n");
    printf("Elemento recuperado de una posicion no nula: %d \n" , *((int*)l_recuperar(lista,aux)));

return 0;}
