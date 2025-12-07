#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <unistd.h>
#include <sys/wait.h>

using std::cout;
using std::endl;
using std::vector;

const int nEntradas = 10000;
vector<int> valores(nEntradas);

enum ResultType { MEDIA, MEDIANA, DESVIO_PADRAO };


void calc_media(int write_fd) {
    long long soma = 0;
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }
    double media = (double)soma / nEntradas;
    
    // Escreve o tipo de resultado e o valor no pipe
    int type = MEDIA;
    write(write_fd, &type, sizeof(int));
    write(write_fd, &media, sizeof(double));
    _exit(0); // Termina o processo filho
}


void calc_mediana(int write_fd) {
    vector<int> v = valores; 
    std::sort(v.begin(), v.end());

    double mediana;
    if (nEntradas % 2 == 0) {
        // N é par: média dos dois elementos centrais
        mediana = (v[nEntradas/2] + v[nEntradas / 2 - 1]) / 2.0;
    } else {
        // N é ímpar: elemento central
        mediana = v[nEntradas/2];
    }

    // Escreve o tipo de resultado e o valor no pipe
    int type = MEDIANA;
    write(write_fd, &type, sizeof(int));
    write(write_fd, &mediana, sizeof(double));
    _exit(0);
}


void calc_desvio_padrao(int write_fd) {
    long long soma = 0;
    for (int i = 0; i < nEntradas; i++) {
        soma += valores[i];
    }
    double media = (double)soma / nEntradas;

    double soma_quad = 0.0;
    for (int i = 0; i < nEntradas; i++) {
        soma_quad += std::pow(valores[i] - media, 2);
    }
    double desvioPadrao = std::sqrt(soma_quad / nEntradas);

    int type = DESVIO_PADRAO;
    write(write_fd, &type, sizeof(int));
    write(write_fd, &desvioPadrao, sizeof(double));
    _exit(0);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100); 

    for (int i = 0; i < nEntradas; i++) {
        valores[i] = dist(gen);
    }

    cout << "Exemplo de execucao com 3 processos (fork/pipe)" << endl;

    int pipe_fds[2]; 
    if (pipe(pipe_fds) == -1) {
        perror("pipe failed");
        return 1;
    }
    
    // Início da Medição Total e da Criação
    auto t_inicio_total = std::chrono::high_resolution_clock::now(); 

    pid_t pids[3];
    int num_processos = 3;

    // Processo 1: Média
    pids[0] = fork();
    if (pids[0] == 0) {
        close(pipe_fds[0]);
        calc_media(pipe_fds[1]);
    } else if (pids[0] < 0) {
        perror("fork failed for media");
        return 1;
    }

    // Processo 2: Mediana
    pids[1] = fork();
    if (pids[1] == 0) {
        close(pipe_fds[0]);
        calc_mediana(pipe_fds[1]);
    } else if (pids[1] < 0) {
        perror("fork failed for mediana");
        return 1;
    }

    // Processo 3: Desvio Padrão
    pids[2] = fork();
    if (pids[2] == 0) {
        close(pipe_fds[0]);
        calc_desvio_padrao(pipe_fds[1]);
    } else if (pids[2] < 0) {
        perror("fork failed for desvio padrao");
        return 1;
    }

    // Fim da Medição da Criação (após o último fork)
    auto t_fim_criacao = std::chrono::high_resolution_clock::now(); 
    
    close(pipe_fds[1]); 

    double final_media = 0;
    double final_mediana = 0;
    double final_desvio = 0;
    
    int valoresRecebidos = 0;
    while (valoresRecebidos < num_processos) {
        int type;
        double value;
        
        if (read(pipe_fds[0], &type, sizeof(int)) > 0) {
            if (read(pipe_fds[0], &value, sizeof(double)) > 0) {
                switch (type) {
                    case MEDIA:
                        final_media = value;
                        break;
                    case MEDIANA:
                        final_mediana = value;
                        break;
                    case DESVIO_PADRAO:
                        final_desvio = value;
                        break;
                }
                valoresRecebidos++;
            }
        }
    }
    
    for (int i = 0; i < num_processos; i++) {
        waitpid(pids[i], nullptr, 0); 
    }

    // Fim da Medição Total
    auto t_fim_total = std::chrono::high_resolution_clock::now();

    // Cálculo dos Tempos
    double tempo_criacao = std::chrono::duration<double, std::milli>(t_fim_criacao - t_inicio_total).count();
    double tempo_total = std::chrono::duration<double, std::milli>(t_fim_total - t_inicio_total).count();

    cout << "--- Resultados Estatisticos ---" << endl;
    cout << "Media: " << final_media << endl;
    cout << "Mediana: " << final_mediana << endl;
    cout << "Desvio padrao: " << final_desvio << endl;
    
    cout << "--- Metricas de Tempo ---" << endl;
    cout << "Tempo Total:" << tempo_total << " ms" << endl;
    cout << "Tempo de Criacao dos processos: " << tempo_criacao << " ms" << endl;

    return 0;
}