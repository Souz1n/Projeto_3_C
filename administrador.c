#include <stdio.h>
#include <string.h>
#include "funcao.h"

void salvar_criptos(Criptomoeda criptos[], int num_criptos);
int carregar_criptos(Criptomoeda criptos[]);
void menu_administrador(USUARIO usuarios[], Criptomoeda criptos[], int num_usuarios, int num_criptos);
void listar_usuarios(USUARIO usuarios[], int num_usuarios);
void resetar_saldos(USUARIO usuarios[], int num_usuarios);
void verificar_cotacoes(Criptomoeda criptos[], int num_criptos);
void resetar_transacoes(USUARIO usuarios[], int num_usuarios);
void cadastrar_usuario(USUARIO usuarios[], int *num_usuarios);
void excluir_usuario(USUARIO usuarios[], int *num_usuarios);
void excluir_usuario(USUARIO usuarios[], int *num_usuarios);
void excluir_criptomoeda();


void menu_administrador(USUARIO usuarios[], Criptomoeda criptos[], int num_usuarios, int num_criptos) {
    int opcao;
    do {
        printf("\n--- MENU ADMINISTRADOR ---\n");
        printf("1. Listar Usuarios\n");
        printf("2. Cadastrar Usuarios\n");
        printf("3. Excluir Usuarios\n");
        printf("4. Cadastrar Cripto\n");
        printf("5. Sair\n");
        printf("6. Excluir Criptomoeda\n");
        printf("7. Consultar Saldo\n");
        printf("8. Consultar Extrato\n");
        printf("9. Atualizar Cotacao\n");
        printf("Digite sua opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Tente novamente.\n");
            flush_input();
            continue;
        }
        flush_input();

        switch (opcao) {
            case 1:
                listar_usuarios(usuarios, num_usuarios);
                break;
            case 2:
                cadastrar_usuario(usuarios, &num_usuarios);
                break;
            case 3:
                excluir_usuario(usuarios, &num_usuarios);
                break;
            case 4:
                cadastrar_criptomoeda(criptos, &num_criptos);
                salvar_criptos(criptos, num_criptos);
                break;
            case 5:
                printf("Saindo do sistema do administrador...\n");
                break;
            case 6:
                excluir_criptomoeda(criptos, &num_criptos);
                break;
            case 7:
                consultar_saldo_usuario();
                break;  
            case 8:
                consultar_extrato_usuario(&usuarios);
                break;
            case 9:
                atualizar_cotacao();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

void listar_usuarios(USUARIO usuarios[], int num_usuarios) {
    printf("\n--- Lista de Usuários ---\n");
    for (int i = 0; i < num_usuarios; i++) {
        printf("Usuário %d:\n", i + 1);
        printf("Nome: %s\n", usuarios[i].nome);
        printf("CPF: %s\n", usuarios[i].cpf);
        printf("Saldo em reais: %.2f\n", usuarios[i].reais);
        printf("Saldo em Bitcoin: %.4f\n", usuarios[i].bitcoin);
        printf("Saldo em Ethereum: %.2f\n", usuarios[i].ethereum);
        printf("Saldo em Ripple: %.2f\n\n", usuarios[i].ripple);
    }
}

// Função para adicionar um novo usuário
void cadastrar_usuario(USUARIO usuarios[], int *num_usuarios) {
    if (*num_usuarios >= NUM_USUARIOS) {
        printf("Nao e possivel cadastrar mais usuarios. Limite atingido.\n");
        return;
    }

    USUARIO novo_usuario;
    printf("Digite o nome do novo investidor: ");
    fgets(novo_usuario.nome, sizeof(novo_usuario.nome), stdin);
    novo_usuario.nome[strcspn(novo_usuario.nome, "\n")] = '\0'; // Remove a nova linha

    printf("Digite o CPF do novo investidor: ");
    fgets(novo_usuario.cpf, sizeof(novo_usuario.cpf), stdin);
    novo_usuario.cpf[strcspn(novo_usuario.cpf, "\n")] = '\0'; // Remove a nova linha

    // Verifica se o CPF já está cadastrado
    for (int i = 0; i < *num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, novo_usuario.cpf) == 0) {
            printf("CPF ja cadastrado.\n");
            return;
        }
    }

    printf("Digite a senha do novo investidor: ");
    fgets(novo_usuario.senha, sizeof(novo_usuario.senha), stdin);
    novo_usuario.senha[strcspn(novo_usuario.senha, "\n")] = '\0'; // Remove a nova linha

    // Inicializa os valores financeiros e transações
    novo_usuario.reais = 0.0;
    novo_usuario.bitcoin = 0.0;
    novo_usuario.ethereum = 0.0;
    novo_usuario.ripple = 0.0;
    novo_usuario.num_transacoes = 0;

    usuarios[*num_usuarios] = novo_usuario;
    (*num_usuarios)++;

    // Salva os dados atualizados no arquivo binário
    salvar_dados(usuarios, *num_usuarios);
    printf("Investidor cadastrado com sucesso!\n");
}

void excluir_usuario(USUARIO usuarios[], int *num_usuarios) {
    if (*num_usuarios == 0) {
        printf("Erro: Não há investidores para excluir.\n");
        return;
    }

    char cpf[12];
    printf("Digite o CPF do investidor que deseja excluir: ");
    scanf("%11s", cpf);
    flush_input();

    int indice = -1;
    for (int i = 0; i < *num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Erro: Investidor com CPF %s não encontrado.\n", cpf);
        return;
    }

    // Remover o investidor movendo os elementos para "tapar" o buraco
    for (int i = indice; i < *num_usuarios - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }

    (*num_usuarios)--;
    printf("Investidor com CPF %s excluído com sucesso!\n", cpf);
}

void cadastrar_criptomoeda(Criptomoeda criptos[], int *num_criptos) {
    if (*num_criptos >= 30) {
        printf("Não é possível adicionar mais criptomoedas. Limite alcançado.\n");
        return;
    }

    Criptomoeda nova_cripto;
    printf("\n--- Cadastro de Criptomoeda ---\n");

    // Solicita as informações para a nova criptomoeda
    printf("Digite o nome da criptomoeda: ");
    fgets(nova_cripto.nome, sizeof(nova_cripto.nome), stdin);
    nova_cripto.nome[strcspn(nova_cripto.nome, "\n")] = '\0'; // Remove a nova linha do nome

    printf("Digite a cotação inicial: ");
    scanf("%f", &nova_cripto.cotacao);
    flush_input();

    printf("Digite a taxa de compra: ");
    scanf("%f", &nova_cripto.taxa_compra);
    flush_input();

    printf("Digite a taxa de venda: ");
    scanf("%f", &nova_cripto.taxa_venda);
    flush_input();

    // Adiciona a nova criptomoeda ao array
    criptos[*num_criptos] = nova_cripto;
    (*num_criptos)++;  // Incrementa o número de criptomoedas cadastradas

    printf("Criptomoeda %s cadastrada com sucesso!\n", nova_cripto.nome);
}


void excluir_criptomoeda() {
    FILE *arquivo = fopen("criptos.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
        return;
    }

    int num_criptos;
    // Lê o número de criptomoedas no arquivo
    if (fread(&num_criptos, sizeof(int), 1, arquivo) != 1) {
        printf("Erro ao ler o número de criptomoedas.\n");
        fclose(arquivo);
        return;
    }

    // Verifica se há criptomoedas cadastradas
    if (num_criptos == 0) {
        printf("Nenhuma criptomoeda cadastrada.\n");
        fclose(arquivo);
        return;
    }

    // Cria um array temporário para armazenar as criptomoedas
    Criptomoeda criptos[num_criptos];
    if (fread(criptos, sizeof(Criptomoeda), num_criptos, arquivo) != num_criptos) {
        printf("Erro ao ler as criptomoedas do arquivo.\n");
        fclose(arquivo);
        return;
    }
    fclose(arquivo);

    // Listar as criptomoedas disponíveis
    printf("\n--- Criptomoedas Disponíveis ---\n");
    for (int i = 0; i < num_criptos; i++) {
        printf("%d. %s - Cotação: %.2f | Taxa de Compra: %.2f | Taxa de Venda: %.2f\n",
               i + 1, criptos[i].nome, criptos[i].cotacao, criptos[i].taxa_compra, criptos[i].taxa_venda);
    }

    char nome_cripto[50];
    int encontrado = 0;

    // Solicita o nome da criptomoeda a ser excluída
    printf("\nDigite o nome da criptomoeda a ser excluída: ");
    fgets(nome_cripto, sizeof(nome_cripto), stdin);
    nome_cripto[strcspn(nome_cripto, "\n")] = '\0'; // Remove a nova linha do nome

    // Procura a criptomoeda no array
    for (int i = 0; i < num_criptos; i++) {
        if (strcmp(criptos[i].nome, nome_cripto) == 0) {
            // Encontrou a criptomoeda, desloca as outras para preencher o espaço
            for (int j = i; j < num_criptos - 1; j++) {
                criptos[j] = criptos[j + 1];
            }
            num_criptos--;  // Decrementa o número total de criptomoedas
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Criptomoeda %s não encontrada.\n", nome_cripto);
        return;
    }

    // Reescreve o arquivo com as criptomoedas restantes
    arquivo = fopen("criptos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar o arquivo de criptomoedas.\n");
        return;
    }

    // Grava o número atualizado de criptomoedas
    if (fwrite(&num_criptos, sizeof(int), 1, arquivo) != 1) {
        printf("Erro ao salvar o número de criptomoedas no arquivo.\n");
        fclose(arquivo);
        return;
    }

    // Grava as criptomoedas restantes
    if (num_criptos > 0) {
        if (fwrite(criptos, sizeof(Criptomoeda), num_criptos, arquivo) != num_criptos) {
            printf("Erro ao salvar as criptomoedas no arquivo.\n");
            fclose(arquivo);
            return;
        }
    }

    fclose(arquivo);
    printf("Criptomoeda %s excluída com sucesso!\n", nome_cripto);
}

void consultar_saldo_usuario() {
    FILE *arquivo = fopen("usuarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    char cpf_busca[12];
    USUARIO usuario;
    int encontrado = 0;

    // Solicita o CPF do investidor
    printf("Digite o CPF do investidor (somente números): ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = '\0'; // Remove a nova linha do CPF

    // Busca pelo usuário no arquivo
    while (fread(&usuario, sizeof(USUARIO), 1, arquivo) == 1) {
        if (strcmp(usuario.cpf, cpf_busca) == 0) {
            encontrado = 1;
            printf("\n--- Saldo do Usuário ---\n");
            printf("Nome: %s\n", usuario.nome);
            printf("CPF: %s\n", usuario.cpf);
            printf("Saldo em reais: %.2f\n", usuario.reais);
            printf("Bitcoin: %.6f\n", usuario.bitcoin);
            printf("Ethereum: %.6f\n", usuario.ethereum);
            printf("Ripple: %.6f\n", usuario.ripple);
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Usuário com o CPF %s não encontrado.\n", cpf_busca);
    }
}

void consultar_extrato_usuario() {
    FILE *arquivo_usuario = fopen("usuarios.dat", "rb");
    if (arquivo_usuario == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    char cpf_busca[12];
    USUARIO usuario;
    int encontrado = 0;

    // Solicita o CPF do administrador para buscar o usuário
    printf("Digite o CPF do investidor (somente números): ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = '\0'; // Remove a nova linha do CPF

    // Busca o usuário pelo CPF no arquivo
    while (fread(&usuario, sizeof(USUARIO), 1, arquivo_usuario) == 1) {
        if (strcmp(usuario.cpf, cpf_busca) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo_usuario);

    if (!encontrado) {
        printf("Usuário com o CPF %s não encontrado.\n", cpf_busca);
        return;
    }

    // Exibe o histórico de transações do usuário
    printf("\n--- Histórico de Transações de %s (CPF: %s) ---\n", usuario.nome, usuario.cpf);

    if (usuario.num_transacoes == 0) {
        printf("Nenhuma transação realizada.\n");
    } else {
        for (int i = 0; i < usuario.num_transacoes; i++) {
            Transacao *t = &usuario.transacoes[i];
            printf("Data: %s | Tipo: %s | Valor: R$ %.2f | Cripto: %s | Cotação: %.4f | Taxa: %.2f\n", 
                t->data_hora, t->tipo, t->valor, t->cripto, t->cotacao, t->tx);
        }
    }
}