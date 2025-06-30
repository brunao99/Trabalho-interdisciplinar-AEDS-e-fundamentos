# Trabalho Interdisciplinar - AEDS / Fundamentos

## 📋 Backlog do Trabalho

### 0. Menu principal
- [ ] **0.1** Criar Menu Pricipal da loja

### 1. Cadastro de Produtos
- [ ] **1.0** Gerar arquivo para Produto
- [ ] **1.1** Criar `struct` de Produtos
- [ ] **1.2** Função para criar Produto
- [ ] **1.21** Geracao do id automaticamente.
- [ ] **1.3** Garantir que não haja conflito entre os códigos dos produtos
- [ ] **1.4** Consultar, alterar, inserir e excluir produtos no cadastro

### 2. Cadastro de Vendedores
- [ ] **2.0** 2.0 Gerar o arquivo para vendedores
- [ ] **2.1** Criar `struct` de Vendedores
- [ ] **2.2** Função para criar Vendedores
- [ ] **2.3** Garantir comissão dos vendedores e aumento de salário
- [ ] **2.4** Garantir que não haverá conflito no cadastro de dois vendedores
- [ ] **2.5** Consultar, alterar, inserir e excluir vendedores no cadastro

### 3. Cadastro de Comprador
- [ ] **3.1** Criar `struct` de Comprador
- [ ] **3.2** Função para criar Comprador
- [ ] **3.3** Consultar, alterar, inserir e excluir compradores no cadastro

### 4. Cadastro da Venda
- [ ] **4.1** Criar `struct` de Venda
- [ ] **4.2** Criar `struct` de VendaItem
- [ ] **4.3** Função para registrar Vendas
- [ ] **4.4** Verificar quantidade em estoque

### 5. Emissão da Nota Fiscal
- [ ] **5.1** Criar `struct` de Nota Fiscal
- [ ] **5.2** Função para emitir Nota Fiscal
- [ ] **5.3** Incluir restrições de frete (pago e grátis)

### 6. Documentação e Testes
- [ ] **6.1**  Documentar os módulos existentes (funções)
      - [ ]  **6.1.1** Especificar objetivo, parâmetros e retorno de cada função
      - [ ]  **6.1.2** Verificar se os nomes dos módulos são coerentes com sua funcionalidade
- [ ] **6.2** Criar e executar casos de teste para cada módulo (incluindo exceções)
     - [ ]  **6.2.1** Elaborar casos de teste de sucesso (entrada válida)
     - [ ]  **6.2.2** Elaborar casos de teste de exceção (entradas inválidas, arquivos inexistentes, etc.)
     - [ ]  **6.2.3** Executar os testes e registrar as saídas reais
- [ ] **6.3** Gerar relatório comparando saída esperada vs real
     - [ ]  **6.3.1** Criar tabela com nome da função, entrada, saída esperada, saída real e status
     - [ ]  **6.3.2** Identificar falhas e anotar correções necessárias
- [ ] **6.4** Garantir que as alterações não quebrem funcionalidades anteriores
     - [ ]  **6.4.1** Reexecutar os testes anteriores após mudanças
     - [ ]  **6.4.2** Confirmar se os módulos ainda funcionam corretamente
     - [ ]  **6.4.2** Atualizar o relatório com os resultados pós-refatoração

---

## 🚀 Sprints

### ✅ Sprint 1 - *24/06/2025 a 27/06/2025*

#### Bruno e Rafael
- Bruno:
  - [✅] 0.1 Criar Menu principal do E-commerce
  - [✅]  1.1 Criar `struct` de Produtos  
  - [✅] 1.3 Garantir que não haja conflito entre os códigos dos produtos  
  - [✅ ] 1.4 Consultar, alterar e excluir produtos no cadastro
- Rafael:
  - [✅]  1.0 Gerar arquivo para Produto
  - [✅]  1.2 Função para criar Produto
  - [✅]  1.21 Geracao do id automaticamente


#### João e Matheus
- Matheus:
  - [✅] 2.1 Criar `struct` de Vendedores  
  - [✅] 2.3 Garantir comissão dos vendedores e aumento de salário
- João:
  - [✅] 2.0 Gerar o arquivo para vendedores
  - [✅] 2.2 Função para criar Vendedores  
  - [✅] 2.4 Garantir que não haverá conflito no cadastro de dois vendedores  
  - [✅] 2.5 Consultar, alterar, inserir e excluir vendedores no cadastro  

### 📅 Sprint 2 - *27/06/2025 a 29/06/2025*

#### Bruno e Rafael
- Bruno:
  - [✅] **3.1** Criar `struct` de Comprador
  - [✅] **3.2** Função para criar Comprador
- Rafael:
  - [✅] **4.2** Criar `struct` de VendaItem
  - [✅] **4.1** Criar `struct` de Venda
  - [✅] **4.2** Função para registrar Vendas

   #### João e Matheus
- Matheus:
  - [✅ ] **5.1** Criar `struct` de Nota Fiscal
  - [✅ ] **5.2** Função para emitir Nota Fiscal
  - [✅] **5.3** Incluir restrições de frete (pago e grátis)
- João:
  - [✅] **3.3** Consultar, alterar, inserir e excluir compradores no cadastro
  - [✅] **4.3** Verificar quantidade em estoque


### 📅 Sprint 3 - *30/06/2025 a 02/07/2025

#### Bruno e Rafael

- Bruno:
   - [ ] **6.1**  Documentar os módulos existentes (funções)
     ##### todos participaram da documentação, é apenas as correções necessárias.
      - [ ]  **6.1.1** Especificar objetivo, parâmetros e retorno de cada função
      - [ ]  **6.1.2** Verificar se os nomes dos módulos são coerentes com sua funcionalidade
- Rafael:
    - [ ] **6.2** Criar e executar casos de teste para cada módulo (incluindo exceções)
     - [ ]  **6.2.1** Elaborar casos de teste de sucesso (entrada válida)
     - [ ]  **6.2.2** Elaborar casos de teste de exceção (entradas inválidas, arquivos inexistentes, etc.)
     - [ ]  **6.2.3** Executar os testes e registrar as saídas reais

#### João e Matheus
- Matheus:
    - [ ] **6.4** Garantir que as alterações não quebrem funcionalidades anteriores
     - [ ]  **6.4.1** Reexecutar os testes anteriores após mudanças
     - [ ]  **6.4.2** Confirmar se os módulos ainda funcionam corretamente
     - [ ]  **6.4.2** Atualizar o relatório com os resultados pós-refatoração
- João:
    - [ ] **6.3** Gerar relatório comparando saída esperada vs real
     - [ ]  **6.3.1** Criar tabela com nome da função, entrada, saída esperada, saída real e status
     - [ ]  **6.3.2** Identificar falhas e anotar correções necessárias

---

## 👥 Integrantes
- Bruno  
- Rafael  
- João  
- Matheus
