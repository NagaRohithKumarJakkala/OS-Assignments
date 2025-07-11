// #define TESTING
// above line is for testing
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

extern bool *row;
extern bool *col;
extern bool *grid;

// structs for thread input

extern int **sudoku;
extern int c;
extern bool *waiting;

extern std::vector<
    std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoreqenter;
extern std::vector<
    std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoenter;
extern std::vector<
    std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoexit;
extern std::vector<std::vector<
    std::pair<std::chrono::time_point<std::chrono::system_clock>, int>>>
    timestograb;
extern std::vector<std::vector<
    std::pair<std::chrono::time_point<std::chrono::system_clock>, int>>>
    timestocomplete;
extern std::vector<std::chrono::duration<double, std::micro>> timeToEnterCs;
extern std::vector<std::chrono::duration<double, std::micro>> timeToExitCs;

class TASLock {
  std::atomic_bool lock_flag = false;

public:
  void lock() {
    while (lock_flag.exchange(true))
      ;
  }
  void unlock() { lock_flag.store(false); }
};

class CASLock {
  std::atomic<bool> lock_flag = false;

public:
  void lock() {
    bool expected = false;
    while (!lock_flag.compare_exchange_strong(expected, true))
      expected = false;
  }
  void unlock() { lock_flag.store(false); }
};

class bCASLock {
  std::atomic<bool> lock_flag = false;

public:
  void lock(int i) {
    bool expected = false;
    while (waiting[i] && lock_flag.compare_exchange_strong(expected, true))
      ;
    waiting[i] = false;
  }
  void unlock(int i, int n) {
    int j = (i + 1) % n;
    while ((j != i) && !waiting[j])
      j = (j + 1) % n;
    if (j == i)
      lock_flag.store(false);
    else
      waiting[j] = false;
  }
};

extern TASLock Tlock;
extern CASLock Clock;
extern bCASLock Block;

void execTAS(int k, int n, int taskInc);
void execCAS(int k, int n, int taskInc);
void execBoundedCAS(int k, int n, int taskInc);
void execSequential(int n);

void rowcheck(int rowno, int n);
void colcheck(int colno, int n);
void gridcheck(int gridno, int n);

void TASselectCnt(int threadno, int n, int taskInc);
void CASselectCnt(int threadno, int n, int taskInc);
void bCASselectCnt(int threadno, int k, int n, int taskInc);
void selectGrid(int n, int taskInc);
void selectRow(int n, int taskInc);
void selectCol(int n, int taskInc);

void printvalids(int n);
void processTimes(
    std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>,
                          std::pair<int, std::pair<int, int>>>> &times,
    int k);
bool *row;
bool *col;
bool *grid;

// structs for thread input

int **sudoku = nullptr;
TASLock Tlock;
CASLock Clock;
bCASLock Block;
int c = 0;
bool *waiting = nullptr;

std::vector<std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoreqenter;
std::vector<std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoenter;
std::vector<std::vector<std::chrono::time_point<std::chrono::system_clock>>>
    timestoexit;
std::vector<std::vector<
    std::pair<std::chrono::time_point<std::chrono::system_clock>, int>>>
    timestograb;
std::vector<std::vector<
    std::pair<std::chrono::time_point<std::chrono::system_clock>, int>>>
    timestocomplete;
std::vector<std::chrono::duration<double, std::micro>> timeToEnterCs;
std::vector<std::chrono::duration<double, std::micro>> timeToExitCs;

int main(int argc, char *argv[]) {
  // taking input from file as stream
#ifdef TESTING
  std::string filename(argv[1]);
  std::cout << filename << "\n";
#else
  std::string filename("inp.txt");
#endif
#ifdef TESTING
  std::string arg2(argv[2]);
  std::string timefile = "tests/times/" + arg2 + ".txt";
#else
  std::string timefile = "times.txt";
#endif

  std::ifstream inp(filename);
  int k, n, taskInc;
  inp >> k >> n >> taskInc;

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

  // resizing the global variables
  row = new bool[n];
  col = new bool[n];
  grid = new bool[n];
  waiting = new bool[k];
  std::fill(waiting, waiting + k, false);

#ifdef TESTING
  std::string argv2(argv[2]);
  std::string outputfile = "tests/outputs/" + argv2 + ".txt";
#else
  std::string outputfile("output.txt");
#endif

  std::ofstream output(outputfile);
  std::ofstream timings(timefile, std::ios::app);

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

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    timings << elapsed_mseconds.count() << "\n";
  }

  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "Tas:" << "\n";
  {
    timestoreqenter.resize(k);
    timestoenter.resize(k);
    timestoexit.resize(k);
    timestograb.resize(k);
    timestocomplete.resize(k);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execTAS(k, n, taskInc);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>,
                          std::pair<int, std::pair<int, int>>>>
        times;
    std::chrono::duration<double, std::micro> avgEnter;
    for (auto i : timeToEnterCs) {
      avgEnter += i;
    }
    avgEnter /= timeToEnterCs.size();
    std::chrono::duration<double, std::micro> avgExit;
    for (auto i : timeToExitCs) {
      avgExit += i;
    }
    avgExit /= timeToExitCs.size();
    std::chrono::duration<double, std::micro> worstEnter =
        *std::max_element(timeToEnterCs.begin(), timeToEnterCs.end());
    std::chrono::duration<double, std::micro> worstExit =
        *std::max_element(timeToExitCs.begin(), timeToExitCs.end());
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";

#ifndef TESTING
    processTimes(times, k);
    for (auto i : times) {
      std::time_t temptime = std::chrono::system_clock::to_time_t(i.first);
      std::tm local_tm = *std::localtime(&temptime);
      if (i.second.first == 0) {
        output << "Thread " << i.second.second.first
               << " requests to enter CS1 at " << local_tm.tm_hour << ":"
               << local_tm.tm_min << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 1) {
        output << "Thread " << i.second.second.first << " enters CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 2) {
        output << "Thread " << i.second.second.first << " leaves CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";

      } else if (i.second.first == 3) {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first << " grabs " << rcgs << " "
               << rcgno << " " << local_tm.tm_hour << ":" << local_tm.tm_min
               << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        bool validity;
        if (rcg == 0) {
          validity = row[rcgno - 1];
        } else if (rcg == 1) {
          validity = col[rcgno - 1];
        } else {
          validity = grid[rcgno - 1];
        }
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first
               << " completes checking of " << rcgs << " " << rcgno << " "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs and finds it as "
               << (validity ? "valid" : "invalid") << "\n";
      }
    }
#endif
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    output << "Average time taken by a thread to enter the CS is "
           << avgEnter.count() << " microseconds" << "\n";
    output << "Average time taken by a thread to exit the CS is "
           << avgExit.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to enter the CS is "
           << worstEnter.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to exit the CS is "
           << worstExit.count() << " microseconds" << "\n";
    timings << avgEnter.count() << " " << avgExit.count() << " "
            << worstEnter.count() << " " << worstExit.count() << " "
            << elapsed_mseconds.count() << "\n";
  }
  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "Cas:" << "\n";
  output << "Cas:" << "\n";
  {
    timestoreqenter.clear();
    timestoenter.clear();
    timestoexit.clear();
    timestograb.clear();
    timestocomplete.clear();
    timestoreqenter.resize(k);
    timestoenter.resize(k);
    timestoexit.resize(k);
    timestograb.resize(k);
    timestocomplete.resize(k);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execCAS(k, n, taskInc);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    std::chrono::duration<double, std::micro> avgEnter;
    for (auto i : timeToEnterCs) {
      avgEnter += i;
    }
    avgEnter /= timeToEnterCs.size();
    std::chrono::duration<double, std::micro> avgExit;
    for (auto i : timeToExitCs) {
      avgExit += i;
    }
    avgExit /= timeToExitCs.size();
    std::chrono::duration<double, std::micro> worstEnter =
        *std::max_element(timeToEnterCs.begin(), timeToEnterCs.end());
    std::chrono::duration<double, std::micro> worstExit =
        *std::max_element(timeToExitCs.begin(), timeToExitCs.end());

    std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>,
                          std::pair<int, std::pair<int, int>>>>
        times;
#ifndef TESTING
    processTimes(times, k);

    for (auto i : times) {
      std::time_t temptime = std::chrono::system_clock::to_time_t(i.first);
      std::tm local_tm = *std::localtime(&temptime);
      if (i.second.first == 0) {
        output << "Thread " << i.second.second.first
               << " requests to enter CS1 at " << local_tm.tm_hour << ":"
               << local_tm.tm_min << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 1) {
        output << "Thread " << i.second.second.first << " enters CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 2) {
        output << "Thread " << i.second.second.first << " leaves CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";

      } else if (i.second.first == 3) {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first << " grabs " << rcgs << " "
               << rcgno << " " << local_tm.tm_hour << ":" << local_tm.tm_min
               << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        bool validity;
        if (rcg == 0) {
          validity = row[rcgno - 1];
        } else if (rcg == 1) {
          validity = col[rcgno - 1];
        } else {
          validity = grid[rcgno - 1];
        }
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first
               << " completes checking of " << rcgs << " " << rcgno << " "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs and finds it as "
               << (validity ? "valid" : "invalid") << "\n";
      }
    }
#endif
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    output << "Average time taken by a thread to enter the CS is "
           << avgEnter.count() << " microseconds" << "\n";
    output << "Average time taken by a thread to exit the CS is "
           << avgExit.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to enter the CS is "
           << worstEnter.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to exit the CS is "
           << worstExit.count() << " microseconds" << "\n";
    timings << avgEnter.count() << " " << avgExit.count() << " "
            << worstEnter.count() << " " << worstExit.count() << " "
            << elapsed_mseconds.count() << "\n";
  }
  std::fill(row, row + n, false);
  std::fill(col, col + n, false);
  std::fill(grid, grid + n, false);

  output << "bounded Cas:" << "\n";
  output << "bounded Cas:" << "\n";
  {
    timestoreqenter.clear();
    timestoenter.clear();
    timestoexit.clear();
    timestograb.clear();
    timestocomplete.clear();
    timestoreqenter.resize(k);
    timestoenter.resize(k);
    timestoexit.resize(k);
    timestograb.resize(k);
    timestocomplete.resize(k);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    execBoundedCAS(k, n, taskInc);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::micro> elapsed_mseconds = end - start;

    bool issudokuvalid = true;
    for (int i = 0; i < n; i++) {
      if (!row[i] || !col[i] || !grid[i]) {
        issudokuvalid = false;
        break;
      }
    }
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    std::chrono::duration<double, std::micro> avgEnter;
    for (auto i : timeToEnterCs) {
      avgEnter += i;
    }
    avgEnter /= timeToEnterCs.size();
    std::chrono::duration<double, std::micro> avgExit;
    for (auto i : timeToExitCs) {
      avgExit += i;
    }
    avgExit /= timeToExitCs.size();
    std::chrono::duration<double, std::micro> worstEnter =
        *std::max_element(timeToEnterCs.begin(), timeToEnterCs.end());
    std::chrono::duration<double, std::micro> worstExit =
        *std::max_element(timeToExitCs.begin(), timeToExitCs.end());

    std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>,
                          std::pair<int, std::pair<int, int>>>>
        times;
#ifndef TESTING
    processTimes(times, k);

    for (auto i : times) {
      std::time_t temptime = std::chrono::system_clock::to_time_t(i.first);
      std::tm local_tm = *std::localtime(&temptime);
      if (i.second.first == 0) {
        output << "Thread " << i.second.second.first
               << " requests to enter CS1 at " << local_tm.tm_hour << ":"
               << local_tm.tm_min << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 1) {
        output << "Thread " << i.second.second.first << " enters CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";
      } else if (i.second.first == 2) {
        output << "Thread " << i.second.second.first << " leaves CS1 at "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs" << "\n";

      } else if (i.second.first == 3) {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first << " grabs " << rcgs << " "
               << rcgno << " " << local_tm.tm_hour << ":" << local_tm.tm_min
               << ":" << local_tm.tm_sec << " hrs" << "\n";
      } else {
        int rcg = (i.second.second.second - 1) / n;
        int rcgno = i.second.second.second - rcg * n;
        std::string rcgs;
        bool validity;
        if (rcg == 0) {
          validity = row[rcgno - 1];
        } else if (rcg == 1) {
          validity = col[rcgno - 1];
        } else {
          validity = grid[rcgno - 1];
        }
        if (rcg == 0) {
          rcgs = "row";
        } else if (rcg == 1) {
          rcgs = "col";
        } else {
          rcgs = "grid";
        }
        output << "Thread " << i.second.second.first
               << " completes checking of " << rcgs << " " << rcgno << " "
               << local_tm.tm_hour << ":" << local_tm.tm_min << ":"
               << local_tm.tm_sec << " hrs and finds it as "
               << (validity ? "valid" : "invalid") << "\n";
      }
    }
#endif
    output << "Sudoku is " << (issudokuvalid ? "valid" : "invalid") << "\n";

    output << "Total time taken is " << elapsed_mseconds.count() << "\n";
    output << "Average time taken by a thread to enter the CS is "
           << avgEnter.count() << " microseconds" << "\n";
    output << "Average time taken by a thread to exit the CS is "
           << avgExit.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to enter the CS is "
           << worstEnter.count() << " microseconds" << "\n";
    output << "Worst-case time taken by a thread to exit the CS is "
           << worstExit.count() << " microseconds" << "\n";

    timings << avgEnter.count() << " " << avgExit.count() << " "
            << worstEnter.count() << " " << worstExit.count() << " "
            << elapsed_mseconds.count() << "\n";
  }

  output.close();
  timings.close();

  // clean up
  delete[] row;
  delete[] col;
  delete[] grid;
  delete[] waiting;

  for (int i = 0; i < n; i++) {
    delete[] sudoku[i];
  }
  delete[] sudoku;
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
void TASselectCnt(int threadno, int n, int taskInc) {
  int cnt = 0;
  while (cnt < 3 * n) {
    auto reqenter = std::chrono::system_clock::now();
    timestoreqenter[threadno].push_back(reqenter);
    Tlock.lock();
    auto entertime = std::chrono::system_clock::now();
    timestoenter[threadno].push_back(entertime);
    timeToEnterCs.push_back(entertime - reqenter);
    cnt = c;
    c += taskInc;
    auto exittime = std::chrono::system_clock::now();
    timestoexit[threadno].push_back(exittime);
    timeToExitCs.push_back(exittime - entertime);
    Tlock.unlock();
    for (int i = cnt; (i < (cnt + taskInc)); i++) {
      if (i >= 0 && i < n) {
        timestograb[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
        rowcheck(i, n);
        timestocomplete[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
      } else if (i >= n && i < 2 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        colcheck(i - n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else if (i >= 2 * n && i < 3 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        gridcheck(i - 2 * n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else
        break;
    }
  }
}
void CASselectCnt(int threadno, int n, int taskInc) {

  int cnt = 0;
  while (cnt < 3 * n) {
    auto reqenter = std::chrono::system_clock::now();
    timestoreqenter[threadno].push_back(reqenter);
    Clock.lock();
    auto entertime = std::chrono::system_clock::now();
    timestoenter[threadno].push_back(entertime);
    timeToEnterCs.push_back(entertime - reqenter);
    cnt = c;
    c += taskInc;
    auto exittime = std::chrono::system_clock::now();
    timestoexit[threadno].push_back(exittime);
    timeToExitCs.push_back(exittime - entertime);
    Clock.unlock();
    for (int i = cnt; (i < (cnt + taskInc)); i++) {
      if (i >= 0 && i < n) {
        timestograb[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
        rowcheck(i, n);
        timestocomplete[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
      } else if (i >= n && i < 2 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        colcheck(i - n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else if (i >= 2 * n && i < 3 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        gridcheck(i - 2 * n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else
        break;
    }
  }
}
void bCASselectCnt(int threadno, int k, int n, int taskInc) {

  int cnt = 0;
  while (cnt < 3 * n) {
    auto reqenter = std::chrono::system_clock::now();
    timestoreqenter[threadno].push_back(reqenter);
    Block.lock(threadno);
    auto entertime = std::chrono::system_clock::now();
    timestoenter[threadno].push_back(entertime);
    timeToEnterCs.push_back(entertime - reqenter);
    cnt = c;
    c += taskInc;
    auto exittime = std::chrono::system_clock::now();
    timestoexit[threadno].push_back(exittime);
    timeToExitCs.push_back(exittime - entertime);
    Block.unlock(threadno, k);
    for (int i = cnt; (i < (cnt + taskInc)); i++) {
      if (i >= 0 && i < n) {
        timestograb[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
        rowcheck(i, n);
        timestocomplete[threadno].push_back(
            {std::chrono::system_clock::now(), i + 1});
      } else if (i >= n && i < 2 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        colcheck(i - n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else if (i >= 2 * n && i < 3 * n) {
        timestograb[threadno].emplace_back(std::chrono::system_clock::now(),
                                           i + 1);
        gridcheck(i - 2 * n, n);
        timestocomplete[threadno].emplace_back(std::chrono::system_clock::now(),
                                               i + 1);
      } else
        break;
    }
  }
}

void execTAS(int k, int n, int taskInc) {
  std::vector<std::thread> threads;
  timeToEnterCs.clear();
  timeToExitCs.clear();
  c = 0;
  for (int i = 0; i < k; ++i) {
    threads.emplace_back(TASselectCnt, i, n, taskInc);
  }

  for (auto &t : threads) {
    t.join();
  }
  threads.clear();
}

void execCAS(int k, int n, int taskInc) {
  std::vector<std::thread> threads;
  timeToEnterCs.clear();
  timeToExitCs.clear();
  c = 0;
  for (int i = 0; i < k; ++i) {
    threads.emplace_back(CASselectCnt, i, n, taskInc);
  }

  for (auto &t : threads) {
    t.join();
  }
  threads.clear();
}

void execBoundedCAS(int k, int n, int taskInc) {
  std::vector<std::thread> threads;
  timeToEnterCs.clear();
  timeToExitCs.clear();
  c = 0;
  for (int i = 0; i < k; ++i) {
    threads.emplace_back(bCASselectCnt, i, k, n, taskInc);
  }

  for (auto &t : threads) {
    t.join();
  }
  threads.clear();
}
void processTimes(
    std::vector<std::pair<std::chrono::time_point<std::chrono::system_clock>,
                          std::pair<int, std::pair<int, int>>>> &times,
    int k) {
  times.clear();

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < timestoenter[i].size(); j++) {
      times.push_back({timestoenter[i][j], {0, {i + 1, 0}}});
    }
  }
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < timestoenter[i].size(); j++) {
      times.push_back({timestoenter[i][j], {1, {i + 1, 0}}});
    }
  }

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < timestoexit[i].size(); j++) {
      times.push_back({timestoexit[i][j], {2, {i + 1, 0}}});
    }
  }

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < timestograb[i].size(); j++) {
      times.push_back(
          {timestograb[i][j].first, {3, {i + 1, timestograb[i][j].second}}});
    }
  }

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < timestocomplete[i].size(); j++) {
      times.push_back({timestocomplete[i][j].first,
                       {4, {i + 1, timestocomplete[i][j].second}}});
    }
  }
  sort(times.begin(), times.end());
}
