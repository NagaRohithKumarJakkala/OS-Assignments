build:
	g++ Assgn2Src-cs23btech11021.cpp -o  a.out
run:
	g++ Assgn2Src-cs23btech11021.cpp -o  a.out
	./a.out
testcases:
	
	g++ tests/generators/valid_exp1.cpp -o test
	./test
	g++ tests/generators/invalid_exp1.cpp -o test 
	./test
	g++ tests/generators/valid_exp2.cpp -o test
	./test
	g++ tests/generators/invalid_exp2.cpp -o test
	./test
	g++ tests/generators/valid_exp3.cpp -o test
	./test
	g++ tests/generators/invalid_exp3.cpp -o test
	./test
	rm -f test
test:
	python3 test.py
clean:
	rm -f a.out output.txt times.txt
cleantests:
	rm -f tests/inputs/exp1/valids/*
	rm -f tests/inputs/exp1/invalids/*
	rm -f tests/inputs/exp2/valids/*
	rm -f tests/inputs/exp2/invalids/*
	rm -f tests/inputs/exp3/valids/*
	rm -f tests/inputs/exp3/invalids/*
	rm -f tests/outputs/*
	rm -f tests/times/*
