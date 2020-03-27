#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct pessoas{

    char nome[51];
    struct pessoas *antes;
    struct pessoas *depois;

} pessoas;

pessoas* inicio = NULL;
pessoas* fim = NULL;

void adicionar(char* n, FILE* output){
    pessoas* p;
    int cont = 0;

    if(inicio == NULL){
        p = (pessoas*) malloc(sizeof(pessoas));
        strcpy(p->nome, n);
        inicio = p;
        fim = inicio;
        p->antes = inicio;
        p->depois = inicio;

        fprintf(output,"[ OK  ] ADD %s\n",p->nome);
    }
    else{

        pessoas* aux = inicio;
        do{
            if(strcmp (aux->nome, n) == 0){
                
                cont++;
                break;
                
            }
            aux = aux->depois;
        }while(aux != inicio);

        if(cont > 0){
            fprintf(output,"[ERROR] ADD %s\n",n);
        }
        else{
            p = (pessoas*) malloc(sizeof(pessoas));
            strcpy(p->nome, n);
            p->antes = fim;
            p->depois = inicio;
            p->antes->depois = p;
            fim = p;
            inicio->antes = fim;

            fprintf(output,"[ OK  ] ADD %s\n",p->nome);
        }

    }

}

void remover(char* n, FILE* output){

    pessoas* aux = inicio;
    pessoas* p;
    int cont = 0;

        do{
            if(aux == NULL){
                break;
            }
            if(strcmp (aux->nome, n) == 0){
                if(aux == inicio){
                    inicio = aux->depois;
                }
                if(aux == fim){
                    fim = aux->antes;
                }
                if(aux == inicio && aux == fim){
                    inicio = NULL;
                    fim = NULL;
                }
                p = aux;
                aux->antes->depois = aux->depois;
                aux->depois->antes = aux->antes;
                free(p);
                fprintf(output,"[ OK  ] REMOVE %s\n",n);
                cont++;
                break;
            }
            aux = aux->depois;
        }while(aux != inicio);

        if(cont == 0){
            fprintf(output,"[ERROR] REMOVE %s\n",n);
        }

}

void mostrar(char* n, FILE* output){
    pessoas* aux = inicio;
    int cont = 0;
        do{
            if(aux == NULL){
                break;
            }
            if(strcmp (aux->nome, n) == 0){
                fprintf(output,"[ OK  ] %s<-%s->%s\n",aux->antes->nome,aux->nome,aux->depois->nome);
                cont++;
                break;
            }
            aux = aux->depois;
        }while(aux != inicio);

        if(cont == 0){
            fprintf(output,"[ERROR] ?<-%s->?\n",n);
        }
}


int main(int argc, char* argv[]){

    char op[7], nome[51];
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

        while((fscanf(input,"%s %[^\n]s\n", op, nome))!=EOF ){
            if(strcmp (op, "ADD") == 0){
                adicionar(nome,output);
            }

            if(strcmp (op, "SHOW") == 0){
                mostrar(nome,output);
            }

            if(strcmp (op, "REMOVE") == 0){
                remover(nome,output);
            }
            fflush(stdin);
        }

    fclose(input);
    fclose(output);

 return 0;
}
