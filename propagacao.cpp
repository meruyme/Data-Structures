#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef struct pessoa{

    int altura = 0;
    struct pessoa *pai;

}pessoa;

int cont = 0;
pessoa ***infectados;
pessoa *representante = NULL;
int x0,y0;

uint32_t myrand(){
    static uint32_t next = 1;
    next = next * 1103515245 + 12345;
    return next;
}

pessoa* encontrar_conjunto(pessoa* x){
    if(x!=x->pai) {
        x->pai = encontrar_conjunto(x->pai) ;
    }
    return x->pai;
}

void unir_conjuntos (pessoa* naoDoente, pessoa* rep) {
    pessoa* rx = encontrar_conjunto(naoDoente) ;
    pessoa* ry = encontrar_conjunto(rep) ;
    if(rx->altura > ry->altura) {
        ry->pai = rx;
    }
    else {
        rx->pai = ry;
        if(rx->altura == ry->altura) {
            ry->altura++;
        }
    }
}

uint32_t propagar_x(uint32_t linhas, uint32_t colunas, uint32_t x, uint32_t y){
    uint32_t x_novo;
    x_novo = x+(-1 + (myrand()%3));
    while(x_novo >= linhas){
        x_novo = x+(-1 + (myrand()%3));
    }
    if(x_novo < linhas){
        return x_novo;
    }
}

uint32_t propagar_y(uint32_t linhas, uint32_t colunas, uint32_t x, uint32_t y){
    uint32_t y_novo;
    y_novo = y+(-1 + (myrand()%3));
    while(y_novo >= colunas){
        y_novo = y+(-1 + (myrand()%3));
    }
    if(y_novo < colunas){
        return y_novo;
    }
}

pessoa* criar_conjunto(){
    pessoa* x = new pessoa;
    x->pai = x;
    return x;
}


int main(int argc, char* argv[]){
    int qtd_locais;
    uint32_t x, y;
    int qtd = 1;
    uint32_t linhas, colunas;
    ifstream input;
    ofstream output;
    input.open(argv[1]);
    output.open(argv[2]);

    input >> qtd_locais;


    for(int i=0;i<qtd_locais;i++){
        input >> linhas >> colunas >> x0 >> y0;
        infectados = new pessoa**[linhas]();
        for(uint32_t j=0;j<linhas;j++){
            infectados[j] = new pessoa*[colunas]();
        }

        for(int j=0;j<linhas;j++){
            for(int k=0;k<colunas;k++){
                infectados[j][k] = criar_conjunto();
            }
        }

        representante = infectados[x0][y0];
        output << i+1 << ":(" << x0 << "," << y0 << ")";

        int aux_x = x0;
        int aux_y = y0;
        while(qtd < linhas*colunas){
            x = propagar_x(linhas,colunas,aux_x,aux_y);
            y = propagar_y(linhas,colunas,aux_x,aux_y);

            if(encontrar_conjunto(infectados[x][y]) == representante){
                aux_x = x;
                aux_y = y;
            }
            else{
                unir_conjuntos(infectados[x][y],representante);
                output << ";(" << x << "," << y << ")";
                aux_x = x;
                aux_y = y;
                qtd++;
            }
        }
        output << endl;
        qtd = 1;
    }

    input.close();
    output.close();

 return 0;
}
