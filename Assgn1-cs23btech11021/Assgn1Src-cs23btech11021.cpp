#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <string>

// below line is for testing
// #define TESTING

bool *row;
bool *col;
bool *grid;

int *c_row_t;
int *c_col_t;
int *c_grid_t;

int *m_row_t;
int *m_col_t;
int *m_grid_t;

// structs for thread input

struct pos {
  int reqno, n;
};

struct chunkargs {
  int start, end, n, no;
};

struct mixedargs {
  int no, k, n;
};

int **sudoku = nullptr;

void rowcheck(int rowno, int n);
void colcheck(int colno, int n);
void gridcheck(int gridno, int n);

void execSequential(int n);
void execChunk(int k, int n);
void execMixed(int k, int n);

void *cRowThreadFn(void *args);
void *cColThreadFn(void *args);
void *cGridThreadFn(void *args);

void *mThreadFn(void *args);

void printvalids(int n);

int main(int argc, char *argv[]) {
  // taking input from file as stream
#ifdef TESTING
  std::string filename(argv[1]);
  std::cout << filename << "\n";
#else
  std::string filename("inp.txt");
#endif

  std::ifstream inp(filename);
  int k, n;
  inp >> k >> n;

  sudoku = new int *[n];
  for (int i = 0; i < n; i++) {
    sudoku[i] = new int[n];
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      inp >> sudoku[i][j];
    }
  }

  inp.close();

  std::chrono::duration<double, std::micro> timings[3];

  // resizing the global variables
  row = new bool[n];
  col = new bool[n];
  grid = new bool[n];

  c_row_t = new int[n];
  c_col_t = new int[n];
  c_grid_t = new int[n];

  m_row_t = new int[n];
  m_col_t = new int[n];
  m_grid_t = new int[n];

#ifdef TESTING
  std::string argv2(argv[2]);
  std::string outputfile = "tests/outputs/" + argv2 + ".txt";
#else
  std::string outputfile("output.txt");
#endif

  std::ofstream output(outputfile);

  // sequential

  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "sequential:" << "\n";
  {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execSequential(n);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;

    timings[0] = elapsed_mseconds;

    for (int i = 0; i < n; i++) {
      output << "row " << i + 1 << " is " << (row[i] ? "valid" : "invalid")
             << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "col " << i + 1 << " is " << (col[i] ? "valid" : "invalid")
             << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "grid " << i + 1 << " is " << (grid[i] ? "valid" : "invalid")
             << "\n";
    }

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
  }

  // chunk

  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "chunk:" << "\n";

  {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execChunk(k, n);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;
    timings[1] = elapsed_mseconds;
    for (int i = 0; i < n; i++) {
      output << "Thread " << c_row_t[i] << " checks row " << i + 1 << " and is "
             << (row[i] ? "valid" : "invalid") << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "Thread " << c_col_t[i] << " checks col " << i + 1 << " and is "
             << (col[i] ? "valid" : "invalid") << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "Thread " << c_grid_t[i] << " checks grid " << i + 1
             << " and is " << (grid[i] ? "valid" : "invalid") << "\n";
    }

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
  }

  // mixed

  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "mixed:" << "\n";

  {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execMixed(k, n);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;
    timings[2] = elapsed_mseconds;

    for (int i = 0; i < n; i++) {
      output << "Thread " << m_row_t[i] << " checks row " << i + 1 << " and is "
             << (row[i] ? "valid" : "invalid") << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "Thread " << m_col_t[i] << " checks col " << i + 1 << " and is "
             << (col[i] ? "valid" : "invalid") << "\n";
    }

    for (int i = 0; i < n; i++) {
      output << "Thread " << m_grid_t[i] << " checks grid " << i + 1
             << " and is " << (grid[i] ? "valid" : "invalid") << "\n";
    }

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
  }

  output.close();
  // clean up

  delete[] row;
  delete[] col;
  delete[] grid;

  delete[] c_row_t;
  delete[] c_col_t;
  delete[] c_grid_t;

  delete[] m_row_t;
  delete[] m_col_t;
  delete[] m_grid_t;

  for (int i = 0; i < n; i++) {
    delete[] sudoku[i];
  }
  delete[] sudoku;

  // timings collection

#ifdef TESTING
  std::string arg2(argv[2]);
  std::string timefile = "tests/times/time" + arg2 + ".txt";
#else
  std::string timefile = "times.txt";
#endif
  std::ofstream times(timefile, std::ios::app);
  times << timings[0].count() << " " << timings[1].count() << " "
        << timings[2].count() << "\n";
  times.close();

  return 0;
}

void rowcheck(int rowno, int n) {
  bool counter[n + 1];
  // memset(counter, 0, sizeof(counter));
  std::fill(counter, counter + n + 1, false);

  bool valid = true;
  for (int i = 0; i < n; i++) {
    if ((sudoku[rowno][i] > n) || counter[sudoku[rowno][i]]) {
      valid = false;
      break;
    }
    counter[sudoku[rowno][i]] = true;
  }
  row[rowno] = valid;
}

void colcheck(int colno, int n) {
  bool counter[n + 1];
  // memset(counter, 0, sizeof(counter));
  std::fill(counter, counter + n + 1, false);
  bool valid = true;
  for (int i = 0; i < n; i++) {
    if ((sudoku[i][colno] > n) || counter[sudoku[i][colno]]) {
      valid = false;
      break;
    }
    counter[sudoku[i][colno]] = true;
  }
  col[colno] = valid;
}

void gridcheck(int gridno, int n) {
  bool counter[n + 1];
  // memset(counter, 0, sizeof(counter));
  std::fill(counter, counter + n + 1, false);
  bool valid = true;
  int rootn = (int)(sqrt(n) + 0.01);
  int rowstart = gridno - (gridno % rootn);
  int colstart = (gridno % rootn) * rootn;
  for (int i = 0; i * i < n; i++) {
    for (int j = 0; j * j < n; j++) {
      if ((sudoku[rowstart + i][colstart + j] > n) ||
          counter[sudoku[rowstart + i][colstart + j]]) {
        valid = false;
        break;
      }
      counter[sudoku[rowstart + i][colstart + j]] = true;
    }
  }
  grid[gridno] = valid;
}
void execChunk(int k, int n) {

  // thread creation for chunk
  int k1 = k / 3;
  int k2 = k / 3;
  int k3 = k / 3;
  if (k % 3 == 1) {
    k1++;
  } else if (k % 3 == 2) {
    k1++;
    k2++;
  }
  pthread_t trows[k1];
  pthread_t tcols[k2];
  pthread_t tgrids[k3];

  int trowsid[k1];
  int tcolsid[k1];
  int tgridsid[k1];

  int p1 = (n % k1 == 0) ? (n / k1) : (n / k1 + 1);
  int p2 = (n % k2 == 0) ? (n / k2) : (n / k2 + 1);
  int p3 = (n % k3 == 0) ? (n / k3) : (n / k3 + 1);
  chunkargs rArgs[k1];
  for (int i = 0; i < k1; i++) {
    rArgs[i] = {i * p1, (i + 1) * p1, n, i + 1};
    pthread_create(&trows[i], NULL, cRowThreadFn, &rArgs[i]);
  }
  chunkargs cArgs[k2];
  for (int i = 0; i < k2; i++) {
    cArgs[i] = {i * p2, (i + 1) * p2, n, k1 + i + 1};
    pthread_create(&tcols[i], NULL, cColThreadFn, &cArgs[i]);
  }
  chunkargs gArgs[k3];
  for (int i = 0; i < k3; i++) {
    gArgs[i] = {i * p3, (i + 1) * p3, n, k1 + k2 + i + 1};
    pthread_create(&tgrids[i], NULL, cGridThreadFn, &gArgs[i]);
  }
  // waiting for joining
  for (int i = 0; i < k1; i++) {
    pthread_join(trows[i], NULL);
  }
  for (int i = 0; i < k2; i++) {
    pthread_join(tcols[i], NULL);
  }
  for (int i = 0; i < k3; i++) {
    pthread_join(tgrids[i], NULL);
  }
}
void execMixed(int k, int n) {
  // thread creation for mixed
  pthread_t mthreads[k];
  mixedargs mArgs[k];
  for (int i = 0; i < k; i++) {
    mArgs[i] = {i, k, n};
    pthread_create(&mthreads[i], NULL, mThreadFn, &mArgs[i]);
  }
  // waiting for joining
  for (int i = 0; i < k; i++) {
    pthread_join(mthreads[i], NULL);
  }
}

void execSequential(int n) {
  for (int i = 0; i < n; i++) {
    rowcheck(i, n);
  }

  for (int i = 0; i < n; i++) {
    colcheck(i, n);
  }

  for (int i = 0; i < n; i++) {
    gridcheck(i, n);
  }
}
void *cRowThreadFn(void *args) {
  chunkargs *arg = (chunkargs *)args;
  for (int i = arg->start; i < arg->end && i < arg->n; i++) {
    c_row_t[i] = arg->no;
    rowcheck(i, arg->n);
  }
  return NULL;
}
void *cColThreadFn(void *args) {
  chunkargs *arg = (chunkargs *)args;
  for (int i = arg->start; i < arg->end && i < arg->n; i++) {
    c_col_t[i] = arg->no;
    colcheck(i, arg->n);
  }
  return NULL;
}
void *cGridThreadFn(void *args) {
  chunkargs *arg = (chunkargs *)args;
  for (int i = arg->start; i < arg->end && i < arg->n; i++) {
    c_grid_t[i] = arg->no;
    gridcheck(i, arg->n);
  }
  return NULL;
}

void *mThreadFn(void *args) {
  mixedargs *arg = (mixedargs *)args;
  for (int i = arg->no; i < arg->n; i += arg->k) {
    m_row_t[i] = arg->no;
    rowcheck(i, arg->n);
  }
  for (int i = arg->no; i < arg->n; i += arg->k) {
    m_col_t[i] = arg->no;
    colcheck(i, arg->n);
  }
  for (int i = arg->no; i < arg->n; i += arg->k) {
    m_grid_t[i] = arg->no;
    gridcheck(i, arg->n);
  }
  return NULL;
}
