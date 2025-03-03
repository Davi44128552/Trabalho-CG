# Variáveis do compilador e flags
CXX       = g++
CXXFLAGS  = -Wall -g $(shell sdl2-config --cflags) -I. -I./imgui -I./imgui/backends -I/usr/include/eigen3
LDFLAGS   = $(shell sdl2-config --libs)

# Fontes e objetos
MAIN_SRC   = $(wildcard *.cpp)
IMGUISRC   = $(wildcard imgui/*.cpp)

IMGUI_BACKEND_SRC = imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_sdlrenderer2.cpp

IMGUI_BACKEND_OBJ = $(patsubst imgui/backends/%.cpp, obj/imgui_backends_%.o, $(IMGUI_BACKEND_SRC))


MAIN_OBJ   = $(patsubst %.cpp, obj/%.o, $(notdir $(MAIN_SRC)))
IMGUI_OBJ  = $(patsubst imgui/%.cpp, obj/imgui_%.o, $(IMGUISRC))

OBJ        = $(MAIN_OBJ) $(IMGUI_OBJ) $(IMGUI_BACKEND_OBJ)

TARGET     = bin/main

# Regra principal
all: $(TARGET)

# Linka todos os objetos para gerar o executável na pasta bin
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)
obj/imgui_backends_%.o: imgui/backends/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Regra para compilar os .cpp que estão na raiz
obj/%.o: %.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para compilar os .cpp da pasta imgui
obj/imgui_%.o: imgui/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa os arquivos compilados
clean:
	rm -rf obj/*.o bin/main
