/*
Nome do Programa:
    threads.cpp

Autores:
    João Victor Oliveira, 20240008468
    Kevin Gabriel Mangueira, 20240008000

Descrição:
    Este programa gera um vetor de 10.000 valores aleatórios e calcula
    três métricas estatísticas fundamentais: média, mediana e desvio padrão.
    A execução é feita utilizando três threads independentes, cada uma
    responsável por um cálculo específico. O objetivo é comparar o ganho de
    desempenho em relação à versão single-thread e à versão multiprocessos
    usando fork(). O programa também mede o tempo de criação das threads e
    o tempo total de execução.
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

// Variáveis globais para armazenar resultados
double media = 0;
double mediana = 0;
double desvioPadrao = 0;

// Quantidade fixa de entradas
const int nEntradas = 10000;
vector<int> valores(nEntradas);

// Função que calcula a média aritmética
void calc_media() {
    long long soma = 0;

    // Soma todos os valores do vetor
    for (int i = 0; i < nEntradas; i++)
        soma += valores[i];

    // Média aritmética simples
    media = (double) soma / valores.size();
}

// Função que calcula a mediana
void calc_mediana() {
    // Cria cópia local para realizar ordenação
    vector<int> v = valores;

    std::sort(v.begin(), v.end());

    // Cálculo da mediana depende da paridade do tamanho
    if (nEntradas % 2 == 0)
        mediana = (v[nEntradas/2] + v[nEntradas/2 - 1]) / 2.0;
    else
        mediana = v[nEntradas/2];
}

// Função que calcula o desvio padrão populacional
void calc_desvio() {
    double acumulado = 0;

    // Usa a média global previamente calculada
    for (int i = 0; i < nEntradas; i++)
        acumulado += (valores[i] - media) * (valores[i] - media);

    desvioPadrao = sqrt(acumulado / nEntradas);
}

// Função main
int main() {

    // Gerador de números aleatórios entre 0 e 100
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    // Preenche o vetor com valores aleatórios
    for (int i = 0; i < nEntradas; i++)
        valores[i] = dist(gen);

    cout << "Exemplo de execucao com 3 threads" << endl;

    // Marca início do tempo de execução geral
    auto inicio = std::chrono::high_resolution_clock::now();

    // Criação das três threads
    auto t_inicio = std::chrono::high_resolution_clock::now();
    std::thread t1(calc_media);
    std::thread t2(calc_mediana);
    std::thread t3(calc_desvio);
    auto t_fim = std::chrono::high_resolution_clock::now();

    // Espera todas as threads terminarem
    t1.join();
    t2.join();
    t3.join();

    // Marca fim da execução
    auto fim = std::chrono::high_resolution_clock::now();

    // Cálculo dos tempos
    double tempo_exec = std::chrono::duration<double, std::milli>(fim - inicio).count();
    double tempo_criacao = std::chrono::duration<double, std::milli>(t_fim - t_inicio).count();

    // Exibe resultados
    cout << "Media: " << media << endl;
    cout << "Mediana: " << mediana << endl;
    cout << "Desvio padrao: " << desvioPadrao << endl;

    cout << "--- Metricas de Tempo ---" << endl;
    cout << "Tempo total: " << tempo_exec << " ms" << endl;
    cout << "Tempo de criacao das threads: " << tempo_criacao << " ms" << endl;

    return 0;
}