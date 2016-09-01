OBJS= main.o  multiplicacionMatrices.o generadorMatrices.o
EXE = main
GCC= g++
INCLUDES= -I. 
CFLAGS= -g  
CXXFLAGS=
LDFLAGS = -lpthread

generarMatrices: generarMatrices.o generadorMatrices.o
	$(GCC) $(INCLUDES) $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) generarMatrices.o generadorMatrices.o -o generarMatrices

all: $(OBJS)         
	$(GCC) $(OBJS) $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) -o $(EXE)
	
main.o: main.cpp
	$(GCC) $(INCLUDES) $(CFLAGS) $(CXXFLAGS)-c main.cpp -o main.o

multiplicacionMatrices.o: multiplicacionMatrices.cpp
	$(GCC) $(INCLUDES) $(CFLAGS) $(CXXFLAGS)-c multiplicacionMatrices.cpp -o multiplicacionMatrices.o

generadorMatrices.o: generadorMatrices.cpp
	$(GCC) $(INCLUDES) $(CFLAGS) $(CXXFLAGS)-c generadorMatrices.cpp -o generadorMatrices.o

clean: 
	rm *.o $(EXE)
