#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct ganhador{
    string nome;
    int acertos = 0;
} ganhador;

ganhador* maximo;
ganhador* minimo;
int* sorteados = new int[10];
int tamanho_vetor,valor_premio,ultimo_maximo,ultimo_minimo;

void heapify_maximo(ganhador* maximo,uint32_t i) {
    uint32_t p = i;
    uint32_t e = 2*i + 1;
    uint32_t d = 2*i + 2;
    if(e < ultimo_maximo && (maximo[e]).acertos > (maximo[p]).acertos){
        p = e;
    }
    if(d < ultimo_maximo && (maximo[d]).acertos > (maximo[p]).acertos){
        p = d;
    }
    if(p!=i){
        ganhador aux = maximo[i];
        maximo[i] = maximo[p];
        maximo[p] = aux;
        heapify_maximo(maximo,p);
    }
}

void heapify_minimo(ganhador* minimo,uint32_t i) {
    uint32_t p = i;
    uint32_t e = 2*i + 1;
    uint32_t d = 2*i + 2;
    if(e < ultimo_minimo && (minimo[e]).acertos < (minimo[p]).acertos){
        p = e;
    }
    if(d < ultimo_minimo && (minimo[d]).acertos < (minimo[p]).acertos){
        p = d;
    }
    if(p!=i){
        ganhador aux = minimo[i];
        minimo[i] = minimo[p];
        minimo[p] = aux;
        heapify_minimo(minimo,p);
    }
}


int main(int argc, char* argv[]){
    int numero;
    string nome;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    input >> valor_premio >> tamanho_vetor;
    maximo = new ganhador[tamanho_vetor];
    minimo = new ganhador[tamanho_vetor];
    ultimo_maximo = tamanho_vetor;
    ultimo_minimo = tamanho_vetor;


    for(int i=0;i<10;i++){
        input >> numero;
        sorteados[i] = numero;
    }

    for(int i=0;i<tamanho_vetor;i++){
        input >> nome;
        maximo[i].nome = nome;
        minimo[i].nome = nome;

        for(int j=0;j<15;j++){
            input >> numero;

            for(int k=0;k<10;k++){
                if(numero == sorteados[k]){
                    maximo[i].acertos++;
                    minimo[i].acertos++;
                    break;
                }
            }

        }

    }

    //construção heap máximo
    for(int i = (tamanho_vetor - 2)/2;i>=0;i--){
        heapify_maximo(maximo,i);
    }

    //construção heap minimo
    for(int i = (tamanho_vetor - 2)/2;i>=0;i--){
        heapify_minimo(minimo,i);
    }

    int maximo_acerto = maximo[0].acertos;
    int minimo_acerto = minimo[0].acertos;
    int ganhadores_maximo = 1, ganhadores_minimo = 1;
    string maximo_hexa = maximo[0].nome;
    string minimo_hexa = minimo[0].nome;

    minimo[0] = minimo[ultimo_minimo-1];
    ultimo_minimo--;
    heapify_minimo(minimo,0);
    maximo[0] = maximo[ultimo_maximo-1];
    ultimo_maximo--;
    heapify_maximo(maximo,0);

    for(int i=1;i<tamanho_vetor;i++){
        if(maximo[0].acertos != maximo_acerto){
            break;
        }
        if(maximo[0].acertos == maximo_acerto){
            ganhadores_maximo++;
            maximo_hexa += "\n" + maximo[0].nome;
            maximo[0] = maximo[ultimo_maximo-1];
            ultimo_maximo--;
            heapify_maximo(maximo,0);
        }
    }

    for(int i=1;i<tamanho_vetor;i++){
        if(minimo[0].acertos != minimo_acerto){
            break;
        }
        if(minimo[0].acertos == minimo_acerto){
            ganhadores_minimo++;
            minimo_hexa += "\n" + minimo[0].nome;
            minimo[0] = minimo[ultimo_minimo-1];
            ultimo_minimo--;
            heapify_minimo(minimo,0);
        }
    }

    output << "[" << ganhadores_maximo << ":" << maximo_acerto << ":" << (valor_premio/2)/ganhadores_maximo << "]\n";
    output << maximo_hexa;

    output << "\n[" << ganhadores_minimo << ":" << minimo_acerto << ":" << (valor_premio/2)/ganhadores_minimo << "]\n";
    output << minimo_hexa;

    input.close();
    output.close();

 return 0;
}
