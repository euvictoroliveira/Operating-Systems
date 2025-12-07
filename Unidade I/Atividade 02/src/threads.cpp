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


// Variávies globais(fiz isso pra tornar os cálculos mais práticos)
double media = 0;
double mediana = 0;
double desvioPadrao = 0;

// Declara a quantidade de entradas e um vector com de tamanho N
const int nEntradas = 10000;
vector<int> valores(nEntradas);

// Função para cálculo de média
void calc_media(){
    long long int soma = 0;

    for(int i = 0; i < nEntradas; i++)
        soma += valores[i];

    media = (double) soma / (int) valores.size();
}

// Função para cálculo de mediana
void calc_mediana(){
    vector<int> v = valores; // cria vector local como cópia do vector global
    // ordenal em rol
    sort(v.begin(), v.end());

    // realiza cálculo da mediana
    if(nEntradas % 2 == 0)
        mediana = (v[nEntradas/2] + v[nEntradas / 2 - 1]) / 2.0;
    else
        mediana = v[nEntradas/2];

}

void calc_desvio(){
    double soma = 0;
    // Percorre vector acumulando seus valores
    for(int i = 0; i < nEntradas; i++)
        soma += valores[i];

    double acumulado = 0.0;
    for(int i = 0; i < nEntradas; i++)
        acumulado += (valores[i] - media) * (valores[i] - media);

    desvioPadrao =  sqrt(acumulado / nEntradas);

}

// main
int main(){
    // Gera os valores de maneira aleatório
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,100); // valores aleatórios de 0 a 100

    // preenche o vetor com os valores aleatórios usanod dist()
    for(int i = 0; i < nEntradas; i++)
        valores[i] = dist(gen);

    cout << "Exemplo de execucao com 3 threads" << endl;

    // seta o inicio do time 
    auto inicio = std::chrono::high_resolution_clock::now();

    // Cria as threads;
    auto t_inicio = std::chrono::high_resolution_clock::now();
    std::thread t1(calc_media); // calcula media
    std::thread t2(calc_mediana); // calcula mediana
    std::thread t3(calc_desvio); // calcula desvio padrão
    auto t_fim  = std::chrono::high_resolution_clock::now();    

    // aguarda fim das threads
    t1.join();
    t2.join();
    t3.join();

    auto fim = std::chrono::high_resolution_clock::now();

    double tempo_exec = std::chrono::duration<double, std::milli>(fim - inicio).count();
    double tempo_criacao = std::chrono::duration<double, std::milli>(t_fim - t_inicio).count();

    cout << "Media: " << media << endl;
    cout << "Mediana: " << mediana << endl;
    cout << "Desvio padrao: " << desvioPadrao << endl;

    cout << "Tempo total: " << tempo_exec << " ms" << endl;
    cout << "Tempo de criacao das threads: " << tempo_criacao << " ms" << endl;

    return 0;
}