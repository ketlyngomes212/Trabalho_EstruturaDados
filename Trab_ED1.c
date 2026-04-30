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
// cria uma estrtutura de Lista externa
typedef struct ListaExterna{
    Matriz_Esparsa *lista;  // Aponta para o primeiro valor não-zero desta matriz. 
    int  lin, col;          // dimensões da matriz
    int id;                 //id da matriz
    struct ListaExterna *prox; //prox ponterio da lista externa
} ListaExterna;

ListaExterna *matrizes = NULL; //Ponteiro global da lista externa, aponta para a primeira matriz
int contadorID = 1; //contador pra id das matrizes

//--------------------------------------------------------------------------------------------------------
// Protótipos das funções
Matriz_Esparsa * Cria_Nodo(float dado, int lin, int col);
void inserir_lista(Matriz_Esparsa **lista, float dado, int lin, int col);
void criar_Matriz();
void listar_Matrizes();
float buscar_Lista(Matriz_Esparsa *lista, int lin, int col);
ListaExterna *buscar_IdMatriz(int id);
void imprimir_Matriz(int id);
ListaExterna *criar_resultado(int lin, int col);
void inserir_acumulando(Matriz_Esparsa **lista, float valor, int lin, int col);
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
Matriz_Esparsa * Cria_Nodo(float dado, int lin, int col){
    Matriz_Esparsa *p; // Declara um ponteiro
    p = (Matriz_Esparsa *) malloc(sizeof(Matriz_Esparsa)); //Aloca memória para 1 nodo
     if(!p){ //Se p == NULL → falha de memória
        printf("Problema de alocação");
        exit(0); // mostra o erro e encerra
    }

    p->dado = dado; // guardar dado
    p->lin = lin; //guarda a linha
    p->col = col; //guarda a colunha
    p->prox = NULL; //ainda não aponta pra ninguem
    return p; // Retorna o endereço do nodo criado
}

//--------------------------------------------------------------------------------------------------------
//Uma função que insere na lista encadeada um nodo alocado;
//é um insere ordernado
void inserir_lista(Matriz_Esparsa **lista, float dado, int lin, int col){
    Matriz_Esparsa *novo; // Cria um novo nodo
    novo = Cria_Nodo(dado, lin, col); // Cria um novo nodo na memoria
    Matriz_Esparsa *aux  = *lista; // começa no começo da lista
    Matriz_Esparsa *ant  = NULL; //anterior começa como null
 
    //enq n acabar a lista, linha do "1" nó < menor que do novo OU(linha igual a linha do novo E coluna do "1" nó menor que a coluna do novo)
    while(aux != NULL && (aux->lin < lin || (aux->lin == lin && aux->col < col))){
        ant = aux; //ant recebe o aux
        aux = aux->prox; //aux caminha pro proximo nó
    }
 
    novo->prox = aux; // quando achou o maior, o novo nó aponta pra esse maior
    if(ant == NULL) { // se for o primeiro ou unico
        *lista = novo;  // inseriu no início
    } else{
        ant->prox = novo;}  // anterior agora aponta pro novo nó
}

//--------------------------------------------------------------------------------------------------------
//Uma função que lê os dados da matriz, via teclado, e insere na lista encadeada somente os dados diferentes de zero;
void criar_Matriz() {

    ListaExterna *m = (ListaExterna *) malloc(sizeof(ListaExterna)); //cria um nodo pra lista externa
    if(!m){ // se der erro de alocação
        printf("Erro de memoria\n");
        exit(0);
    }
    
    m->lista = NULL; // Lista interna começa vazia
    m->id = contadorID; // recebe o numero do contador de id
    contadorID++;  // contador id soma;
    
    printf("\nMatriz %d", m->id);
    printf("\nDigite a quantidade de colunas:"); //pede qtd de linhas da matriz
    scanf("%d", &m->col);
    printf("Digite a quantidade de linhas:"); //pede qtd de coluna da matriz
    scanf("%d", &m->lin);
    
    if (m->lin == 0 || m->col == 0) { // se alguma for zero dá erro
        printf("Matriz sem dimensoes!\n");
        free(m); //libera a matriz já que n criou
        contadorID--; // tira o contador
        return; //volta
    }

    float dado;
    int lin, col;
    printf("---Informe os dados, para encerrar digite 0 no valor---\n");
    
    while(1){
        printf("Digite o valor: "); //pede valor
        scanf("%f", &dado);
        if(dado == 0){ //se valor for zero ele encerra 
            break;}
        printf("Digite a linha: "); //informar linha
        scanf("%d", &lin);

        printf("Digite a coluna: "); //informar coluna
        scanf("%d", &col);
        

        if(lin < 0 || lin >= m->lin || col < 0 || col >= m->col){ // se a linha for negat ou se linha for maior que a linha definida pra dimensao (msm coisa pra coluna)
            printf("Posicao invalida! Tente novamente.\n"); // dá erro
        } else {
            int existe = 0;      // verifica se posição já foi usada
            Matriz_Esparsa *aux = m->lista;
            while(aux != NULL){
                if(aux->lin == lin && aux->col == col){ 
                    existe = 1; 
                    break; }
                aux = aux->prox;
            }

            if(existe){
                printf("Posicao [%d][%d] ja ocupada por %g, tente outra.\n", lin, col,buscar_Lista(m->lista, lin, col));
            } else {
                inserir_lista(&m->lista, dado, lin, col);
                printf("[%d][%d] = %.2f inserido.\n", lin, col, dado);
            }
        }
    }
    
    if(m->lista == NULL) { //se a lista interna continuar zerada
        printf("Matriz nao criada.\n");
        free(m); //libera a memoria alocada
        contadorID--; // diminui contador
        return;
    }
    
    
    m->prox  = matrizes; // o prox da lista externa se conecta com a nova matriz
    matrizes = m; // ponteiro global agora aponta pra nova matriz (ela é a primeira agora)
    
    printf("\nMatriz %d [%dx%d] criada com sucesso!\n", m->id, m->lin, m->col);
}
//--------------------------------------------------------------------------------------------------------
//Uma função que lista as matrizes existentes
void listar_Matrizes(){
    ListaExterna *aux = matrizes; // aux pra percorre a lista externa recebe o 1 nó(matriz)

    if(aux == NULL){ //se o pont externo é null = lista vazia
        printf("\nNenhuma matriz criada ainda.");
        return;
    }

    printf("\nMATRIZES:\n");
    while(aux != NULL){ //eqt aux não for null
         printf("Matriz %d: [%dx%d]\n", aux->id, aux->lin, aux->col); // imprime o id,lin e col
         aux = aux->prox; //percorre a lista
    }
    printf("\n");

}
//--------------------------------------------------------------------------------------------------------
//Uma função que busca os dados em uma lista encadeada;
float buscar_Lista(Matriz_Esparsa *lista, int lin, int col){
    Matriz_Esparsa *aux = lista; // aux recebe oq aponta pro inicio lista interna
    while (aux != NULL) { //enq não acabar lista
        if (aux->lin == lin && aux->col == col){ // Compara linha e coluna
             return aux->dado;} // retorna o valor
        aux = aux->prox; // vai pro prox nó
    }
    return 0; // se n achou nada retorna zero
}


//--------------------------------------------------------------------------------------------------------
//Uma função que retorna o id de uma matriz;
ListaExterna *buscar_IdMatriz(int id){
    ListaExterna *aux = matrizes; // recebe o primeiro nó da lista externa
    while (aux != NULL) { //enq não acabar lista
        if(aux->id == id){
            return aux;} // Retorna o ponteiro para a Matriz
    aux = aux->prox; // vai pro prox nó
    }

    return NULL; //id não existe
}

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime todos os dados da matriz, inclusive os zeros;
void imprimir_Matriz(int id){
    ListaExterna *m = buscar_IdMatriz(id); //retorna o ponteiro da matriz com o id pedido
    
    if(!m){
        printf("Matriz %d nao existe.\n", id);
        return;
    }

    printf("\nMatriz %d: [%dx%d]\n", m->id,m->lin,m->col);

    for(int i = 0; i < m->lin; i++){
        for(int j = 0; j < m->col; j++){
            printf("%g\t", buscar_Lista(m->lista, i, j)); //busca o valor da posição informada
        }
        printf("\n");
    }
}

//--------------------------------------------------------------------------------------------------------
// Função que insere um novo valor na matriz ou atualiza um valor já existente (MULTIPLICAÇÃO)
void inserir_acumulando(Matriz_Esparsa **lista, float valor, int lin, int col){
    Matriz_Esparsa *aux = *lista; //começa no inicio 

    while(aux != NULL){
        if(aux->lin == lin && aux->col == col){ //se posição já existe
            aux->dado += valor; //soma no valor existente
            return; // volta
        }
        aux = aux->prox; // se não continua procurando
    }

    if(valor != 0){ 
        inserir_lista(lista, valor, lin, col); // se valor diferente de zero, cria um novo nó
    }
}
//--------------------------------------------------------------------------------------------------------
// Função que cria e inicializa uma nova matriz esparsa vazia
ListaExterna *criar_resultado(int lin, int col){
    ListaExterna *m; // declara  um ponteiro
    m = (ListaExterna *) malloc(sizeof(ListaExterna)); //alocar um nó na lista externa

    if(!m){
        printf("Erro de memoria\n");
        exit(0);
    }

    m->lista = NULL; // lista interna vazia
    m->lin = lin; //guarda a linha
    m->col = col; //guarda a coluna
    m->id = contadorID++; //soma o contador
    m->prox = matrizes; //nova matriz aponta pro ponteiro externo (primeira matriz) 
    matrizes = m; // ponteiro externo agora aponta pra nova matriz (ela se tornou a primeira)
    
    return m; //retorna matrizResultado criada
}


//--------------------------------------------------------------------------------------------------------
//Uma função que soma duas matrizes;
void soma_matrizes(int id1, int id2){
    ListaExterna *m1 = buscar_IdMatriz(id1); //Retorna os ponteiros
    ListaExterna *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){ //verfica se existe
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->lin != m2->lin || m1->col != m2->col){ // se tiverem dimensões diferentes
        printf("Dimensoes incompativeis.\n");
        return;
    }

    ListaExterna *res = criar_resultado(m1->lin, m1->col); //cria uma matriz para o resultado

    for(int i = 0; i < m1->lin; i++){
        for(int j = 0; j < m1->col; j++){
          // busca o valor daquela posicao da matriz 1 e soma com o valor da matriz 2
            float v = buscar_Lista(m1->lista, i, j) + buscar_Lista(m2->lista, i, j);

            if(v != 0){ // se o valor não for zero
                inserir_lista(&res->lista, v, i, j); // ele é adicionado na matriz resultado
            }
        }
    }

    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que subtrai duas matrizes;
void subtrair_matrizes(int id1, int id2){
    ListaExterna *m1 = buscar_IdMatriz(id1); // recebe o ponteiro da matriz
    ListaExterna *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){ 
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->lin != m2->lin || m1->col != m2->col){ // se tiverem dimensões diferentes
        printf("Dimensoes incompativeis.\n");
        return;
    }

    ListaExterna *res = criar_resultado(m1->lin, m1->col); //cria a matriz resultado

    for(int i = 0; i < m1->lin; i++){
        for(int j = 0; j < m1->col; j++){
        // busca o valor daquela posicao da matriz 1 e subtrai com o valor da matriz 2
            float v = buscar_Lista(m1->lista, i, j) - buscar_Lista(m2->lista, i, j);

            if(v != 0){ // se o valor for diferente de zero adiciona na nova matriz
                inserir_lista(&res->lista, v, i, j);
            }
        }
    }

    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}
//--------------------------------------------------------------------------------------------------------
//Uma função que multiplica duas matrizes;
void multiplicar_matrizes(int id1, int id2){
    ListaExterna *m1 = buscar_IdMatriz(id1); //recebe ponteiro
    ListaExterna *m2 = buscar_IdMatriz(id2);

    if(!m1 || !m2){
        printf("Uma das matrizes nao existe.\n");
        return;
    }

    if(m1->col != m2->lin){
        printf("Dimensoes incompativeis.\n"); 
        return;
    }

    ListaExterna *res = criar_resultado(m1->lin, m2->col); //cria a matriz resultado com as dimensões certas

    Matriz_Esparsa *a = m1->lista; //pega a lista interna da matriz 1

     while(a != NULL){  // Para cada elemento não-nulo de A
        Matriz_Esparsa *b = m2->lista;  // lista interna da matriz B
        
        while(b != NULL){  // Para cada elemento não-nulo de B
            // Se coluna de A == linha de B 
            if(a->col == b->lin){
                int i = a->lin;      // Linha do resultado = linha de A
                int j = b->col;      // Coluna do resultado = coluna de B
                float valor = a->dado * b->dado;  
                
                // innserir na posição do resultado
                inserir_acumulando(&res->lista, valor, i, j);
            }
            b = b->prox;
        }
        a = a->prox;
    }

    printf("\nResultado armazenado na matriz %d [%dx%d]\n", res->id, res->lin, res->col);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que gera a matriz transposta;
void transposta(int id){
    ListaExterna *m = buscar_IdMatriz(id);

    if(!m){
        printf("Matriz nao encontrada.\n");
        return;
    }

    ListaExterna *res = criar_resultado(m->col, m->lin);

    Matriz_Esparsa *aux = m->lista; //lista interna da matriz

    while(aux != NULL){
        inserir_lista(&res->lista, aux->dado, aux->col, aux->lin); // troca a coluna pela linha na hora de inserir na matriz
        aux = aux->prox;
    }
    
    printf("\nResultado armazenado na matriz %d\n", res->id);
    printf("\nMatriz resultado:\n");
    imprimir_Matriz(res->id);
}

//--------------------------------------------------------------------------------------------------------
//Uma função que imprime os elementos da diagonal principal, inclusive os zeros caso existam.
void diagonal_principal(int id){
    ListaExterna *m = buscar_IdMatriz(id);

    if(!m){
        printf("Matriz nao encontrada.\n");
        return;
    }

    if(m->lin != m->col){
        printf("Matriz %d nao e quadrada;\n", id);
        return;
    }


    printf("\nDiagonal principal da matriz %d:\n", id);

    for(int i = 0; i < m->lin; i++){
        printf(" %g ", buscar_Lista(m->lista, i, i)); //Linha e coluna são o mesmo valor i.
    }
    printf("\n");
}

//--------------------------------------------------------------------------------------------------------
//Uma função que libera da memória uma lista encadeada;
void liberar_Lista(Matriz_Esparsa **lista){
    Matriz_Esparsa *aux; //cria uma aux
    while (*lista != NULL){ //enq n terminar lista
        aux = *lista; //aux recebe "1" valor
        *lista = (*lista)->prox; // lista avança pro prox
        free(aux); // libera o valor
    }
}

//--------------------------------------------------------------------------------------------------------
//liberar toda a lista
void liberar_TodasMatrizes(){
    ListaExterna *aux;
    while(matrizes != NULL){
        aux = matrizes; // aux recebe o primeiro valor da lista externa
        matrizes = matrizes->prox; // ponteiro externo começa a apontar pro segundo valor
        liberar_Lista(&aux->lista); // libera lista interna
        free(aux); //libera lista externa
    }

    //reseta tudo
    matrizes = NULL;
    contadorID = 1;
    printf("Todas as matrizes foram apagadas");

}

//--------------------------------------------------------------------------------------------------------
//liberar especifica
void liberar_Matriz(int id){
    ListaExterna *aux = matrizes; //aux recebe inicio
    ListaExterna *ant = NULL; //anterior null por eqt
    
    while( aux != NULL && aux->id != id ){ //enquanto não achar o a matriz do id
        ant = aux; 
        aux = aux->prox;
    }

    if(aux == NULL){
        printf("\n Matriz nao encontrada.");
        return;
    }

    liberar_Lista(&aux->lista); //liberar a lista interna

    if(ant == NULL){  //se ela é a primeira ou unica
        matrizes = aux->prox; //o ponteiro externo aponta pro 2 valor /se unica = NULL
    } else {
        ant->prox = aux->prox; //anterior passa a apontar pra prox da matriz retirada (a->b->c // a->c)
    }
    free(aux);

    if (matrizes == NULL) { // se a loista ficou vazia o cont vira 1 dnv
        contadorID = 1;
    }

    printf("Matriz removida da lista!");
    
}

// busca um valor em uma matriz e mostra onde ele está
void buscar_valorEspecifico(int id){
    ListaExterna *m = buscar_IdMatriz(id); //retorna o pont
    if(!m){ 
        printf("Matriz nao encontrada.\n"); 
        return; }
 
    float valor;
    printf("Digite o numero para buscar: ");
    scanf("%f", &valor);

    int achou = 0;
    Matriz_Esparsa *aux = m->lista;
    while(aux != NULL){
        if(aux->dado == valor){
            printf("Valor %g encontrado em [%d][%d]\n", valor, aux->lin, aux->col);
            achou = 1;
        }
        aux = aux->prox;
    }
    if(!achou) 
        printf("Valor %g nao encontrado na matriz %d.\n", valor, id);
}

//--------------------------------------------------------------------------------------------------------
int main (){
    int opcao, id1, id2;
    
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
        printf("10 - Listar todas as matrizes\n");
        printf("11 - Buscar valor\n");
        printf("0 - Sair\n");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        
    switch (opcao){
        case 1:
            criar_Matriz();
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

        case 10: 
            listar_Matrizes();
            break;

        case 11:
            if(matrizes == NULL){ 
                printf("\nNenhuma matriz cadastrada.\n"); 
            } else { 
                listar_Matrizes(); 
                printf("Digite o ID: "); 
                scanf("%d", &id1); 
                buscar_valorEspecifico(id1); }
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