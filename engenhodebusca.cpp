#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct servidor{

    string historico = "";
    int total = 0;

} servidor;

int tamanho_vetor;
int capacidade_maxima;

int h1(int checksum){
    int i = (7919 * checksum)%tamanho_vetor;
    return i;
}

uint32_t h2(int checksum, servidor* server){
    uint32_t i = 0;
    uint32_t result;

    do{
        result = ((7919 * checksum) + (i*((104729 * checksum) + 123)))%tamanho_vetor;
        i++;
    }while(server[result].total == capacidade_maxima);

    return result;
}

void concatenar_string(servidor* server, string texto){

    if(server->historico == ""){
        server->historico = texto;
    }
    else{
        server->historico = server->historico + "," + texto;
    }

}

void incrementar_vetor(servidor* server, int checksum, string texto, ofstream* output){
    int h1_hash = h1(checksum);
    if(server[h1_hash].total < capacidade_maxima){
        concatenar_string(&server[h1_hash], texto);
        server[h1_hash].total++;
        *output << "[S" << h1_hash << "]" << server[h1_hash].historico << "\n";
    }
    else{
        uint32_t h2_hash = h2(checksum, server);
        *output << "S" << h1_hash << "->S" << h2_hash << "\n";
        concatenar_string(&server[h2_hash], texto);
        server[h2_hash].total++;
        *output << "[S" << h2_hash << "]" << server[h2_hash].historico << "\n";
    }
}



int main(int argc, char* argv[]){

    servidor* server;
    int quant_textos, quant_palavras, checksum, aux_checksum;
    string texto, texto_concatenado;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

        input >> tamanho_vetor; input >> capacidade_maxima;
        server = new servidor[tamanho_vetor];
        input >> quant_textos;
        for(int i=0;i<quant_textos;i++){
            input >> quant_palavras;
            texto_concatenado = "";
            for(int j=0;j<quant_palavras;j++){
                input >> texto;
                aux_checksum = texto[0];
                for(int k=1;k<texto.length();k++){
                    aux_checksum = aux_checksum^texto[k];
                }
                if(j == 0){
                    checksum = aux_checksum;
                }
                else{
                    checksum ^= aux_checksum;
                }
                texto_concatenado = texto_concatenado + " " + texto;
            }
            incrementar_vetor(server,checksum,texto_concatenado,&output);

        }


    input.close();
    output.close();

 return 0;
}
