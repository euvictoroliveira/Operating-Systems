/*
Nome do Programa:
    atividade01.cpp

Autores:
    João Victor Oliveira, 20240008468
    Kevin Gabriel Mangueira, 20240008000

Descrição Geral:
    Este programa demonstra a criação hierárquica de processos usando chamadas
    fork(), formando uma estrutura de Pai → Filhos → Netos.
    Cada processo neto executa um comando do shell por meio da função execl(),
    permitindo rodar instruções como "ls", "pwd", "date" ou "whoami".

Organização dos Processos:
    - O processo pai P1 cria dois processos filhos (F1 e F2).
    - Cada filho cria dois processos netos:
         F1 → N1("ls")   e N2("pwd")
         F2 → N3("date") e N4("whoami")
    - Os filhos aguardam o término de seus respectivos netos.
    - O pai aguarda o término de ambos os filhos.
*/
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;



// Função responsável por criar o processo neto e execuda comando do shell
void criarNeto(const char* comando) {
    pid_t pid = fork(); // cria um novo processo neto

    if (pid < 0) { 
        cerr << "Erro ao criar processo N." << endl; // caso o fork retorne um valor negativo, entra no caso e aponta erro de criação do processo
        exit(1);
    } else if (pid == 0) { // Caso o forl retorne 0, isso indica que nó atual da árvore é o processo neto
        // O código abaixo substitui o processo atual por um camando do shell
        // "/bin/sh -c comando" executa strings como: "ls", "pwd", "date" etc.
        execl("/bin/sh", "sh", "-c", comando, (char*)nullptr);

        // Mensangem caso a execução do comando falhe
        cerr << "Falha ao executar comando." << endl;
        exit(1);
    }

    // Nota: Se o PID for maior que 0, isso indica que estamos no processo filho ou pai
}

// Função responsável por criar o processo filho
void criarFilho(const char* cmd1, const char* cmd2) {
    pid_t pid = fork(); // Cria um processo filho(F1 ou F2)

    if (pid < 0) { // Acusa erro de criação de processo caso o pid seja negativo
        cerr << "Erro ao criar processo filho." << endl;
        exit(1);
    } else if (pid == 0) {
        // Indica que estamos no processo filho F1 ou F2    
        // Etapa de criação de dois processos netos(cada um excecuta um comando)
        criarNeto(cmd1); // N1 ou N3
        criarNeto(cmd2); // N2 ou N4

        // Espera N1 e N2 terminarem
        wait(nullptr);
        wait(nullptr);

        // Log do processo filho
        cout << "Sou processo filho. PID: " << getpid()
             << ", PID do pai: " << getppid() << endl;

        exit(0); // processo filhos encerram aqui.
    }
}

// Função main
int main() {
    // Criando os filhos F1 e F2
    criarFilho("ls", "pwd");        // F1 -> N1(ls) e N2(pwd)
    criarFilho("date", "whoami");   // F2 -> N3(date) e N4(whoami)

    // Espera F1 e F2 terminarem
    wait(nullptr);
    wait(nullptr);

    // Mensagem do processo pai
    cout << "Sou processo pai P1. PID: " << getpid() << endl;
    cout << "Encerrando o programa." << endl;

    return 0;
}
