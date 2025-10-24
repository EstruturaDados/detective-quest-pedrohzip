#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala na mansão
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função para criar uma nova sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas interativamente
void explorarSalas(Sala* salaAtual) {
    char escolha;
    
    while (salaAtual != NULL) {
        printf("\nVocê está na %s\n", salaAtual->nome);
        
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
        free(raiz);
    }
}

int main() {
    // Criando as salas da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");
    Sala* jardimInverno = criarSala("Jardim de Inverno");
    Sala* salaJantar = criarSala("Sala de Jantar");
    Sala* adega = criarSala("Adega");
    
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
    printf("s - Sair da exploração\n\n");
    
    explorarSalas(hallEntrada);
    
    // Liberando a memória
    liberarArvore(hallEntrada);
    
    return 0;
}
