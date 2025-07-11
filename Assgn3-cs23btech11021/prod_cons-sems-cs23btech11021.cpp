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
sem_t empty, full, mutex;

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
    sem_wait(&empty);
    sem_wait(&mutex);
    int item = i;
    buffer[fill] = item;
    fill = (fill + 1) % capacity;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout << i << " th item: " << item << " produced by the thread " << id
              << " at " << std::put_time(std::localtime(&time), "%F %T")
              << " into buffer location " << fill << "\n";
    sem_post(&mutex);
    sem_post(&full);
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
    sem_wait(&full);
    sem_wait(&mutex);
    int item = buffer[use];
    use = (use + 1) % capacity;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::cout << i << " th item: " << item << " read from buffer by the thread "
              << id << " at " << std::put_time(std::localtime(&time), "%F %T")
              << " from location " << use << "\n";
    sem_post(&mutex);
    sem_post(&empty);
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

  sem_init(&empty, 0, capacity);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

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

  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);

  std::ofstream times1("timings1.txt", std::ios::app);

  double total_time_prod = std::accumulate(
      times_for_producers.begin(), times_for_producers.end(), 0.0,
      [](double sum, const std::chrono::duration<double, std::micro> &dur) {
        return sum + dur.count();
      });

  times1 << total_time_prod / times_for_producers.size() << " ";

  times1.close();
  std::ofstream times2("timings2.txt", std::ios::app);
  double total_time_cons = std::accumulate(
      times_for_consumers.begin(), times_for_consumers.end(), 0.0,
      [](double sum, const std::chrono::duration<double, std::micro> &dur) {
        return sum + dur.count();
      });

  times2 << total_time_cons / times_for_consumers.size() << " ";
  times2.close();
}
