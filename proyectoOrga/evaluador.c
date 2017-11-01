#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_ordenada.h"
#include "trie.h"

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



//FUNCIONES AUXILIARES

void ev_liberar_auxiliar(TTrie tr, TNodo nodo){
	/*
		Libera recursivamente los espacios de memoria correspondiente a cada estructura
	*/
	TListaOrdenada hijos=nodo->hijos;
	TPosicion pos=lo_primera(hijos);


	while(pos!=POS_NULA){
		ev_liberar_auxiliar(tr,pos->elemento);
		lo_eliminar(hijos,pos);
		pos=lo_siguiente(hijos,pos);
	}

	free(nodo->hijos);

}

int ev_toLowerCase_auxiliar(int c){
	//Retorna el entero correspondiente al código ASCII del caracter pasado por parámetro en minúscula
	//Se asume que previamente se controlo que el caracter c fuera una letra mayúscula entre A y Z.
	return c+=32;
}

void ev_vaciar_auxiliar(char c[]){
	//Vacia el arreglo de caractere pasado por parámetro
    int i=0;
	while(i<50){
		c[i]='\0';
		i++;
	}

}


int ev_porcentaje_auxiliar(TNodo nodo){
		/*Recorre el trie hacia abajo a través de los hijos del nodo pasado por
		por parámetro contando la cantidad de palabras de la cual es prefija la palabra
		que se pasa por parámetro en la función cascar "prefijos"*/

    int cont=nodo->contador;
    TPosicion aux;
    if(lo_size(nodo->hijos)>0){
        aux=lo_primera(nodo->hijos);

        while(aux!=NULL){
            cont+=ev_porcentaje_auxiliar((TNodo)aux->elemento);
            aux=lo_siguiente(nodo->hijos,aux);
        }
    }

    return cont;

}

TNodo ev_recuperarHijo_auxiliar(TNodo padre, char* s){
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



TNodo ev_buscar_auxiliar(TTrie tr,char* str){
    //Busca y retorna el nodo que contiene el ultimo caracter de str, si la palabra está en el trie.
    //Si no lo encuentra retorna NULL
    TNodo nodoActual=ev_recuperarHijo_auxiliar(tr->raiz,str);
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
                        nodoActual=ev_recuperarHijo_auxiliar(nodoActual,aux);

                        if(nodoActual==NULL)
                            parar=2;
                    }
            }
        }//fin WHile


        if(parar==2) //No se encontro el nodo.
            nodoActual=NULL;
    }

    return nodoActual;
}

void ev_imprimir_auxiliar(TTrie tr , char c[]){
	/*Imprime la palabra pasada por parámetro, indicando cuánta
		veces se encuentra en el Trie tr.*/

	TNodo ultimo=ev_buscar_auxiliar(tr,c);
	printf("%s (%d)" , c,ultimo->contador);

	printf("\n");
}

void ev_mostrar_auxiliar(TTrie tr,char buffer[], TNodo nodo){
	/*Recorre el trie y concatena los rotulos de los nodos para formas las
		palabras que se encuentran en el Trie, para luego imprimirlas.*/

	TPosicion hijo;
	char * auxchar;
	char auxiliar[70]={""};

	buffer[strlen(buffer)]=nodo->rotulo;

	//OBSERVACION: strcat(buffer,rotulo); (funcion strcat de la libreria string.h) no funciona, al menos en la maquina virtual.
	int n=0;

	if (nodo->contador > 0){

		ev_imprimir_auxiliar(tr,buffer);

		if(lo_size(nodo->padre->hijos) > 1 && lo_size(nodo->hijos)==0)
			buffer[strlen(buffer) - 1]='\0';

	}


	if (lo_size(nodo->hijos) > 0){

			hijo=lo_primera(nodo->hijos);

			while(hijo!=NULL){

				//Copia el contenido de buffer[] en auxiliar[]
				auxchar=buffer;

				n=0;

				while(*auxchar!='\0'){
					auxiliar[n]=*auxchar;
					n++;
					auxchar++;
				}


				ev_mostrar_auxiliar(tr,auxiliar,hijo->elemento);
				hijo=lo_siguiente(nodo->hijos,hijo);

				if(lo_size(nodo->hijos)>1)
					auxiliar[strlen(auxiliar) - 1]='\0';

			}
	}

}

int ev_comienzaCon_auxiliar(TNodo nodito,char c){
	/*Recorre el trie hacia abajo a través de los hijos del nodo nodito pasado por
		parámetro, contando cúantas palabras comienzan con el caracter c.*/

	int cont=0;

	if(lo_size(nodito->hijos)==0)
		cont=1;
	else{

		if(nodito->contador>0)
			cont+=1;

		TPosicion aux=lo_primera(nodito->hijos);

		while(aux!=NULL){
			cont+=ev_comienzaCon_auxiliar(aux->elemento,c);
			aux=lo_siguiente(nodito->hijos,aux);
		}

	}
	return cont;
}


//FUNCIONES DE EVALUADOR

void mostrarPalabras(TTrie tr){
	/*
		permite visualizar el listado de todas las palabras que posee el archivo,
		junto con la cantidad de apariciones de la misma.
	*/
	char buffer[50]={""};
	TPosicion hijo=lo_primera(tr->raiz->hijos);

	while(hijo!=NULL){
		ev_vaciar_auxiliar(buffer);
		ev_mostrar_auxiliar(tr,buffer,hijo->elemento);
		hijo=lo_siguiente(tr->raiz->hijos,hijo);

	}
}

int consultar(TTrie tr,char* str){
	/*
	permite determinar si una palabra ingresada pertenece o no al archivo,
	y en consecuencia, cuántas veces esta se repite en el archivo.
	*/
    char* c=str;
    while(*c!='\0'){
      if(*c>64 && *c<91) //A=65 , Z=90
        *c=ev_toLowerCase_auxiliar(*c);
      c++;
    }

	int i=tr_pertenece(tr,str);
	int cant=0;
	if(i==TRUE)
		cant=tr_recuperar(tr,str);

	return cant;
}




int comienzaCon(TTrie tr,char c){
	//Retorna la cantidad de palabras que comienzan con la letra dada.


     if(c>64 && c<91) //A=65 , Z=90
        c=ev_toLowerCase_auxiliar(c);



	int i=0;
	char* ch=malloc(sizeof(char));
	*ch=c;
	TNodo aux=ev_recuperarHijo_auxiliar(tr->raiz,ch);

	if(aux!=NULL)
		i=ev_comienzaCon_auxiliar(aux,c);

	free(ch);
	return i;
}


int esPrefijo(TTrie tr,char* str){
	/*Determina si al menos una palabra perteneciente al Trie tr es posfija de una palabra pasada por
		parámetro.*/

	char* c=str;
    while(*c!='\0'){
      if(*c>64 && *c<91) //A=65 , Z=90
        *c=ev_toLowerCase_auxiliar(*c);
      c++;
    }

	int res=FALSE;
	TNodo ultimo=NULL;

	ultimo=ev_buscar_auxiliar(tr,str);

	if(ultimo!=NULL && lo_size(ultimo->hijos)>0)
		res=TRUE;


	return res;
}



int porcentajePrefijo(TTrie tr,char* str){
    /*Determina y retorna que porcentaje de palabras pertenecientes al Trie tr
        son posfijas de una palabra pasada por parámetro, que puede o no
        pertenecer al Trie tr.
    */

    char* c=str;
    while(*c!='\0'){
      if(*c>64 && *c<91) //A=65 , Z=90
        *c=ev_toLowerCase_auxiliar(*c);
      c++;
    }

    TNodo nodo=ev_buscar_auxiliar(tr,str);
    int cont=0;
		TPosicion aux;

    if(nodo!=NULL && lo_size(nodo->hijos) > 0){
    	aux= lo_primera(nodo->hijos);

			while(aux!=NULL){
            cont+=ev_porcentaje_auxiliar((TNodo)aux->elemento);
            aux=lo_siguiente(nodo->hijos,aux);
        }
		}

    return (cont*100)/tr_size(tr);

}

int main(int i, char *argv[])  {

    if(argv[1]=='\0'){
        printf("< NO SE HA INGRESADO EL NOMBRE DE ARCHIVO >\n");
        exit(0);
    }

	f_comp=f_comparador;

	FILE* archivo=fopen(argv[1],"r+");

	if(archivo==NULL){
        printf("\n");
        printf("< NO ESE HA ENCONTRADO EL ARCHIVO >\n");
        printf("\n");
        exit(0);
	}

	int caracter; //caracter que lee el archivo
	char palabra[50]; //String al cual se le agregan los  caracteres del archivo para luego ingresarlos al Trie
	int n=0;
	int seguir=TRUE;
	TTrie tr=crear_trie();
	TListaOrdenada hijos;
	TPosicion pos;

    printf("\n");
    printf("-------<<<EVALUADOR>>>-------\n");



	char* puntero; //Permite insertar las palabras leídas al trie


	/*Lectura de caracteres del archivo

     Toda palabra que contenga al menos un caracter fuera de los siguientes rangos:
     a..z ; A..Z  , será descartada.

	*/
	caracter = fgetc(archivo);
	while(seguir==TRUE && caracter!= EOF){

		n=0;
		seguir=TRUE;
        ev_vaciar_auxiliar(palabra);


		while(seguir==TRUE && caracter!=32){

			if(caracter>64 && caracter<91) //A=65 , Z=90
				caracter=ev_toLowerCase_auxiliar(caracter);
			else
				if((caracter<97 || caracter>122) && caracter!=EOF && caracter!='\n'){ //a=97 , z=122
					seguir=FALSE;

                }


			if (seguir==TRUE && (caracter == EOF || caracter=='\n' || caracter==32)){
				seguir=FALSE;
                ev_vaciar_auxiliar(palabra);
            }

			if(seguir==TRUE){
				palabra[n]=caracter;
				caracter = fgetc(archivo);
				n++;
			}

		}

		if (caracter==32 && caracter!=EOF){
				caracter = fgetc(archivo);
				seguir=TRUE;
		}




		puntero=palabra;

     if(seguir==TRUE)
        tr_insertar(tr,puntero);
     else
        if (caracter!=EOF){
            while(caracter!=' ' && caracter!=EOF)
                caracter = fgetc(archivo);
            seguir=TRUE;
        }

	}


	int opcion=0;
    printf("\n");
    printf("INGRESE LA OPCION DESEADA\n");
	do {
        opcion=0;

		printf("[1] Mostrar palabras\n");
		printf("[2] Consultar\n");
		printf("[3] Comienza con\n");
		printf("[4] Es prefijo\n");
		printf("[5] Porcentaje prefijo\n");
		printf("[6] Salir\n");

        printf("\n");
		scanf("%d",&opcion);
		printf("\n");

		switch(opcion) {

		case 1:{

			printf("palabras (apariciones)\n" );
			printf("\n");
			mostrarPalabras(tr);
			printf("\n");
            break;
		}

		case 2:{
			printf("Ingrese la palabra a consultar: \n");
			char str[50];
			scanf("%s",str);
			int cant=consultar(tr,str);
			if(cant>0)
				printf("La palabra pertenece al archivo y aparece %i veces \n",cant);
			else
				printf("La palabra no pertenece al archivo\n");

            printf("\n");
			break;
		}

		case 3:{

			char c,str[3];
			int i;
			printf("Ingrese una letra:\n");
			scanf("\n %s",str);
			c=str[0];
			i=comienzaCon(tr,c);
			printf("Hay %d palabras que comienzan con la letra %c \n",i,c);
            printf("\n");
			break;
		}

		case 4:{
			printf("Ingrese la palabra a consultar: ");
			char s[50];
			scanf("\n%s",s);
			int k=esPrefijo(tr,s);
			if(k==1) printf("La palabra es prefijo de al menos una palabra en el archivo\n");
			else printf("La palabra no es prefijo de ninguna palabra\n");
			printf("\n");
			break;
		}

		case 5:{
                printf("Ingrese el prefijo a consultar: ");
				char s[50];
				scanf("\n%s",s);
			if(tr->cantidad_elementos>0){
            	int k=porcentajePrefijo(tr,s);
            	printf("La palabra es prefijo del %d porciento de las palabras en el archivo\n" , k);
   			}
			else{
                printf("[NO SE PUEDE CALCULAR PORCENTAJE DE UN ARCHIVO VACIO]\n");
            }
            printf("\n");
			break;
		}


		case 6: {

						//LIBERACION DE MEMORIA
						if(tr_size(tr)>0){
							hijos=tr->raiz->hijos;
							pos=lo_primera(hijos);

							while(pos!=POS_NULA){
								ev_liberar_auxiliar(tr,pos->elemento);
								lo_eliminar(hijos,pos);
								pos=lo_siguiente(hijos,pos);
							}

						}

						free(tr->raiz);
						free(tr);

            fclose(archivo);
            printf("GRACIAS, VUELVA PRONTOS\n");
            printf("\n");
            exit(0);

		}

		default: {printf("[OPCION INCORRECTA,REINGRESE OPCION]\n");}


   }

        printf("opcion: %d \n" , opcion);

        printf("INGRESE LA OPCION DESEADA\n");


	} while(opcion!=0);

	if (opcion==0)
        printf("RESPETE LAS OPCIONES. SOLO NUMEROS DISTINTOS DE CERO. SE CIERRA EL PROGRAMA. \n");

    printf("\n");

    fclose(archivo);


	return 0;
}


