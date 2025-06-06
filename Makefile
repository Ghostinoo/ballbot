CXX = g++
CXXFLAGS = -I. -Wall -Wextra -std=c++17
LDFLAGS =
OUT = bbot
BUILD_DIR = build

SRC = $(shell find . -name "*.cpp")
OBJ = $(patsubst ./%.cpp, $(BUILD_DIR)/%.o, $(SRC))

all: $(OUT)

# Link step
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS)
	@echo "Applicando la priorità..."
	@sudo setcap cap_sys_nice+ep $@ || echo "Impossibile applicare la priorità. Esegui come root."
	@echo "Compilazione completata: '$(OUT)'"

# Compile step: make sure build/ path exists
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += -DDEBUG -g
debug: clean $(OUT)

clean:
	rm -rf $(BUILD_DIR) $(OUT)