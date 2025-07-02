# Trabalho Interdisciplinar - AEDS / Fundamentos (Documentos)

## 📋 Backlog do Trabalho
#### Aqui vai persistir a documentação do trabalho, casos de teste, fotos de sprint, etc...


# 📦 Sistema de E-commerce em C

## 1. Introdução

### 1.1. Objetivo
Este projeto implementa um sistema de e-commerce desenvolvido em linguagem C, permitindo o gerenciamento de produtos, vendedores, compradores, vendas e emissão de notas fiscais. Foi desenvolvido como parte de um trabalho interdisciplinar entre as disciplinas de **Algoritmos e Estruturas de Dados (AEDS)** e **Fundamentos de Engenharia de Software**.

### 1.2. Escopo

O sistema contempla as seguintes funcionalidades:
- ✅ Cadastro de Produtos (CRUD)  
- ✅ Cadastro de Vendedores (CRUD)  
- ✅ Cadastro de Compradores (CRUD)  
- ✅ Registro de Vendas (com controle de estoque)  
- ✅ Emissão de Nota Fiscal (com cálculo de frete)  
- ✅ Testes e Validações (documentação e casos de teste)

### 1.3. Tecnologias Utilizadas

| Item                 | Descrição           |
|----------------------|---------------------|
| Linguagem            | C (ISO C99)         |
| Compilador           | GCC                 |
| Sistemas Operacionais| Windows / Linux     |
| Versionamento        | Git (opcional)      |
| Testes               | Testes manuais      |

---

## 2. Arquitetura do Sistema

### 2.1. Estrutura de Dados

As principais entidades do sistema são representadas por structs:

```c
typedef struct new_produtos {
    char nome_produto[50];
    int codigo_produto;
    int qnt_estoque_produto;
    int preco_venda_produto;
} produtos;

typedef struct vendedores {
    int id;
    char nome[50];
    float salarioFixo;
    float comissao;
} Vendedor;

typedef struct comprador {
    int id;
    char nome[50];
    char cpf[12];
    char email[50];
    char rua[50];
    char bairro[50];
    char cidade[50];
    char estado[3];
    char cep[10];
} Comprador;

typedef struct venda {
    int codigoDaVenda;
    int idVendedor;
    VendaItem ProdutosVenda[10];
    int quantidadeItens;
    float totalVenda;
} Vendas;

## ⚙️ Funções do Sistema

Esta seção descreve as funções principais do sistema de e-commerce em C, separadas por módulo.

---

### 🛒 1. Módulo de Produtos

| Função               | Descrição                                                                 |
|----------------------|---------------------------------------------------------------------------|
| `CriarProduto()`     | Cadastra um novo produto com nome, preço, código e estoque.               |
| `EditarProduto()`    | Permite modificar dados de um produto já existente.                       |
| `ExcluirProduto()`   | Remove um produto do sistema.                                             |
| `ConsultarProduto()` | Lista todos os produtos cadastrados no sistema.                          |
| `ObterUltimoCodigoProduto()` | Garante unicidade dos códigos de produto, obtendo o último utilizado. |

---

### 👤 2. Módulo de Vendedores

| Função                    | Descrição                                                                 |
|---------------------------|---------------------------------------------------------------------------|
| `CadastrarVendedor()`     | Registra um novo vendedor com nome, salário fixo e ID automático.         |
| `EditarVendedor()`        | Permite alterar informações do vendedor.                                 |
| `ExcluirVendedor()`       | Remove o cadastro de um vendedor.                                        |
| `ListarVendedores()`      | Lista todos os vendedores cadastrados.                                   |
| `AtualizarComissaoVendedor()` | Atualiza a comissão com base nas vendas (3% sobre total vendido).       |

---

### 👥 3. Módulo de Compradores

| Função                   | Descrição                                                                  |
|--------------------------|----------------------------------------------------------------------------|
| `CadastrarComprador()`   | Registra um comprador com nome, CPF, email e endereço completo.            |
| `EditarComprador()`      | Permite modificar dados de um comprador.                                   |
| `ExcluirComprador()`     | Remove o cadastro de um comprador.                                         |
| `ListarCompradores()`    | Exibe todos os compradores registrados no sistema.

# Casos de Teste - Sistema de E-commerce em C

## 🧪 1. Menu de Produtos

### ✅ Teste 1.2 – Cadastro com código duplicado
**Objetivo**: Validar se o sistema impede o cadastro de produto com código já existente  
**Entradas**:
- Nome: "Mouse Gamer"
- Código: 1001 (já existente)

**Pré-condição**: Produto com código 1001 já cadastrado  
**Passos**:
1. Acessar menu de produtos
2. Tentar cadastrar produto com código duplicado

**Saída esperada**:  
`Erro: "Código de produto já cadastrado."`

---

### ✅ Teste 1.3 – Edição de produto existente
**Objetivo**: Confirmar edição de dados de produto  
**Entradas**:
- Código: 1001
- Novo preço: R$ 230.00

**Pré-condição**: Produto com código 1001 cadastrado  
**Passos**:
1. Acessar menu de produtos
2. Selecionar "Editar produto"
3. Informar código 1001 e novo preço

**Saída esperada**:  
`"Produto atualizado com sucesso."`

---

### ✅ Teste 1.4 – Exclusão de produto
**Objetivo**: Verificar remoção de produto  
**Entradas**: Código: 1001  
**Pré-condição**: Produto com código 1001 existe  
**Passos**:
1. Acessar menu de produtos
2. Escolher "Excluir produto"
3. Informar código 1001

**Saída esperada**:  
`"Produto removido com sucesso."`

---

## 🧪 2. Menu de Vendedores

### ✅ Teste 2.1 – Cadastro válido
**Entradas**:
- Nome: "Carlos"
- Número: 2001
- Salário fixo: R$ 2.000,00

**Pré-condição**: Número 2001 não utilizado  
**Saída esperada**:  
`"Vendedor cadastrado com sucesso."`

---

### ✅ Teste 2.2 – Cadastro com número duplicado
**Entradas**:
- Nome: "Fernanda"
- Número: 2001 (já usado)

**Saída esperada**:  
`"Erro: número já cadastrado."`

---

### ✅ Teste 2.3 – Edição de salário
**Entradas**:
- Número: 2001
- Novo salário: R$ 2.500,00

**Passos**:
1. Menu vendedores → Editar vendedor
2. Informar número e novo salário

**Saída esperada**:  
`"Salário atualizado com sucesso."`

---

## 🧪 3. Menu de Vendas

### ✅ Teste 3.1 – Venda com estoque suficiente
**Entradas**:
- Código venda: 3001
- Produto: Código 1002, Qtd: 2

**Pré-condição**: Estoque ≥ 2  
**Saída esperada**:  
`"Venda realizada com sucesso. Total: R$ X"`

---

### ✅ Teste 3.2 – Estoque insuficiente
**Entradas**:
- Produto: Código 1002, Qtd: 1000

**Saída esperada**:  
`"Erro: quantidade em estoque insuficiente."`

---

## 🧪 4. Menu de Compradores

### ✅ Teste 4.1 – Cadastro válido
**Entradas**:
- Nome: "João"
- CPF: "12345678900"
- Endereço: Rua A, Bairro B, BH-MG

**Saída esperada**:  
`"Comprador cadastrado com sucesso."`

---

### ✅ Teste 4.2 – CPF duplicado
**Entradas**:
- CPF: "12345678900" (já usado)

**Saída esperada**:  
`"Erro: CPF já cadastrado."`

---

### ✅ Teste 4.4 – Exclusão de comprador
**Entradas**: CPF: "12345678900"  
**Saída esperada**:  
`"Comprador excluído com sucesso."`



## 👥 Integrantes
- Bruno  
- Rafael  
- João  
- Matheus
