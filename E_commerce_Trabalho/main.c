#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Criação da struct dos produtos #Bruno Henrique
typedef struct new_produtos{
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
}produtos;

//A persistencia dos dados sera criada em uma função responsavel por criar
//o arquivo.txt # Rafael Abras
void CriarProdutoArquivoDados(){
const char *nomearquivo = "./Dados/DadosDosProdutos.txt";

FILE *verificarExistencia = fopen(nomearquivo, "r");

if (verificarExistencia){
fclose(verificarExistencia);
}
else{
FILE *dados_produtos;
dados_produtos = fopen(nomearquivo, "w");
fclose(dados_produtos);
}
}

// Codigo para pegar o ultimo codigo do produto lendo o arquivo ate EOF, e assim adicionando 1 NO ULTIMO ID. arquivo.txt # Rafael Abras
int ObterUltimoCodigoProduto(const char *nomearquivo) {
FILE *arquivo = fopen(nomearquivo, "r");
if (!arquivo) return 0;

char linha[200];
int ultimoCodigo = 0;

// le automaticamente ate o fim da linha sem usar o EOF
while (fgets(linha, sizeof(linha), arquivo)) {
        produtos temp;
        char nome[50];

// Le ate 49 caracteres,e o [^,] le ate uma virgula armazenando em uma variavel separada, e == 4 pq le quantos elementos foram lindos.
if (sscanf(linha, "%49[^,],%d,%d,%d", nome, &temp.codigo_produto,
                   &temp.qnt_estoque_produto, &temp.preco_venda_produto) == 4){
ultimoCodigo = temp.codigo_produto;
    }
}

fclose(arquivo);
return ultimoCodigo;
}

// Cria o produto no arquivo do diretorio ./Dados/DadosDosProdutos.Txt
void CriarProduto(const char *nome_produto, int precovenda, int quantidadeEstoqueProduto){

const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
FILE *dados_produtos = fopen(nomearquivo, "a");

int codigo = ObterUltimoCodigoProduto(nomearquivo);

fprintf(dados_produtos, "%s,%d,%d,%d\n", nome_produto, codigo+1, quantidadeEstoqueProduto, precovenda);
fclose(dados_produtos);
}

int main()
{
    CriarProdutoArquivoDados();
   CriarProduto("Teste", 10, 10);
    return 0;
}
