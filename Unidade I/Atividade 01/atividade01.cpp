#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void criarNeto(const char* comando) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Erro ao criar processo N." << endl;
        exit(1);
    } else if (pid == 0) {
        // Processo N
        execl("/bin/sh", "sh", "-c", comando, (char*)nullptr);
        cerr << "Falha ao executar comando." << endl;
        exit(1);
    }
}

void criarFilho(const char* cmd1, const char* cmd2) {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Erro ao criar processo filho." << endl;
        exit(1);
    } else if (pid == 0) {
        // Processo F
        criarNeto(cmd1); // N1 ou N3
        criarNeto(cmd2); // N2 ou N4

        // Espera N1 e N2 terminarem
        wait(nullptr);
        wait(nullptr);

        cout << "Sou processo filho. PID: " << getpid()
             << ", PID do pai: " << getppid() << endl;
        exit(0);
    }
}

int main() {
    // Criando os filhos F1 e F2
    criarFilho("ls", "pwd");        // F1 -> N1(ls) e N2(pwd)
    criarFilho("date", "whoami");   // F2 -> N3(date) e N4(whoami)

    // Espera F1 e F2 terminarem
    wait(nullptr);
    wait(nullptr);

    cout << "Sou processo pai P1. PID: " << getpid() << endl;
    cout << "Encerrando o programa." << endl;

    return 0;
}
