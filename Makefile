CC = g++ -std=c++17
OMP = -fopenmp
CFLAGS = -c $(OMP)

OPENCV = -I  /usr/local/include/opencv4
LIBS = -L $(shell ls /usr/local/lib/*libopencv*.so -1)

OBJ = csv.o helper.o fft_utils.o fourier_sharpness_base.o fft.o

INC = lib/include/
SRC = lib/src/
BIN = bin/

MAIN = src/
OUT = output/descriptor/

all: ./main

main: $(OBJ) $(MAIN)main.cpp
	$(info Building main)
	@$(CC) -o main $(OBJ) $(MAIN)main.cpp $(OMP) $(OPENCV) $(LIBS)

fft.o: $(INC)fft.hpp $(SRC)fft.cpp 
	$(info Building fft.o)
	@$(CC) $(INC)fft.hpp $(SRC)fft.cpp -w $(CFLAGS) $(OPENCV)

fourier_sharpness_base.o: $(INC)fourier_sharpness_base.hpp $(SRC)fourier_sharpness_base.cpp 
	$(info Building fourier_sharpness_base.o)
	@$(CC) $(INC)fourier_sharpness_base.hpp $(SRC)fourier_sharpness_base.cpp -w $(CFLAGS)  $(OPENCV)

fft_utils.o: $(INC)fft_utils.hpp $(SRC)fft_utils.cpp 
	$(info Building fft_utils.o)
	@$(CC) $(INC)fft_utils.hpp $(SRC)fft_utils.cpp -w $(CFLAGS) $(OPENCV)

helper.o: $(INC)helper.hpp $(SRC)helper.cpp 
	$(info Building helper.o)
	@$(CC) $(INC)helper.hpp $(SRC)helper.cpp -w $(CFLAGS) $(OPENCV)

csv.o: $(INC)csv.hpp $(SRC)csv.cpp 
	$(info Building csv.o)
	@$(CC) $(INC)csv.hpp $(SRC)csv.cpp  -w $(CFLAGS) $(OMP)

run:
	@./main

clean:
	rm main
	rm *.o
	rm $(INC)*.gch
	rm $(OUT)*.csv