# Definindo se estamos em modo de debug ou release
DEBUG ?= 1  # Se DEBUG estiver setado, irá ativar a compilação com depuração. Caso contrário, será em modo release.

# Comandos do compilador
CXX = g++

# Flags de compilação: C++17, Eigen3, SDL2 e multithreading (necessário para SDL2)
# Se DEBUG for 1, adiciona a flag -g para depuração
ifeq ($(DEBUG), 1)
    CXXFLAGS = -std=c++17 -I/usr/include/eigen3 -I/usr/include/SDL2 -D_REENTRANT -g -O0
else
    CXXFLAGS = -std=c++17 -I/usr/include/eigen3 -I/usr/include/SDL2 -D_REENTRANT -O2 -g
endif

# Flags de linkagem
LDFLAGS = -lSDL2 -lSDL2_image

# Diretórios
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Arquivo de saída
TARGET = $(BIN_DIR)/executavel

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Arquivos objeto
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regras de compilação
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Excuete "make clean" para remover os arquivos objeto e o executável
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./bin/executavel

# Se você quiser rodar em modo debug, basta rodar "make DEBUG=1"
.PHONY: all clean
