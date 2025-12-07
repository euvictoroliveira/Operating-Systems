/*
Nome do Programa:
    single-thread.cpp

Autores:
    João Victor Oliveira, 20240008468
    Kevin Gabriel Mangueira, 20240008000

Descrição:
    Este programa gera um vetor com 10.000 valores aleatórios e calcula
    três métricas estatísticas fundamentais: média, mediana e desvio padrão.
    Toda a execução é realizada dentro de uma única thread, servindo como
    referência de desempenho para comparação com versões paralelas que usam
    múltiplas threads ou processos. O programa também mede o tempo total
    gasto nos cálculos.
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>
#include <random>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

// Define tamanho fixo do vetor de dados
const int nEntradas = 10000;

int main() {

    // Cria vetor local contendo nEntradas valores
    vector<int> valores(nEntradas);

    // Geradores de número aleatório (0 a 100)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    // Preenche o vetor com valores aleatórios
    for (int i = 0; i < nEntradas; i++)
        valores[i] = dist(gen);

    // Início da medição de tempo
    auto inicio = std::chrono::high_resolution_clock::now();

    // Cálculo da Média
    long long soma = 0;
    for (int i = 0; i < nEntradas; i++)
        soma += valores[i];

    // Média aritmética simples
    double media = double(soma) / nEntradas;

    // Cálculo da Mediana
    // Ordena vetor (necessário para encontrar a mediana)
    std::sort(valores.begin(), valores.end());

    // Como nEntradas = 10000 → valor central é valores[5000]
    double mediana = valores[nEntradas / 2];

    // Cálculo do Desvio Padrão
    double soma_quad = 0;

    // Soma dos quadrados das diferenças para cada elemento
    for (int i = 0; i < nEntradas; i++)
        soma_quad += std::pow(valores[i] - media, 2);

    // Fórmula do desvio padrão populacional
    double desvioPadrao = std::sqrt(soma_quad / nEntradas);

    // Fim da medição
    auto fim = std::chrono::high_resolution_clock::now();
    double tempo = std::chrono::duration<double, std::milli>(fim - inicio).count();

    // Exibição dos Resultados
    cout << "Exemplo de execucao single thread" << endl;
    cout << "Media: " << media << endl;
    cout << "Mediana: " << mediana << endl;
    cout << "Desvio padrao: " << desvioPadrao << endl;

    cout << "--- Metricas de Tempo ---" << endl;
    cout << "Tempo total: " << tempo << " ms" << endl;

    return 0;
}