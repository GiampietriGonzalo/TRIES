#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_ordenada.h"
#include "trie.h"
#define FALSE 0
#define TRUE 1


int (* f_comp)(void *, void *);



//FUNCIONES AUXILIARES

void vaciar(char c[]){
    int i=0;
	while(c[i]!='\0'){
		c[i]='\0';
		i++;
	}

}


int cant_prefijos_auxiliar(TNodo nodo){

    int cont=nodo->contador;
    TPosicion aux;
    if(lo_size(nodo->hijos)>0){
        aux=lo_primera(nodo->hijos);

        while(aux!=NULL){
            cont+=cant_prefijos_auxiliar((TNodo)aux->elemento);
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

void imprimir(TTrie tr , char c[]){


	TNodo ultimo=ev_buscar_auxiliar(tr,c);
	printf("%s (%d)" , c,ultimo->contador);

	printf("\n");
}

void ev_mostrar_auxiliar(TTrie tr,char buffer[], TNodo nodo){


	TPosicion hijo;
	char * auxchar = malloc(sizeof(char));
	char auxiliar[70]={""};

	buffer[strlen(buffer)]=nodo->rotulo;

	//OBSERVACION: strcat(buffer,rotulo); (funcion strcat de la libreria string.h) no funciona, al menos en la maquina virtual.
	int n=0;

	if (nodo->contador > 0){

		imprimir(tr,buffer);

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

	char buffer[50]={""};
	TPosicion hijo=lo_primera(tr->raiz->hijos);

	while(hijo!=NULL){
		vaciar(buffer);
		ev_mostrar_auxiliar(tr,buffer,hijo->elemento);
		hijo=lo_siguiente(tr->raiz->hijos,hijo);

	}
}

int consultar(TTrie tr,char* str){
	int i=tr_pertenece(tr,str);
	int cant=0;
	if(i==TRUE)
		cant=tr_recuperar(tr,str);

	return cant;
}




int comienzaCon(TTrie tr,char c){
	//Retorna la cantidad de palabras que comienzan con la letra dada.

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

	int res=FALSE;
	TNodo ultimo=NULL;
	if(tr_pertenece(tr,str))
		ultimo=ev_buscar_auxiliar(tr,str);

	if(ultimo!=NULL && lo_size(ultimo->hijos)>0)
		res=TRUE;


	return res;
}



int porcentajePrefijo(TTrie tr,char* str){

    TNodo nodo=ev_buscar_auxiliar(tr,str);
    int cont=0;
    if(nodo!=NULL)
        cont=cant_prefijos_auxiliar(nodo);

    return (cont*100)/tr_size(tr);

}

int main(int i, char *argv[])  {

    if(argv[1]=='\0'){
        printf("NO SE HA INGRESADO EL NOMBRE DE ARCHIVO\n");
        exit(0);
    }

	f_comp=f_comparador;

	FILE* archivo=fopen(argv[1],"r+");
	int caracter;
	char palabra[50];
	int n=0;
	int seguir=1;
	TTrie tr=crear_trie();

	if(archivo!=NULL)
		printf("Se abrio el archivo\n");
	else
		printf("NO se abrio el archivo\n");

	printf("\n");


	char* puntero;

	//REIMPLEMENTAR ESTA PARTE PARA QUE SOLO ADMITA CIERTOS CARACTERES
	caracter = fgetc(archivo);
	while(seguir==1 && caracter!= EOF){

		n=0;
		seguir=1;
		puntero=palabra;

		while(*puntero!='\0'){
			*puntero='\0';
			puntero++;
			n++;
		}

		n=0;

		while(seguir==1 && caracter!=32){

				palabra[n]=caracter;
				caracter = fgetc(archivo);
				n++;

				if (caracter == EOF || caracter=='\n' || caracter==32)
						seguir=0;

		}

		if (caracter==32 && caracter!=EOF){
				caracter = fgetc(archivo);
				seguir=1;
		}


		printf("%s \n" , palabra);
		puntero=palabra;

		tr_insertar(tr,puntero);

	}


	int opcion=0;
    printf("\n");

	do {

		printf("1.Mostrar palabras\n");
		printf("2.Consultar\n");
		printf("3.Comienza con\n");
		printf("4.Es prefijo\n");
		printf("5.Porcentaje prefijo\n");
		printf("6.Salir\n");

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

			break;
		}

		case 3:{
			char c;
			int i;
			printf("Ingrese una letra:\n");
			scanf("\n %c",&c);
			i=comienzaCon(tr,c);
			printf("Hay %d palabras que comienzan con la letra %c \n",i,c);

			break;
		}

		case 4:{
			printf("Ingrese la palabra a consultar: ");
			char s[50];
			scanf("\n%s",s);
			int k=esPrefijo(tr,s);
			if(k==1) printf("La palabra es prefijo de alguna palabra en el archivo\n");
			else printf("La palabra no es prefijo de ninguna plabra, o no se encuentra en el archivo\n");
			break;
		}

		case 5:{
			if(tr->cantidad_elementos>0){
				printf("Ingrese el prefijo a consultar: ");
				char s[50];
				scanf("\n%s",s);
            	int k=porcentajePrefijo(tr,s);
            	printf("La palabra es prefijo del %d porciento de las palabras en el archivo\n" , k);
            	printf("\n");
   			}	
   			
			else printf("No puedo calcular porcentaje de un archivo vacio");
			break;
		}


		case 6: {fclose(archivo); exit(0); }

		default: {fclose(archivo); exit(0);}

        }



	} while(opcion!=6);

    fclose(archivo);

	return 0;
}


