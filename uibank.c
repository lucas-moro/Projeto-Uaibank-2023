#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char nome[50];
    int idade;
    float saldo_atual;
} Usuario;

void inserirUsuario(Usuario** usuarios, int* totalUsuarios) {
    Usuario* novoUsuario = (Usuario*)malloc(sizeof(Usuario));
    if (novoUsuario == NULL) {
        printf("Erro na alocação de memória.\n");
        return;
    }

    printf("Digite o nome: ");
    fgets(novoUsuario->nome, sizeof(novoUsuario->nome), stdin);
    
    // Remove o caractere de nova linha se presente
    size_t len = strlen(novoUsuario->nome);
    if (len > 0 && novoUsuario->nome[len - 1] == '\n') {
        novoUsuario->nome[len - 1] = '\0';
    }

    printf("Digite a idade: ");
    scanf("%d", &(novoUsuario->idade));
    
    printf("Digite o saldo atual: ");
    scanf("%f", &(novoUsuario->saldo_atual));

    // Limpar o buffer após o scanf
    while (getchar() != '\n');

    novoUsuario->id = *totalUsuarios + 1;
    
    // Redimensiona o vetor de usuários
    Usuario* temp = (Usuario*)realloc(*usuarios, (*totalUsuarios + 1) * sizeof(Usuario));
    if (temp == NULL) {
        printf("Erro na realocação de memória.\n");
        free(novoUsuario);
        return;
    }
    *usuarios = temp;

    (*usuarios)[*totalUsuarios] = *novoUsuario;
    (*totalUsuarios)++;
    
    free(novoUsuario);

    printf("Novo usuário inserido com sucesso!\n");
}

void inserirVariosUsuarios(Usuario** usuarios, int* totalUsuarios) {
    int qtde;
    printf("Digite a quantidade de usuários a serem inseridos: ");
    scanf("%d", &qtde);
    
    // Limpar o buffer após o scanf
    while (getchar() != '\n');

    for (int i = 0; i < qtde; i++) {
        printf("Usuário %d:\n", i + 1);
        inserirUsuario(usuarios, totalUsuarios);
    }
    
    printf("Todos os usuários foram inseridos com sucesso!\n");
}

void buscarUsuarioPorId(Usuario* usuarios, int totalUsuarios, int id) {
    int encontrado = 0;
    
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].id == id) {
            printf("Usuário encontrado:\n");
            printf("ID: %d\n", usuarios[i].id);
            printf("Nome: %s\n", usuarios[i].nome);
            printf("Idade: %d\n", usuarios[i].idade);
            printf("Saldo Atual: %.2f\n", usuarios[i].saldo_atual);
            encontrado = 1;
            break;
        }
    }
    
    if (!encontrado) {
        printf("Usuário não encontrado.\n");
    }
}

void realizarTransferencia(Usuario* usuarios, int totalUsuarios, int idOrigem, int idDestino, float quantia) {
    int indiceOrigem = -1, indiceDestino = -1;
    
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].id == idOrigem) {
            indiceOrigem = i;
        }
        
        if (usuarios[i].id == idDestino) {
            indiceDestino = i;
        }
        
        if (indiceOrigem != -1 && indiceDestino != -1) {
            break;
        }
    }
    
    if (indiceOrigem == -1 || indiceDestino == -1) {
        printf("Usuário de origem ou destino não encontrado.\n");
    } else if (usuarios[indiceOrigem].saldo_atual < quantia) {
        printf("Saldo insuficiente para transferência.\n");
    } else {
        usuarios[indiceOrigem].saldo_atual -= quantia;
        usuarios[indiceDestino].saldo_atual += quantia;
        printf("Transferência realizada com sucesso!\n");
    }
}

void removerUsuarioPorId(Usuario** usuarios, int* totalUsuarios, int id) {
    int indiceRemover = -1;
    
    for (int i = 0; i < *totalUsuarios; i++) {
        if ((*usuarios)[i].id == id) {
            indiceRemover = i;
            break;
        }
    }
    
    if (indiceRemover == -1) {
        printf("Usuário não encontrado.\n");
    } else {
        // Movendo o último usuário para ocupar a posição do usuário removido
        (*usuarios)[indiceRemover] = (*usuarios)[(*totalUsuarios) - 1];
        
        // Redimensionando o vetor de usuários
        Usuario* temp = (Usuario*)realloc(*usuarios, ((*totalUsuarios) - 1) * sizeof(Usuario));
        if (temp == NULL && *totalUsuarios > 1) {
            printf("Erro na realocação de memória.\n");
            exit(EXIT_FAILURE);
        }
        *usuarios = temp;
        
        (*totalUsuarios)--;
        
        printf("Usuário removido com sucesso!\n");
    }
}

void salvarUsuariosEmArquivo(Usuario* usuarios, int totalUsuarios) {
    FILE* arquivo = fopen("usuarios.txt", "w");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(arquivo, "%d,%s,%d,%.2f\n", usuarios[i].id, usuarios[i].nome, usuarios[i].idade, usuarios[i].saldo_atual);
    }
    
    fclose(arquivo);
    
    printf("Usuários salvos no arquivo com sucesso!\n");
}

int main() {
    Usuario* usuarios = NULL;
    int totalUsuarios = 0;
    
    int opcao;
    int id, idOrigem, idDestino, idRemover;
    float quantia;
    
    do {
        printf("========= UAIBANK - Sistema de Gestão de Usuários =========\n");
        printf("Selecione uma opção:\n");
        printf("1. Inserir um novo usuário\n");
        printf("2. Inserir vários usuários\n");
        printf("3. Buscar um usuário por ID\n");
        printf("4. Realizar transferência entre usuários\n");
        printf("5. Remover um usuário por ID\n");
        printf("0. Sair do sistema\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        
        // Limpar o buffer após o scanf
        while (getchar() != '\n');
        
        switch (opcao) {
            case 1:
                inserirUsuario(&usuarios, &totalUsuarios);
                break;
            case 2:
                inserirVariosUsuarios(&usuarios, &totalUsuarios);
                break;
            case 3:
                printf("Digite o ID do usuário: ");
                scanf("%d", &id);
                buscarUsuarioPorId(usuarios, totalUsuarios, id);
                break;
            case 4:
                printf("Digite o ID do usuário de origem: ");
                scanf("%d", &idOrigem);
                printf("Digite o ID do usuário de destino: ");
                scanf("%d", &idDestino);
                printf("Digite a quantia a ser transferida: ");
                scanf("%f", &quantia);
                realizarTransferencia(usuarios, totalUsuarios, idOrigem, idDestino, quantia);
                break;
            case 5:
                printf("Digite o ID do usuário a ser removido: ");
                scanf("%d", &idRemover);
                removerUsuarioPorId(&usuarios, &totalUsuarios, idRemover);
                break;
            case 0:
                salvarUsuariosEmArquivo(usuarios, totalUsuarios);
                free(usuarios);
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
                break;
        }
        
        printf("\n");
        
    } while (opcao != 0);
    
    return 0;
}
