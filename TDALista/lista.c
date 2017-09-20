#include <stdio.h>
#include <stdlib.h>


#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1


typedef struct celda * TPosicion;
typedef void * TElemento;

typedef struct celda {
    TElemento elemento;
    struct celda* proxima_celda;
} * TCelda;

typedef struct lista {
    unsigned int cantidad_elementos;
    TCelda primer_celda;
} * TLista;


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

          if (aux->proxima_celda==pos){/*Lo encontro
              Eliminar celda*/
              aux->proxima_celda=pos->proxima_celda;
          }
        }
        r=TRUE;


        if(pos->proxima_celda!=POS_NULA){
            //free(pos->proxima_celda);
            printf("Entro\n");
        }
        /*free(pos->elemento);*/

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
    void* elem2=mallic(sizeof(int));
    *elem2=2;
    TElemento elem=malloc(sizeof(int));
    *elem=77;
    TPosicion pos;
    TCelda aux;

    printf("---Insertar---\n");
    r=l_insertar(lista,NULL,elem);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" , lista->primer_celda->elemento);
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    printf("\n");

    printf("Inserción de otro elemento\n");
    *elem=2;
    r=l_insertar(lista,lista->primer_celda,elem);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" , lista->primer_celda->elemento);
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    printf("\n");

    printf("Inserción de otro elemento\n");
    *elem=88;
    r=l_insertar(lista,lista->primer_celda->proxima_celda,elem);
    printf("Resultado de insertar: %d \n" , r);
    printf("Elemento insertado: %d \n" , lista->primer_celda->proxima_celda->elemento);
    printf("Cantidad de elementos de la lista: %d \n",lista->cantidad_elementos);

    //LISTA HASTA AHORA 2-88-77
    printf("\n");

    printf("---Eliminar---\n");
    pos=lista->primer_celda->proxima_celda->proxima_celda;
    printf("Elemento eliminado: %d \n",pos->elemento);
    r=l_eliminar(lista,pos);
    printf("Resultado de eliminar con un elemento: %d \n" , r);

    //LISTA HASTA AHORA 2-88

    printf("\n");
    l_eliminar(lista,l_primera(lista));


    for(i;i!=5;i--){
        *elem=i;
        l_insertar(lista,NULL,elem);
    }
    //LISTA HASTA AHORA 6-7-8-9-88

    printf("Lista: ");
    aux=l_primera(lista);
    while(aux!=NULL){
        printf("[%d] ", aux->elemento);
        aux=aux->proxima_celda;
    }


    printf("\n");
    printf("\n");

    printf("---Primera---\n");
    printf("Elemento de la primera posicion de la lista: %d \n" , l_primera(lista)->elemento);

    printf("\n");

    printf("---Ultima---\n");
    printf("Elemento de la última posicion de la lista: %d \n" , l_ultima(lista)->elemento);

    printf("\n");

    printf("---Anterior---\n");
    r=l_anterior(lista,l_primera(lista));
    printf("Resultado del Anterior del primer elemento: %d \n" , r);
    printf("Anterior del último elemento: %d \n" , l_anterior(lista,l_ultima(lista))->elemento);

    printf("\n");

    printf("---Recuperar---\n");
    r=l_recuperar(lista,NULL);
    printf("Resultado del recuperar el elemento de una posición nula: %d \n" , r);
    printf("Elemento recuperado de la primer posicion: %d \n" , l_recuperar(lista,l_primera(lista)));

    aux=lista->primer_celda->proxima_celda->proxima_celda;

    printf("\n");

    printf("---Recuperar elemento que no es el primero ni ultmo---\n");
    printf("Elemento recuperado de una posicion no nula: %d \n" , l_recuperar(lista,aux));






return 0;}

