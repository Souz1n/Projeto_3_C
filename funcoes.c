#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcao.h"

#define ARQUIVO_BINARIO "usuarios.dat"

USUARIO usuario_logado; 

USUARIO usuarios_fixos[NUM_USUARIOS] = {
    {"Alice Silva", "11111111111", "senha1", 0.0, 0.0, 0.0, 0.0, {{0}}, 0},
    {"Bruno Santos", "22222222222", "senha2", 0.0, 0.0, 0.0, 0.0, {{0}}, 0},
    {"Carla Oliveira", "33333333333", "senha3", 0.0, 0.0, 0.0, 0.0, {{0}}, 0},
    {"Diego Souza", "44444444444", "senha4", 0.0, 0.0, 0.0, 0.0, {{0}}, 0},
};

int verifica_senha(USUARIO *usuario) {
    char senha[20];
    printf("Digite sua senha para confirmar a transacao: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remove a nova linha

    return strcmp(usuario->senha, senha) == 0;
}

int flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return 1;
}

void inicializaUsuario(USUARIO *usuario, const char *nome, const char *cpf, float reais, float bitcoin, float ethereum, float ripple) {
    strcpy(usuario->nome, nome);
    strcpy(usuario->cpf, cpf);
    usuario->reais = reais;
    usuario->bitcoin = bitcoin;
    usuario->ethereum = ethereum;
    usuario->ripple = ripple;
    usuario->num_transacoes = 0;
}

void obter_data_hora_atual(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void registrar_transacao(USUARIO *usuario, const char *tipo, float valor, const char *cripto, float cotacao, float tx) {
    Transacao nova_transacao;
    obter_data_hora_atual(nova_transacao.data_hora);
    strcpy(nova_transacao.tipo, tipo);
    strcpy(nova_transacao.cripto, cripto);
    nova_transacao.valor = valor;
    nova_transacao.cotacao = cotacao;
    nova_transacao.tx = tx;
    nova_transacao.reais = usuario->reais;
    nova_transacao.bitcoin = usuario->bitcoin;
    nova_transacao.ethereum = usuario->ethereum;
    nova_transacao.ripple = usuario->ripple;

    if (usuario->num_transacoes >= MAX_TRANSACOES) {
        for (int i = 1; i < MAX_TRANSACOES; i++) {
            usuario->transacoes[i - 1] = usuario->transacoes[i];
        }
        usuario->transacoes[MAX_TRANSACOES - 1] = nova_transacao;
    } else {
        usuario->transacoes[usuario->num_transacoes] = nova_transacao;
        usuario->num_transacoes++;
    }
}

void consultar_saldo(USUARIO *usuario) {
    printf("\n--- Consultar Saldo ---\n");
    printf("Nome: %s\n", usuario->nome);
    printf("CPF: %s\n", usuario->cpf);
    printf("Saldo em Reais: %.2f\n", usuario->reais);

    // Abrir o arquivo de criptomoedas para ler as criptos disponíveis
    FILE *arquivo_criptos = fopen("criptos.dat", "rb");
    if (arquivo_criptos == NULL) {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
        return;
    }

    int num_criptos;
    fread(&num_criptos, sizeof(int), 1, arquivo_criptos);

    // Lê as informações das criptomoedas
    Criptomoeda criptos[num_criptos];
    fread(criptos, sizeof(Criptomoeda), num_criptos, arquivo_criptos);
    fclose(arquivo_criptos);

    // Exibir os saldos de cada criptomoeda com base no nome
    printf("\n--- Saldos de Criptomoedas ---\n");
    for (int i = 0; i < num_criptos; i++) {
        if (strcmp(criptos[i].nome, "Bitcoin") == 0) {
            printf("%s: %.4f\n", criptos[i].nome, usuario->bitcoin);
        } else if (strcmp(criptos[i].nome, "Ethereum") == 0) {
            printf("%s: %.2f\n", criptos[i].nome, usuario->ethereum);
        } else if (strcmp(criptos[i].nome, "Ripple") == 0) {
            printf("%s: %.2f\n", criptos[i].nome, usuario->ripple);
        } else {
            printf("%s: %.2f\n", criptos[i].nome, criptos[i].valor_total);
        }
    }
}

void consultar_extrato(USUARIO *usuario) {
    printf("\nConsultar Extrato:\n");
    gerar_extrato(usuario); 
}

void depositar(USUARIO *usuario) {
    float deposito;
    printf("\nDigite o valor que voce deseja depositar: ");
    scanf("%f", &deposito);
    flush_input();  

    if (deposito > 0) {
        usuario->reais += deposito;
        registrar_transacao(usuario, "deposito", deposito, "Reais", 0.0, 0.0);
        printf("Deposito realizado com sucesso!\n");
    } else {
        printf("Valor invalido para deposito.\n");
    }
}

void sacar(USUARIO *usuario) {
    if (!verifica_senha(usuario)) {
        printf("Senha incorreta. Transacao cancelada.\n");
        return;
    }
    float valor;
    printf("\nDigite o valor que voce deseja sacar: ");
    scanf("%f", &valor);
    flush_input();  

    if (valor > 0 && valor <= usuario->reais) {
        usuario->reais -= valor;
        registrar_transacao(usuario, "saque", valor, "Reais", 0.0, 0.0);
        printf("Saque realizado com sucesso!\n");
    } else {
        printf("Valor invalido ou saldo insuficiente.\n");
    }
}

float gerar_variacao() {
    return ((float)rand() / RAND_MAX * 2 - 1) * 0.05;
}

void atualizar_cotacao() {
    FILE *arquivo = fopen("criptos.dat", "r+b");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
        return;
    }

    int num_criptos;
    // Lê o número de criptos no arquivo
    fread(&num_criptos, sizeof(int), 1, arquivo);
    
    if (num_criptos == 0) {
        printf("Nenhuma criptomoeda cadastrada.\n");
        fclose(arquivo);
        return;
    }

    Criptomoeda criptos[num_criptos];
    // Lê todas as criptomoedas no arquivo
    fread(criptos, sizeof(Criptomoeda), num_criptos, arquivo);

    // Atualiza as cotações das criptomoedas
    srand(time(NULL));  // Inicializa a semente para números aleatórios

    for (int i = 0; i < num_criptos; i++) {
        float variacao = gerar_variacao();
        criptos[i].cotacao *= (1 + variacao);  // Atualiza a cotação com a variação
        printf("Nova cotação de %s: %.2f (variação: %.2f%%)\n", criptos[i].nome, criptos[i].cotacao, variacao * 100);
    }

    // Reposiciona o ponteiro de arquivo para o início e grava novamente os dados atualizados
    fseek(arquivo, sizeof(int), SEEK_SET);  // Vai para a posição onde as criptomoedas começam no arquivo
    fwrite(criptos, sizeof(Criptomoeda), num_criptos, arquivo);

    fclose(arquivo);
}

void comprar_criptomoedas(USUARIO *usuario) {
    FILE *arquivo = fopen("criptos.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
        return;
    }

    int num_criptos;
    // Lê o número de criptomoedas no arquivo
    fread(&num_criptos, sizeof(int), 1, arquivo);

    // Cria um array temporário para armazenar as criptomoedas
    Criptomoeda criptos[num_criptos];
    fread(criptos, sizeof(Criptomoeda), num_criptos, arquivo);

    fclose(arquivo);

    printf("\n--- Criptomoedas Disponíveis ---\n");
    for (int i = 0; i < num_criptos; i++) {
        printf("%s - Cotação: %.2f | Taxa de Compra: %.2f | Taxa de Venda: %.2f\n",
               criptos[i].nome, criptos[i].cotacao, criptos[i].taxa_compra, criptos[i].taxa_venda);
    }

    char nome_cripto[50];
    float valor, saldo_necessario;

    printf("\nDigite o nome da criptomoeda que deseja comprar: ");
    fgets(nome_cripto, sizeof(nome_cripto), stdin);
    nome_cripto[strcspn(nome_cripto, "\n")] = '\0'; // Remove a nova linha

    // Procurar a criptomoeda pelo nome
    for (int i = 0; i < num_criptos; i++) {
        if (strcmp(criptos[i].nome, nome_cripto) == 0) {
            printf("Digite o valor que deseja comprar em %s: ", criptos[i].nome);
            scanf("%f", &valor);
            flush_input();

            // Verifica se o saldo do usuário é suficiente
            saldo_necessario = valor * criptos[i].cotacao;

            if (usuario->reais >= saldo_necessario) { 
                // Atualiza o saldo do usuário
                usuario->reais -= saldo_necessario;

                // Adiciona a criptomoeda ao saldo do usuário
                if (strcmp(criptos[i].nome, "Bitcoin") == 0) {
                    usuario->bitcoin += valor;
                } else if (strcmp(criptos[i].nome, "Ethereum") == 0) {
                    usuario->ethereum += valor;
                } else if (strcmp(criptos[i].nome, "Ripple") == 0) {
                    usuario->ripple += valor;
                }
                else if (strcmp(criptos[i].nome, "criptos[i].nome") == 0) {
                    criptos[i].valor_total += valor;
                }
                registrar_transacao(usuario, "compra", valor, nome_cripto, criptos[i].cotacao, criptos[i].taxa_compra);
                printf("Compra de %.2f %s realizada com sucesso! Saldo em reais: %.2f\n", valor, criptos[i].nome, usuario->reais);
            } else {
                printf("Saldo insuficiente para comprar %.2f %s.\n", valor, criptos[i].nome);
            }
            return;
        }
    }

    printf("Criptomoeda não encontrada.\n");
}

void vender_criptomoedas(USUARIO *usuario) {
    if (!verifica_senha(usuario)) {
        printf("Senha incorreta. Transação cancelada.\n");
        return;
    }

    // Abre o arquivo de criptomoedas
    FILE *arquivo_criptos = fopen("criptos.dat", "rb");
    if (arquivo_criptos == NULL) {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
        return;
    }

    int num_criptos;
    // Lê o número de criptomoedas no arquivo
    fread(&num_criptos, sizeof(int), 1, arquivo_criptos);

    // Cria um array temporário para armazenar as criptomoedas
    Criptomoeda criptos[num_criptos];
    fread(criptos, sizeof(Criptomoeda), num_criptos, arquivo_criptos);

    fclose(arquivo_criptos);

    // Exibe as criptomoedas que o usuário possui
    printf("\n--- Criptomoedas Disponíveis para Venda ---\n");
    int encontrou_cripto = 0;

    for (int i = 0; i < num_criptos; i++) {
        if ((strcmp(criptos[i].nome, "Bitcoin") == 0 && usuario->bitcoin > 0) ||
            (strcmp(criptos[i].nome, "Ethereum") == 0 && usuario->ethereum > 0) ||
            (strcmp(criptos[i].nome, "Ripple") == 0 && usuario->ripple > 0)) {
            
            printf("%d. %s (Saldo: %.2f)\n", i + 1, criptos[i].nome,
                   strcmp(criptos[i].nome, "Bitcoin") == 0 ? usuario->bitcoin :
                   (strcmp(criptos[i].nome, "Ethereum") == 0 ? usuario->ethereum : usuario->ripple));
            encontrou_cripto = 1;
        }
    }

    if (!encontrou_cripto) {
        printf("Você não possui criptomoedas para vender.\n");
        return;
    }

    // Solicita a escolha da criptomoeda a ser vendida
    char nome_cripto[50];
    printf("\nDigite o nome da criptomoeda que deseja vender: ");
    fgets(nome_cripto, sizeof(nome_cripto), stdin);
    nome_cripto[strcspn(nome_cripto, "\n")] = '\0'; // Remove a nova linha

    // Procura a criptomoeda escolhida pelo usuário
    for (int i = 0; i < num_criptos; i++) {
        if (strcmp(criptos[i].nome, nome_cripto) == 0) {
            // Solicita a quantidade a ser vendida
            float valor;
            printf("Digite a quantidade de %s que deseja vender: ", criptos[i].nome);
            scanf("%f", &valor);
            flush_input();

            // Verifica se o usuário tem saldo suficiente
            if ((strcmp(criptos[i].nome, "Bitcoin") == 0 && valor > usuario->bitcoin) ||
                (strcmp(criptos[i].nome, "Ethereum") == 0 && valor > usuario->ethereum) ||
                (strcmp(criptos[i].nome, "Ripple") == 0 && valor > usuario->ripple)) {
                printf("Quantidade insuficiente para venda.\n");
                return;
            }

            // Calcula o valor a ser recebido pela venda
            float taxa = valor * criptos[i].cotacao * 0.01;
            float total = valor * criptos[i].cotacao - taxa;

            printf("\n--- Detalhes da Venda ---\n");
            printf("Criptomoeda: %s\n", criptos[i].nome);
            printf("Quantidade: %.2f\n", valor);
            printf("Cotação: %.2f\n", criptos[i].cotacao);
            printf("Taxa de venda: %.2f\n", taxa);
            printf("Total a receber: %.2f\n", total);

            // Confirmação da venda
            char confirmacao;
            printf("Deseja confirmar a venda? (s/n): ");
            scanf(" %c", &confirmacao);

            if (confirmacao == 's' || confirmacao == 'S') {
                // Atualiza os saldos do usuário
                if (strcmp(criptos[i].nome, "Bitcoin") == 0) {
                    usuario->bitcoin -= valor;
                } else if (strcmp(criptos[i].nome, "Ethereum") == 0) {
                    usuario->ethereum -= valor;
                } else if (strcmp(criptos[i].nome, "Ripple") == 0) {
                    usuario->ripple -= valor;
                }

                // Atualiza o saldo em reais do usuário
                usuario->reais += total;

                // Atualiza o arquivo do usuário com o novo saldo
                char nome_arquivo[50];
                sprintf(nome_arquivo, "usuario_%s.dat", usuario->cpf);
                FILE *arquivo_usuario = fopen(nome_arquivo, "rb+");
                if (arquivo_usuario == NULL) {
                    printf("Erro ao abrir o arquivo do usuário.\n");
                    return;
                }
                fseek(arquivo_usuario, 0, SEEK_SET);
                fwrite(usuario, sizeof(USUARIO), 1, arquivo_usuario);
                fclose(arquivo_usuario);

                // Registra a transação
                registrar_transacao(usuario, "venda", total, criptos[i].nome, criptos[i].cotacao, taxa);
                printf("Venda de %.2f %s realizada com sucesso! Saldo em reais: %.2f\n", valor, criptos[i].nome, usuario->reais);
            } else {
                printf("Venda cancelada.\n");
            }
            return;
        }
    }

    printf("Criptomoeda não encontrada.\n");
}



void salvar_criptos(Criptomoeda criptos[], int num_criptos) {
    FILE *arquivo = fopen("criptos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar as criptomoedas.\n");
        return;
    }

    // Grava o número de criptos no início do arquivo
    fwrite(&num_criptos, sizeof(int), 1, arquivo);

    // Grava as informações de cada criptomoeda
    for (int i = 0; i < num_criptos; i++) {
        fwrite(&criptos[i], sizeof(Criptomoeda), 1, arquivo);
    }

    fclose(arquivo);
    printf("Criptomoedas salvas com sucesso no arquivo.\n");
}

int carregar_criptos(Criptomoeda criptos[]) {
    FILE *arquivo = fopen("criptos.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para carregar as criptomoedas.\n");
        return 0; // Retorna 0 se o arquivo não existe ou ocorreu erro
    }

    int num_criptos;
    // Lê o número de criptos
    fread(&num_criptos, sizeof(int), 1, arquivo);

    // Lê as criptomoedas
    for (int i = 0; i < num_criptos; i++) {
        fread(&criptos[i], sizeof(Criptomoeda), 1, arquivo);
    }

    fclose(arquivo);
    return num_criptos; // Retorna o número de criptos carregados
}

void gerar_extrato(USUARIO *usuario) {
    char nome_arquivo[50];
    sprintf(nome_arquivo, "%s_%s.txt", usuario->nome, usuario->cpf);
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de extrato");
        return;
    }
    fprintf(arquivo, "Extrato de %s (CPF: %s)\n", usuario->nome, usuario->cpf);
    fprintf(arquivo, "Reais: %.2f\n", usuario->reais);
    fprintf(arquivo, "Transacoes:\n");

    for (int i = 0; i < usuario->num_transacoes; i++) {
        Transacao *t = &usuario->transacoes[i];
        fprintf(arquivo, "%s - %s: R$ %.2f - %s (Cotacao: %.4f, Taxa: %.2f)\n", 
                t->data_hora, t->tipo, t->valor, t->cripto, t->cotacao, t->tx);
    }

    fclose(arquivo);
}

void salvar_dados(USUARIO usuarios[], int num_usuarios) {
    FILE *arquivo = fopen("usuarios.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvamento.\n");
        return;
    }

    fwrite(usuarios, sizeof(USUARIO), num_usuarios, arquivo);
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

void carregar_dados(USUARIO usuarios[], int *num_usuarios) {
    FILE *arquivo = fopen("usuarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo não encontrado. Iniciando com dados padrão.\n");
        *num_usuarios = NUM_USUARIOS;
        // Inicialize com usuários fixos padrão se o arquivo não existir
        inicializaUsuario(&usuarios[0], "Usuario 1", "11111111111", 1000.0, 0.5, 1.0, 2.0);
        inicializaUsuario(&usuarios[1], "Usuario 2", "22222222222", 2000.0, 0.2, 0.5, 1.0);
        inicializaUsuario(&usuarios[2], "Usuario 3", "33333333333", 1500.0, 0.1, 0.3, 0.7);
        inicializaUsuario(&usuarios[3], "Usuario 4", "44444444444", 3000.0, 0.7, 1.2, 1.5);
    } else {
        fread(usuarios, sizeof(USUARIO), NUM_USUARIOS, arquivo);
        fclose(arquivo);
        printf("Dados carregados com sucesso!\n");
    }
}

void carregar_usuarios(USUARIO usuarios[], int *num_usuarios) {
    FILE *arquivo = fopen("usuarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Arquivo não encontrado. Nenhum usuário carregado.\n");
        return;
    }

    // Carregar os usuários do arquivo
    while (fread(&usuarios[*num_usuarios], sizeof(USUARIO), 1, arquivo) == 1) {
        (*num_usuarios)++;
    }

    fclose(arquivo);
}

