CC = g++ -std=c++17
OMP = -fopenmp
CFLAGS = -c $(OMP)

OPENCV = -I  /usr/local/include/opencv4
LIBS = -L $(shell ls /usr/local/lib/*libopencv*.so -1)

OBJ = csv.o helper.o fft_utils.o sample_function_base.o statistical_analysis.o sample_function.o

INC = lib/include/
SRC = lib/src/
BIN = bin/

MAIN = src/
OUT = output/descriptor/

all: ./main

main: $(OBJ) $(MAIN)main.cpp
	$(info Building main)
	@$(CC) -o main $(OBJ) $(MAIN)main.cpp $(OMP) $(OPENCV) $(LIBS)

sample_function.o: $(INC)sample_function.hpp $(SRC)sample_function.cpp 
	$(info Building sample_function.o)
	@$(CC) $(INC)sample_function.hpp $(SRC)sample_function.cpp -w $(CFLAGS) $(OPENCV)

statistical_analysis.o: $(INC)statistical_analysis.hpp $(SRC)statistical_analysis.cpp 
	$(info Building statistical_analysis.o)
	@$(CC) $(INC)statistical_analysis.hpp $(SRC)statistical_analysis.cpp -w $(CFLAGS)

sample_function_base.o: $(INC)sample_function_base.hpp $(SRC)sample_function_base.cpp 
	$(info Building sample_function_base.o)
	@$(CC) $(INC)sample_function_base.hpp $(SRC)sample_function_base.cpp -w $(CFLAGS)  $(OPENCV)

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