.SILENT: all
.SILENT: debug
.SILENT: clean
.SILENT: clear

all:
	clang -std=c99 -O3 -Wextra -Wall -Wno-unused -MMD -c *.c
	clang -lm -o NeuralNetwork *.o
	make clean

debug:
	clang -v -std=c99 -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wextra -Wall -MMD -c *.c
	clang -lm -o NeuralNetwork *.o -fsanitize=address

runTrain:
	make
	./NeuralNetwork TrainXOR TrainedNetwork/NN.cfg $(attempt)

runPredict:
	make
	./NeuralNetwork PredictXOR TrainedNetwork/NeuralNetData_3layers_XOR_100.0.dnn $(v1) $(v2)

clean:
	rm -rf *[.do] main
	clear

clear:
	rm ./NeuralNetwork
	make clean
