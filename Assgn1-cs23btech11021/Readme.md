# Programming Assignment-1
##use makefile to build, run or test and clean the outputs

- build: to build/compile the project to default file main.

  make build

- run: to compile and run the project with inp.txt.

  make run

- test: to test the input with data in the tests.
    
  **you have to create the tests using the codes in the tests/generateinputs
  and uncomment the #define TESTING in the Assign1Srccs23btech11021.cpp
  (if testcases not present)(see testcases section)**

  after the above steps, use the following command:

  make test

- clean: remove the executable main, output.txt and times.txt using this

  make clean

- testcases: creating testcases (into folder tests/inputs)
  
  cd tests/generateinputs
  make testcases

##OR

##you can manually compile the file.

the project contain only one cpp file Assign1Srccs23btech11021.cpp

you can compile using gcc with neccessary tags:

 g++ Assgn1Src-cs23btech11021.cpp -lpthread -o main
                                                ^
                                                |
                          you can replace this with your executable name

## creating graphs
- first run the testcases by following above instructions in test section
- and run the following commands:
  
  (go to generategraph directory)

  cd generategraph

  (create the virtual environment for python)

  python3 -m venv env

  pip install matplotlib

  source env/bin/activate

  python3 graph.py

 ** assuming having python3 and pip and ran the tests using the makefile
  
