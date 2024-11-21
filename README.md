## OBJETIVO DO PROJETO

Este projeto tem como objetivo desenvolver uma aplicação de Exchange de criptomoedas, que permite aos usuários realizar operações de compra, venda, e consulta de três tipos de moedas:

o Bitcoin: Taxa de 2% na compra. Taxa de 3% na venda.

o Ethereum: Taxa de 1% na compra. Taxa de 2% na venda.

o Ripple: Taxa de 1% na compra. Taxa de 1% na venda.

A aplicação simula uma carteira de investimentos com funcionalidades básicas de transação e consulta.

## PROCESSOS

o Efetuar login com CPF e senha.

o Consultar saldo de reais, Bitcoin, Ethereum e Ripple na carteira de investimentos.

o Consultar extrato de operações da carteira.

o Depositar reais na carteira de investimentos.

o Sacar reais da carteira (validação por senha necessária).

o Comprar criptomoedas (com validação de senha e confirmação).

o Vender criptomoedas (com validação de senha e confirmação).

o Atualizar cotação das criptomoedas (variação aleatória de até ±5%).

o Cadastro de novo investidor: o usuário informa nome, CPF e senha do novo investidor;

o Excluir investidor: o usuário informa o CPF do investidor. O programa exibe os dados encontrados para o CPF e pede a confirmação para realizar a exclusão;

o Cadastro de criptomoeda: devem ser informados nome, cotação inicial, taxa de compra e taxa de venda;

o Excluir criptomoeda: usuário deve informa o nome da criptomoeda. O programa exibE os dados da moeda e pede a confirmação da exclusão;

## FORMA DE USO DO PROGRAMA

Login: O usuário deve fornecer seu CPF e SENHA para acessar o sistema.

Operações de Carteira: Após o login, o usuário pode consultar seu saldo, extrato,depósitos, saques, e transações com criptomoedas.

Atualizações de Cotação: A cotação das criptomoedas pode ser atualizada automaticamente com variações de até ±5% a cada transação.

# Exchange de Criptomoedas

Este projeto é uma aplicação desenvolvida em C para a gestão de uma exchange de criptomoedas, permitindo que investidores e administradores realizem diferentes operações. Ele é dividido em dois programas independentes: um para o **investidor** e outro para o **administrador**.

## Estrutura do Projeto

O projeto contém os seguintes arquivos principais:

- **main.c**: Programa principal para o investidor.
- **admin.c**: Programa principal para o administrador.
- **funcoes.c** / **funcoes.h**: Implementação das funcionalidades comuns aos dois programas.
- **login.c** / **login.h**: Módulo responsável pelo login de usuários.
- **usuario.c**: Arquivo contendo operações relacionadas ao investidor.

## Como Compilar
Para compilar os programas, utilize os seguintes comandos:

usuario:
gcc -o programa1 main.c funcoes.c login.c usuario.c

admin:
gcc -o programa2 main.c funcoes.c loginadm.c administrador.c

## Rodar o projeto usuario/adm:

usuario: ./programa1

admin: ./programa2

## PARTICIPANTES:

GUSTAVO SOUZA ALVARENGA RA: 22124058-3

GUSTAVO DELFINO DE C PEREIRA RA: 22124039-3

PEDRO HENRIQUE L DE OLIVEIRA RA: 22124019-5
