#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <pthread.h>
#include <random>
#include <semaphore.h>
#include <unistd.h>

int capacity, np, nc, cntp, cntc;
double mp, mc;

int *buffer;

std::atomic<int> use = 0, fill = 0;
std::atomic<int> count = 0;

pthread_mutex_t lock;

std::vector<std::chrono::duration<double, std::micro>> times_for_producers;
std::vector<std::chrono::duration<double, std::micro>> times_for_consumers;

void *producer(void *args) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::exponential_distribution<double> exp_dist(1.0 / (mp * 1000));
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  int id = *((int *)args);
  delete (int *)args;
  for (int i = 0; i < cntp; i++) {

    pthread_mutex_lock(&lock);
    bool running = false;
    if (count == capacity) {
      running = true;
      pthread_mutex_unlock(&lock);
    }

    while (count == capacity)
      ;

    if (running)
      pthread_mutex_lock(&lock);

    int item = i;
    buffer[fill] = item;
    fill = (fill + 1) % capacity;
    count++;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout << i << " th item: " << item << " produced by the thread " << id
              << " at " << std::put_time(std::localtime(&time), "%F %T")
              << " into buffer location " << fill << "\n";

    pthread_mutex_unlock(&lock);

    int sleep_microseconds = static_cast<int>(exp_dist(gen));
    usleep(sleep_microseconds);
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::micro> elapsed_seconds = end - start;
  times_for_producers[id] = elapsed_seconds;
  return NULL;
}

void *consumer(void *args) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::exponential_distribution<double> exp_dist(1.0 / (mc * 1000));

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  int id = *((int *)args);
  delete (int *)args;

  for (int i = 0; i < cntc; i++) {

    pthread_mutex_lock(&lock);
    bool running = false;
    if (count == 0) {
      running = true;
      pthread_mutex_unlock(&lock);
    }

    while (count == 0)
      ;

    if (running)
      pthread_mutex_lock(&lock);

    int item = buffer[use];
    use = (use + 1) % capacity;
    count--;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout << i << " th item: " << item << " read from buffer by the thread "
              << id << " at " << std::put_time(std::localtime(&time), "%F %T")
              << " from location " << use << "\n";

    pthread_mutex_unlock(&lock);

    int sleep_microseconds = static_cast<int>(exp_dist(gen));
    usleep(sleep_microseconds);
  }

  end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::micro> elapsed_seconds = end - start;
  times_for_consumers[id] = elapsed_seconds;
  return NULL;
}

int main() {
  std::ifstream inp_params("inp-params.txt");

  inp_params >> capacity;
  inp_params >> np;
  inp_params >> nc;
  inp_params >> cntp;
  inp_params >> cntc;
  inp_params >> mp;
  inp_params >> mc;
  inp_params.close();

  buffer = new int[capacity];
  times_for_producers.resize(np);
  times_for_consumers.resize(nc);

  pthread_t producer_threads[np];
  pthread_t consumer_threads[nc];

  for (int i = 0; i < np; i++) {
    int *temp = new int(i);
    pthread_create(&producer_threads[i], NULL, producer, temp);
  }
  for (int i = 0; i < nc; i++) {
    int *temp = new int(i);
    pthread_create(&consumer_threads[i], NULL, consumer, temp);
  }

  for (int i = 0; i < np; i++) {
    pthread_join(producer_threads[i], NULL);
  }
  for (int i = 0; i < nc; i++) {
    pthread_join(consumer_threads[i], NULL);
  }

  delete buffer;

  std::ofstream times1("timings3.txt", std::ios::app);

  double total_time_prod = std::accumulate(
      times_for_producers.begin(), times_for_producers.end(), 0.0,
      [](double sum, const std::chrono::duration<double, std::micro> &dur) {
        return sum + dur.count();
      });

  times1 << total_time_prod / times_for_producers.size() << " ";
  times1.close();
  std::ofstream times2("timings4.txt", std::ios::app);

  double total_time_cons = std::accumulate(
      times_for_consumers.begin(), times_for_consumers.end(), 0.0,
      [](double sum, const std::chrono::duration<double, std::micro> &dur) {
        return sum + dur.count();
      });

  times2 << total_time_cons / times_for_consumers.size() << " ";
  times2.close();
}
