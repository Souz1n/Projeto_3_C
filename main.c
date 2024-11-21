#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcao.h"  
#include "login.h"   

#define NUM_USUARIOS 10

Criptomoeda CT[3] = {
    {"Bitcoin", 30000.0},
    {"Ethereum", 2000.0},
    {"Ripple", 0.5}
};

int main() {
    carregar_dados(usuarios_fixos, NULL);
    login(usuarios_fixos);
    salvar_dados(usuarios_fixos, NUM_USUARIOS);

    return 0;
}
