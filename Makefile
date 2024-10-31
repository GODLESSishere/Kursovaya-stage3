.PHONY: all clean format static sanitize

CXX=g++
CXXFLAGS=-O2 -Wall -DNDEBUG -std=c++17
DBGFLAGS=-g -Og
SANFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS=-lcrypto++ 
PROJECT=server
STATIC=server_static 
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)

all : $(PROJECT)
static : $(STATIC)

sanitize : $(SOURCES)
	$(CXX) $^ $(DBGFLAGS) $(LDFLAGS) $(SANFLAGS) -o server_san

$(PROJECT) : $(OBJECTS)
	$(CXX)  $^ $(LDFLAGS) -o $@

$(STATIC) : $(OBJECTS)
	$(CXX) -static  $^  $(LDFLAGS) -o $@

server.o : server.cpp $(wildcard *.h)
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

format:
	astyle *.cpp *.h
	
clean:
	rm -f $(PROJECT) $(STATIC) *.o *.orig
