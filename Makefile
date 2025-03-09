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

# Adicionando flags para Dear ImGui
CXXFLAGS += -I. -I./imgui -I./imgui/backends $(shell sdl2-config --cflags)

# Flags de linkagem
LDFLAGS = -lSDL2 -lSDL2_image $(shell sdl2-config --libs)

# Diretórios
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Arquivo de saída
TARGET = $(BIN_DIR)/executavel

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
IMGUISRC = $(wildcard imgui/*.cpp)
IMGUI_BACKEND_SRC = imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_sdlrenderer2.cpp

# Arquivos objeto
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
IMGUI_OBJ = $(patsubst imgui/%.cpp, $(OBJ_DIR)/imgui_%.o, $(IMGUISRC))
IMGUI_BACKEND_OBJ = $(patsubst imgui/backends/%.cpp, $(OBJ_DIR)/imgui_backends_%.o, $(IMGUI_BACKEND_SRC))

# Todos os objetos
ALL_OBJS = $(OBJS) $(IMGUI_OBJ) $(IMGUI_BACKEND_OBJ)

# Regras de compilação
all: $(TARGET)

$(TARGET): $(ALL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(ALL_OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/imgui_%.o: imgui/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/imgui_backends_%.o: imgui/backends/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Execute "make clean" para remover os arquivos objeto e o executável
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./bin/executavel

# Se você quiser rodar em modo debug, basta rodar "make DEBUG=1"
.PHONY: all clean run