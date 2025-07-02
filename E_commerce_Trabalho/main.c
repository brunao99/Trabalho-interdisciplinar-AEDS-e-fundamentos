#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// limpar tela para melhor interacao com o usuario
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Criacao da struct dos produtos #Bruno Henrique
typedef struct new_produtos {
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
} produtos;

// Criacao da struct dos vendedores #Matheus
typedef struct vendedores {
    int id;
    char nome[50];
    float salarioFixo;
    float comissao;
} Vendedor;

// Criacao da struct dos compradores #Bruno Henrique
typedef struct comprador {
    int id;
    char nome[50];
    char cpf[12]; // 11 digitos + '\0'
    char email[50];
    char rua[50];
    char bairro[50];
    char cidade[50];
    char estado[3]; // UF
    char cep[10]; // 8 digitos + '\0'
} Comprador;

// Criacao da struct dos itens que vao ir na venda #Rafael Abras
typedef struct vendaitem_ {
    produtos Produto;
    int quantidade_comprada_produto;
    float totalVenda;
} VendaItem;

// Criacao da struct das vendas #Rafael Abras
typedef struct venda {
    int codigoDaVenda;
    int idVendedor;  // Novo campo para associar o vendedor
    VendaItem ProdutosVenda[10];
    int quantidadeItens;
    float totalVenda;
} Vendas;

// Prototipo da funcao de Nota Fiscal
void emitirNotaFiscal(int idComprador, int codigoVenda);

// Criacao da struct de Nota Fiscal #Matheus
typedef struct nota_fiscal {
    char nome_cliente[100];
    char endereco_entrega[150];
    float total_compra;
    float frete;
    float total_final;
} NotaFiscal;

// Funcao para emitir Nota Fiscal #Matheus
void emitirNotaFiscal(int idComprador, int codigoVenda) {
    FILE *arquivoCompradores = fopen("./Dados/DadosDosCompradores.txt", "r");
    FILE *arquivoVendas = fopen("./Dados/DadosDasVendas.txt", "r");
    if (!arquivoCompradores || !arquivoVendas) {
        printf("Erro ao abrir arquivos para nota fiscal.\n");
        return;
    }

    Comprador c;
    char linha[200];
    int compradorEncontrado = 0;

    while (fgets(linha, sizeof(linha), arquivoCompradores)) {
        if (sscanf(linha, "%d,%49[^,],%11[^,],%49[^,],%49[^,],%49[^,],%49[^,],%2[^,],%9[^\n]",
                   &c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep) == 9) {
            if (c.id == idComprador) {
                compradorEncontrado = 1;
                break;
            }
        }
    }

    if (!compradorEncontrado) {
        printf("Comprador com ID %d nao encontrado.\n", idComprador);
        fclose(arquivoCompradores);
        fclose(arquivoVendas);
        return;
    }

    Vendas v;
    v.totalVenda = 0;
    int vendaEncontrada = 0;

    while (fgets(linha, sizeof(linha), arquivoVendas)) {
        if (sscanf(linha, "Venda #%d", &v.codigoDaVenda) == 1 && v.codigoDaVenda == codigoVenda) {
            vendaEncontrada = 1;
            v.quantidadeItens = 0;
            while (fgets(linha, sizeof(linha), arquivoVendas) && strncmp(linha, "Total", 5) != 0) {
                VendaItem item;
                if (sscanf(linha, "%49[^,],%d,%d,%d,%f",
                           item.Produto.nome_produto, &item.Produto.codigo_produto,
                           &item.quantidade_comprada_produto, &item.Produto.preco_venda_produto,
                           &item.totalVenda) == 5) {
                    v.ProdutosVenda[v.quantidadeItens++] = item;
                    v.totalVenda += item.totalVenda;
                }
            }
            break;
        }
    }

    fclose(arquivoCompradores);
    fclose(arquivoVendas);

    if (!vendaEncontrada) {
        printf("Venda com codigo %d nao encontrada.\n", codigoVenda);
        return;
    }

    NotaFiscal nf;
    strcpy(nf.nome_cliente, c.nome);
    sprintf(nf.endereco_entrega, "%s, %s, %s - %s, %s", c.rua, c.bairro, c.cidade, c.estado, c.cep);
    nf.total_compra = v.totalVenda;

    if (nf.total_compra <= 100.0)
        nf.frete = 30.0;
    else if (nf.total_compra <= 300.0)
        nf.frete = 20.0;
    else
        nf.frete = 0.0;

    nf.total_final = nf.total_compra + nf.frete;

    printf("\n===== NOTA FISCAL =====\n");
    printf("Cliente: %s\n", nf.nome_cliente);
    printf("Endereco: %s\n", nf.endereco_entrega);
    printf("Produtos:\n");
    for (int i = 0; i < v.quantidadeItens; i++) {
        VendaItem item = v.ProdutosVenda[i];
        printf("  %s (Cod: %d) - Qtd: %d - Preco: R$%d - Total: R$%.2f\n",
               item.Produto.nome_produto, item.Produto.codigo_produto,
               item.quantidade_comprada_produto, item.Produto.preco_venda_produto, item.totalVenda);
    }
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
    printf("Digite o codigo do produto a ser editado: ");
    scanf("%d", &codigo);
    getchar(); // Limpa o \n do buffer

    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        produtos temp;
        char nome[50];
        int codigo_prod, qnt_estoque, preco_venda;

        if (sscanf(linha, "%49[^,],%d,%d,%d", nome, &codigo_prod, &qnt_estoque, &preco_venda) == 4) {
            if (codigo_prod == codigo) {
                encontrado = 1;
                printf("Produto encontrado: %s\n", nome);

                printf("Digite o novo nome do produto: ");
                fgets(temp.nome_produto, sizeof(temp.nome_produto), stdin);
                temp.nome_produto[strcspn(temp.nome_produto, "\n")] = '\0';

                printf("Digite o novo preco de venda: ");
                scanf("%d", &temp.preco_venda_produto);
                getchar();

                printf("Digite a nova quantidade em estoque: ");
                scanf("%d", &temp.qnt_estoque_produto);
                getchar();

                fprintf(arquivoTemp, "%s,%d,%d,%d\n",
                        temp.nome_produto, codigo_prod, temp.qnt_estoque_produto, temp.preco_venda_produto);
            } else {
                fprintf(arquivoTemp, "%s,%d,%d,%d\n",
                        nome, codigo_prod, qnt_estoque, preco_venda);
            }
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    if (remove(nomearquivo) != 0)
        perror("Erro ao remover arquivo original");

    if (rename(tempArquivo, nomearquivo) != 0)
        perror("Erro ao renomear arquivo temp");

    if (encontrado)
        printf("Produto editado com sucesso.\n");
    else
        printf("Produto com codigo %d nao encontrado.\n", codigo);
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
    printf("Digite o codigo do produto a ser excluido: ");
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
                    temp.nome_produto, temp.codigo_produto, temp.qnt_estoque_produto, temp.preco_venda_produto);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);

    if (!encontrado)
        printf("Produto com codigo %d nao encontrado.\n", codigo);
}

void ConsultarProdutoArquivoDados() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *arquivocomdados = fopen(nomearquivo, "r");
    if (!arquivocomdados) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    char linha[100];
    printf("=== Lista de Produtos ===\n");
    while (fgets(linha, sizeof(linha), arquivocomdados)) {
        printf("%s", linha);
    }
    fclose(arquivocomdados);
}

// Consulta os produtos disponiveis no estoque #Joao Felipe Manferrari
void VerificarEstoque() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *arquivo = fopen(nomearquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    produtos p;
    char linha[200];
    int encontrados = 0;

    printf("\n--- Produtos com Estoque Disponivel ---\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%49[^,],%d,%d,%d", p.nome_produto, &p.codigo_produto,
                   &p.qnt_estoque_produto, &p.preco_venda_produto) == 4) {
            if (p.qnt_estoque_produto > 0) {
                printf("Produto: %s | Codigo: %d | Estoque: %d | Preco: R$ %d\n",
                       p.nome_produto, p.codigo_produto, p.qnt_estoque_produto, p.preco_venda_produto);
                encontrados = 1;
            }
        }
    }

    if (!encontrados)
        printf("Nenhum produto com estoque disponivel.\n");

    fclose(arquivo);
}

// Criar arquivo de vendas #Rafael Abras
void CriarVendasArquivoDados() {
    const char *nomearquivo = "./Dados/DadosDasVendas.txt";
    FILE *f = fopen(nomearquivo, "r");
    if (f) fclose(f);
    else {
        FILE *novo = fopen(nomearquivo, "w");
        fclose(novo);
    }
}

// Atualizar estoque apos venda
void atualizarEstoqueAposVenda(VendaItem itens[], int quantidadeItens) {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    const char *tempArquivo = "./Dados/temp.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        produtos temp;
        char nome[50];
        int codigo, qnt_estoque, preco;

        if (sscanf(linha, "%49[^,],%d,%d,%d", nome, &codigo, &qnt_estoque, &preco) == 4) {
            for (int i = 0; i < quantidadeItens; i++) {
                if (itens[i].Produto.codigo_produto == codigo) {
                    qnt_estoque -= itens[i].quantidade_comprada_produto;
                    break;
                }
            }
            fprintf(arquivoTemp, "%s,%d,%d,%d\n", nome, codigo, qnt_estoque, preco);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    remove(nomearquivo);
    rename(tempArquivo, nomearquivo);
}


// Funcao para buscar vendedor por ID #Matheus
int buscarVendedorPorId(int id, Vendedor *v) {
    FILE *arquivo = fopen("./Dados/DadosDosVendedores.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo de vendedores.\n");
        return 0;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove quebra de linha se existir
        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v->id, v->nome, &v->salarioFixo, &v->comissao) == 4) {
            if (v->id == id) {
                fclose(arquivo);
                return 1; // Vendedor encontrado
            }
        }
    }

    fclose(arquivo);
    return 0; // Vendedor nao encontrado
}

// Funcao para consultar vendas ##Bruno Henrique
void ConsultarVendas() {
    const char *nomeArquivoVendas = "./Dados/DadosDasVendas.txt";
    FILE *arquivoVendas = fopen(nomeArquivoVendas, "r");
    if (!arquivoVendas) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    int opcao;
    printf("\n====== Consultar Vendas ======\n");
    printf("1. Listar todas as vendas\n");
    printf("2. Consultar venda por codigo\n");
    printf("3. Listar vendas por vendedor\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    char linha[200];
    int codigoBusca = -1;
    int idVendedorBusca = -1;

    if (opcao == 2) {
        printf("Digite o codigo da venda: ");
        scanf("%d", &codigoBusca);
    } else if (opcao == 3) {
        printf("Digite o ID do vendedor: ");
        scanf("%d", &idVendedorBusca);
    }

    printf("\n--- Resultado da Consulta ---\n");
    int encontrouVenda = 0;

    char proximaLinha[200] = "";
    int temProximaLinha = 0;

    while (temProximaLinha || fgets(linha, sizeof(linha), arquivoVendas)) {
        if (temProximaLinha) {
            strcpy(linha, proximaLinha);
            temProximaLinha = 0;
        }

        int codigoVenda, idVendedor;
        if (sscanf(linha, "Venda #%d,Vendedor ID: %d", &codigoVenda, &idVendedor) == 2) {
            int deveExibir = 0;

            switch(opcao) {
                case 1: deveExibir = 1; break;
                case 2: if (codigoVenda == codigoBusca) deveExibir = 1; break;
                case 3: if (idVendedor == idVendedorBusca) deveExibir = 1; break;
            }

            if (deveExibir) {
                encontrouVenda = 1;
                printf("\n=== Venda #%d - Vendedor ID: %d ===\n", codigoVenda, idVendedor);
                printf("Produtos:\n");

                while (fgets(linha, sizeof(linha), arquivoVendas)) {
                    linha[strcspn(linha, "\n")] = '\0';

                    if (strncmp(linha, "Total da Venda:", 15) == 0) {
                        printf("%s\n", linha);
                        break;
                    } else if (strlen(linha) == 0) {
                        continue;
                    } else if (strncmp(linha, "Venda #", 7) == 0) {
                        strcpy(proximaLinha, linha);
                        temProximaLinha = 1;
                        break;
                    } else {
                        char nomeProduto[50];
                        int codigoProduto, quantidade, preco;
                        float total;

                        if (sscanf(linha, "%49[^,],%d,%d,%d,%f",
                                   nomeProduto, &codigoProduto, &quantidade, &preco, &total) == 5) {
                            printf("  - %s (Código: %d) | Qtd: %d | Preço: R$ %d | Total: R$ %.2f\n",
                                   nomeProduto, codigoProduto, quantidade, preco, total);
                        }
                    }
                }

                printf("=====================================\n");
            }
        }
    }

    if (!encontrouVenda) {
        switch(opcao) {
            case 1:
                printf("Nenhuma venda cadastrada no sistema.\n");
                break;
            case 2:
                printf("Venda com código %d não encontrada.\n", codigoBusca);
                break;
            case 3:
                printf("Nenhuma venda encontrada para o vendedor ID %d.\n", idVendedorBusca);
                break;
        }
    }

    fclose(arquivoVendas);

    printf("\nPressione Enter para continuar...");
    getchar();
    getchar();


    if (!encontrouVenda) {
        switch(opcao) {
            case 1:
                printf("Nenhuma venda cadastrada no sistema.\n");
                break;
            case 2:
                printf("Venda com código %d não encontrada.\n", codigoBusca);
                break;
            case 3:
                printf("Nenhuma venda encontrada para o vendedor ID %d.\n", idVendedorBusca);
                break;
        }
    }

    fclose(arquivoVendas);

    // pausa para o usuário ver o resultado
    printf("\nPressione Enter para continuar...");

}

// registrar vendas #Rafael Abras
void RegistrarVenda() {
    const char *nomeArquivoProdutos = "./Dados/DadosDosProdutos.txt";
    const char *nomeArquivoVendas = "./Dados/DadosDasVendas.txt";

    FILE *arquivoProdutos = fopen(nomeArquivoProdutos, "r");
    if (!arquivoProdutos) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Vendas novaVenda;
    novaVenda.codigoDaVenda = rand() % 10000 + 1;
    novaVenda.quantidadeItens = 0;
    novaVenda.totalVenda = 0;

    printf("Digite o ID do vendedor: ");
    scanf("%d", &novaVenda.idVendedor);

    // Buscar vendedor ANTES de continuar
    Vendedor vendedor;
    if (!buscarVendedorPorId(novaVenda.idVendedor, &vendedor)) {
        printf("ERRO: Vendedor com ID %d nao encontrado no sistema.\n", novaVenda.idVendedor);
        printf("Cadastre o vendedor antes de registrar a venda.\n");
        fclose(arquivoProdutos);
        return;
    }
    printf("Vendedor encontrado: %s (Comissao atual: R$%.2f)\n", vendedor.nome, vendedor.comissao);

    int continuar = 1;
    while (continuar && novaVenda.quantidadeItens < 10) {
        int codigoProduto, quantidadeDesejada;
        produtos temp;
        int encontrado = 0;
        rewind(arquivoProdutos);

        printf("\nDigite o codigo do produto: ");
        scanf("%d", &codigoProduto);

        char linha[200];
        while (fgets(linha, sizeof(linha), arquivoProdutos)) {
            if (sscanf(linha, "%49[^,],%d,%d,%d", temp.nome_produto, &temp.codigo_produto,
                       &temp.qnt_estoque_produto, &temp.preco_venda_produto) == 4) {
                if (temp.codigo_produto == codigoProduto) {
                    encontrado = 1;
                    printf("Produto encontrado: %s\n", temp.nome_produto);
                    printf("Quantidade em estoque: %d\n", temp.qnt_estoque_produto);
                    printf("Digite a quantidade desejada: ");
                    scanf("%d", &quantidadeDesejada);

                    if (quantidadeDesejada > temp.qnt_estoque_produto) {
                        printf("Quantidade insuficiente em estoque.\n");
                        break;
                    }

                    VendaItem item;
                    item.Produto = temp;
                    item.quantidade_comprada_produto = quantidadeDesejada;
                    item.totalVenda = (float)(quantidadeDesejada * temp.preco_venda_produto);

                    novaVenda.ProdutosVenda[novaVenda.quantidadeItens++] = item;
                    novaVenda.totalVenda += item.totalVenda;
                    printf("Item adicionado com sucesso!\n");
                    break;
                }
            }
        }

        if (!encontrado) {
            printf("Produto com codigo %d nao encontrado.\n", codigoProduto);
        }

        printf("Deseja adicionar outro produto? (1 - Sim / 0 - Nao): ");
        scanf("%d", &continuar);
    }

    fclose(arquivoProdutos);

    // Verificar se pelo menos um item foi adicionado
    if (novaVenda.quantidadeItens == 0) {
        printf("Nenhum item foi adicionado a venda. Operacao cancelada.\n");
        return;
    }

    // Salvar a venda no arquivo
    FILE *arquivoVendas = fopen(nomeArquivoVendas, "a");
    if (!arquivoVendas) {
        printf("Erro ao abrir arquivo de vendas.\n");
        return;
    }

    fprintf(arquivoVendas, "Venda #%d,Vendedor ID: %d\n", novaVenda.codigoDaVenda, novaVenda.idVendedor);
    for (int i = 0; i < novaVenda.quantidadeItens; i++) {
        VendaItem item = novaVenda.ProdutosVenda[i];
        fprintf(arquivoVendas, "%s,%d,%d,%d,%.2f\n",
                item.Produto.nome_produto, item.Produto.codigo_produto,
                item.quantidade_comprada_produto, item.Produto.preco_venda_produto, item.totalVenda);
    }
    fprintf(arquivoVendas, "Total da Venda: %.2f\n\n", novaVenda.totalVenda);
    fclose(arquivoVendas);

    // Atualizar estoque
    atualizarEstoqueAposVenda(novaVenda.ProdutosVenda, novaVenda.quantidadeItens);
    aplicarComissao(&vendedor, novaVenda.totalVenda);  // passa endereço do vendedor

    // Aplicar comissao - CORRIGIDO
    printf("\n--- Aplicando Comissao ---\n");
    printf("Comissao antes da venda: R$%.2f\n", vendedor.comissao);

    // Aplicar a comissao (modifica o vendedor)
    aplicarComissao(&vendedor, novaVenda.totalVenda);

    printf("Comissao apos a venda: R$%.2f\n", vendedor.comissao);

    printf("\nVenda registrada com sucesso!\n");
    printf("Codigo da venda: %d\n", novaVenda.codigoDaVenda);
    printf("Total da venda: R$%.2f\n", novaVenda.totalVenda);
    printf("------------------------\n");
}


// A persistencia dos dados sera criada em uma funcao responsavel por criar o arquivo.txt # Rafael Abras
void CriarProdutoArquivoDados() {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *verificarExistencia = fopen(nomearquivo, "r");
    if (verificarExistencia) {
        fclose(verificarExistencia);
    } else {
        FILE *dados_produtos = fopen(nomearquivo, "w");
        fclose(dados_produtos);
    }
}

// Codigo para pegar o ultimo codigo do produto lendo o arquivo ate EOF, e assim adicionando 1 NO ULTIMO ID. arquivo.txt # Rafael Abras
int ObterUltimoCodigoProduto(const char *nomearquivo) {
    FILE *arquivo = fopen(nomearquivo, "r");
    if (!arquivo) return 0;

    char linha[200];
    int ultimoCodigo = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int codigo;
        if (sscanf(linha, "%*[^,],%d,%*d,%*d", &codigo) == 1) {
            if (codigo > ultimoCodigo) {
                ultimoCodigo = codigo;
            }
        }
    }

    fclose(arquivo);
    return ultimoCodigo;
}

// Cria o produto no arquivo do diretorio ./Dados/DadosDosProdutos.Txt
void CriarProduto(const char *nome_produto, int precovenda, int quantidadeEstoqueProduto) {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *dados_produtos = fopen(nomearquivo, "a");
    int codigo = ObterUltimoCodigoProduto(nomearquivo);
    fprintf(dados_produtos, "%s,%d,%d,%d\n", nome_produto, codigo + 1, quantidadeEstoqueProduto, precovenda);
    fclose(dados_produtos);
}

// Para cadastrar o produto com entrada de dados #Rafael Abras
void CadastrarProduto() {
    char nome_produto[50];
    int preco_venda, quantidade_estoque;

    getchar(); // limpa o \n pendente do scanf anterior
    printf("Digite o nome do produto: ");
    fgets(nome_produto, sizeof(nome_produto), stdin);
    nome_produto[strcspn(nome_produto, "\n")] = '\0';

    printf("Digite o preco de venda: ");
    scanf("%d", &preco_venda);

    printf("Digite a quantidade em estoque: ");
    scanf("%d", &quantidade_estoque);

    CriarProduto(nome_produto, preco_venda, quantidade_estoque);
    printf("Produto cadastrado com sucesso!\n");
}

// Criacao do arquivo txt para cadastro dos vendedores #Joao Felipe Manferrari
void CriarVendedoresArquivoDados() {
    const char *nomearquivo = "./Dados/DadosDosVendedores.txt";
    FILE *verificarExistencia = fopen(nomearquivo, "r");
    if (verificarExistencia) {
        fclose(verificarExistencia);
    } else {
        FILE *dados_produtos = fopen(nomearquivo, "w");
        fclose(dados_produtos);
    }
}

// Funcao para cadastrar vendedor e salvar no arquivo #Matheus
void cadastrarVendedor() {
    const char *nomearquivo = "./Dados/DadosDosVendedores.txt";
    FILE *arquivo = fopen(nomearquivo, "a+");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de vendedores.\n");
        return;
    }

    Vendedor v;
    int ultimoId = 0;
    char linha[200];

    // correção: ler o último ID corretamente
    rewind(arquivo);
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%*[^,],%*f,%*f", &ultimoId);
    }

    v.id = (ultimoId > 0) ? ultimoId + 1 : 1;

    getchar();
    printf("Nome do vendedor: ");
    fgets(v.nome, sizeof(v.nome), stdin);
    v.nome[strcspn(v.nome, "\n")] = '\0';

    printf("Salario fixo: ");
    scanf("%f", &v.salarioFixo);

    v.comissao = 0;

    fprintf(arquivo, "%d,%s,%.2f,%.2f\n", v.id, v.nome, v.salarioFixo, v.comissao);
    fclose(arquivo);
    printf("Vendedor cadastrado com sucesso! ID: %d\n", v.id);
}

// Funcao para atualizar vendedor no arquivo apos comissao
void atualizarVendedorNoArquivo(Vendedor *v) {
    const char *nomearquivo = "./Dados/DadosDosVendedores.txt";
    const char *tempArquivo = "./Dados/temp_vendedor.txt";

    FILE *arquivoOriginal = fopen(nomearquivo, "r");
    FILE *arquivoTemp = fopen(tempArquivo, "w");

    if (!arquivoOriginal || !arquivoTemp) {
        printf("Erro ao abrir os arquivos para atualizacao.\n");
        if (arquivoOriginal) fclose(arquivoOriginal);
        if (arquivoTemp) fclose(arquivoTemp);
        return;
    }

    char linha[200];
    Vendedor temp;
    int vendedorAtualizado = 0;

     while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &temp.id, temp.nome, &temp.salarioFixo, &temp.comissao) == 4) {
            if (temp.id == v->id) {  // Usar v->id
                fprintf(arquivoTemp, "%d,%s,%.2f,%.2f\n",
                        v->id, v->nome, v->salarioFixo, v->comissao);  // Usar ponteiro
                vendedorAtualizado = 1;
            } else {
                fprintf(arquivoTemp, "%d,%s,%.2f,%.2f\n",
                        temp.id, temp.nome, temp.salarioFixo, temp.comissao);
            }
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);

    // substitui o arquivo original pelo temporario
    if (remove(nomearquivo) != 0) {
        printf("Erro ao remover arquivo original de vendedores.\n");
        return;
    }

    if (rename(tempArquivo, nomearquivo) != 0) {
        printf("Erro ao renomear arquivo temporario de vendedores.\n");
        return;
    }

    if (vendedorAtualizado) {
        printf("Dados do vendedor atualizados no arquivo com sucesso.\n");
    } else {
        printf("ERRO: Vendedor nao foi encontrado para atualizacao no arquivo.\n");
    }
}

// Funcao para aplicar comissao de 3% ao vendedor apos uma venda #Matheus
void aplicarComissao(Vendedor *v, float valorVenda) {
    if (v == NULL) {
        printf("ERRO: Ponteiro do vendedor e nulo.\n");
        return;
    }

    float bonus = valorVenda * 0.03f;
    v->comissao += bonus;

    printf("\n=== APLICACAO DE COMISSAO ===\n");
    printf("Vendedor: %s (ID: %d)\n", v->nome, v->id);
    printf("Valor da venda: R$%.2f\n", valorVenda);
    printf("Comissao aplicada (3%%): R$%.2f\n", bonus);
    printf("Comissao total acumulada: R$%.2f\n", v->comissao);
    printf("=============================\n");

    // atualizar o arquivo com os novos dados
     atualizarVendedorNoArquivo(v);
}

void ListarVendedores() {
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
            printf("ID: %d | Nome: %s | Salario: R$%.2f | Comissao: R$%.2f\n",
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
    printf("Digite o ID do vendedor a ser excluido: ");
    scanf("%d", &codigo);

    char linha[200];
    int encontrado = 0;
    Vendedor v;

    while (fgets(linha, sizeof(linha), arquivoOriginal)) {
        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v.id, v.nome, &v.salarioFixo, &v.comissao) == 4) {
            if (v.id == codigo) {
                encontrado = 1;
                printf("Vendedor '%s' excluido com sucesso.\n", v.nome);
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
        printf("Vendedor com ID %d nao encontrado.\n", codigo);
}

// Funcao para cadastrar comprador #Bruno Henrique
void cadastrarComprador() {
    FILE *arquivo = fopen("./Dados/DadosDosCompradores.txt", "a+");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de compradores.\n");
        return;
    }

    Comprador c;
    int ultimoId = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,]", &ultimoId);
    }

    c.id = ultimoId + 1;

    getchar();
    printf("Nome do comprador: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("CPF (apenas numeros): ");
    fgets(c.cpf, sizeof(c.cpf), stdin);
    c.cpf[strcspn(c.cpf, "\n")] = '\0';

    printf("E-mail: ");
    fgets(c.email, sizeof(c.email), stdin);
    c.email[strcspn(c.email, "\n")] = '\0';

    printf("Rua: ");
    fgets(c.rua, sizeof(c.rua), stdin);
    c.rua[strcspn(c.rua, "\n")] = '\0';

    printf("Bairro: ");
    fgets(c.bairro, sizeof(c.bairro), stdin);
    c.bairro[strcspn(c.bairro, "\n")] = '\0';

    printf("Cidade: ");
    fgets(c.cidade, sizeof(c.cidade), stdin);
    c.cidade[strcspn(c.cidade, "\n")] = '\0';

    printf("Estado (UF): ");
    fgets(c.estado, sizeof(c.estado), stdin);
    c.estado[strcspn(c.estado, "\n")] = '\0';

    printf("CEP: ");
    fgets(c.cep, sizeof(c.cep), stdin);
    c.cep[strcspn(c.cep, "\n")] = '\0';

    fprintf(arquivo, "%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
            c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep);

    fclose(arquivo);
    printf("Comprador cadastrado com sucesso!\n");
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

                printf("Digite o novo salario fixo: ");
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
        printf("Vendedor com ID %d nao encontrado.\n", codigo);
}

void listarCompradores() {
    limparTela();
    FILE *arquivo = fopen("./Dados/DadosDosCompradores.txt", "r");
    if (!arquivo) {
        printf("Nenhum comprador cadastrado ainda.\n");
        return;
    }

    Comprador c;
    char linha[200];

    printf("\n--- Lista de Compradores ---\n");
    while (fgets(linha, sizeof(linha), arquivo)) {
        int result = sscanf(linha, "%d,%49[^,],%11[^,],%49[^,],%49[^,],%49[^,],%49[^,],%2[^,],%9[^\n]",
                            &c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep);
        if (result == 8) {
            printf("ID: %d | Nome: %s | CPF: %s | E-mail: %s\n", c.id, c.nome, c.cpf, c.email);
            printf("  Endereco: %s, %s, %s - %s, %s\n", c.rua, c.bairro, c.cidade, c.estado, c.cep);
        }
    }

    fclose(arquivo);
}

void excluirComprador() {
    const char *arquivoOriginal = "./Dados/DadosDosCompradores.txt";
    const char *arquivoTemp = "./Dados/temp.txt";

    FILE *orig = fopen(arquivoOriginal, "r");
    FILE *temp = fopen(arquivoTemp, "w");
    if (!orig || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    int id;
    printf("Digite o ID do comprador a ser excluido: ");
    scanf("%d", &id);

    Comprador c;
    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), orig)) {
        if (sscanf(linha, "%d,%49[^,],%11[^,],%49[^,],%49[^,],%49[^,],%49[^,],%2[^,],%9[^\n]",
                   &c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep) == 9) {
            if (c.id == id) {
                encontrado = 1;
                printf("Comprador '%s' excluido.\n", c.nome);
                continue;
            }
            fprintf(temp, "%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep);
        }
    }

    fclose(orig);
    fclose(temp);
    remove(arquivoOriginal);
    rename(arquivoTemp, arquivoOriginal);

    if (!encontrado)
        printf("Comprador com ID %d nao encontrado.\n", id);
}

void editarComprador() {
    const char *arquivoOriginal = "./Dados/DadosDosCompradores.txt";
    const char *arquivoTemp = "./Dados/temp.txt";

    FILE *orig = fopen(arquivoOriginal, "r");
    FILE *temp = fopen(arquivoTemp, "w");
    if (!orig || !temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    int id;
    printf("Digite o ID do comprador a ser editado: ");
    scanf("%d", &id);
    getchar();

    Comprador c;
    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), orig)) {
        if (sscanf(linha, "%d,%49[^,],%11[^,],%49[^,],%49[^,],%49[^,],%49[^,],%2[^,],%9[^\n]",
                   &c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep) == 9) {
            if (c.id == id) {
                encontrado = 1;
                printf("Novo nome: ");
                fgets(c.nome, sizeof(c.nome), stdin);
                c.nome[strcspn(c.nome, "\n")] = '\0';

                printf("Novo CPF (apenas numeros): ");
                fgets(c.cpf, sizeof(c.cpf), stdin);
                c.cpf[strcspn(c.cpf, "\n")] = '\0';

                printf("Novo e-mail: ");
                fgets(c.email, sizeof(c.email), stdin);
                c.email[strcspn(c.email, "\n")] = '\0';

                printf("Nova rua: ");
                fgets(c.rua, sizeof(c.rua), stdin);
                c.rua[strcspn(c.rua, "\n")] = '\0';

                printf("Novo bairro: ");
                fgets(c.bairro, sizeof(c.bairro), stdin);
                c.bairro[strcspn(c.bairro, "\n")] = '\0';

                printf("Nova cidade: ");
                fgets(c.cidade, sizeof(c.cidade), stdin);
                c.cidade[strcspn(c.cidade, "\n")] = '\0';

                printf("Novo estado (UF): ");
                fgets(c.estado, sizeof(c.estado), stdin);
                c.estado[strcspn(c.estado, "\n")] = '\0';

                printf("Novo CEP: ");
                fgets(c.cep, sizeof(c.cep), stdin);
                c.cep[strcspn(c.cep, "\n")] = '\0';
            }
            fprintf(temp, "%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    c.id, c.nome, c.cpf, c.email, c.rua, c.bairro, c.cidade, c.estado, c.cep);
        }
    }

    fclose(orig);
    fclose(temp);
    remove(arquivoOriginal);
    rename(arquivoTemp, arquivoOriginal);

    if (encontrado)
        printf("Comprador editado com sucesso!\n");
    else
        printf("Comprador com ID %d nao encontrado.\n", id);
}

// MENU DOS PRODUTOS
void menu_produtos() {
    limparTela();
    int flag;
    do {
        printf("\n====== Menu de Produtos ======\n");
        printf("1. Consultar Produtos\n");
        printf("2. Criar Produtos\n");
        printf("3. Editar Produto\n");
        printf("4. Excluir Produto\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &flag);

        switch (flag) {
            case 1: ConsultarProdutoArquivoDados(); break;
            case 2: CadastrarProduto(); break;
            case 3: EditarProduto(); break;
            case 4: ExcluirProduto(); break;
            case 5: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (flag != 5);
}

// MENU VENDEDORES
void menu_vendedores() {
    limparTela();
    int opcao;
    do {
        printf("\n====== Menu de Vendedores ======\n");
        printf("1. Cadastrar Vendedor\n");
        printf("2. Listar Vendedores\n");
        printf("3. Editar Vendedor\n");
        printf("4. Excluir Vendedor\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarVendedor(); break;
            case 2: ListarVendedores(); break;
            case 3: EditarVendedor(); break;
            case 4: ExcluirVendedor(); break;
            case 5: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 5);
}

void menu_compradores() {
    limparTela();
    int opcao;
    do {
        printf("\n====== Menu de Compradores ======\n");
        printf("1. Cadastrar Comprador\n");
        printf("2. Listar Compradores\n");
        printf("3. Editar Comprador\n");
        printf("4. Excluir Comprador\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarComprador(); break;
            case 2: listarCompradores(); break;
            case 3: editarComprador(); break;
            case 4: excluirComprador(); break;
            case 5: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 5);
}

void menu_vendas() {
    limparTela();
    int opcao;
    do {
        printf("\n====== Menu de Vendas ======\n");
        printf("1. Registrar Venda\n");
        printf("2. Verificar Estoque Disponivel\n");
        printf("3. Listar Vendas\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: RegistrarVenda(); break;
            case 2: VerificarEstoque(); break;
            case 3: ConsultarVendas(); break;
            case 4: printf("Voltando ao menu principal...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 3);
}

void menu_principal() {
    limparTela();
    int flag;
    do {
            limparTela();
        printf("\n====== E-commerce do Djabe e da Mafe ======\n");
        printf("1. Produtos\n");
        printf("2. Vendedores\n");
        printf("3. Compradores\n");
        printf("4. Vendas\n");
        printf("5. Nota Fiscal\n");
        printf("6. Sair da Loja\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &flag);

        switch (flag) {
            case 1: menu_produtos(); break;
            case 2: menu_vendedores(); break;
            case 3: menu_compradores(); break;
            case 4: menu_vendas(); break;
            case 5: {
                int idComprador, codigoVenda;
                printf("Digite o ID do comprador: ");
                scanf("%d", &idComprador);
                printf("Digite o codigo da venda: ");
                scanf("%d", &codigoVenda);
                emitirNotaFiscal(idComprador, codigoVenda);
                break;
            }
            case 6:
                printf("Encerrando o sistema. Obrigado por utilizar nossa loja!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (flag != 6);
}

int main() {
    srand(time(NULL)); // inicializa a semente do gerador de numeros aleatorios

    CriarProdutoArquivoDados();
    CriarVendasArquivoDados();
    CriarVendedoresArquivoDados();
    menu_principal();
    return 0;
}
