# README

Este documento fornece uma visão geral do código desenvolvido em C++ que implementa uma busca paralela para encontrar o menor valor em uma matriz, utilizando múltiplas threads. O código é configurável, permitindo que o usuário especifique as dimensões da matriz, a quantidade de threads a serem utilizadas e se deseja ou não utilizar semáforos para gerenciar o acesso à seção crítica.

## Descrição do Código

### Dependências
O código utiliza as seguintes bibliotecas:
- `<bits/stdc++.h>`: para incluir várias bibliotecas padrão do C++.
- `<pthread.h>`: para manipulação de threads.
- `<semaphore.h>`: para utilização de semáforos.

### Estruturas de Dados
- **Matriz**: Uma matriz de números de ponto flutuante (`double`) é alocada dinamicamente. O tamanho da matriz é determinado pelas variáveis `X` (número de linhas) e `Y` (número de colunas).
- **Índice de Partição**: Um vetor `v_index` é utilizado para armazenar os índices que delimitam a faixa de linhas que cada thread irá processar.

### Funções Principais
1. **`matrixAlloc(double X, double Y)`**: Aloca e inicializa a matriz com valores aleatórios.
2. **`minorSearch(void* id)`**: Função executada por cada thread que busca o menor valor na parte da matriz que lhe foi atribuída, utilizando semáforos para garantir a exclusão mútua.
3. **`minorSearchNoSemaphore(void* id)`**: Versão da função de busca sem o uso de semáforos, executando uma busca simples.
4. **`matrixDiv(int tam, int n, int *vindex)`**: Divide a matriz em partes iguais para cada thread, armazenando os índices iniciais em `v_index`.
5. **`checkMinorSearch(int x, int y)`**: Verifica a validade do resultado da busca comparando com uma busca sequencial sem paralelismo.

### Fluxo Principal
No `main`, o usuário é solicitado a inserir as dimensões da matriz, o número de threads e a opção de utilizar semáforos. Dependendo da escolha do usuário, o programa cria threads que chamam a função apropriada para buscar o menor valor na matriz.

- Caso o semáforo seja utilizado, cada thread espera sua vez para acessar e potencialmente atualizar o valor mínimo encontrado.
- Se o semáforo não for usado, cada thread pode modificar o valor mínimo diretamente, o que pode levar a resultados incorretos se não houver controle de acesso.

### Resultados
Após a execução, o programa imprime:
- O menor valor encontrado na matriz.
- O tempo gasto na operação de busca em milissegundos.
- Se o semáforo não foi usado, uma validação do resultado é feita através de uma busca sequencial.

## Compilação e Execução
Para compilar e executar o código, utilize os seguintes comandos:

```bash
g++ -o busca_menor busca_menor.cpp -lpthread
./busca_menor
```

## Observações
- Certifique-se de que as dimensões da matriz não excedam os limites definidos (máximo de 15000x10000 ou 10000x15000).
- O uso de semáforos é recomendado para garantir resultados válidos em cenários de concorrência.

Este código exemplifica a utilização de programação paralela em C++, sendo uma boa base para estudos sobre eficiência em busca de dados em estruturas de grande volume.
