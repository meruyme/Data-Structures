#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct arquivo{
    int cont;
    string titulo;
    uint32_t tamanho;
    string tipo;
    struct arquivo *esquerda;
    struct arquivo *direita;
} arquivo;

arquivo* raiz = NULL;
int tamanho_arvore;

void add(string titulo, string tipo, uint32_t tamanho,int cont, arquivo* no){
    if(no == NULL){
        arquivo *p = new arquivo;
        p->cont = cont;
        p->titulo = titulo;
        p->tipo = tipo;
        p->tamanho = tamanho;
        p->esquerda = NULL;
        p->direita = NULL;
        raiz = p;
    }
    else{
        if(titulo.compare(no->titulo) == 0){
            if(no->tipo == "rw"){
                no->titulo = titulo;
                no->cont = cont;
                no->tamanho = tamanho;
                no->tipo = tipo;
            }
        }
        if(titulo.compare(no->titulo) < 0){
            if(no->esquerda == NULL){
                arquivo *p = new arquivo;
                p->cont = cont;
                p->titulo = titulo;
                p->tipo = tipo;
                p->tamanho = tamanho;
                p->esquerda = NULL;
                p->direita = NULL;
                no->esquerda = p;
            }
            else{
                add(titulo,tipo,tamanho,cont,no->esquerda);
            }
        }
        if(titulo.compare(no->titulo) > 0){
            if(no->direita == NULL){
                arquivo *p = new arquivo;
                p->cont = cont;
                p->titulo = titulo;
                p->tipo = tipo;
                p->tamanho = tamanho;
                p->esquerda = NULL;
                p->direita = NULL;
                no->direita = p;
            }
            else{
                add(titulo,tipo,tamanho,cont,no->direita);
            }
        }
    }
}

void preOrdem(arquivo* p, ofstream *output){
    if(p!=NULL){
        if(p->tamanho == 1){
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " byte\n";
        }
        else{
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " bytes\n";
        }
        preOrdem(p->esquerda,output);
        preOrdem(p->direita,output);
    }
}

void ordem(arquivo* p, ofstream *output){
    if(p!=NULL){
        ordem(p->esquerda,output);
        if(p->tamanho == 1){
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " byte\n";
        }
        else{
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " bytes\n";
        }
        ordem(p->direita,output);
    }
}

void posOrdem(arquivo* p, ofstream *output){
    if(p!=NULL){
        posOrdem(p->esquerda,output);
        posOrdem(p->direita,output);
        if(p->tamanho == 1){
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " byte\n";
        }
        else{
            *output << p->cont << " " << p->titulo << " " << p->tipo << " " << p->tamanho << " bytes\n";
        }
    }
}

int main(int argc, char* argv[]){

    uint32_t tamanho;
    string titulo, tipo;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    input >> tamanho_arvore;

    for(int i=0;i<tamanho_arvore;i++){
        input >> titulo >> tipo >> tamanho;
        add(titulo,tipo,tamanho,i,raiz);
    }

    output << "EPD:\n";
    ordem(raiz,&output);
    output << "PED:\n";
    preOrdem(raiz,&output);
    output << "EDP:\n";
    posOrdem(raiz,&output);


 return 0;
}
