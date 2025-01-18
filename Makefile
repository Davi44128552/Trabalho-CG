CXX = g++

# Flags de compilação: C++17, Eigen3, SDL2 e multithreading (necessário para SDL2)
CXXFLAGS = -std=c++17 -I/usr/include/eigen3 -I/usr/include/SDL2 -D_REENTRANT

# Flags de linkagem
LDFLAGS = -lSDL2

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

.PHONY: all clean
