CXX = g++
CXXFLAGS = -I. -Wall -Wextra -std=c++17
SRC = $(shell find . -name "*.cpp")
OUT = test

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

debug: CXXFLAGS += -DDEBUG -g
debug: clean $(OUT)

clean:
	rm -f $(OUT)