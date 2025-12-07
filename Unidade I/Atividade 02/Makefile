CXX = g++
CXXFLAGS = -O2 -Wall -pthread

# Gera as pastas src e bin
SRC = src
BIN = bin

# informa a rota dos arquivos fonte
THREADS_SRC = $(SRC)/threads.cpp
SINGLE_SRC  = $(SRC)/single-thread.cpp
PROC_SRC    = $(SRC)/processos.cpp

# geração de executáveis(salvos em bin)
THREADS_BIN = $(BIN)/threads
SINGLE_BIN  = $(BIN)/single-thread
PROC_BIN    = $(BIN)/processos

# Alvo padrão
all: $(THREADS_BIN) $(SINGLE_BIN) $(PROC_BIN)

# Compila threads.cpp
$(THREADS_BIN): $(THREADS_SRC)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@

# Compila single-thread.cpp
$(SINGLE_BIN): $(SINGLE_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Compila processos.cpp
$(PROC_BIN): $(PROC_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Limpeza
clean:
	rm -rf $(BIN)/*

# Comandos de execução
run-threads:
	./$(BIN)/threads

run-single:
	./$(BIN)/single-thread

run-processos:
	./$(BIN)/processos
