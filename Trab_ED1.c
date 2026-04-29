#include <stdio.h>
#include <stdlib.h>

// Cria o nodo - Lista interna
typedef struct nodo 
{
 float dado;
 int lin, col;
 struct nodo *prox;
}Matriz_Esparsa; 

//--------------------------------------------------------------------------------------------------------
// cria uma estrtutura de matriz - Lista externa
typedef struct Matriz{
    Matriz_Esparsa *lista;  // lista com os valores não-zero
    int  lin, col;          // dimensões da matriz
    int id;
    struct Matriz* prox;
} Matriz;

Matriz *matrizes = NULL; //Ponteiro global da lista externa - aponta para a primeira matriz
int contadorID = 1; //contador pra id das matrizes

//--------------------------------------------------------------------------------------------------------
// Protótipos das funções
Matriz_Esparsa * Cria_Matriz(float dado, int lin, int col);
void inserir_lista(Matriz_Esparsa **lista, float dado, int lin, int col);

void criar_matriz();
void listar_Matrizes();
Matriz *criar_resultado(int lin, int col);
void adicionar_matriz(Matriz *m);

float buscar_Lista(Matriz_Esparsa *lista, int lin, int col);
Matriz *buscar_IdMatriz(int id);

void imprimir_Matriz(int id);

void soma_matrizes(int id1, int id2);
void subtrair_matrizes(int id1, int id2);
void multiplicar_matrizes(int id1, int id2);
void transposta(int id);
void diagonal_principal(int id);

void liberar_Lista(Matriz_Esparsa **lista);
void liberar_TodasMatrizes();
void liberar_Matriz(int id);

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
void inserir_lista(Matriz_Esparsa **lista, float dado, int lin, int col){
    Matriz_Esparsa *novo; // Cria um novo nodo
    novo = Cria_Matriz(dado, lin, col); // Cria um novo nodo na memoria
    novo->prox = *lista; // novo nodo aponta para o antigo primeiro/ ou null se estiver vazia
    *lista = novo; //N (lista) agora aponta para o novo primeiro nodo
}
//--------------------------------------------------------------------------------------------------------
// Função que insere um novo valor na matriz ou atualiza um valor já existente
void inserir_ou_atualizar(Matriz_Esparsa **lista, float valor, int lin, int col){
    Matriz_Esparsa *aux = *lista;

    while(aux != NULL){
        if(aux->lin == lin && aux->col == col){
            aux->dado += valor;
            return;
        }
        aux = aux->prox;
    }

    if(valor != 0){
        inserir_lista(lista, valor, lin, col);
    }
}

//--------------------------------------------------------------------------------------------------------
//Uma função que lê os dados da matriz, via teclado, e insere na lista encadeada somente os dados diferentes de zero;
void criar_matriz() {
    float dado; // variavel temporaria
    Matriz *m = (Matriz *) malloc(sizeof(Matriz)); //cria um nodo pra lista externa
    if(!m){
        printf("Erro de memoria\n");
        exit(0);
    }
    
    m->lista = NULL; // Lista interna começa vazia
    m->id = contadorID; // recebe o numero do contador de id
    contadorID++;  // contador id soma;
    
    printf("\nMatriz %d", m->id);
    printf("\nDigite a quantidade de colunas:");
    scanf("%d", &m->col);
    printf("Digite a quantidade de linhas:");
    scanf("%d", &m->lin);
    
    if (m->lin == 0 || m->col == 0) {
        printf("Matriz sem dimensoes!\n");
        free(m); //libera a matriz já que n criou
        contadorID--; // tira o contador
        return; //volta
    }
    for (int i = 0; i < m->lin; i++){
        for (int j = 0; j < m->col; j++){
            printf("Digite o valor para [%d][%d]:", i, j);
            scanf("%f", &dado);
            
            if(dado != 0){
                inserir_lista(&m->lista, dado, i, j); // insere na lista interna os numeros diferente de 0
            }
        }
    }
    
    m->prox  = matrizes; // o prox da lista externa se conecta com a nova matriz
    matrizes = m; // ponteiro global agora aponta pra nova matriz (ela é a primeira agora)
    
    printf("\nMatriz %d [%dx%d] criada com sucesso!\n", m->id, m->lin, m->col);
}

//--------------------------------------------------------------------------------------------------------
// Função que cria e inicializa uma nova matriz esparsa vazia
Matriz *criar_resultado(int lin, int col){
    Matriz *m = (Matriz *) malloc(sizeof(Matriz));

    if(!m){
        printf("Erro de memoria\n");
        exit(0);
    }

    m->lista = NULL;
    m->lin = lin;
    m->col = col;
    m->id = contadorID++;
    m->prox = NULL;

    return m;
}
//--------------------------------------------------------------------------------------------------------
//função que adiciona a nova matriz na lista de matrizes
void adicionar_matriz(Matriz *m){
    m->prox = matrizes;
    matrizes = m;
}

//--------------------------------------------------------------------------------------------------------
//Uma função que lista as matrizes existentes
void listar_Matrizes(){
    Matriz *aux = matrizes; // recebe o primeiro nó

    if(aux == NULL){ //se o pont externo é null = lista vazia
        printf("\nNenhuma matriz criada ainda.");
        return;
    }

    printf("\nMATRIZES:\n");
    while(aux != NULL){
         printf("Matriz %d: [%dx%d]\n", aux->id, aux->lin, aux->col);
         aux = aux->prox;
    }
    printf("\n");

}
//--------------------------------------------------------------------------------------------------------
//Uma função que busca os dados em uma lista encadeada;
float buscar_Lista(Matriz_Esparsa *lista, int lin, int col){
    Matriz_Esparsa *aux = lista;
    while (aux != NULL) { //enq não acabar lista
        if (aux->lin == lin && aux->col == col){ // Compara linha e coluna
             return aux->dado;} // retorna o valor

        aux = aux->prox; // vai pro prox nó
    }
    return 0.0; // se n achou nada retorna zero
}


//--------------------------------------------------------------------------------------------------------
//Uma função que retorna o id de uma matriz;
Matriz *buscar_IdMatriz(int id){
    Matriz *aux = matrizes; // recebe o primeiro nó
    while (aux != NULL) { //enq não acabar lista
        if(aux->id == id){
            return aux; // Retorna o ponteiro para a Matriz
        }
    aux = aux->prox; // vai pro prox nó
    }

    return NULL; //id não existe
}

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime todos os dados da matriz, inclusive os zeros;
void imprimir_Matriz(int id){
    Matriz *m = buscar_IdMatriz(id); //retorna o ponteiro da matriz com o id pedido
    
    if(!m){
        printf("Matriz %d não existe.\n", id);
        return;
    }

    printf("\nMatriz %d: [%dx%d]\n", m->id,m->lin,m->col);

    for(int i = 0; i < m->lin; i++){
        for(int j = 0; j < m->col; j++){
            printf(" %.2f ", buscar_Lista(m->lista, i, j));
        }
        printf("\n");
    }
}

//--------------------------------------------------------------------------------------------------------
//Uma função que soma duas matrizes;
void soma_matrizes(int id1, int id2){
    Matriz *m1 = buscar_IdMatriz(id1);
    Matriz *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->lin != m2->lin || m1->col != m2->col){
        printf("Dimensoes incompatíveis.\n");
        return;
    }

    Matriz *res = criar_resultado(m1->lin, m1->col);

    for(int i = 0; i < m1->lin; i++){
        for(int j = 0; j < m1->col; j++){

            float v = buscar_Lista(m1->lista, i, j) +
                      buscar_Lista(m2->lista, i, j);

            if(v != 0){
                inserir_lista(&res->lista, v, i, j);
            }
        }
    }

    adicionar_matriz(res);

    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que subtrai duas matrizes;
void subtrair_matrizes(int id1, int id2){
    Matriz *m1 = buscar_IdMatriz(id1);
    Matriz *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->lin != m2->lin || m1->col != m2->col){
        printf("Dimensoes incompatíveis.\n");
        return;
    }

    Matriz *res = criar_resultado(m1->lin, m1->col);

    for(int i = 0; i < m1->lin; i++){
        for(int j = 0; j < m1->col; j++){

            float v = buscar_Lista(m1->lista, i, j) -
                      buscar_Lista(m2->lista, i, j);

            if(v != 0){
                inserir_lista(&res->lista, v, i, j);
            }
        }
    }

    adicionar_matriz(res);

    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}
//--------------------------------------------------------------------------------------------------------
//Uma função que multiplica duas matrizes;
void multiplicar_matrizes(int id1, int id2){
    Matriz *m1 = buscar_IdMatriz(id1);
    Matriz *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->col != m2->lin){
        printf("Dimensoes incompatíveis.\n");
        return;
    }

    Matriz *res = criar_resultado(m1->lin, m2->col);

    Matriz_Esparsa *a = m1->lista;

    while(a != NULL){ // percorre só valores não nulos de A

        Matriz_Esparsa *b = m2->lista;

        while(b != NULL){ // percorre só valores não nulos de B

            // condição de multiplicação válida
            if(a->col == b->lin){

                int i = a->lin;
                int j = b->col;

                float valor = a->dado * b->dado;

                // soma acumulada em C[i][j]
                float atual = buscar_Lista(res->lista, i, j);
                float novo = atual + valor;

                // remove valor antigo, se existir
                if(atual != 0){
            
                }

                // insere atualizado
                if(novo != 0){
                    inserir_lista(&res->lista, novo, i, j);
                }
            }

            b = b->prox;
        }

        a = a->prox;
    }

    adicionar_matriz(res);

    printf("\nResultado armazenado na matriz %d [%dx%d]\n", res->id, res->lin, res->col);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que gera a matriz transposta;
void transposta(int id){
    Matriz *m = buscar_IdMatriz(id);

    if(!m){
        printf("Matriz nao encontrada.\n");
        return;
    }

    Matriz *res = criar_resultado(m->col, m->lin);

    Matriz_Esparsa *aux = m->lista;

    while(aux != NULL){
        inserir_lista(&res->lista, aux->dado, aux->col, aux->lin);
        aux = aux->prox;
    }

    adicionar_matriz(res);
    
    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime os elementos da diagonal principal, inclusive os zeros caso existam.
void diagonal_principal(int id){
    Matriz *m = buscar_IdMatriz(id);

    if(!m){
        printf("Matriz nao encontrada.\n");
        return;
    }

    printf("\nDiagonal principal da matriz %d:\n", id);

    for(int i = 0; i < m->lin; i++){
        printf(" %.2f ", buscar_Lista(m->lista, i, i));
    }
    printf("\n");
}

//--------------------------------------------------------------------------------------------------------
//Uma função que libera da memória uma lista encadeada;
void liberar_Lista(Matriz_Esparsa **lista){
    Matriz_Esparsa *aux;
    while (*lista != NULL){
        aux = *lista;
        *lista = (*lista)->prox;
        free(aux);
}
}

//liberar toda a lista
void liberar_TodasMatrizes(){
    Matriz *aux;
    while(matrizes != NULL){
        aux = matrizes;
        matrizes = matrizes->prox;
        liberar_Lista(&aux->lista);
        free(aux);
    }

    matrizes = NULL;
    contadorID = 1;
    printf("Todas as matrizes foram apagadas");

}

//liberar especifica
void liberar_Matriz(int id){
    Matriz *aux = matrizes;
    Matriz *ant = NULL;
    
    while( aux != NULL && aux->id != id ){
        ant = aux;
        aux = aux->prox;
    }

    if(aux == NULL){
        printf("\n Matriz não encontrada.");
        return;
    }

    liberar_Lista(&aux->lista);

    if(ant == NULL){
        matrizes = aux->prox;
    } else {
        ant->prox = aux->prox;
    }
    free(aux);

    printf("Matriz %d removida da lista!", id);
    
}

//--------------------------------------------------------------------------------------------------------
int main (){
    int opcao, id1;
    
    do {
        printf("\n===========  MENU ===========\n");
        printf("1 - Criar matriz\n");
        printf("2 - Imprimir matriz\n");
        printf("3 - Diagonal principal\n");
        printf("4 - Soma das matrizes\n");
        printf("5 - Subtracao das matrizes\n");
        printf("6 - Multiplicacao das matrizes\n");
        printf("7 - Transposta da matriz\n");
        printf("8 - Apagar matriz especifica\n");
        printf("9 - Apagar todas as matrizes\n");
        printf("0 - Sair\n");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        
    switch (opcao){
        case 1:
            criar_matriz();
            break;
        case 2:
            if(matrizes == NULL){
                printf("\nNenhuma matriz para imprimir");
            } else {
                listar_Matrizes();
                printf("Digite o ID: "); 
                scanf("%d", &id1); 
                imprimir_Matriz(id1);
            }
            break;
        case 3:
            if(matrizes == NULL){
                printf("\nNenhuma matriz cadastrada\n");
            } else {
                listar_Matrizes();
                printf("Digite o ID: ");
                scanf("%d", &id1);
                diagonal_principal(id1);
            }
            break;
        case 4: { 
            int id2;
            if(matrizes == NULL){
                printf("\nNenhuma matriz cadastrada\n");
            } else {
                listar_Matrizes();
                printf("Digite ID da matriz 1: ");
                scanf("%d", &id1);
                printf("Digite ID da matriz 2: ");
                scanf("%d", &id2);
                soma_matrizes(id1, id2);
            }
            break;
        }
        case 5: { 
            int id2;
            if(matrizes == NULL){
                printf("\nNenhuma matriz cadastrada\n");
            } else {
                listar_Matrizes();
                printf("Digite ID da matriz 1: ");
                scanf("%d", &id1);
                printf("Digite ID da matriz 2: ");
                scanf("%d", &id2);
                subtrair_matrizes(id1, id2);
            }
            break;
        }
        case 6: { 
            int id2;
            if(matrizes == NULL){
                printf("\nNenhuma matriz cadastrada\n");
            } else {
                listar_Matrizes();
                printf("Digite ID da matriz 1: ");
                scanf("%d", &id1);
                printf("Digite ID da matriz 2: ");
                scanf("%d", &id2);
                multiplicar_matrizes(id1, id2);
            }
            break;
        }
        case 7: 
            if(matrizes == NULL){
                printf("\nNenhuma matriz cadastrada\n");
            } else {
                listar_Matrizes();
                printf("Digite o ID: ");
                scanf("%d", &id1);
                transposta(id1);
            }
            break;
        case 8:
            if(matrizes == NULL){
                printf("\nNenhuma matriz para apagar");
            } else {
                listar_Matrizes();
                printf("Digite o ID para apagar: "); 
                scanf("%d", &id1); 
                liberar_Matriz(id1);
            }
            break;
        case 9:
            if(matrizes == NULL){
                printf("\nNenhuma matriz para apagar");
            } else {
                liberar_TodasMatrizes();
            }
            break;

        case 0:
            liberar_TodasMatrizes();
            printf("\nSaindo..\n");
            break;

        default:
            printf("Opcao invalida!\n");
} 

    } while ( opcao != 0);

    return 0;
}