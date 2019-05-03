DEBUG=0
# g++ instead of gcc, because nvcc uses g++
# using gcc here will create linkage problem
#CC=/afs/andrew.cmu.edu/usr17/jiongtay/private/gcc/bin/gcc
CXX=/afs/andrew.cmu.edu/usr17/jiongtay/private/gcc/bin/g++
OMP=-fopenmp -DOMP
#CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=gnu99
CXXFLAGS=-g -O3 -Wall

#CFILES = benchmark.c coding_loop.c cycletimer.c galois.c matrix.c reed_solomon.c
#HFILES = coding_loop.h cycletimer.h galois.h matrix.h reed_solomon.h

# actually cuda version also works for cpu as well, not need to do this
#cpu: $(CFILES) $(HFILES)
#	$(CC) $(CFLAGS) $(OMP) -o benchmark $(CFILES) $(LDFLAGS)

################################################

OBJDIR=objs
CUFILES = cuda_encoder.cu
HFILES += cuda_encoder.h

NVCCFLAGS=-O3 -m64 --gpu-architecture compute_61
LDFLAGS = -L/usr/local/depot/cuda-8.0/lib64/ -lcudart
LIBS = GL glut cudart
LDLIBS  := $(addprefix -l, $(LIBS))
NVCC=nvcc
OBJS=$(OBJDIR)/benchmark.o $(OBJDIR)/coding_loop.o $(OBJDIR)/cycletimer.o $(OBJDIR)/galois.o \
	$(OBJDIR)/matrix.o $(OBJDIR)/reed_solomon.o $(OBJDIR)/cuda_encoder.o

.PHONY: dirs clean

cuda: dirs $(OBJS)
	$(CXX) $(CXXFLAGS) $(OMP) -o benchmark $(OBJS) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	$(CXX) $< $(CXXFLAGS) $(OMP) -c -o $@

$(OBJDIR)/%.o: %.cu
	$(NVCC) $< $(NVCCFLAGS) -c -o $@

dirs:
	mkdir -p $(OBJDIR)/

clean:
	rm -rf $(OBJDIR)

