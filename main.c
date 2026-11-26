#include <stdio.h>
#include "ferramenta.h"

int main(void) {
    Ferramenta *vetor[MAX];
    int opcao, codigo, pos, carregados;

    inicializar(vetor);

    carregados = carregar(vetor);
    printf("%d ferramenta(s) carregada(s) do arquivo.\n", carregados);

    do {
        printf("\n===== CONTROLE DE FERRAMENTAL E OFICINA =====\n");
        printf("1 - Cadastrar ferramenta\n");
        printf("2 - Listar todas\n");
        printf("3 - Buscar por codigo\n");
        printf("4 - Relatorio patrimonial\n");
        printf("5 - Salvar e sair\n");
        opcao = lerInteiro("Escolha: ");

        switch (opcao) {
            case 1:
                cadastrar(vetor);
                break;
            case 2:
                listar(vetor);
                break;
            case 3:
                codigo = lerInteiro("Codigo a buscar: ");
                pos = buscarPorCodigo(vetor, codigo);
                if (pos == -1) {
                    printf("Ferramenta nao encontrada.\n");
                } else {
                    exibirFerramenta(vetor[pos]);
                }
                break;
            case 4:
                relatorio(vetor);
                break;
            case 5:
                if (salvar(vetor)) {
                    liberar(vetor);
                    printf("Dados salvos. Ate logo!\n");
                } else {
                    printf("Nao foi possivel salvar. Tente novamente.\n");
                    opcao = 0;   /* não sai, para não perder os dados */
                }
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 5);

    return 0;
}
