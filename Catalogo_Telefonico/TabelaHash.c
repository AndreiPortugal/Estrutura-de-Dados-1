#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TabelaHash.h"

typedef struct NO *ArvAVL;
//definindo struct hash, que terá: mesmo tamanho da tabela, elementos e ponteiros para esses elementos
struct hash {
    int qtd, TABLE_SIZE;
    struct discagem **itens;
};

// cria a tabela hash
Hash* criaHash(int TABLE_SIZE) {
    Hash *ha = (Hash *) malloc(sizeof(Hash));//alocando memoria para a estrutura da tabela hash
    if(ha != NULL) { // se a tabela for não nula
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->itens = (struct discagem**) malloc(TABLE_SIZE * sizeof(struct discagem*));//alocando memoria para os elementos
        if(ha->itens == NULL) {//verificando se a alocação funcionou
            free(ha);// se não, ele libera a tabela 
            return NULL;
        }
        ha->qtd = 0;//definindo a quantidade de elementos da tabela em zero
        for(i=0; i<ha->TABLE_SIZE; i++)//percorrendo a tabela hash
            ha->itens[i] = NULL;//inicia a tabela com todos os elementos como nulo
    }
    return ha;
}

// libera a tabela hash
void liberaHash(Hash *ha) {
    if(ha != NULL) {// verifica se a tabela existe
        int i;
        for(i=0; i<ha->TABLE_SIZE; i++) {
            if(ha->itens[i] != NULL)//verificando se existe itens
                free(ha->itens[i]);//liberando os elementos apontados por 'itens'
        }
        free(ha->itens);//libera o ponteiro
        free(ha);//libera a tabela
    }
}

/*
Método da Divisao: Consiste em calcular o "resto da divisão" do valor inteiro que
representa o elemento pelo tamanho da tabela, "TABLE_SIZE".
- A operação de "E bit-a-bit" com o valor 0x7FFFFFFF elimina o bit de sinal do
valor da "chave".
- Evita o risco de ocorrer um overflow e obtermos um número negativo
*/

int chaveDivisao(int chave, int TABLE_SIZE) {//calcucando o indice da tabela por meio de uma chave
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

/*
"Tratando uma string como chave"
- Calcular um valor numérico a partir dos valores ASCII dos caracteres
- Devemos considerar a posição da letra
"Cuidado"
- Não devemos apenas somar os caracteres, pois palavras com letras trocadas irão
produzir o mesmo valor
cama: 99 + 97 + 109 + 97 = 402
maca: 109 + 97 + 99 + 97 = 402
*/

int valorString(char *str) {
    int i, valor = 7;
    int tam = strlen(str);
    for(i=0; i < tam; i++)
        valor = 31 * valor + (int) str[i];
    return valor;
}

// insere uma arvore na tabela hash dentro da lsita encadeada
int insereHash(Hash *ha, int ddd, struct dados pessoa) {
    if (ha == NULL || ha->qtd == ha->TABLE_SIZE)  
        return 0;
    int chave = ddd;
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);
    struct discagem* novo = (struct discagem*)malloc(sizeof(struct discagem));
    novo->prefixo = ddd;
    novo->prox = NULL;
    if(ha->itens[pos] == NULL) {
        ha->itens[pos] = novo;  // coloca a árvore na tabela hash caso não exista nenhuma
    } else {
        struct discagem* aux = ha->itens[pos];
        while (aux != NULL) {
            if (aux->prefixo == ddd) {
                int resultInsereArv = insere_ArvAVL(aux->arv, pessoa);
                if (resultInsereArv == 0) {
                    free(novo); // libera a memória alocada para 'novo'
                    return 0;
                }
                return 1;
            }
            aux = aux->prox;
        }
        aux = novo; // atualiza o ponteiro aux para 'novo'
    }
    ArvAVL *arv = cria_ArvAVL();
    insere_ArvAVL(arv, pessoa);
    novo->arv = arv;
    ha->qtd++;
    return 1;
}


int buscaHash(Hash *ha, int prefixo, ArvAVL *arv) {
    if(ha == NULL)
        return 0;
    int pos = chaveDivisao(prefixo, ha->TABLE_SIZE);
    if(ha->itens[pos] == NULL){
        return 0;
    }
    struct discagem* aux = ha->itens[pos];
    while(aux != NULL){
        if(aux->prefixo == prefixo){
            *arv = (*aux->arv);
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
}

/* varre toda a tabela hash e imprime os dados das arvores das discagems encadeadas que estao 
na tabela hash ordenado por numero*/
void imprimirTabelaOrdenadaNumero(int TABLE_SIZE, Hash *ha) {
    int i, existePessoa = 0;
    printf("\n------------------------TABELA--------------------------\n");
    for(i = 0; i < TABLE_SIZE; i++) {
        struct discagem* aux = ha->itens[i];
        while(aux != NULL){
            printf("DDD: %d\n", aux->prefixo);
            emOrdem_ArvAVL(aux->arv);
            aux = aux->prox;
            existePessoa++;
        } 
    }
    if(existePessoa == 0) {
        printf("Nao existe pessoas cadastradas no catalogo.\n");
    }
    printf("----------------------FIM TABELA------------------------\n");
    return;
}

void imprimirTabelaOrdenadaNome(int TABLE_SIZE, Hash *ha) {
    int existePessoa = 0;
    printf("\n------------------------TABELA--------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct discagem* aux = ha->itens[i];
        while (aux != NULL) {
            ArvAVL* auxNome = cria_ArvAVL();
            reorganizarArvAVL(aux->arv,&auxNome);
            printf("DDD: %d\n", aux->prefixo);
            imprimirEmOrdemNome(*(auxNome)); // Passa o valor da árvore, não o ponteiro para a árvore
            aux = aux->prox;
            existePessoa++;
            libera_ArvAVL(auxNome);
        }
    }
    if (existePessoa == 0) {
        printf("Nao existe pessoas cadastradas no catalogo.\n");
    }
    printf("----------------------FIM TABELA------------------------\n");
    return;
}