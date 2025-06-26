#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct new_produtos{
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
}produtos;

int main()
{
    printf("Hello world!\n");
    return 0;
}
