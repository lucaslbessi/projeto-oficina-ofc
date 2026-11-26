#ifndef FERRAMENTA_H
#define FERRAMENTA_H

#define MAX        100
#define TAM_DESC   50
#define TAM_MARCA  30
#define ARQUIVO    "dados.txt"

typedef struct {
    int   codigo;
    char  descricao[TAM_DESC];
    char  marca[TAM_MARCA];
    int   quantidade;
    float valor;
} Ferramenta;


void inicializar(Ferramenta *vetor[]);
void liberar(Ferramenta *vetor[]);


int  carregar(Ferramenta *vetor[]);
int  salvar(Ferramenta *vetor[]);


int  cadastrar(Ferramenta *vetor[]);
void listar(Ferramenta *vetor[]);
int  buscarPorCodigo(Ferramenta *vetor[], int codigo);
void exibirFerramenta(const Ferramenta *f);
void relatorio(Ferramenta *vetor[]);


int  lerInteiro(const char *mensagem);

#endif
