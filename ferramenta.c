#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ferramenta.h"


int lerInteiro(const char *mensagem) {
    char buffer[64];
    int valor;

    while (1) {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }
        if (sscanf(buffer, "%d", &valor) == 1) {
            return valor;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
}

/* Lê um float com validação (use ponto: 12.50). */
static float lerFloat(const char *mensagem) {
    char buffer[64];
    float valor;

    while (1) {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }
        if (sscanf(buffer, "%f", &valor) == 1) {
            return valor;
        }
        printf("Valor invalido. Use ponto como separador (ex.: 12.50).\n");
    }
}

/* Lê um texto (com espaços), remove o '\n' e não aceita vazio. */
static void lerTexto(const char *mensagem, char *destino, int tamanho) {
    char *p;

    while (1) {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) == NULL) {
            destino[0] = '\0';
            return;
        }

        p = strchr(destino, '\n');
        if (p != NULL) {
            *p = '\0';                 /* tira o \n */
        } else {
            int c;                     /* texto maior que o limite: descarta o resto */
            while ((c = getchar()) != '\n' && c != EOF);
        }

        for (p = destino; *p != '\0'; p++) {
            if (*p == ';') *p = ',';   /* ';' é o separador do arquivo */
        }

        if (destino[0] != '\0') return;
        printf("Este campo nao pode ficar vazio.\n");
    }
}

void inicializar(Ferramenta *vetor[]) {
    int i;
    for (i = 0; i < MAX; i++) {
        vetor[i] = NULL;       /* exigência: todos começam em NULL */
    }
}

void liberar(Ferramenta *vetor[]) {
    int i;
    for (i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            free(vetor[i]);
            vetor[i] = NULL;   /* evita ponteiro "pendurado" */
        }
    }
}

int buscarPorCodigo(Ferramenta *vetor[], int codigo) {
    int i;
    for (i = 0; i < MAX; i++) {
        if (vetor[i] != NULL && vetor[i]->codigo == codigo) {
            return i;
        }
    }
    return -1;
}

void exibirFerramenta(const Ferramenta *f) {
    printf("Codigo     : %d\n", f->codigo);
    printf("Descricao  : %s\n", f->descricao);
    printf("Marca      : %s\n", f->marca);
    printf("Quantidade : %d\n", f->quantidade);
    printf("Valor unit.: R$ %.2f\n", f->valor);
    printf("Valor total: R$ %.2f\n", f->quantidade * f->valor);
}

int cadastrar(Ferramenta *vetor[]) {
    int i, livre = -1, codigo;
    Ferramenta *nova;


    for (i = 0; i < MAX; i++) {
        if (vetor[i] == NULL) {
            livre = i;
            break;
        }
    }
    if (livre == -1) {
        printf("Capacidade maxima (%d) atingida.\n", MAX);
        return 0;
    }


    codigo = lerInteiro("Codigo patrimonial: ");
    if (buscarPorCodigo(vetor, codigo) != -1) {
        printf("Ja existe uma ferramenta com esse codigo.\n");
        return 0;
    }


    nova = (Ferramenta *) malloc(sizeof(Ferramenta));
    if (nova == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return 0;
    }

    /* 4. preenche os campos */
    nova->codigo = codigo;
    lerTexto("Descricao: ", nova->descricao, TAM_DESC);
    lerTexto("Marca: ", nova->marca, TAM_MARCA);

    do {
        nova->quantidade = lerInteiro("Quantidade disponivel: ");
        if (nova->quantidade < 0) printf("Nao pode ser negativa.\n");
    } while (nova->quantidade < 0);

    do {
        nova->valor = lerFloat("Valor estimado (unidade): R$ ");
        if (nova->valor < 0) printf("Nao pode ser negativo.\n");
    } while (nova->valor < 0);

    
    vetor[livre] = nova;
    printf("Ferramenta cadastrada com sucesso!\n");
    return 1;
}

void listar(Ferramenta *vetor[]) {
    int i, total = 0;

    for (i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            printf("\n--- Ferramenta %d ---\n", ++total);
            exibirFerramenta(vetor[i]);
        }
    }
    if (total == 0) {
        printf("Nenhuma ferramenta cadastrada.\n");
    }
}

void relatorio(Ferramenta *vetor[]) {
    int i, qtdRegistros = 0, qtdItens = 0, idxMaior = -1;
    double totalGeral = 0.0, totalItem, maiorTotal = 0.0;

    for (i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            totalItem = vetor[i]->quantidade * (double) vetor[i]->valor;
            totalGeral += totalItem;
            qtdItens += vetor[i]->quantidade;
            qtdRegistros++;

            if (idxMaior == -1 || totalItem > maiorTotal) {
                maiorTotal = totalItem;
                idxMaior = i;
            }
        }
    }

    if (qtdRegistros == 0) {
        printf("Nao ha dados para gerar o relatorio.\n");
        return;
    }

    printf("\n===== RELATORIO PATRIMONIAL =====\n");
    printf("Tipos de ferramentas : %d\n", qtdRegistros);
    printf("Total de unidades    : %d\n", qtdItens);
    printf("Valor total acumulado: R$ %.2f\n", totalGeral);
    printf("Maior valor acumulado: %s (R$ %.2f)\n",
           vetor[idxMaior]->descricao, maiorTotal);
}

int carregar(Ferramenta *vetor[]) {
    FILE *arq;
    char linha[256];
    Ferramenta *f;
    int n = 0;

    arq = fopen(ARQUIVO, "r");
    if (arq == NULL) {
        return 0;          /* arquivo não existe: começa vazio, sem erro */
    }

    while (n < MAX && fgets(linha, sizeof(linha), arq) != NULL) {
        f = (Ferramenta *) malloc(sizeof(Ferramenta));
        if (f == NULL) {
            printf("Erro: memoria insuficiente ao carregar.\n");
            break;
        }

        /* 49 e 29 = TAM_DESC-1 e TAM_MARCA-1 (reserva espaço para o '\0') */
        if (sscanf(linha, "%d;%49[^;];%29[^;];%d;%f",
                   &f->codigo, f->descricao, f->marca,
                   &f->quantidade, &f->valor) == 5) {
            vetor[n++] = f;
                   } else {
                       free(f);       /* linha inválida: devolve a memória */
                   }
    }

    fclose(arq);
    return n;
}

int salvar(Ferramenta *vetor[]) {
    FILE *arq;
    int i;

    arq = fopen(ARQUIVO, "w");
    if (arq == NULL) {
        printf("Erro ao abrir '%s' para escrita.\n", ARQUIVO);
        return 0;
    }

    for (i = 0; i < MAX; i++) {
        if (vetor[i] != NULL) {
            fprintf(arq, "%d;%s;%s;%d;%.2f\n",
                    vetor[i]->codigo, vetor[i]->descricao,
                    vetor[i]->marca, vetor[i]->quantidade,
                    vetor[i]->valor);
        }
    }

    fclose(arq);
    return 1;
}

