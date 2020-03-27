#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct livro{

    string titulo;
    string autor;
    long long int isbn;

} livro;

int tamanho_vetor, quant_pesquisas;
int cont_binaria = 0, aux_binaria = 1;
int cont_interpolacao = 0, aux_interpolacao = 1;
int vitoria_binaria = 0; int vitoria_interpolada = 0;

void find_string(int i, livro* li, string linha){
    string sub = linha.substr(0,13);
    li[i].isbn = atoll(sub.c_str());
    int find_separator = linha.find("&",13);
    li[i].autor = linha.substr(14,find_separator-14);
    li[i].titulo = linha.substr(find_separator+1);
}

int busca_binaria(livro* li, long long int searches){

    int i=0, j = tamanho_vetor - 1;
    int result = (i+j)/2;

    while(j>=i && li[result].isbn != searches){
        if(li[result].isbn < searches){
            i = result + 1;
        }
        if(li[result].isbn > searches){
            j = result - 1;
        }
        result = (i+j)/2;
        aux_binaria++;
    }

    if(li[result].isbn == searches){
        return result;
    }
    else{
        return -1;
    }
}

long long int busca_interpolada(livro* li, long long int searches){

    long long int i=0, j = tamanho_vetor - 1;
    long long int result = i+((li[j].isbn-li[i].isbn)%(j-i+1));

    while(j>=i && li[result].isbn != searches){

        if(li[result].isbn < searches){
            i = result + 1;
        }
        if(li[result].isbn > searches){
            j = result - 1;
        }
        if(j>=i){
            result = i+((li[j].isbn-li[i].isbn)%(j-i+1));
            aux_interpolacao++;
        }
    }

    if(li[result].isbn == searches){
        return result;
    }
    else{
        return -1;
    }
}


int main(int argc, char* argv[]){

    livro* li;
    long long int pesquisa;
    string linha;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    input >> tamanho_vetor;

    li = new livro[tamanho_vetor];
    input.ignore();

    for(int i=0;i<tamanho_vetor;i++){
        getline(input,linha);
        find_string(i, li, linha);
    }
    input >> quant_pesquisas;
    for(int i=0;i<quant_pesquisas;i++){
        input >> pesquisa;

        long long int in = busca_interpolada(li,pesquisa);
        busca_binaria(li,pesquisa);

        cont_binaria += aux_binaria;
        cont_interpolacao += aux_interpolacao;

        if(in != -1){
            output << "[" << li[in].isbn << "]B=" << aux_binaria << ",I=" << aux_interpolacao << ":Author:" << li[in].autor << ",Title:" << li[in].titulo << "\n";
        }
        if(in == -1){
            output << "[" << pesquisa << "]B=" << aux_binaria << ",I=" << aux_interpolacao << ":ISBN_NOT_FOUND\n";
        }
        if(aux_interpolacao > aux_binaria){
            vitoria_binaria++;
        }
        else{
            vitoria_interpolada++;
        }

        aux_interpolacao = 1;
        aux_binaria = 1;
    }

    output << "BINARY=" << vitoria_binaria << ":" << cont_binaria/quant_pesquisas << "\n";
    output << "INTERPOLATION=" << vitoria_interpolada << ":" << cont_interpolacao/quant_pesquisas;

    input.close();
    output.close();

 return 0;
}
