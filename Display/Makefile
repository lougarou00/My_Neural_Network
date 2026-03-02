CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := main.cpp loadFen.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := chess

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SFMLFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
