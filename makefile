CXX = mpiCC
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = myprogram

.PHONY: all clean

all: $(EXECUTABLE)
	@mpirun --oversubscribe -np 6 ./$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXECUTABLE)

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(EXECUTABLE)
