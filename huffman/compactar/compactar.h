#ifndef compactar_h
#define compactar_h

#define TAM 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Estruturas
typedef unsigned char uchar;
typedef unsigned long long int ulli;
typedef unsigned short ushor;

typedef struct no{
    void* byte;
    void* frequencia;
    struct no* dir;
    struct no* esq;
    struct no* prox;
}No;

typedef struct fila{
    No* inicio;
}Fila;


//Funcao para criar a fila
Fila* criar_fila();

//Cria o noh da arvore
//void* byte: aqui serah colocar o byte do noh
//void* frequencia: aqui terah a frequencia do byte
No* criar_noh(void* byte, void* frequencia);


//Inserir na fila de prioridade ( o inicio da fila sempre vai ficar com o valor de menor frequencia)
/*
Caso 1: A fila estah vazia. Caso isso ocorra o noh a ser inserido ficarah no inicio
Caso 2: A fila nao estah mais vazia,vamos ter que verificar se o elemento que queremos inserir tem frequencia menor que o 1º noh da lista
        Neste caso o noh vai ficar na frente do noh do inicio
Caso 3: O noh a ser inserido vai ficar no meio ou no fim
*/

void fila_prioridade(Fila* queue, No* no);

void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]);

void preencher( Fila* queue, ulli *table);

No* dequeue(Fila* queue);

No* gerar_arvore(Fila* queue);

ulli contar_bits_totais(const char* nome_arquivo, char** tabela_codigos);

void obter_extensao(const char* nome_arquivo, char* extensao) 
void tabela_de_codigo(char *tb_codigo[], No *arvore, char *caminho);

void compactar_arquivo (const char* nome_arquivo_original, const char* nome_arquivo_compactado);

void imprimir_frequencia( ulli frequencia[TAM]);

void imprimir_arvore(No* raiz);


void imprimir_fila(Fila* queue);

#endif