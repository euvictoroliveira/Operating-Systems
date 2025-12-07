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


// Declara a quantidade de entradas e um vector com de tamanho N
const int nEntradas = 10000;

// main
int main(){
    vector<int> valores(nEntradas);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100);

    for(int i = 0; i < nEntradas; i++)
        valores[i] = dist(gen);

    auto inicio = std::chrono::high_resolution_clock::now();

    // Calcular média:
    long long soma = 0;
    for(int i = 0; i < nEntradas; i++) 
        soma += valores[i];
    double media = double(soma) / nEntradas;

    // Mediana
    sort(valores.begin(), valores.end());
    double mediana = valores[nEntradas/2];

    // Desvio padrão
    double soma_quad = 0;
    for (int i = 0; i < nEntradas; i++)
        soma_quad += pow(valores[i] - media, 2);

    double desvioPadrao = sqrt(soma_quad / nEntradas);

    auto fim = std::chrono::high_resolution_clock::now();
    double tempo = std::chrono::duration<double, std::milli> (fim - inicio).count();

    cout << "Exemplo de execucao single thread" << endl;
    cout << "Media: " << media << endl;
    cout << "Mediana: " << mediana << endl;
    cout << "Desvio padrao: " << desvioPadrao << endl;
    cout << "Tempo total: " << tempo << " ms" << endl;

    return 0;
}