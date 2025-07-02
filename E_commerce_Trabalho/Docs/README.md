# Trabalho Interdisciplinar - AEDS / Fundamentos (Documentos)

## 📋 Backlog do Trabalho
#### Aqui vai persistir a documentação do trabalho, casos de teste, fotos de sprint, etc...
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
