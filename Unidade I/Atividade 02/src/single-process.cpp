#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using std::cout;
using std::endl;
using std::vector;

// Valores globais
const int nEntradas = 10000;
vector<int> valores(nEntradas);

enum ResultType { MEDIA, MEDIANA, DESVIO_PADRAO };

double calc_media() {
    long long soma = 0;
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }
    return (double)soma / nEntradas;
}

double calc_mediana() {
    vector<int> v = valores;
    std::sort(v.begin(), v.end());

    double mediana;
    if (nEntradas % 2 == 0) {
        mediana = (v[nEntradas/2] + v[nEntradas / 2 - 1]) / 2.0;
    } else {
        mediana = v[nEntradas/2];
    }
    return mediana;
}

double calc_desvio_padrao() {
    // Recalcular a média
    long long soma = 0;
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }
    double media = (double)soma / nEntradas;

    // Calcular desvio padrão
    double soma_quad = 0.0;
    for (int i = 0; i < nEntradas; i++) {
        soma_quad += std::pow(valores[i] - media, 2);
    }
    double desvioPadrao = std::sqrt(soma_quad / nEntradas);
    return desvioPadrao;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    for (int i = 0; i < nEntradas; i++) {
        valores[i] = dist(gen);
    }

    cout << "Exemplo de execucao com 1 processo (single-process)" << endl;
    auto t_inicio_total = std::chrono::high_resolution_clock::now();

    double final_media = calc_media();
    double final_mediana = calc_mediana();
    double final_desvio = calc_desvio_padrao();

    auto t_fim_total = std::chrono::high_resolution_clock::now();

    double tempo_total = std::chrono::duration<double, std::milli>(t_fim_total - t_inicio_total).count();

    cout << "--- Resultados Estatisticos ---" << endl;
    cout << "Media: " << final_media << endl;
    cout << "Mediana: " << final_mediana << endl;
    cout << "Desvio padrao: " << final_desvio << endl;

    cout << "--- Metricas de Tempo ---" << endl;
    cout << "Tempo Total:" << tempo_total << " ms" << endl;

    return 0;
}