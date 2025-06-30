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
typedef struct vendedores {
    int id;
    char nome[50];
    float salarioFixo;
    float comissao;
} Vendedor;
// Criação da struct das vendas #Rafael Abras
typedef struct venda {
    int codigoDaVenda;
    VendaItem ProdutosVenda[10];
    int quantidadeItens;
    float totalVenda;
} Vendas;
// Criação da struct dos itens que vão ir na venda #Rafael Abras

typedef struct vendaitem_ {
    produtos Produto;
    int quantidade_comprada_produto;
    float totalVenda;
} VendaItem;
// Protótipo da função de Nota Fiscal
void emitirNotaFiscal(char nome[], char endereco[], float totalCompra);

// Criação da struct de Nota Fiscal #Matheus
typedef struct nota_fiscal {
    char nome_cliente[100];
    char endereco_entrega[150];
    float total_compra;
    float frete;
    float total_final;
} NotaFiscal;

// Função para emitir Nota Fiscal #Matheus
void emitirNotaFiscal(char nome[], char endereco[], float totalCompra) {
    NotaFiscal nf;
    strcpy(nf.nome_cliente, nome);
    strcpy(nf.endereco_entrega, endereco);
    nf.total_compra = totalCompra;

    if (totalCompra <= 100.0)
        nf.frete = 30.0;
    else if (totalCompra <= 300.0)
        nf.frete = 20.0;
    else
        nf.frete = 0.0;

    nf.total_final = nf.total_compra + nf.frete;

    printf("\n===== NOTA FISCAL =====\n");
    printf("Cliente: %s\n", nf.nome_cliente);
    printf("Endereço: %s\n", nf.endereco_entrega);
    printf("Total da compra: R$ %.2f\n", nf.total_compra);
    printf("Frete: R$ %.2f\n", nf.frete);
    printf("TOTAL FINAL: R$ %.2f\n", nf.total_final);
    printf("========================\n\n");
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
//Consulta os produtos disponívies no estoque #João Felipe Manferrari
void VerificarProdutosComEstoqueDisponivel() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *arquivo = fopen(nomearquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    produtos p;
    char linha[200];
    int encontrados = 0;

    printf("\n--- Produtos com Estoque Disponível ---\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%49[^,],%d,%d,%d", p.nome_produto, &p.codigo_produto,
                   &p.qnt_estoque_produto, &p.preco_venda_produto) == 4) {

            if (p.qnt_estoque_produto > 0) {
                printf("Produto: %s | Código: %d | Estoque: %d | Preço: R$ %d\n",
                       p.nome_produto, p.codigo_produto, p.qnt_estoque_produto, p.preco_venda_produto);
                encontrados = 1;
            }
        }
    }

    if (!encontrados)
        printf("Nenhum produto com estoque disponível.\n");

    fclose(arquivo);
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
    FILE *arquivo = fopen("./Dados/DadosDosVendedores.txt", "a+");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de vendedores.\n");
        return;
    }

    Vendedor v;
    int ultimoId = 0;
    char linha[200];

    // le o último id do arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%*[^,],%*[^,],%*[^,],%*f,%*f", &ultimoId);
    }

    v.id = ultimoId + 1;

    getchar();
    printf("Nome do vendedor: ");
    fgets(v.nome, sizeof(v.nome), stdin);
    v.nome[strcspn(v.nome, "\n")] = '\0';

    printf("Salário fixo: ");
    scanf("%f", &v.salarioFixo);

    v.comissao = 0;

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


void ListarVendedores(){
FILE *arquivo = fopen("./Dados/DadosDosVendedores.txt", "r");
    if (!arquivo) {
        printf("Nenhum vendedor cadastrado ainda.\n");
        return;
    }

    char linha[200];
    Vendedor v;

    printf("\n--- Lista de Vendedores ---\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v.id, v.nome, &v.salarioFixo, &v.comissao) == 4) {
            printf("ID: %d | Nome: %s | Salário: R$%.2f | Comissão: R$%.2f\n",
                   v.id, v.nome, v.salarioFixo, v.comissao);
        }
    }

    fclose(arquivo);
}

void ExcluirVendedor() {
    const char *nomearquivo = "./Dados/DadosDosVendedores.txt";
    const char *tempArquivo = "./Dados/temp.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int codigo;
    printf("Digite o ID do vendedor a ser excluído: ");
    scanf("%d", &codigo);

    char linha[200];
    int encontrado = 0;
    Vendedor v;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v.id, v.nome, &v.salarioFixo, &v.comissao) == 4) {

            if (v.id == codigo) {
                encontrado = 1;
                printf("Vendedor '%s' excluído com sucesso.\n", v.nome);
                continue;
            }

            fprintf(arquivoTemp, "%d,%s,%.2f,%.2f\n",
                    v.id, v.nome, v.salarioFixo, v.comissao);
        }
    }


    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);

    if (!encontrado)
        printf("Vendedor com ID %d não encontrado.\n", codigo);
}

void EditarVendedor() {
    const char *nomearquivo = "./Dados/DadosDosVendedores.txt";
    const char *tempArquivo = "./Dados/temp.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int codigo;
    printf("Digite o ID do vendedor a ser editado: ");
    scanf("%d", &codigo);
    getchar();

    char linha[200];
    int encontrado = 0;
    Vendedor v;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
       if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v.id, v.nome, &v.salarioFixo, &v.comissao) == 4) {

            if (v.id == codigo) {
                encontrado = 1;
                printf("Vendedor encontrado: %s\n", v.nome);

                printf("Digite o novo nome: ");
                fgets(v.nome, sizeof(v.nome), stdin);
                v.nome[strcspn(v.nome, "\n")] = '\0';

                printf("Digite o novo salário fixo: ");
                scanf("%f", &v.salarioFixo);
                getchar();
            }

            fprintf(arquivoTemp, "%d,%s,%.2f,%.2f\n",
                    v.id, v.nome, v.salarioFixo, v.comissao);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);

    if (encontrado)
        printf("Vendedor editado com sucesso.\n");
    else
        printf("Vendedor com ID %d não encontrado.\n", codigo);
}

//MENU DOS PRODUTOS
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

//MENU VENDEDORES
void menu_vendedores() {
    int opcao;
   do {
        printf("\n--- Menu de Vendedores ---\n");
        printf("1. Cadastrar Vendedor\n");
        printf("2. Listar Vendedores\n");
        printf("3. Editar Vendedor\n");
        printf("4. Excluir Vendedor\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarVendedor();
                break;
            case 2:
                ListarVendedores();
                break;
            case 3:
                EditarVendedor();
                break;
            case 4:
                ExcluirVendedor();
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);
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
   char nomeCliente[50], enderecoEntrega[100];
    float totalCompra;

    getchar(); // limpar buffer
    printf("Nome do cliente: ");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    nomeCliente[strcspn(nomeCliente, "\n")] = '\0';

    printf("Endereço de entrega: ");
    fgets(enderecoEntrega, sizeof(enderecoEntrega), stdin);
    enderecoEntrega[strcspn(enderecoEntrega, "\n")] = '\0';

    printf("Valor total da compra: R$ ");
    scanf("%f", &totalCompra);

    emitirNotaFiscal(nomeCliente, enderecoEntrega, totalCompra);
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
