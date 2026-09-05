CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = main.exe
SRCS = main.cpp prim_variations.cpp structure_scc.cpp structure_topo.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /f /q $(OBJS) $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
