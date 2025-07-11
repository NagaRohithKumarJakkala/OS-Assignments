## usage rules:

- build the program (executable a.out)

  ```
  make build
  ```

- build and run the program (executable a.out)

  ```
  make run
  ```

- clean the trash in main folder
  
  ```
  make clean
  ```

*** don't run below parts if not necessary it creates 13 GB of testcases and requires lot of time
*** if you still wnat to run the below commands uncomment the first line of source file Assgn2-cs23btech11021.cpp 
*** and comment it if not for testing with testcases.

- produce testcases

  ```
  make testcases
  ```

- run the testcases and collect the data in tests (make testcases required before this)

  ```
  make test 
  ```

- clean the tests folder trash
  
  ```
  make cleantests
  ```

- produce tables for latex from the testcases data(make testcases required for this)

  ```
  cd tables
  g++ latextables.cpp
  ./a.out
  ```

- produce graph from the testcases data(produced tables required)

  ```
  cd graphs
  python3 -m venv env
  source env/bin/activate
  pip install matplotlib
  pip install pandas
  python3 graph.py
  ```
