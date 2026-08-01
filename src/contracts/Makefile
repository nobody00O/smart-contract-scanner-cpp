CXX = g++
# Turns on "All Warnings" : -Wall 
CXXFLAGS = -std=c++17 -Wall    

all: scanner

scanner: src/main.cpp
# This line actually runs the compiler using the variables defined above:
# Translates to: g++ -std=c++17 -Wall src/main.cpp -o scanner
	$(CXX) $(CXXFLAGS) src/main.cpp -o scanner

clean:
	rm -f scanner
