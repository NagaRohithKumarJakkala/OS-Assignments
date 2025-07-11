BIN = main
SRC = Assgn1Src-cs23btech11021.cpp 

build:$(SRC)
	g++ $(SRC) -lpthread -o $(BIN)
$(BIN):$(SRC)
	g++ $(SRC) -lpthread -o $(BIN)
run: $(BIN)
	./$(BIN) inp.txt
test:
	python3 test.py
clean:
	rm -f $(BIN) output.txt times.txt
