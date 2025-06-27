#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Criacao da struct dos produtos #Bruno Henrique
typedef struct new_produtos{
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
}produtos;
// Criação da struct dos vendedores #Matheus
typedef struct {
    int id;
    char nome[50];
    float salarioFixo;
    float comissao;
} Vendedor;

// Armazenamento em memória dos vendedores cadastrados
// Será preenchido ao cadastrar e listado no menu de vendedores
Vendedor vendedores[100];         //  100 vend
int total_vendedores = 0;         


//COMEÇO DA PARTE PRODUTOS
void menu_produtos(){
int flag;
printf("======Menu produtos======");
printf("\nConsultar Produtos (1)\nCriar Produtos (2)\nEditar produto (3)\nExcluir produto (4)\nVoltar para Menu principal (5)\n\nDigite o numero do respectivo processo para acessa-lo: ");
scanf("%d",&flag);
switch(flag){
    case 1:
    ConsultarProdutoArquivoDados();
    break;
    case 2:
    CriarProdutoArquivoDados();
    break;
    case 3:
    EditarProduto();
    break;
    case 4:
    ExcluirProduto();
    break;
    case 5:
    menu_principal();
    break;
}

}

void EditarProduto() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    const char *tempArquivo = "./Dados/temp.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int codigo;
    printf("Digite o código do produto a ser editado: ");
    scanf("%d", &codigo);

    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        produtos temp;

        if (sscanf(linha, "%49[^,],%d,%d,%d", temp.nome_produto, &temp.codigo_produto,
                   &temp.qnt_estoque_produto, &temp.preco_venda_produto) == 4) {

            if (temp.codigo_produto == codigo) {
                encontrado = 1;
                printf("Produto encontrado: %s\n", temp.nome_produto);

                printf("Digite o novo nome do produto: ");
                scanf(" %[^\n]", temp.nome_produto); 
                printf("Digite o novo preço de venda: ");
                scanf("%d", &temp.preco_venda_produto);
                printf("Digite a nova quantidade em estoque: ");
                scanf("%d", &temp.qnt_estoque_produto);
            }

            fprintf(arquivoTemp, "%s,%d,%d,%d\n",
                    temp.nome_produto,
                    temp.codigo_produto,
                    temp.qnt_estoque_produto,
                    temp.preco_venda_produto);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);

    if (encontrado)
        printf("Produto editado com sucesso.\n");
    else
        printf("Produto com código %d não encontrado.\n", codigo);
}

void ExcluirProduto() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    const char *tempArquivo = "./Dados/temp.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int codigo;
    printf("Digite o código do produto a ser excluído: ");
    scanf("%d", &codigo);

    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        produtos temp;

        if (sscanf(linha, "%49[^,],%d,%d,%d", temp.nome_produto, &temp.codigo_produto,
                   &temp.qnt_estoque_produto, &temp.preco_venda_produto) == 4) {

            if (temp.codigo_produto == codigo) {
                encontrado = 1;
                printf("Produto '%s' removido com sucesso.\n", temp.nome_produto);
 
                continue;
            }

            fprintf(arquivoTemp, "%s,%d,%d,%d\n",
                    temp.nome_produto,
                    temp.codigo_produto,
                    temp.qnt_estoque_produto,
                    temp.preco_venda_produto);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);

    if (!encontrado)
        printf("Produto com código %d não encontrado.\n", codigo);
}

void ConsultarProdutoArquivoDados(){
const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
FILE *arquivocomdados = fopen(nomearquivo, "r");
char linha[100];
printf("=== Lista de Produtos ===\n");
    while (fgets(linha, sizeof(linha), arquivocomdados)) {
        printf("%s", linha);

}
}

//A persistencia dos dados sera criada em uma funcao responsavel por criar
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

// Criação do arquivo txt para cadastro dos vendedores #João Felipe Manferrari


void CriarVendedoresArquivoDados(){
const char *nomearquivo = "./Dados/DadosDosVendedores.txt";

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
//FIM DA PARTE DE PRODUTOS

// Função para cadastrar vendedor e salvar no arquivo #Matheus
void cadastrarVendedor() {
    FILE *arquivo = fopen("./Dados/DadosDosVendedores.txt", "a");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de vendedores.\n");
        return;
    }

    Vendedor v;
    v.id = total_vendedores + 1;

    printf("Nome do vendedor: ");
    getchar(); // limpa o buffer do scanf anterior
    fgets(v.nome, 50, stdin);
    v.nome[strcspn(v.nome, "\n")] = '\0';

    printf("Salário fixo: ");
    scanf("%f", &v.salarioFixo);

    v.comissao = 0;

    // Salva no vetor em memória
    vendedores[total_vendedores++] = v;

    // Salva no arquivo
    fprintf(arquivo, "%d,%s,%.2f,%.2f\n", v.id, v.nome, v.salarioFixo, v.comissao);
    fclose(arquivo);

    printf("Vendedor cadastrado com sucesso!\n");
}
// Função para aplicar comissão de 3% ao vendedor após uma venda #Matheus
void aplicarComissao(Vendedor *v, float valorVenda) {
    float bonus = valorVenda * 0.03;
    v->comissao += bonus;
    v->salarioFixo += bonus;
    printf("Comissão de R$%.2f aplicada ao vendedor %s\n", bonus, v->nome);
}



//Permite visualizar os vendedores
void menu_vendedores() {
    int opcao;

    do {
        printf("\n--- Menu de Vendedores ---\n");
        printf("1. Cadastrar Vendedor\n");
        printf("2. Listar Vendedores\n");
        printf("3. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                CriarVendedoresArquivoDados();
                break;
            case 2:
                printf("\n--- Lista de Vendedores ---\n");
                for (int i = 0; i < total_vendedores; i++) {
                    printf("ID: %d | Nome: %s | Email: %s | Telefone: %s\n",
                        vendedores[i].id, vendedores[i].nome,
                        vendedores[i].email, vendedores[i].telefone);
                }
                break;
            case 3:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 3);
}


void menu_principal(){
    int flag;
printf("======E-commerce do Djabe e da Mafe======\n\n");
printf("Produtos (1)\nVendedores (2)\nCompradores (3)\nVendas (4)\nNota Fiscal (5)\nSair da loja (6)\n Digite o numero do respectivo processo para acessa-lo: ");
scanf("%d",&flag);
switch(flag){
case 1:
    menu_produtos();
    break;
case 2:
   menu_vendedores();
    break;
case 3:
   // menu_compradores();
    break;
case 4:
   // menu_vendas();
    break;
case 5:
   // menu_nota_fiscal();
    break;
case 6:
    break;
}
}

int main()
{
    CriarVendedoresArquivoDados();
    menu_principal();
    return 0;
}
