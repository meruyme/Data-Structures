#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct palavra{
    string palavra = "";
    struct palavra **filhos;
} palavra;

palavra raiz;
string *vetor;
int vetor_ultimo = 0;
int tamanho_arvore;

void add(palavra **x,string nome,uint32_t d){
    if(*x == 0){
        *x = new palavra;
        (*x)->filhos = new palavra*[26]();
    }
    if(d == nome.length()) {
        (*x)->palavra = nome;
    }
    else{
        int p_int = nome[d];
        add(&(*x)->filhos[p_int-97],nome,d + 1) ;
    }
}

void prefixos(palavra* x,uint32_t d,uint32_t quant){
    int cont = 0;
    if(x != 0){
        if(d < quant){
            for(int i=0;i<26;i++){
                cont = 0;
                if(x->filhos[i] != 0){
                    if(x->filhos[i]->palavra != ""){
                        for(int j=0;j<vetor_ultimo;j++){
                            if(x->filhos[i]->palavra == vetor[j]){
                                cont++;
                                break;
                            }
                        }
                        if(cont == 0){
                            vetor[vetor_ultimo] = x->filhos[i]->palavra;
                            vetor_ultimo++;
                        }
                    }
                    prefixos(x->filhos[i],d+1,quant);
                }
            }
        }
    }
}

void busca(palavra *x,string nome,uint32_t d) {
    if(x != 0){
        if(d <= nome.length()){
            prefixos(x,d,d*2);
            int p_int = nome[d];
            busca(x->filhos[p_int-97],nome,d+1);
        }
    }
}

int main(int argc, char* argv[]){
    int tamanho_busca;
    string pala;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    input >> tamanho_arvore;

    vetor = new string[tamanho_arvore];
    raiz.filhos = new palavra*[26]();

    for(int i=0;i<tamanho_arvore;i++){
        input >> pala;
        //cout << pala.length() << endl;
        int p_int = pala[0];
        add(&raiz.filhos[p_int-97],pala,1);
    }

    input >> tamanho_busca;

    for(int i=0;i<tamanho_busca;i++){
        input >> pala;
        int p_int = pala[0];
        busca(raiz.filhos[p_int-97],pala,1);
        output << pala << ":";
        for(int j=0;j<vetor_ultimo;j++){
            if(j == 0){
                output << vetor[j];
            }
            else{
                output << "," << vetor[j];
            }
        }
        if(vetor_ultimo == 0){
            output << "-";
        }
        output << "\n";
        vetor_ultimo = 0;
    }

    input.close();
    output.close();

 return 0;
}
