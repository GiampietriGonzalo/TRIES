#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
//#include "lista_ordenada.h" TESTAR

#define POS_NULA NULL
#define ELE_NULO NULL
#define FALSE 0
#define TRUE 1
#define LST_NO_INI 2
#define LST_POS_INI 3
#define LST_VAC 4
#define TRI_NO_INI 5
#define STR_NO_PER -1


//int (* f_comp)(void *, void *);  TESTEAR

int f_comparador(void* v1, void* v2){

    TNodo n1=v1;
    TNodo n2=v2;
    
		char c1= n1->rotulo;
    char c2= n2->rotulo;
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

				//CASO ESPECIAL: EL TNODO padre ES LA RAIZ DEL TRIE
        if(padre==tr->raiz){
            nodoHijo=tr_recuperarHijo_auxiliar(padre,aux);
                if(nodoHijo!=NULL) //Se encontro al nodo hijo que tiene el caracter s
                    resultado=tr_pertenece_auxiliar(nodoHijo,aux,longitud,tr);

        }
				
        if(*aux==padre->rotulo && longitud>0){
            aux++;
            nodoHijo=tr_recuperarHijo_auxiliar(padre,aux);

                if(nodoHijo!=NULL) //Se encontro al nodo hijo que tiene el caracter s
                    resultado=tr_pertenece_auxiliar(nodoHijo,aux,longitud-1,tr);


            }
          }

    return resultado;
}

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


        if(parar==2) //No se encontro el nodo.
            nodoActual=NULL;
    }

    return nodoActual;
}

TPosicion tr_recuperarPos_auxiliar(TNodo nodo){
		//Busca y retorna la posicion del nodo en la lista de hijos de su padre.
		
		TListaOrdenada hermanos = nodo->padre->hijos; 
    TPosicion aux;
		aux=lo_primera(hermanos);
    
		while(aux->elemento!=nodo)
        aux=lo_siguiente(hermanos,aux);
    
	return aux;
}

int tr_eliminar_auxiliar(TTrie tr, TNodo ultimo, int long original , int longitud){
		/* 
		Retorna TRUE si se ha eliminado la palabra con exito, FALSE en caso contrario.
		En el caso de que la palabra esté repetida o sea prefijo de otra, se descuenta 
		el contador en 1 y se retorna TRUE.
		
		TNodo ultimo: Representa al último nodo en el trie de la palabra pasada.
		*/		
		
		TListaOrdenada hijos=ultimo->hijos;
		int resultado=FALSE;
		TNodo padre= ultimo->padre;
		TPosicion posNodo;
		TListaOrdenada hermanos;
	
	
		if(padre==tr->raiz){
			//Eliminacion completa			
			hermanos=padre->hijos;
			posNodo=tr_recuperarPos_auxiliar(ultimo);
			free(hijos);
			lo_eliminar(hermanos,posNodo);
		
			return TRUE;
		}
		
		if (lo_size(hijos)==0){
			
				if(ultimo->contador==1){
					
					if (longitud==original){
						ultimo->contador--;
						hermanos=padre->hijos;
						posNodo=tr_recuperarPos_auxiliar(ultimo);
						free(hijos);
						lo_eliminar(hermanos,posNodo);
						longitud--;
						resultado=tr_eliminar_auxiliar(tr,padre,original,longitud);
					}
					else
						resultado=TRUE;
				}
				else				
					if(longitud>0){
						//Nodo hoja	
						hermanos=padre->hijos;
						posNodo=tr_recuperarPos_auxiliar(ultimo);
						free(hijos);
						lo_eliminar(hermanos,posNodo);
						longitud--;
						resultado=tr_eliminar_auxiliar(tr,padre,original,longitud);
					}
		}
		else{
			if(longitud>0 && ultimo->contador>0)
					ultimo->contador--;
			
			resultado=TRUE;		
		
		}	
					
		return resultado;
}



//FUNCIONES DE trie.h

TTrie crear_trie(){

    TTrie tri= malloc(sizeof(struct trie));
    TNodo node= malloc(sizeof(struct nodo));
    node->hijos=crear_lista_ordenada(&f_comparador);
    tri->raiz=node;
    tri->cantidad_elementos=0;

    return tri;
}



int tr_pertenece(TTrie tr, char* str){

		if(tr==NULL)
			exit(TRI_NO_INI);
	
    int longitud= strlen(str);
    int resultado=FALSE;
    TNodo primerNodo=tr_recuperarHijo_auxiliar(tr->raiz,str);

    if(primerNodo!=NULL)
        resultado=tr_pertenece_auxiliar(tr->raiz,str,longitud,tr);

    return resultado;
}

int tr_recuperar(TTrie tr, char* str){

		if(tr==NULL)
			exit(TRI_NO_INI);
	
    int resultado= STR_NO_PER;
    TNodo nodo=tr_buscar_auxiliar(tr,str);
    if (nodo!=NULL)
        resultado=nodo->contador;

    return resultado;
}

int tr_insertar(TTrie tr, char* str){
		/*
		CASO 1: Se ingresa una palabra que no está en el TRIE.
		CASO 1A: Se ingresa una palabra que tienen un prefijo (ejemplo: se ingresa holanda y está hola).
		CASO 1B: Se ingresa una palabra que es prefija de otra palabra ya ingresada en el TRIE. (ejemplo: se ingresa hola y ya está holanda).
		CASO 2: Se ingresa una palabra que ya se encuentra en el TRIE.		
		*/
		
		if(tr==NULL)
			exit(TRI_NO_INI);
	
		char* aux=str;
		TNodo nuevo,nodo,padre;
		int resultado=FALSE;
		int longitud=strlen(str);
		TListaOrdenada nuevaLista,hijos;
		padre=tr->raiz;
	
	
		if(longitud>0 && tr_pertenece(tr,str)){ //CASO 2
			(tr_buscar_auxiliar(tr,aux))->contador++;
			resultado=TRUE;
		  tr->cantidad_elementos++;
		}
		else{
			//CASO 1, 1A O 1B
			if(longitud>0){
				
				padre=tr->raiz;
				nodo=tr_recuperarHijo_auxiliar(padre,aux);
		
		
				while(longitud>1 && nodo!=NULL){
				
					/*Se desciende en el Trie a través de una palabra ingresada anteriormente cuyas letras
					coinciden con las de la palabra a ingresar*/
					aux++;
					longitud--;
					padre=nodo;
					nodo=tr_recuperarHijo_auxiliar(padre,aux);
				}
				
				if(longitud==1 && nodo!=NULL){
					
					if(nodo->rotulo==*aux)
							//CASO 1B - La palabra a ingresar es prefija de otra más larga.
							nodo->contador++;
					else{
				
						//La palabra a insertar comparte todos sus caracteres excepto el último con otra palabra 
						nuevo=malloc(sizeof(struct nodo));
						nuevo->rotulo=*aux;
						nuevo->padre=padre;
						nuevo->contador=1;
						padre->contador++;
						tr->cantidad_elementos++;
						hijos=padre->hijos;
						lo_insertar(hijos,nuevo);
					}
				}	
				
				
				if(nodo==NULL){ //CASO 1 
			
					while(longitud>0){
						
						nuevo=malloc(sizeof(struct nodo));
  	      	nuevaLista=crear_lista_ordenada(f_comparador);
    	    	nuevo->hijos=nuevaLista;
      	  	nuevo->rotulo=*aux;
        		nuevo->padre=padre;
						hijos=padre->hijos;
						
			
						if(longitud==1)
          		  nuevo->contador=1;
        		else
							nuevo->contador=0;
					
        		lo_insertar(hijos,nuevo); 
						
						
        		aux++;
        		longitud--;
						
						
						padre=nuevo;
        		hijos=nuevaLista;
					}	
			}	
			resultado=TRUE;
			tr->cantidad_elementos++;	
		}
	}
	return resultado;
}


int tr_size(TTrie tr){return tr->cantidad_elementos;}


int tr_eliminar(TTrie tr,char* str){

	if(tr==NULL)
			exit(TRI_NO_INI);
	
	
	int resultado=FALSE;
	TNodo ultimo=NULL;
	
	if(tr_pertenece(tr,str)==TRUE){
		ultimo=tr_buscar_auxiliar(tr,str);
	}		
	if(ultimo!=NULL)
		resultado=tr_eliminar_auxiliar(tr,ultimo,strlen(str),strlen(str));
	
	if(resultado==TRUE) tr->cantidad_elementos--;
	
	return resultado;
}