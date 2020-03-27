#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct pessoa{

    string nome;
    int idade;
    int prioridade = 0;
    int chegada;

} pessoa;

typedef struct orgao{

    pessoa** fila;
    int ultimo_fila = 0;
    string nome;
    int atendentes;

}orgao;

orgao *org;
int tamanho_orgao, quant_pessoas;

void find_string(string linha, int i){
    int find_separator1 = linha.find("|");
    string sub_orgao = linha.substr(0,find_separator1);
    int find_separator2 = linha.find("|",find_separator1+1);
    string nome = linha.substr(find_separator1+1,find_separator2-find_separator1-1);
    string idade_string = linha.substr(find_separator2+1);
    int idade = atoi(idade_string.c_str());
    pessoa* p = new pessoa;
    p->nome = nome;
    p->idade = idade;
    if(idade >= 60){
        p->prioridade = 1;
    }
    p->chegada = i;
    for(int j=0;j<tamanho_orgao;j++){
        if(org[j].nome == sub_orgao){
            org[j].fila[org[j].ultimo_fila] = p;
            org[j].ultimo_fila++;
            break;
        }
    }
}

void heapify_maximo(uint32_t pos_orgao,uint32_t i, int tamanho_vetor) {
    uint32_t p = i;
    uint32_t e = 2*i + 1;
    uint32_t d = 2*i + 2;
    if(e < tamanho_vetor && org[pos_orgao].fila[e]->prioridade >= org[pos_orgao].fila[p]->prioridade){
        if(org[pos_orgao].fila[e]->prioridade > org[pos_orgao].fila[p]->prioridade){
            p = e;
        }
        if(org[pos_orgao].fila[e]->prioridade == org[pos_orgao].fila[p]->prioridade && org[pos_orgao].fila[e]->chegada < org[pos_orgao].fila[p]->chegada){
            p = e;
        }
    }
    if(d < tamanho_vetor && org[pos_orgao].fila[d]->prioridade >= org[pos_orgao].fila[p]->prioridade){
        if(org[pos_orgao].fila[d]->prioridade > org[pos_orgao].fila[p]->prioridade){
            p = d;
        }
        if(org[pos_orgao].fila[d]->prioridade == org[pos_orgao].fila[p]->prioridade && org[pos_orgao].fila[d]->chegada < org[pos_orgao].fila[p]->chegada){
            p = d;
        }
    }
    if(p!=i){
        pessoa* aux = org[pos_orgao].fila[i];
        org[pos_orgao].fila[i] = org[pos_orgao].fila[p];
        org[pos_orgao].fila[p] = aux;
        heapify_maximo(pos_orgao,p,tamanho_vetor);
    }
}

int main(int argc, char* argv[]){

    int cont;
    string linha, nome, atendidos = "";
    int atendentes;
    ifstream input;
    ofstream output;
    input.open("controledesenha_input.txt");
    output.open("controledesenha_output.txt");

    input >> tamanho_orgao;

    org = new orgao[tamanho_orgao];
    input.ignore();

    for(int i=0;i<tamanho_orgao;i++){
        input >> nome >> atendentes;
        org[i].nome = nome;
        org[i].atendentes = atendentes;
    }


    input >> quant_pessoas;
    input.ignore();
    cont = quant_pessoas;

    for(int i=0;i<tamanho_orgao;i++){
        org[i].fila = new pessoa*[quant_pessoas];
    }

    for(int i=0;i<quant_pessoas;i++){
        getline(input,linha);
        find_string(linha,i+1);
    }

    for(int i=0;i<tamanho_orgao;i++){
        for(int j=(org[i].ultimo_fila-1)/2;j>=0;j--){
            heapify_maximo(i,j,org[i].ultimo_fila);
        }
    }

    int i=0;
    while(cont != 0){
        for(int j=0;j<org[i].atendentes;j++){
            if(org[i].ultimo_fila != 0){
                if(atendidos == ""){
                    atendidos = org[i].fila[0]->nome;
                }
                else{
                    atendidos = atendidos + "," + org[i].fila[0]->nome;
                }
                org[i].fila[0] = org[i].fila[org[i].ultimo_fila-1];
                org[i].ultimo_fila--;
                cont--;
                heapify_maximo(i,0,org[i].ultimo_fila);
            }
        }
        if(atendidos != ""){
            output << org[i].nome << ":" << atendidos << endl;
        }
        atendidos = "";
        i++;
        if(i >= tamanho_orgao){
            i=0;
        }
    }

    input.close();
    output.close();

 return 0;
 
}
