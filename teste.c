#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _TNo{
    char ID[16];
    int endereco;
    struct _TNo *prox;
}TNo;

TNo *simbolo_inicial = NULL;

int inserir_simbolo (char *ID){
    if (simbolo_inicial == NULL){
        TNo *inserido = (TNo*)malloc(sizeof(TNo));
        strncpy((*inserido).ID, ID, 15);
        (*inserido).ID[15] = '\0';
        (*inserido).prox = NULL;
        (*inserido).endereco = 0;
        simbolo_inicial = inserido;
        return 0;
    } else {
        TNo *atual = simbolo_inicial;
        TNo *anterior = NULL;
        int endereco = 0;
        while (atual != NULL){
            if (strcmp((*atual).ID,ID) == 0){
                printf("Erro semantico: ID '%s' duplicado ",ID);
                exit(1);
            }
            anterior = atual;
            atual = (*atual).prox;
            endereco++;
        }
        TNo *inserido = (TNo*)malloc(sizeof(TNo));;
        strncpy((*inserido).ID, ID, 15);
        (*inserido).ID[15] = '\0';
        (*inserido).prox = NULL;
        (*inserido).endereco = endereco;
        (*anterior).prox = inserido;
        return 0;
    }
}

int buscar_simbolo (char *ID){
    TNo *atual = simbolo_inicial;
    while (atual != NULL){
        if (strcmp((*atual).ID,ID) == 0){
            return (*atual).endereco;
        }
        atual = (*atual).prox;
    }
    printf("Erro semantico: ID '%s' nao existe", ID);
    exit(1);
}

void imprimir_tabela() {
    TNo *atual = simbolo_inicial;
    printf("TABELA DE SIMBOLOS\n");

    while (atual != NULL) {
        printf("%-10s | Endereco: %d\n", (*atual).ID, (*atual).endereco);
        atual = (*atual).prox;
    }
}

int rotulo = 0;

int proximo_rotulo(){
    rotulo++;
    return rotulo;
}

int main(){

    inserir_simbolo("a");
    inserir_simbolo("b");
    inserir_simbolo("c");
    inserir_simbolo("d");
    inserir_simbolo("e");
    imprimir_tabela();

    int endereco = buscar_simbolo("d");
    printf("Endereco do simbolo 'd': %d\n", endereco);

    int rotulo1 = proximo_rotulo();
    printf("Endereco do simbolo 'd': L%d\n", rotulo1);

    int rotulo2 = proximo_rotulo();
    printf("Endereco do simbolo 'd': L%d\n", rotulo2);

    int rotulo3 = proximo_rotulo();
    printf("Endereco do simbolo 'd': L%d\n", rotulo3);

    int rotulo4 = proximo_rotulo();
    printf("Endereco do simbolo 'd': L%d\n", rotulo4);

    return 0;
}