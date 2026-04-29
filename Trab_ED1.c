#include <stdio.h>

// Cria o nodo
typedef struct nodo 
{
 float dado;
 int lin, col;
 struct nodo *prox;
}Matriz_Esparsa; 

//--------------------------------------------------------------------------------------------------------
// cria uma estrtutura de matriz - Lista encadeada + dimensões
typedef struct Matriz{
    Matriz_Esparsa *lista;  // lista com os valores não-zero
    int  lin, col;          // dimensões da matriz
    int id;
    struct Matriz* prox;
} Matriz;

Matriz *matrizes = NULL;
int contadorID = 0;

//--------------------------------------------------------------------------------------------------------
//Uma função que faz a alocação de memória para cada nodo criado para uma lista encadeada;
Matriz_Esparsa * Cria_Matriz(float dado, int lin, int col){
    Matriz_Esparsa *p; // Declara um ponteiro
    p = (Matriz_Esparsa *) malloc(sizeof(Matriz_Esparsa)); //Aloca memória para 1 nodo
     if(!p){ //Se p == NULL → falha de memória
        printf("Problema de alocação");
        exit(0); // mostra o erro e encerra
    }

    p->dado = dado; // guardar dado
    p->lin = lin; //guarda a linha
    p->col = col; //guarda a colunha
    return p; // Retorna o endereço do nodo criado
}

//--------------------------------------------------------------------------------------------------------
//Uma função que insere na lista encadeada um nodo alocado;
void inserir_lista(Matriz_Esparsa **lista, int dado, int lin, int col){
    Matriz_Esparsa *novo; // Cria um novo nodo
    novo = Cria_Matriz(dado, lin, col); // Cria um novo nodo na memoria
    novo->prox = *lista; // novo nodo aponta para o antigo primeiro/ ou null se estiver vazia
    *lista = novo; //N (lista) agora aponta para o novo primeiro nodo
}

//--------------------------------------------------------------------------------------------------------
//Uma função que busca os dados em uma lista encadeada;

//--------------------------------------------------------------------------------------------------------
//Uma função que libera da memória uma lista encadeada;

//--------------------------------------------------------------------------------------------------------
//Uma função que lê os dados da matriz, via teclado, e inseri na lista encadeada somente os dados diferentes de zero;
void criar_Matriz(){
    Matriz *m = (Matriz*) malloc(sizeof(Matriz));
    if(!m){
        printf("Erro de memoria\n");
        exit(0);
    }

    m->lista = NULL;
    m->id = contadorID++;
    m->prox = matrizes;
    matrizes = m;

    float dado;
    m->id = contadorID++;
    printf("\nDigite a quantidade de colunas:");
    scanf("%d", &m->col);
    printf("\nDigite a quantidade de linhas:");
    scanf("%d", &m->lin);

    if (m->lin == 0 || m->col == 0) {
        printf("Matriz sem dimensoes\n");
        return;
    }

    for (int i = 0; i < m->lin; i++){
        for (int j = 0; j < m->col; j++){
            printf("Digite o valor para [%d][%d]:", i, j);
            scanf("%f", &dado);

            if(dado != 0){
                inserir_lista(&m->lista, dado, i, j);
            }
        }
    }

    printf("Matriz %d [%dx%d] criada com sucesso!\n", m->id, m->lin, m->col);

}

//--------------------------------------------------------------------------------------------------------
//Uma função que soma duas matrizes;

//--------------------------------------------------------------------------------------------------------
//Uma função que subtrai duas matrizes;

//--------------------------------------------------------------------------------------------------------
//Uma função que multiplica duas matrizes;

//--------------------------------------------------------------------------------------------------------
//Uma função que gera a matriz transposta;

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime todos os dados da matriz, inclusive os zeros;

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime os elementos da diagonal principal, inclusive os zeros caso existam.

void main (){
    int opcao;
 
    do {
        printf("===========  MENU ===========\n");
        printf("1 - Criar matriz\n");
        printf("2 - Imprimir matriz\n");
        printf("3 - Diagonal principal\n");
        printf("4 - Soma das matrizes\n");
        printf("5 - Subtracao das matrizes\n");
        printf("6 - Multiplicacao das matrizes\n");
        printf("7 - Transposta da matriz\n");
        printf("8 - Apagar matrizes\n");
        printf("0 - Sair\n");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
    } while ( opcao != 0);
    
    switch (opcao){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;

        case 0:
          printf("Saindo..\n");
            break;

        default:
            printf("Opcao invalida!\n");
    } 
}