/*
Nome do Programa:
    single-process.cpp

Autores:
    João Victor Oliveira, 20240008468
    Kevin Gabriel Mangueira, 20240008000

Descrição:
    Este programa gera um vetor de valores aleatórios e realiza cálculos
    estatísticos fundamentais (média, mediana e desvio padrão). Todo o
    processamento ocorre em um único processo, sem uso de fork() ou pipes.
    O programa serve como base de comparação com a versão multiprocessada,
    permitindo medir o tempo de execução e observar diferenças de desempenho.
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using std::cout;
using std::endl;
using std::vector;

// Define quantidade de entradas geradas aleatoriamente
const int nEntradas = 10000;

// Declara vetor global com nEntradas valores
vector<int> valores(nEntradas);

// Identificadores simbólicos dos cálculos realizados
enum ResultType { MEDIA, MEDIANA, DESVIO_PADRAO };

// Função para cálcular media simples
double calc_media() {
    long long soma = 0; // acumulador da soma

    // Soma todos os elementos do vetor
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }

    // Retorna a média aritmética simples
    return (double)soma / nEntradas;
}

// Função para calcular mediana
double calc_mediana() {
    vector<int> v = valores;   // cria cópia local do vetor global
    std::sort(v.begin(), v.end()); // ordena valores em ordem crescente

    double mediana;

    if (nEntradas % 2 == 0) {
        // n é par → média dos dois valores centrais
        mediana = (v[nEntradas/2] + v[nEntradas/2 - 1]) / 2.0;
    } else {
        // n é ímpar → valor central
        mediana = v[nEntradas/2];
    }

    return mediana;
}

// Função para calcular desvio padrão
double calc_desvio_padrao() {
    long long soma = 0;

    // Soma os valores para calcular a média
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }

    double media = (double)soma / nEntradas;

    // Soma dos quadrados das diferenças
    double soma_quad = 0.0;

    for (int i = 0; i < nEntradas; i++) {
        soma_quad += std::pow(valores[i] - media, 2);
    }

    // Fórmula do desvio padrão populacional
    return std::sqrt(soma_quad / nEntradas);
}

// Função main
int main() {

    // Inicializa gerador de números aleatórios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    // Preenche vetor global com números aleatórios de 0 a 100
    for (int i = 0; i < nEntradas; i++) {
        valores[i] = dist(gen);
    }

    cout << "Execucao em 1 processo (single-process)" << endl;

    // Início da medição total de tempo
    auto t_inicio_total = std::chrono::high_resolution_clock::now();

    // Chama funções estatísticas diretamente no mesmo processo
    double final_media   = calc_media();
    double final_mediana = calc_mediana();
    double final_desvio  = calc_desvio_padrao();

    // Fim do tempo total
    auto t_fim_total = std::chrono::high_resolution_clock::now();

    // Cálculo em milissegundos
    double tempo_total =
        std::chrono::duration<double, std::milli>(t_fim_total - t_inicio_total).count();

    // Exibição dos resultados
    cout << "--- Resultados Estatisticos ---" << endl;
    cout << "Media: " << final_media << endl;
    cout << "Mediana: " << final_mediana << endl;
    cout << "Desvio padrao: " << final_desvio << endl;

    // Exibição da métrica de tempo
    cout << "--- Metricas de Tempo ---" << endl;
    cout << "Tempo Total: " << tempo_total << " ms" << endl;

    return 0;
}