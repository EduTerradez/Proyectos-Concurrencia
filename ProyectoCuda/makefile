OBJS= main.o  
EXE = main
GCC= /usr/local/cuda/bin/nvcc
INCLUDES= -I. 
CFLAGS= -g -G -D_DEBUG
CXXFLAGS=
LDFLAGS = 

all: $(OBJS)         
	$(GCC) $(OBJS) $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) -o $(EXE)
	
main.o: main.cu
	$(GCC) $(INCLUDES) $(CFLAGS) $(CXXFLAGS)-c main.cu -o main.o

clean: 
	rm *.o $(EXE)
