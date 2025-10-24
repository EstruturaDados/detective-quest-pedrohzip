#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_HASH 101 // Número primo para melhor distribuição

// Estrutura para armazenar uma associação pista-suspeito na tabela hash
typedef struct NoPistaHash {
    char pista[100];
    char suspeito[50];
    struct NoPistaHash* proximo;
} NoPistaHash;

// Estrutura para contar ocorrências de suspeitos
typedef struct ContadorSuspeito {
    char suspeito[50];
    int contagem;
    struct ContadorSuspeito* proximo;
} ContadorSuspeito;

// Estrutura da Tabela Hash
typedef struct {
    NoPistaHash* tabela[TAMANHO_HASH];
    int totalPistas;
} TabelaHash;

// Função de hash simples usando soma ASCII dos primeiros 5 caracteres
int funcaoHash(const char* pista) {
    int soma = 0;
    int len = strlen(pista);
    for (int i = 0; i < 5 && i < len; i++) {
        soma += (int)pista[i];
    }
    return soma % TAMANHO_HASH;
}

// Inicializa a tabela hash
void inicializarHash(TabelaHash* hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
    hash->totalPistas = 0;
}

// Insere uma associação pista-suspeito na tabela hash
void inserirNaHash(TabelaHash* hash, const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    
    NoPistaHash* novoNo = (NoPistaHash*)malloc(sizeof(NoPistaHash));
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;
    
    if (hash->tabela[indice] == NULL) {
        hash->tabela[indice] = novoNo;
    } else {
        novoNo->proximo = hash->tabela[indice];
        hash->tabela[indice] = novoNo;
    }
    hash->totalPistas++;
}

// Encontra o suspeito mais citado
char* encontrarSuspeitoMaisCitado(TabelaHash* hash) {
    ContadorSuspeito* contadores = NULL;
    
    // Conta ocorrências de cada suspeito
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoPistaHash* atual = hash->tabela[i];
        while (atual != NULL) {
            // Procura o suspeito na lista de contadores
            ContadorSuspeito* contador = contadores;
            ContadorSuspeito* encontrado = NULL;
            
            while (contador != NULL) {
                if (strcmp(contador->suspeito, atual->suspeito) == 0) {
                    encontrado = contador;
                    break;
                }
                contador = contador->proximo;
            }
            
            if (encontrado) {
                encontrado->contagem++;
            } else {
                // Cria novo contador
                ContadorSuspeito* novoContador = (ContadorSuspeito*)malloc(sizeof(ContadorSuspeito));
                strcpy(novoContador->suspeito, atual->suspeito);
                novoContador->contagem = 1;
                novoContador->proximo = contadores;
                contadores = novoContador;
            }
            
            atual = atual->proximo;
        }
    }
    
    // Encontra o suspeito com maior contagem
    ContadorSuspeito* atual = contadores;
    char* suspeitoMaisCitado = NULL;
    int maiorContagem = 0;
    
    while (atual != NULL) {
        if (atual->contagem > maiorContagem) {
            maiorContagem = atual->contagem;
            if (suspeitoMaisCitado != NULL) {
                free(suspeitoMaisCitado);
            }
            suspeitoMaisCitado = strdup(atual->suspeito);
        }
        ContadorSuspeito* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    
    return suspeitoMaisCitado;
}

// Exibe todas as associações pista-suspeito
void exibirAssociacoes(TabelaHash* hash) {
    printf("\n🔍 Associações entre Pistas e Suspeitos:\n");
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoPistaHash* atual = hash->tabela[i];
        while (atual != NULL) {
            printf("Pista: \"%s\" → Suspeito: %s\n", atual->pista, atual->suspeito);
            atual = atual->proximo;
        }
    }
}

// Libera a memória da tabela hash
void liberarHash(TabelaHash* hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoPistaHash* atual = hash->tabela[i];
        while (atual != NULL) {
            NoPistaHash* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

// Lista de suspeitos possíveis
const char* SUSPEITOS[] = {
    "Mordomo James",
    "Senhora Edwards",
    "Dr. Black",
    "Srta. Scarlett",
    "Coronel Mustard"
};
#define NUM_SUSPEITOS 5

// Retorna um suspeito aleatório para associar com uma pista
const char* obterSuspeitoAleatorio() {
    return SUSPEITOS[rand() % NUM_SUSPEITOS];
}

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
void explorarSalas(Sala* salaAtual, NoPista** raizPistas, TabelaHash* hash) {
    char escolha;
    
    while (salaAtual != NULL) {
        printf("\nVocê está na %s\n", salaAtual->nome);
        
        // Se houver uma pista na sala atual e ainda não foi coletada
        if (salaAtual->pista != NULL) {
            printf("\n🔍 Você encontrou uma pista: %s\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            
            // Associa a pista a um suspeito
            const char* suspeitoRelacionado = obterSuspeitoAleatorio();
            inserirNaHash(hash, salaAtual->pista, suspeitoRelacionado);
            printf("👤 Esta pista aponta para: %s\n", suspeitoRelacionado);
            
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
        printf("a - Ver associações pista-suspeito\n");
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
            case 'a':
                exibirAssociacoes(hash);
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
    // Inicializando o gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializando a árvore de pistas e a tabela hash
    NoPista* raizPistas = NULL;
    TabelaHash hash;
    inicializarHash(&hash);
    
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
    printf("a - Ver associações pista-suspeito\n");
    printf("s - Sair da exploração\n\n");
    
    explorarSalas(hallEntrada, &raizPistas, &hash);
    
    // Mostrando todas as pistas coletadas ao final
    printf("\n🏁 Fim da exploração!\n");
    printf("\n📋 Resumo de todas as pistas encontradas:\n");
    listarPistasEmOrdem(raizPistas);
    
    // Mostrando todas as associações pista-suspeito
    printf("\n🔍 Análise das Pistas:\n");
    exibirAssociacoes(&hash);
    
    // Identificando o suspeito mais citado
    char* suspeitoMaisCitado = encontrarSuspeitoMaisCitado(&hash);
    if (suspeitoMaisCitado != NULL) {
        printf("\n⚠️ Principal Suspeito: %s\n", suspeitoMaisCitado);
        free(suspeitoMaisCitado);
    }
    
    // Liberando a memória
    liberarArvore(hallEntrada);
    liberarArvorePistas(raizPistas);
    liberarHash(&hash);
    
    return 0;
}
