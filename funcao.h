#ifndef FUNCAO_H
#define FUNCAO_H

#define MAX_TRANSACOES 100
#define NUM_USUARIOS 10

typedef struct {
    char data_hora[20];
    char tipo[20];
    float valor;
    char cripto[20];
    float cotacao;
    float tx;
    float reais;
    float bitcoin;
    float ethereum;
    float ripple;
} Transacao;

typedef struct {
    char nome[50];
    char cpf[13];
    char senha[20];
    float reais;
    float bitcoin;
    float ethereum;
    float ripple;
    Transacao transacoes[MAX_TRANSACOES];
    int num_transacoes;
} USUARIO;

extern USUARIO usuarios_fixos[NUM_USUARIOS];

typedef struct {
    char nome[20];
    float cotacao;
    float cotacao_inicial;
    float taxa_compra;
    float taxa_venda;
    float valor_total;
} Criptomoeda;

extern Criptomoeda CT[3];

// Protótipos das funções
void inicializaUsuario(USUARIO *usuario, const char *nome, const char *cpf, float reais, float bitcoin, float ethereum, float ripple);
void menu(USUARIO *usuario);
void consultar_saldo(USUARIO *usuario);
void consultar_extrato(USUARIO *usuario);
void depositar(USUARIO *usuario);
void sacar(USUARIO *usuario);
void comprar_criptomoedas(USUARIO *usuario);
void vender_criptomoedas(USUARIO *usuario);
void atualizar_cotacao();
void gerar_extrato(USUARIO *usuario); 
void salvar_dados(USUARIO usuarios[], int num_usuarios);
void carregar_dados(USUARIO usuarios[], int *num_usuarios);
void menu_administrador(USUARIO usuarios[], Criptomoeda criptos[], int num_usuarios, int num_criptos);
void adicionar_usuario(USUARIO usuarios[], int *num_usuarios);
void carregar_usuarios(USUARIO usuarios[], int *num_usuarios);
void cadastrar_criptomoeda(Criptomoeda criptos[], int *num_criptos);
void consultar_saldo_usuario();
void consultar_extrato_usuario();
void salvar_criptos(Criptomoeda criptos[], int num_criptos);
int flush_input();

#endif
