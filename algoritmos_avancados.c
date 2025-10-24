#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa um nó na árvore de pistas
typedef struct NoPista {
    char pista[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;

// Função para criar um novo nó de pista
NoPista* criarNoPista(const char* pista) {
    NoPista* novoNo = (NoPista*)malloc(sizeof(NoPista));
    strcpy(novoNo->pista, pista);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Função para inserir uma pista na árvore de busca
NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// Função para percorrer a árvore em ordem e listar todas as pistas
void listarPistasEmOrdem(NoPista* raiz) {
    if (raiz != NULL) {
        listarPistasEmOrdem(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        listarPistasEmOrdem(raiz->direita);
    }
}

// Função para liberar a memória da árvore de pistas
void liberarArvorePistas(NoPista* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

// Estrutura que representa uma sala na mansão
typedef struct Sala {
    char nome[50];
    char* pista;  // Ponteiro para a pista desta sala (NULL se não houver)
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função para criar uma nova sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL) {
        novaSala->pista = strdup(pista);  // Aloca e copia a pista
    } else {
        novaSala->pista = NULL;
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas interativamente
void explorarSalas(Sala* salaAtual, NoPista** raizPistas) {
    char escolha;
    
    while (salaAtual != NULL) {
        printf("\nVocê está na %s\n", salaAtual->nome);
        
        // Se houver uma pista na sala atual e ainda não foi coletada
        if (salaAtual->pista != NULL) {
            printf("\n🔍 Você encontrou uma pista: %s\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            free(salaAtual->pista);  // Remove a pista da sala após coletá-la
            salaAtual->pista = NULL;
        }
        
        // Verifica se é uma sala final (folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Esta é uma sala final. Voltando ao início...\n");
            return;
        }
        
        // Mostra as opções disponíveis
        printf("\nOpções disponíveis:\n");
        if (salaAtual->esquerda != NULL)
            printf("e - Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("d - Ir para a direita (%s)\n", salaAtual->direita->nome);
        printf("p - Ver todas as pistas coletadas\n");
        printf("s - Sair da exploração\n");
        
        printf("\nEscolha uma opção: ");
        scanf(" %c", &escolha);
        
        switch(escolha) {
            case 'e':
                if (salaAtual->esquerda != NULL)
                    salaAtual = salaAtual->esquerda;
                else
                    printf("Não há sala à esquerda!\n");
                break;
            case 'd':
                if (salaAtual->direita != NULL)
                    salaAtual = salaAtual->direita;
                else
                    printf("Não há sala à direita!\n");
                break;
            case 'p':
                printf("\n📋 Lista de Pistas Coletadas:\n");
                listarPistasEmOrdem(*raizPistas);
                break;
            case 's':
                printf("Saindo da exploração...\n");
                return;
            default:
                printf("Opção inválida!\n");
        }
    }
}

// Função para liberar a memória da árvore
void liberarArvore(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        if (raiz->pista != NULL) {
            free(raiz->pista);
        }
        free(raiz);
    }
}

int main() {
    // Inicializando a árvore de pistas
    NoPista* raizPistas = NULL;
    
    // Criando as salas da mansão com suas respectivas pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Pegadas molhadas no tapete");
    Sala* biblioteca = criarSala("Biblioteca", "Livro de contabilidade com anotações suspeitas");
    Sala* cozinha = criarSala("Cozinha", "Faca com manchas estranhas");
    Sala* sotao = criarSala("Sótão", "Carta antiga revelando segredos da família");
    Sala* jardimInverno = criarSala("Jardim de Inverno", "Frasco de veneno escondido entre as plantas");
    Sala* salaJantar = criarSala("Sala de Jantar", "Taça quebrada com resíduos suspeitos");
    Sala* adega = criarSala("Adega", "Cofre arrombado e vazio");
    
    // Conectando as salas
    hallEntrada->esquerda = biblioteca;
    hallEntrada->direita = cozinha;
    
    biblioteca->esquerda = sotao;
    biblioteca->direita = jardimInverno;
    
    cozinha->esquerda = salaJantar;
    cozinha->direita = adega;
    
    // Iniciando a exploração
    printf("Bem-vindo à Mansão Misteriosa!\n");
    printf("Explore as salas usando as seguintes teclas:\n");
    printf("e - Ir para a sala à esquerda\n");
    printf("d - Ir para a sala à direita\n");
    printf("p - Ver todas as pistas coletadas\n");
    printf("s - Sair da exploração\n\n");
    
    explorarSalas(hallEntrada, &raizPistas);
    
    // Mostrando todas as pistas coletadas ao final
    printf("\n🏁 Fim da exploração!\n");
    printf("\n📋 Resumo de todas as pistas encontradas:\n");
    listarPistasEmOrdem(raizPistas);
    
    // Liberando a memória
    liberarArvore(hallEntrada);
    liberarArvorePistas(raizPistas);
    
    return 0;
}
