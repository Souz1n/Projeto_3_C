#include <stdio.h>
#include <string.h>
#include "funcao.h"

Criptomoeda criptos[3] = {
        {"Bitcoin", 50000.0, 0.02, 0.02},  // Nome, Cotação, Taxa de Compra, Taxa de Venda
        {"Ethereum", 4000.0, 0.03, 0.03},
        {"Ripple", 1.0, 0.01, 0.01}
    };

    int num_criptos = 3;


void menu(USUARIO *usuario) {
    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Consultar Saldo\n");
        printf("2. Consultar Extrato\n");
        printf("3. Depositar\n");
        printf("4. Sacar\n");
        printf("5. Comprar Criptomoedas\n");
        printf("6. Vender Criptomoedas\n");
        printf("7. Atualizar Cotacoes\n");
        printf("8. Login Administrador\n");
        printf("9. Sair\n");
        printf("Digite sua opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Tente novamente.\n");
            flush_input();  
            continue;
        }
        flush_input();  

        switch (opcao) {
            case 1:
                consultar_saldo(usuario);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 2:
                consultar_extrato(usuario);
                gerar_extrato(usuario);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 3:
                depositar(usuario);
                gerar_extrato(usuario);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 4:
                sacar(usuario);
                gerar_extrato(usuario);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 5:
                comprar_criptomoedas(usuario);
                gerar_extrato(usuario);
                // salvar_criptos(criptos, num_criptos);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 6:
                vender_criptomoedas(usuario);
                gerar_extrato(usuario);
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 7:
                atualizar_cotacao();
                // salvar_dados(usuarios_fixos, NUM_USUARIOS);
                break;
            case 8:
                // menu_administrador(usuarios_fixos, CT, NUM_USUARIOS, 3);
                break;
            case 9:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 9);
}
