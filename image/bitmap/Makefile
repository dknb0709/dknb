COMPILER = g++
CFLAGS = -Wall -O2
INCLUDE_DIR = ./include
SRC_DIR = ./src
BIN_DIR = ./bin

$(BIN_DIR)/bmp_util.o: $(SRC_DIR)/bmp_util.cpp $(INCLUDE_DIR)/bmp_util.hpp
	$(COMPILER) -c $< $(CFLAGS) -o $@ -I$(INCLUDE_DIR)

$(BIN_DIR)/test.o: $(SRC_DIR)/test.cpp
	$(COMPILER) -c $< $(CFLAGS) -o $@ -I$(INCLUDE_DIR)

test.exe: $(BIN_DIR)/bmp_util.o $(BIN_DIR)/test.o
	$(COMPILER) $^ $(CFLAGS) -o $@ -I$(INCLUDE_DIR)

