OBJS= mpi_main.o  
EXE = mpi_main
GCC= gcc
INCLUDES= -I. 
CFLAGS= -g
CXXFLAGS=
LDFLAGS = -lmpi

all: $(OBJS)
	$(GCC) $(OBJS) $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) -o $(EXE)

mpi_main.o: mpi_main.c
	$(GCC) $(INCLUDES) $(CFLAGS) $(CXXFLAGS)-c mpi_main.c -o mpi_main.o

clean: 
	rm *.o $(EXE)