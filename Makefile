# Nombre del compilador
CXX = g++

# Flags del compilador
CXXFLAGS = -std=c++11 -Wall -Wextra

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SRCS = $(wildcard $(SRC_DIR)/*.cc)

# Archivos objeto
OBJS = $(SRCS:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

# Nombre del ejecutable
TARGET = $(BIN_DIR)/recommendator

# Reglas

# Regla por defecto: compilar todo
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Crear el directorio bin si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Crear el directorio obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpiar archivos objeto y ejecutable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean