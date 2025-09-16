// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define MAX_STRING 100

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int num_tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* territorio);

// Funções de interface com o usuário:
void exibirMapa(Territorio* territorio);
// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* territorio);
void simularAtaque(int* atacante, int* atacado, Territorio* territorio);
// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.s
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        return 1;
    }
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa);
    // - Define a cor do jogador e sorteia sua missão secreta.
    
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    do {
        exibirMapa(mapa);
        printf("========================\n");
        printf("Digite 0 para sair e 1 para continuar\n");
        printf("========================\n");
        int opcao = 0;
        scanf("%d", &opcao);
        limparBufferEntrada();
        if (opcao == 0) {
            break;
        }

        faseDeAtaque(mapa);
    } while (1);

    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(mapa);
    return 0;
}



// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa.\n");
        return NULL;
    } 
    return mapa;   
}


// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* territorio) {
    printf("========================\n");
    printf("Alocação de território\n");
    printf("========================\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("------------------------\n"); 
        printf("Nome do território %d:\n", i + 1);
        fgets(territorio[i].nome, MAX_STRING, stdin);
        printf("Cor do exército:\n");
        fgets(territorio[i].cor, MAX_STRING, stdin);
        printf("Número de tropas:\n");
        scanf("%d", &territorio[i].num_tropas);
        limparBufferEntrada();
        printf("------------------------\n");
    }
}


// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(Territorio* territorio) {
    printf("========================\n");
    printf("Exibição dos territórios\n");
    printf("========================\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%i - %s(Cor: %s / Tropas %i)\n", i + 1, territorio[i].nome, territorio[i].cor, territorio[i].num_tropas);
    }
}
// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* territorio) {
    printf("------------------------\n");
    printf("Qual território vai atacar? (Digite o número)\n");
    int atacante;
    scanf("%d", &atacante);
    limparBufferEntrada();
    if (atacante < 1 || atacante > NUM_TERRITORIOS) {
        printf("Território inválido!\n");
        return;
    }
    if (territorio[atacante - 1].num_tropas < 2) {
        printf("Esse território não tem tropas suficientes para atacar!\n");
        return;
    }
        
    printf("Qual território vai ser atacado? (Digite o número)\n");
    int atacado;
    scanf("%d", &atacado);
    limparBufferEntrada();
    if (atacado < 1 || atacado > NUM_TERRITORIOS) {
        printf("Território inválido!\n");
        return;
    }


    simularAtaque(&atacante, &atacado, territorio);
    
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(int* atacante, int* atacado, Territorio* territorio) {

    int dadoAtacante = rand() % 6 + 1;
    int dadoAtacado = rand() % 6 + 1;
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do atacado: %d\n", dadoAtacado);

    if (dadoAtacante <= dadoAtacado) {
        territorio[*atacante - 1].num_tropas--;
        return;
    }

    territorio[*atacado - 1].num_tropas--;
    if (territorio[*atacado - 1].num_tropas <= 0) {
        strcpy(territorio[*atacado - 1].cor, territorio[*atacante - 1].cor);
        territorio[*atacado - 1].num_tropas = 1;
        territorio[*atacante - 1].num_tropas--;
    }
}
// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
