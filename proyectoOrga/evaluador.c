#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "lista_ordenada.c"
#include "trie.c"
#define FALSE 0
#define TRUE 1


//IMPLEMENTACIÓN INCOMPLETA :( - LAS POCAS FUNCIONES IMPLEMENTADAS NO ESTAN TESTEADAS

int (* f_comp)(void *, void *);


//FUNCIONES AUXILIARES

TNodo ev_buscar_auxiliar(TTrie tr,char* str){
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


        if(parar==2) //No se encontro el nodo.
            nodoActual=NULL;
    }

    return nodoActual;
}


//FUNCIONES DE EVALUADOR

void mostrarPalabras(TTrie tr){
	
	
	
}

int consultar(TTrie tr,char* str){
	int i=tr_pertenece(tr,str);
	int cant=0;
	if(i==TRUE)
		cant=tr_recuperar(tr,str);
	
	return cant;
}


int comienzaConAux(TNodo nodito,char c){
	int cont=0;
	if(lo_size(nodito->hijos)==0){
		cont=1;
	}
	else{
		if(nodito->contador>0) 
			cont+=1;
		TPosicion aux=lo_primera(nodito->hijos);
		while(aux!=NULL){
			cont+=comienzaConAux(aux->elemento,c);
			aux=lo_siguiente(nodito->hijos,aux);
			
		}
	}
	return cont;
}

int comienzaCon(TTrie tr,char c){
	//Retorna la cantidad de palabras que comienzan con la letra dada.
	int i=0;
	char* ch=malloc(sizeof(char));
	*ch=c;
	TNodo aux=tr_recuperarHijo_auxiliar(tr->raiz,ch);
	
	if(aux!=NULL)
		i=comienzaConAux(aux,c);
	
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
	
}


int main(char *argv[]) {
	
	f_comp=f_comparador;
	
	FILE* archivo=fopen("a.txt","r+");
	int caracter;
	char palabra[50];
	int n=0;
	int seguir=1;
	TTrie tr=crear_trie();
	
	if(archivo!=NULL)
		printf("Se abrio el archivo\n");
	else
		printf("NO se abrio el archivo\n");
	
	/*
	printf("El contenido del archivo de prueba es \n");
  while((caracter = fgetc(archivo)) != EOF){
		printf("%c",caracter);
	}
	*/
	printf("\n");
	
	caracter = fgetc(archivo);
	char* puntero;
	
	//REIMPLEMENTAR ESTA PARTE PARA QUE SOLO ADMITA CIERTOS CARACTERES
	
	while(seguir==1 && caracter!= EOF){
		
		n=0;
		seguir=1;
		puntero=palabra;
		
		while(*puntero!='\0'){
			palabra[n]=NULL;
			puntero++;
			n++;
		}	
		
		n=0;
		
		while(seguir==1 && caracter!=32){
				palabra[n]=caracter;
				caracter = fgetc(archivo);
				n++;
				//printf("caracter: %c \n" , caracter);
				if (caracter == EOF || caracter=='\n' || caracter==32){
						seguir=0;
						//printf("entro");
				}
		}
		if (caracter==32 && caracter!=EOF){
				caracter = fgetc(archivo);
				//printf("palabra: %c , longitd: %d \n" , palabra[0] , strlen(palabra));
				//printf("caracter int: %d \n" , caracter);
				seguir=1;
		}
		else
			if(caracter=='\n')
				seguir=0;
		
		puntero=palabra;
		
		while(*puntero!='\0'){
			printf("[%c] " , *puntero);
			puntero++;
		}
		
		printf("\n");
		puntero=palabra;
		
		tr_insertar(tr,puntero);
	
	}
	char strho[]={"holanda"};
	printf("\n");
	printf("recuperar de holanda: %d" , tr_recuperar(tr,strho));
	printf("\n");
	
	int opcion=0;
	
		
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
				
		case 1:
			
			break;
		
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
			//int j=esPrefijo(tr->raiz,s,strlen(s));
			int k=esPrefijo(tr,s);
			if(k==1) printf("La palabra es prefijo de alguna palabra en el archivo\n");
			else printf("La palabra no es prefijo de ninguna plabra, o no se encuentra en el archivo\n");
			break;
		}
		case 5:
			
			break;
		case 6:
				
			exit(0);
			break;
		}
	
	} while(opcion!=6);
	
	return 0;
}
