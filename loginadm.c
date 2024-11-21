#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcao.h"

USUARIO usuario_logado; 

// Função para realizar o login
void login(USUARIO usuarios_fixos[]) {
    char user[13];
    char password[20];

    while (1) {
        printf("\nLOGIN---ADM\n");

        printf("Digite o seu CPF: ");
        fgets(user, sizeof(user), stdin);
        user[strcspn(user, "\n")] = '\0'; // Remove a nova linha

        printf("Digite sua senha: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0'; // Remove a nova linha

        int login_sucesso = 0; // Flag para controle de sucesso no login

        // Verificando se o login é de um administrador (CPF e senha fixos)
        if (strcmp(user, "12345678910") == 0 && strcmp(password, "admin") == 0) {
            printf("Login de administrador efetuado com sucesso!\n");
            menu_administrador(usuarios_fixos, CT, NUM_USUARIOS, 3); // Chama a função para o menu do administrador
            login_sucesso = 1; // Flag de login de sucesso para administrador
            break;
        }
        if (login_sucesso) {
            break;
        } else {
            printf("Senha ou usuário incorreto\n");
        }
    }
}
