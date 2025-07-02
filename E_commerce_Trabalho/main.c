#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// limpar tela para melhor interacao com o usuario
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Criacao da struct dos produtos
typedef struct new_produtos {
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
} produtos;

// Criacao da struct dos vendedores
typedef struct vendedores {
    int id;
    char nome[50];
    float salarioFixo;
    float comissao;
} Vendedor;

// Criacao da struct dos compradores
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

// Criacao da struct dos itens que vao ir na venda
typedef struct vendaitem_ {
    produtos Produto;
    int quantidade_comprada_produto;
    float totalVenda;
} VendaItem;

// Criacao da struct das vendas
typedef struct venda {
    int codigoDaVenda;
    int idVendedor;
    VendaItem ProdutosVenda[10];
    int quantidadeItens;
    float totalVenda;
} Vendas;


// Criacao da struct de Nota Fiscal
typedef struct nota_fiscal {
    char nome_cliente[100];
    char endereco_entrega[150];
    float total_compra;
    float frete;
    float total_final;
} NotaFiscal;

// Funcao para emitir Nota Fiscal
void emitirNotaFiscal(int idComprador, int codigoVenda) {
    FILE *arquivoCompradores = fopen("./Dados/DadosDosCompradores.txt", "r");
    FILE *arquivoVendas = fopen("./Dados/DadosDasVendas.txt", "r");
    if (!arquivoCompradores || !arquivoVendas) {
        printf("Erro ao abrir arquivos para nota fiscal.\n");
        if (arquivoCompradores) fclose(arquivoCompradores);
        if (arquivoVendas) fclose(arquivoVendas);
        return;
    }

    Comprador c;
    char linha[512];
    int compradorEncontrado = 0;

    // Leitura do comprador com parsing manual robusto
    while (fgets(linha, sizeof(linha), arquivoCompradores)) {
        // Conta vírgulas para garantir formato esperado
        int numVirgulas = 0;
        for (int i = 0; linha[i] != '\0'; i++) if (linha[i] == ',') numVirgulas++;
        if (numVirgulas != 8) continue; // pula linhas mal formatadas

        char *campos[9] = {NULL};
        int campoIndex = 0;
        char *ptr = linha;
        char *inicioCampo = ptr;

        while (*ptr && campoIndex < 9) {
            if (*ptr == ',') {
                *ptr = '\0';
                campos[campoIndex++] = inicioCampo;
                inicioCampo = ptr + 1;
            }
            ptr++;
        }
        campos[campoIndex] = inicioCampo; // último campo

        if (campoIndex == 8) {
            c.id = atoi(campos[0]);
            strcpy(c.nome, campos[1]);
            strcpy(c.cpf, campos[2]);
            strcpy(c.email, campos[3]);
            strcpy(c.rua, campos[4]);
            strcpy(c.bairro, campos[5]);
            strcpy(c.cidade, campos[6]);
            strcpy(c.estado, campos[7]);
            strcpy(c.cep, campos[8]);
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
        if (strncmp(linha, "Venda #", 7) == 0) {
            int codVendaArquivo, idVendedor;
            if (sscanf(linha, "Venda #%d,Vendedor ID: %d", &codVendaArquivo, &idVendedor) == 2) {
                if (codVendaArquivo != codigoVenda) continue; // não é a venda que queremos
                vendaEncontrada = 1;
                v.codigoDaVenda = codVendaArquivo;
                v.idVendedor = idVendedor;
                v.quantidadeItens = 0;
                v.totalVenda = 0;

                while (fgets(linha, sizeof(linha), arquivoVendas)) {
                    if (strncmp(linha, "Total", 5) == 0) break;
                    if (strlen(linha) < 3) continue; // ignora linhas vazias ou curtas

                    VendaItem item;
                    if (sscanf(linha, "%49[^,],%d,%d,%d,%f",
                               item.Produto.nome_produto, &item.Produto.codigo_produto,
                               &item.quantidade_comprada_produto, &item.Produto.preco_venda_produto,
                               &item.totalVenda) == 5) {
                        v.ProdutosVenda[v.quantidadeItens++] = item;
                        v.totalVenda += item.totalVenda;
                    }
                }
                break; // terminou leitura da venda que queremos
            }
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
    getchar();

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

void CriarVendasArquivoDados() {
    const char *nomearquivo = "./Dados/DadosDasVendas.txt";
    FILE *f = fopen(nomearquivo, "r");
    if (f) fclose(f);
    else {
        FILE *novo = fopen(nomearquivo, "w");
        fclose(novo);
    }
}

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

int buscarVendedorPorId(int id, Vendedor *v) {
    FILE *arquivo = fopen("./Dados/DadosDosVendedores.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir arquivo de vendedores.\n");
        return 0;
    }

    char linha[200];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        // Tentar ler a linha
        if (sscanf(linha, "%d,%49[^,],%f,%f",
                   &v->id, v->nome, &v->salarioFixo, &v->comissao) == 4) {
            if (v->id == id) {
                encontrado = 1;
                break;
            }
        }
    }

    fclose(arquivo);
    return encontrado;
}

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
}

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

    if (novaVenda.quantidadeItens == 0) {
        printf("Nenhum item foi adicionado a venda. Operacao cancelada.\n");
        return;
    }

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

    atualizarEstoqueAposVenda(novaVenda.ProdutosVenda, novaVenda.quantidadeItens);

    // Aplicar comissão
    float bonus = novaVenda.totalVenda * 0.03f;
    vendedor.comissao += bonus;
    printf("\nComissao aplicada: R$%.2f\n", bonus);
    printf("Comissao total do vendedor: R$%.2f\n", vendedor.comissao);

    // Atualizar vendedor no arquivo
    FILE *arquivoVendedores = fopen("./Dados/DadosDosVendedores.txt", "r+");
    if (!arquivoVendedores) {
        printf("Erro ao abrir arquivo de vendedores para atualizacao.\n");
        return;
    }

    char tempFile[] = "./Dados/temp_vendedores.txt";
    FILE *temp = fopen(tempFile, "w");
    if (!temp) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(arquivoVendedores);
        return;
    }

    char linha[200];
    int vendedorAtualizado = 0;

    while (fgets(linha, sizeof(linha), arquivoVendedores)) {
        Vendedor v;
        if (sscanf(linha, "%d,%49[^,],%f,%f", &v.id, v.nome, &v.salarioFixo, &v.comissao) == 4) {
            if (v.id == vendedor.id) {
                fprintf(temp, "%d,%s,%.2f,%.2f\n", v.id, v.nome, v.salarioFixo, vendedor.comissao);
                vendedorAtualizado = 1;
            } else {
                fprintf(temp, "%d,%s,%.2f,%.2f\n", v.id, v.nome, v.salarioFixo, v.comissao);
            }
        }
    }

    fclose(arquivoVendedores);
    fclose(temp);

    remove("./Dados/DadosDosVendedores.txt");
    rename(tempFile, "./Dados/DadosDosVendedores.txt");

    if (vendedorAtualizado) {
        printf("Comissao atualizada no sistema!\n");
    } else {
        printf("ERRO: Vendedor nao encontrado para atualizacao.\n");
    }

    printf("\nVenda registrada com sucesso!\n");
    printf("Codigo da venda: %d\n", novaVenda.codigoDaVenda);
    printf("Total da venda: R$%.2f\n", novaVenda.totalVenda);
    printf("------------------------\n");
}

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

int ObterUltimoCodigoProduto(const char *nomearquivo) {
    FILE *arquivo = fopen(nomearquivo, "r");
    if (!arquivo) return 0;

    char linha[200];
    int ultimoCodigo = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        int codigo;
        if (sscanf(linha, "%*[^,],%d,%*d,%*d", &codigo) == 1) {
            ultimoCodigo = codigo; // sempre atualiza com o último lido
        }
    }

    fclose(arquivo);
    return ultimoCodigo;
}

void CriarProduto(const char *nome_produto, int preco_venda, int quantidade_estoque) {
    const char *nomearquivo = "./Dados/DadosDosProdutos.txt";
    FILE *dados_produtos = fopen(nomearquivo, "a");
    if (!dados_produtos) {
        printf("Erro ao abrir arquivo para cadastrar produto.\n");
        return;
    }

    int codigo = ObterUltimoCodigoProduto(nomearquivo) + 1;

    fprintf(dados_produtos, "%s,%d,%d,%d\n", nome_produto, codigo, quantidade_estoque, preco_venda);

    fclose(dados_produtos);
    printf("Produto cadastrado com sucesso! Código gerado: %d\n", codigo);
}


void CadastrarProduto() {
    char nome_produto[50];
    int preco_venda, quantidade_estoque;

    printf("Digite o nome do produto: ");
    getchar(); // consome \n pendente do scanf anterior, se houver
    fgets(nome_produto, sizeof(nome_produto), stdin);
    nome_produto[strcspn(nome_produto, "\n")] = '\0'; // remove \n

    if (strlen(nome_produto) == 0) {
        printf("Nome do produto nao pode ser vazio.\n");
        return;
    }

    printf("Digite o preco de venda: ");
    if (scanf("%d", &preco_venda) != 1) {
        printf("Valor invalido para preco.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Digite a quantidade em estoque: ");
    if (scanf("%d", &quantidade_estoque) != 1) {
        printf("Valor invalido para quantidade.\n");
        while (getchar() != '\n'); // limpa buffer
        return;
    }

    CriarProduto(nome_produto, preco_venda, quantidade_estoque);
}

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
        linha[strcspn(linha, "\n")] = '\0';
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
        linha[strcspn(linha, "\n")] = '\0';
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
        linha[strcspn(linha, "\n")] = '\0';
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
        printf("Nenhum comprador cadastrado ainda ou erro ao abrir arquivo.\n");
        return;
    }

    char linha[400];
    int encontrou = 0;

    printf("\n--- Lista de Compradores ---\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';  // remove \n

        int numVirgulas = 0;
        for (int i = 0; linha[i] != '\0'; i++) {
            if (linha[i] == ',') numVirgulas++;
        }

        if (numVirgulas != 8) {
            printf("\n[AVISO] Linha ignorada: esperado 8 vírgulas, mas encontrou %d: %s\n", numVirgulas, linha);
            continue;
        }

        char *campos[9] = {NULL};
        int campoIndex = 0;
        char *ptr = linha;
        char *inicioCampo = ptr;

        while (*ptr && campoIndex < 9) {
            if (*ptr == ',') {
                *ptr = '\0';
                campos[campoIndex++] = inicioCampo;
                inicioCampo = ptr + 1;
            }
            ptr++;
        }

        // ultimo campo (mesmo vazio) precisa ser registrado
        campos[campoIndex] = inicioCampo;

        if (campoIndex == 8) {
            encontrou = 1;
            printf("\nID: %s\nNome: %s\nCPF: %s\nE-mail: %s\n", campos[0], campos[1], campos[2], campos[3]);
            printf("Endereco: %s, %s, %s - %s, %s\n", campos[4], campos[5], campos[6], campos[7], campos[8]);
            printf("--------------------------------------\n");
        } else {
            printf("\n[ERRO] Linha formatada incorretamente: %s\n", linha);
        }
    }

    if (!encontrou) {
        printf("\nNenhum comprador encontrado no arquivo.\n");
    }

    fclose(arquivo);

    printf("\nPressione Enter para continuar...");
    getchar();
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
        linha[strcspn(linha, "\n")] = '\0';
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
        linha[strcspn(linha, "\n")] = '\0';
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
    } while (opcao != 4);
}

void menu_principal() {
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
                printf("\nPressione Enter para continuar...");
                getchar();
                getchar();
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
    srand(time(NULL));
    system("mkdir -p ./Dados");

    CriarProdutoArquivoDados();
    CriarVendasArquivoDados();
    CriarVendedoresArquivoDados();
    menu_principal();
    return 0;
}
