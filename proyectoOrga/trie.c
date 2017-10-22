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
		//Busca y retona la posicion del nodo en la lista de hijos de su padre.
		
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
		/*
		CASO 1: Se ingresa una palabra que no está en el TRIE.
		CASO 1A: Se ingresa una palabra que tienen un prefijo (ejemplo: se ingresa holanda y está hola).
		CASO 1B: Se ingresa una palabra que es prefija de otra palabra ya ingresada en el TRIE. (ejemplo: se ingresa hola y ya está holanda).
		CASO 2: Se ingresa una palabra que ya se encuentra en el TRIE.		
		*/
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
  	      	nuevaLista=crear_lista_ordenada(f_comp);
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

/*
int main(){

    f_comp=f_comparador;
    char* p=malloc(sizeof(char));
    char str[200] = {"hola"};
    char str1[200] = {"holanda"};
    TNodo n1;
		TNodo point;

    printf("<<<<TESTER TRIE>>>>\n");
    printf("\n");
    printf("<TRIE VACIO>\n");
	
        TTrie trie= malloc(sizeof(struct trie));
        trie->cantidad_elementos=0;
        TNodo root= malloc(sizeof(struct nodo));
        root->contador=0;
        TListaOrdenada lista_hijos=crear_lista_ordenada(f_comp);
        root->hijos=lista_hijos;
        root->padre=NULL;
        trie->raiz=root;
				point=trie->raiz;

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

		printf("<<SE INSERTA <hola> >>>\n"); 	
	
        if(tr_insertar(trie,str)==TRUE)  printf("Insertar funciona con hola\n");
        else printf("Insertar no funciona\n");
        printf("Size: %d \n" , tr_size(trie));
	
	
		printf("\n");
    printf("\n");
    printf("TRIE: ");
		
    while(lo_size(point->hijos)>0){
        printf(" [%c] " , ((TNodo)(lo_primera(point->hijos)->elemento))->rotulo);
        point= ((TNodo)(lo_primera(point->hijos)->elemento));
    }
    printf("\n");
    printf("\n");
    printf("\n");

        if(tr_pertenece(trie,str)==TRUE)  printf("hola no pertenece al Trie en éste punto \n");
        else printf("Pertenece no funciona con hola\n");

        n1=tr_buscar_auxiliar(trie,str);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto. retorno NULL \n");

        printf("String: %s \n" , str);
        printf("Recuperar: %d \n" , tr_recuperar(trie,str));
				printf("<<SE ELIMINA <hola> >>>\n");
        printf("Resultado de eliminar: %d \n" , tr_eliminar(trie,str));
				printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("<TRIE CON 2 ELEMENTO, UNO PREFIJO DE OTRO>\n");

				printf("<<SE INSERTA <holanda> >>>\n");
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
    
				point=trie->raiz;
        while(lo_size(point->hijos)>0){
        printf(" [%c] " , ((TNodo)(lo_primera(point->hijos)->elemento))->rotulo);
        point= ((TNodo)(lo_primera(point->hijos)->elemento));
    }

				printf("\n");
        printf("\n");
	
		//Vuelvo a insertar hola
		
				printf("<<SE VUELVE A INSERTA <hola> >>>\n");
				if(tr_insertar(trie,str)==TRUE)  printf("Insertar funciona con hola\n");
        else printf("Insertar no funciona\n");
        printf("Size: %d \n" , tr_size(trie));

        printf("Recuperar de hola: %d \n" , tr_recuperar(trie,str));
        printf("Recuperar de holanda: %d \n" , tr_recuperar(trie,str1));

        if(tr_pertenece(trie,str)==TRUE)  printf("Pertenece funciona con hola\n");
        else printf("Pertenece no funciona\n");

        n1=tr_buscar_auxiliar(trie,str);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto. retorno NULL \n");
	
	
        //INGRESO holandesas
        char str2[100] = {"holandesas"};
	
				printf("<<SE INSERTA <holandesas> >>>\n");
    
	    	if(tr_insertar(trie,str2)==TRUE)  printf("Insertar funciona con holandasas\n");
        else printf("Insertar no funciona\n");
        
				printf("Size: %d \n" , tr_size(trie));
				
				if(tr_pertenece(trie,str2)==TRUE)  printf("Pertenece funciona con holandasas\n");
      	else printf("Pertenece no funciona con holandasas\n");
				
    		n1=tr_buscar_auxiliar(trie,str2);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto. retorno NULL \n");
	
				char str3[] = {"otro"};
	
		printf("\n");
    printf("\n");
		printf("<<SE INGRESA <otro> >>>");
		printf("\n");
    printf("\n");
	
				if(tr_insertar(trie,str3)==TRUE)  printf("Insertar funciona con otro\n");
        else printf("Insertar no funciona con otro\n");
        
				printf("Size: %d \n" , tr_size(trie));
				
				if(tr_pertenece(trie,str3)==TRUE)  printf("Pertenece funciona con otro\n");
        else printf("Pertenece no funciona con otro\n");
				
				n1=tr_buscar_auxiliar(trie,str3);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto con otro. retorno NULL \n");
					 
			
   	printf("\n");
    printf("\n");
    printf("<<SE INGRESA <otrosos> >>>");        
    
        char str4[] = {"otrosos"};
    
    printf("\n");
    printf("\n");
	
				if(tr_insertar(trie,str4)==TRUE)  printf("Insertar funciona con otrosos\n");
        else printf("Insertar no funciona con otrosos\n");
        
				printf("Size: %d \n" , tr_size(trie));
				
				if(tr_pertenece(trie,str4)==TRUE)  printf("Pertenece funciona con otrosos\n");
        else printf("Pertenece no funciona con otrosos\n");
				
				n1=tr_buscar_auxiliar(trie,str4);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto con otrosos. retorno NULL \n");

    printf("\n");
    printf("\n");
    printf("<<SE INGRESA <rasca> >>>");        
    
        char str5[] = {"rasca"};
    
    printf("\n");
    printf("\n");
	
		if(tr_insertar(trie,str5)==TRUE)  printf("Insertar funciona con rasca\n");
        else printf("Insertar no funciona con rasca\n");
        
		printf("Size: %d \n" , tr_size(trie));
				
		if(tr_pertenece(trie,str5)==TRUE)  printf("Pertenece funciona con rasca\n");
        else printf("Pertenece no funciona con rasca\n");
				
		n1=tr_buscar_auxiliar(trie,str5);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto con rasca. retorno NULL \n");

    printf("\n");
    printf("\n");
        printf("<<SE INGRESA <rascador> >>>");  
        char str6[] = {"rascador"};
    
    printf("\n");
    printf("\n");
	
		if(tr_insertar(trie,str6)==TRUE)  printf("Insertar funciona con rascador\n");
        else printf("Insertar no funciona con rasca\n");
        
		printf("Size: %d \n" , tr_size(trie));
				
		if(tr_pertenece(trie,str6)==TRUE)  printf("Pertenece funciona con rascador\n");
        else printf("Pertenece no funciona con rascador\n");
				
		n1=tr_buscar_auxiliar(trie,str6);
        if(n1!=NULL) printf("tr_buscar_auxiliar correcto. Elemento: %c , Contador: %d\n" , n1->rotulo ,n1->contador);
        else printf("tr_buscar_axuliar incorrecto con rascador. retorno NULL \n");
    
    printf("\n");
    printf("\n");
    
        //SE ELIMINA rascador
        printf("<<SE ELIMINA <rascador> >>>\n");  
        printf("Resultado de eliminar rascador: %d \n" , tr_eliminar(trie,str6));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
    
        //SE ELIMINA otro
        printf("<<SE ELIMINA <otro> >>>\n"); 
        printf("Resultado de eliminar otro: %d \n" , tr_eliminar(trie,str3));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
    
        //SE ELIMINA otrosos
        printf("<<SE ELIMINA <otrosos> >>>\n"); 
        printf("Resultado de eliminar otrosos: %d \n" , tr_eliminar(trie,str4));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
    
        //SE ELIMINA holanda
        printf("<<SE ELIMINA <holanda> >>>\n"); 
				if(tr_pertenece(trie,str1)==TRUE)  printf("holanda pertence en esta parte\n");
        else printf("holanda no pertenece en esta parte\n");
        printf("Resultado de eliminar holanda: %d \n" , tr_eliminar(trie,str1));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
	
				
        //SE ELIMINA holandesas
        printf("<<SE ELIMINA <holandesas> >>>\n"); 
				if(tr_pertenece(trie,str2)==TRUE)  printf("holandesas pertence en esta parte\n");
        else printf("holandesas no pertenece en esta parte\n");
        printf("Resultado de eliminar holandesas: %d \n" , tr_eliminar(trie,str2));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
        
        //SE ELIMINA hola
        printf("<<SE ELIMINA <hola> >>>\n"); 
				if(tr_pertenece(trie,str)==TRUE)  printf("hola pertence en esta parte\n");
        else printf("hola no pertenece en esta parte\n");
        printf("Resultado de eliminar hola: %d \n" , tr_eliminar(trie,str));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
	
				//SE ELIMINA rasca
        printf("<<SE ELIMINA <rasca> >>>\n"); 
				if(tr_pertenece(trie,str5)==TRUE)  printf("rasca pertence en esta parte\n");
        else printf("hola no pertenece en esta parte\n");
        printf("Resultado de eliminar rasca: %d \n" , tr_eliminar(trie,str5));
	    	printf("Size: %d \n" , tr_size(trie));
	
    printf("\n");
    printf("\n");
	
				if(tr_pertenece(trie,str)==TRUE)  printf("hola pertence en esta parte\n");
        else printf("hola no pertenece en esta parte\n");
	
		printf("\n");
    printf("\n");
    printf("<<TEST DE ORDEN HIJOS>>\n");
		printf("\n");
		
			char s3[]={"benito"};
			char s1[]={"ayuda"};
			char s4[]={"carlitos"};
			char s5[]={"diegito"};
			char s2[]={"ayudame"};
		
			tr_insertar(trie,s1);
			tr_insertar(trie,s2);
			tr_insertar(trie,s3);
			tr_insertar(trie,s4);
			tr_insertar(trie,s5);
		
			TListaOrdenada hijos = trie->raiz->hijos;
			int size=lo_size(hijos);
			TPosicion pu = lo_primera(hijos);
	
			while(size>0){
				printf(" [%c] " , ((TNodo)(pu->elemento))->rotulo);
				pu=lo_siguiente(hijos,pu);
				size--;
			}
		
		printf("\n");
    printf("\n");
	
    return 0;
}

*/




