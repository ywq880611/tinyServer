CXX ?= g++

SRC = main.cpp server.cpp server.h

server: $(SRC)
	$(CXX) -o server $(SRC)