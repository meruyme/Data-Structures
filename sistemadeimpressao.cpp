#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;


typedef struct documento{

    string nome;
    int paginas;
    int faltam;
    struct documento *depois_pilha;
    struct documento *depois_fila;

} documento;

typedef struct impressora{

    string nome;
    struct impressora *depois;
    documento *imprimir;
    string historico;
} impressora;

typedef struct pilha{

    documento *topo;

} pilha;

typedef struct fila_doc{

    int tamanho;
    documento *inicio;
    documento *fim;

} fila_doc;

typedef struct fila_imp{

    int tamanho;
    impressora *inicio;
    impressora *fim;

} fila_imp;

void inicializar(fila_doc* doc, fila_imp* imp, pilha* p);

void preencher_impressoras(fila_imp* imp, fila_doc* doc,ofstream* output);

void imprimir_paginas(fila_imp* imp, fila_doc* doc, pilha* p, ofstream* output);

void mostrar_historico(impressora* imp, ofstream* output);

void adicionar_pilha(pilha* p, documento* doc);

void incrementar_historico(impressora* imp);

void adicionar_imp(string n, fila_imp* imp);

void adicionar_doc(string n, int pag, fila_doc* doc);

void mostrar_pilha(pilha* p, ofstream* output,int soma_total);


int main(int argc, char* argv[]){


    fila_imp* imp = new fila_imp;
    pilha* p = new pilha;
    fila_doc* doc = new fila_doc;
    int pag, soma_total = 0;
    string nome;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    inicializar(doc,imp,p);

        input >> imp->tamanho;
        input.ignore();
        for(int i=0;i<imp->tamanho;i++){
            getline(input,nome);
            adicionar_imp(nome,imp);
        }
        input >> doc->tamanho;
        input.ignore();

        for(int i=0;i<doc->tamanho;i++){
            input >> nome;
            input >> pag;
            adicionar_doc(nome,pag,doc);
            soma_total += doc->fim->paginas;
        }

    preencher_impressoras(imp, doc, &output);
    imprimir_paginas(imp,doc,p,&output);
    mostrar_pilha(p, &output, soma_total);

    input.close();
    output.close();

 return 0;
}

void inicializar(fila_doc* doc, fila_imp* imp, pilha *p){
    doc->inicio = NULL; doc->fim = NULL;
    p->topo = NULL;
    imp->inicio = NULL; imp->fim = NULL;
}

void preencher_impressoras(fila_imp* imp, fila_doc* doc,ofstream* output){
    impressora* aux_imp = imp->inicio;
    for(int i=0;i<imp->tamanho;i++){
        aux_imp->imprimir = doc->inicio;
        incrementar_historico(aux_imp);
        mostrar_historico(aux_imp,output);
        doc->inicio = doc->inicio->depois_fila;
        if(doc->inicio != NULL){
            aux_imp = aux_imp->depois;
        }
        else{
            break;
        }
    }

}

void imprimir_paginas(fila_imp* imp, fila_doc* doc, pilha* p, ofstream* output){
    impressora* aux_imp = imp->inicio;
    int controle = doc->tamanho;
    while(controle != 0){
        if(aux_imp->imprimir != NULL){
            aux_imp->imprimir->faltam--;
            if(aux_imp->imprimir->faltam == 0){
                adicionar_pilha(p,aux_imp->imprimir);
                if(doc->inicio != NULL){
                    aux_imp->imprimir = doc->inicio;
                    incrementar_historico(aux_imp);
                    doc->inicio = doc->inicio->depois_fila;
                    mostrar_historico(aux_imp,output);
                }
                controle--;
            }
        }
        aux_imp = aux_imp->depois;
    }

}

void mostrar_historico(impressora* imp, ofstream* output){
    *output << "[" << imp->nome << "] " << imp->historico << "\n";
}

void adicionar_pilha(pilha* p, documento* doc){
    if(p->topo == NULL){
        p->topo = doc;
        doc->depois_pilha = NULL;
    }
    else{
        doc->depois_pilha = p->topo;
        p->topo = doc;
    }
}

void incrementar_historico(impressora* imp){
    stringstream pag_string;
    pag_string << imp->imprimir->paginas;
    if(imp->historico == ""){
        imp->historico = imp->imprimir->nome + "-" + pag_string.str() + "p";
    }
    else{
        imp->historico = imp->imprimir->nome + "-" + pag_string.str() + "p, " + imp->historico;
    }
}

void adicionar_imp(string n, fila_imp* imp){
    impressora* i;

    if(imp->inicio == NULL){
        i = new impressora;
        i->nome = n;
        i->depois = i;
        i->imprimir = NULL;
        i->historico = "";
        imp->inicio = i;
        imp->fim = imp->inicio;
    }
    else{
        i = new impressora;
        i->nome = n;
        i->depois = imp->inicio;
        i->imprimir = NULL;
        i->historico = "";
        imp->fim->depois = i;
        imp->fim = i;
    }

}

void adicionar_doc(string n, int pag, fila_doc* doc){
    documento* d;

    if(doc->inicio == NULL){
        d = new documento;
        d->nome = n;
        d->paginas = pag;
        d->faltam = pag;
        d->depois_pilha = NULL;
        doc->inicio = d;
        doc->fim = doc->inicio;
        d->depois_fila = NULL;

    }
    else{
        d = new documento;
        d->nome = n;
        d->paginas = pag;
        d->faltam = pag;
        d->depois_pilha = NULL;
        doc->fim->depois_fila = d;
        d->depois_fila = NULL;
        doc->fim = d;
    }

}

void mostrar_pilha(pilha* p, ofstream* output,int soma_total){
    documento* aux_p = p->topo;
    *output << soma_total << "p\n";
    do{
        *output << aux_p->nome << "-" << aux_p->paginas << "p\n";
        aux_p = aux_p->depois_pilha;
    }while(aux_p != NULL);
}
