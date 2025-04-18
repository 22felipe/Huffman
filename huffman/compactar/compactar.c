#include "compactar.h"

#define tam 256
//Tabela de frequencia

//Criar fila
Fila* criar_fila(){
    //Alocacao do noh 
    Fila* queue = (Fila*)malloc(sizeof(Fila));

    //Verificar se o noh foi alocado corretamente
    if(queue != NULL){
        queue->inicio = NULL;

        return queue;
    }else{
        printf("Falha na alocacao de memoria, na funcao criar fila ( linha 6)\n");
        return NULL;
    }
}

//Criar noh da arvore
No* criar_noh(void* byte, void* frequencia){
    //Alocacao do noh
    No* noh = (No*)malloc(sizeof(No));

    //Verificacao de seguranca
    if( noh != NULL){
        noh->byte = byte;
        noh->frequencia = frequencia;
        noh->esq = NULL;
        noh->dir = NULL;
        noh->prox = NULL;

        return noh;
    }else{
        printf("Falha na alocacao do noh, na funcao criar_noh\n");
        return NULL;
    }
}

//Usamos o "const char" para que receber o nome do arquivo, e o const serve para garantir que o vetor char serah somente para leitura
void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]){
    
    FILE* arquivo = fopen(nome_arquivo,"rb"); // rb = read binary (leitura binaria, em traducao livre)

    //Verificacao de seguranca
    if( arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }else{
        
        int byte;
        
        while( (byte = fgetc(arquivo)) != EOF){
            frequencia[(uchar)byte]++; // cada byte que ele achar na frequencia serah adicionado mais 1
        }
        fclose(arquivo);

    }

}
//Criar fila de prioridade (O de menor frequencia vem no inicio)
void fila_prioridade(Fila* queue, No* no) {
    if (queue == NULL || no == NULL) {
        return; // Houve algum erro na alocação da fila ou do nó
    }

    // Caso 1: A fila está vazia, insere o nó como o primeiro elemento
    if (queue->inicio == NULL) {
        queue->inicio = no;
        no->prox = NULL; // Garante que o próximo do nó inserido aponte para NULL
    }
    // Caso 2: O nó deve ser inserido antes do primeiro elemento (frequência menor)
    else if (*(ulli*)no->frequencia <= *(ulli*)queue->inicio->frequencia) {
        no->prox = queue->inicio;  // O novo nó aponta para o atual início
        queue->inicio = no;        // Atualiza o início da fila
    }
    // Caso 3: Percorre a fila para encontrar a posição correta
    else {
        No* aux = queue->inicio;

        // Procura o local correto para inserir o nó
        while (aux->prox != NULL && *(ulli*)aux->prox->frequencia < *(ulli*)no->frequencia) {
            aux = aux->prox; // Avança para o próximo nó
        }

        // Insere o nó no local correto
        no->prox = aux->prox; // O próximo do nó inserido será o próximo do auxiliar
        aux->prox = no;       // O auxiliar aponta para o nó recém-inserido
    }

}

void preencher( Fila* queue, ulli *table){
 
    for( int i = 0; i < TAM;i++){
        if( table[i] > 0){ // Para pegar os valores maiores que 1
            // Alocar dinamicamente o byte e a frequência para usar void*, para evitar casting de maneira errada
            uchar* ponteiro_para_byte = (uchar*)malloc(sizeof(uchar));
            ulli* ponteiro_para_frequencia = (ulli*)malloc(sizeof(ulli));
 
            //Verificacao de seguranca
            if( ponteiro_para_byte != NULL && ponteiro_para_frequencia != NULL){
                
                *ponteiro_para_byte = (uchar)i;
                *ponteiro_para_frequencia = table[i];
            
 
                No* noh = criar_noh(ponteiro_para_byte,ponteiro_para_frequencia);
 
                //Verificacao de seguranca
                if( noh != NULL){
                    //Chamo a funcao para inserir ordenado na queuea
                    fila_prioridade(queue,noh);
 
                }else{
                    printf("Falha na alocacao de preenchimento da queuea\n");
                    return; // parar o procedimento
                }
            }else{
                printf("Falha na alocacao dos ponteiros para byte e frenquencia, na funcao preencher\n");
                return;
            }
 
        }
    }
}


No* dequeue(Fila* queue){

    /*
    Temos quer ter um ponteiro auxiliar e inicializa-lo com NULL, pois vamos retornar o auxiliar, pois se nao tiver ninguem na queuea removeremos o null
    */
    No* aux = NULL;

    if( queue->inicio != NULL){
        aux = queue->inicio; // o auxiliar aponta para o inicio
        //O inicio da queuea eh o proximo ponteiro
        queue->inicio = aux->prox;  // O inicio vai para o proximo para nao perdermos o encadeamento da queuea

        //Para nao termos problemas de tentar acessar, de maneira indevida, o campo proximo fora da manipulacao da queuea
        aux->prox = NULL;
    }

    return aux;
}

//Montando a arvore de huffman
No* gerar_arvore(Fila* queue){
    /*
    Criaremos 3 ponteiros:
        - O primeiro ponteiro "primeiro" irah receber o noh com a frequencia da queuea(sendo a queuea organizada para ser retirado o noh de menor frequencia)
        - O segundo ponteiro "segundo" irah receber o segundo noh de menor frequencia da queuea
        - O terceiro ponteiro "novo" serah o noh que juntaremos a frequencia do "primeiro" e "segundo" ponteiro 
    */
    No* primeiro;
    No* segundo;
    No* novo;
    
    //Enquanto o tamanho da queuea for maior que 1 vamos continuar executando o codigo, pois quando o tamanho da queuea for igual a 1, significa que temos apenas um noh e esse noh serah o noh raiz

    while( queue->inicio->prox != NULL){
        primeiro = dequeue(queue);
        segundo = dequeue(queue);

        ulli freq_primeiro = *(ulli*)primeiro->frequencia;
        ulli freq_segundo = *(ulli*)segundo->frequencia;

        // Alocar memória para a nova frequência
        ulli* nova_frequencia = (ulli*)malloc(sizeof(ulli));

        if( nova_frequencia != NULL){
            // Soma das frequências dos dois nós
            *nova_frequencia = freq_primeiro + freq_segundo;
            //Agora precisamos criar o noh "novo" e alocar memoria e concetar o "primeiro" e o "segundo"
            novo = criar_noh(NULL,nova_frequencia);
            //Verificacao de seguranca
            if( novo != NULL){
                novo->esq = primeiro;
                novo->dir = segundo;
                novo->prox = NULL; // Neste momento nao aponta para ninguem

                //Agora vamos inserir o noh "novo" novamente na queuea e repetir o processo ate o tamanho da queuea ser 1
                fila_prioridade(queue,novo);
                
                
            }else{
                printf("Falha na alocacao do noh novo, na funcao gerar_arvore\n");
                return NULL;
            }

        }else{
            printf("Falha na alocacao de nova frequencia\n");
            return NULL;

        }
        
    }

    //Quando o loop acabar, ou seja, "list->tam" == 1, retornaremos o noh raiz da arvore que sera o inicio da lista
    return dequeue(queue);
}

void tabela_de_codigo(char *tb_codigo[], No *arvore, char *caminho) {
    if (arvore == NULL) return;

    // Caso base: nó folha
    if (arvore->esq == NULL && arvore->dir == NULL) {
        tb_codigo[(unsigned char)arvore->byte] = strdup(caminho);
        return;
    }

    // Caminho para esquerda: adiciona '0'
    char caminho_esq[TAM];
    strcpy(caminho_esq, caminho);
    strcat(caminho_esq, "0");
    tabela_de_codigo(tb_codigo, arvore->esq, caminho_esq);

    // Caminho para direita: adiciona '1'
    char caminho_dir[TAM];
    strcpy(caminho_dir, caminho);
    strcat(caminho_dir, "1");
    tabela_de_codigo(tb_codigo, arvore->dir, caminho_dir);
}

ulli contar_bits_totais(const char* nome_arquivo, char** tabela_codigos) {
    FILE* arquivo = fopen(nome_arquivo, "rb"); // leitura binária
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    ulli total_bits = 0;
    int byte;

    while ((byte = fgetc(arquivo)) != EOF) {
        // Fazemos um cast para unsigned char para garantir que o valor do byte esteja entre 0 e 255,
        // evitando acessos inválidos fora do vetor tabela_codigos (que tem tamanho 256).
        unsigned char c = (unsigned char)byte;
        if (tabela_codigos[byte] != NULL) {
            total_bits += strlen(tabela_codigos[byte]);
        } else {
            // Caso não tenha código para esse byte, pode ser um erro
            fprintf(stderr, "Aviso: byte %d não tem código na tabela.\n", byte);
        }
    }

    fclose(arquivo);
    return total_bits;
}

void obter_extensao(const char* nome_arquivo, char* extensao) {
    // Encontra o último ponto no nome do arquivo
    const char* ponto = strrchr(nome_arquivo, '.');
    if (ponto != NULL) {
        // Copia a extensão para a variável fornecida
        strcpy(extensao, ponto + 1); // +1 para pular o ponto
    } else {
        // Se não houver ponto, define uma string vazia ou um valor padrão
        strcpy(extensao, ""); // ou "sem_extensao"
    }
}



void compactar_arquivo (const char* nome_arquivo_original, const char* nome_arquivo_compactado)
{
    //Passos;
    /*
        Passo 1: Gerar tabela de frequências
        Passo 2: Criar fila de nós e gerar a árvore de Huffman
        Passo 3: Gerar tabela de códigos
        Passo 4: Calcular o tamanho do arquivo original e o tamanho do arquivo compactado
    */

    ulli frequencia[TAM] = {0};

    //gerar a tabela de frequencia
    tabela_frequencia(nome_arquivo_original, frequencia);

    //Criar fila de nós 
    Fila * fp = criar_fila(); 
    preencher(fp, frequencia);

    //gerar a árvore de Huffman
    No * raiz=  gerar_arvore(fp);

    //gerar tabela de códigos
    char* tabela_codigos[TAM] = {0};
    char caminho_na_arvore[TAM] = "";
    tabela_de_codigo(tabela_codigos, raiz, caminho_na_arvore);

    // calculando: tamanho do arquivo original, tamanho do arquivo compactado e lixo
    ulli tam_aquivo = contar_bits_totais(nome_arquivo_original, tabela_codigos);
    int bit_lixo= (8 - (tam_aquivo % 8)) % 8; // Calcula o número de bits de lixo
    ulli tam_aquivo_compactado = (tam_aquivo + bit_lixo) / 8; // Tamanho do arquivo compactado em bytes 

    //Obtendo a extensão do arquivo original (jpg, png, txt, etc)
    char extensao[5];
    obter_extensao(nome_arquivo_original, extensao);


    //Escrever o cabeçalho do arquivo compactado
    FILE* arquivo_compactado = fopen(nome_arquivo_compactado,"wb");
    if( arquivo_compactado == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }
    


}

//--------------------------------Funcoes de debug---------------------------------------------------

//Funcao para imprimir a tabela de frequencia
void imprimir_frequencia( ulli frequencia[TAM]){
    for( int i = 0; i < TAM; i++){
        if( frequencia[i] > 0){
            printf("%c: %u\n", (uchar)i,frequencia[i]);
        }
    }
}

//imprimir a arvore em pre-ordem
void imprimir_arvore(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Se o nó for uma folha, imprime o byte e um caractere especial para identificar folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("(%c)", *(uchar*)raiz->byte);
    } else {
        // Se for um nó interno, imprimimos um caractere especial
        printf("*");
    }

    // Recursivamente imprime a subárvore esquerda e direita (pré-ordem)
    imprimir_arvore(raiz->esq);
    imprimir_arvore(raiz->dir);
}

void imprimir_fila(Fila* queue) {
    No* atual = queue->inicio;  // Começa pelo primeiro nó da lista
    while (atual != NULL) {
        // Converte os ponteiros void* para os tipos corretos antes de imprimir
        printf("byte: %c freq: %llu --> ", *(uchar*)atual->byte, *(ulli*)atual->frequencia);
        atual = atual->prox;  // Avança para o próximo nó
    }
    printf("NULL\n");  // Indica o fim da lista
}

//----------------------------------------------------------------------------------------------------------