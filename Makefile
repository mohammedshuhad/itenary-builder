CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = tsp_solver
SRCS = TspSolver.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = TspSolver.hpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)