#define HAVE_STRUCT_TIMESPEC
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_RAND_S
#define RAND_MAX
#include <stdio.h>
#include <semaphore.h>
#include <thread>
#include <iostream>
#include <random>

using namespace std;

mt19937 mersenne(static_cast<unsigned int>(time(0)));
int database[11] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
sem_t  semaphore;

void dataWriter(int threadNum) {
	int data;
	int id;
	int result;

	for (int i = 0; i < 100; i++) {
		sem_wait(&semaphore);

		data = mersenne() % 20;
		id = mersenne() % 11;
		result = database[id];
		database[id] = data;
		printf("Operation %d || Writer %d: reads old value %d \tand writes new value %d to the cell [%d]\n"
			, i, threadNum, result, data, id, result);

		sem_post(&semaphore);
		this_thread::sleep_for(chrono::seconds(threadNum));
	}
}

void dataReader(int threadNum) {
	int result;
	int id;

	for (int i = 0; i < 100; i++) {
		id = mersenne() % 11;
		result = database[id];
		printf("Operation %d // Reader %d: reads value %d \tfrom cell [%d]\n", i, threadNum, result, id);
		this_thread::sleep_for(chrono::seconds(threadNum));
	}
}

int main() {
	const int countReader = 100;
	const int countWriter = 100;

	sem_init(&semaphore, 0, 1);

	thread writers[countReader];
	thread readers[countReader];

	for (int i = 0; i < countReader; i++)
		readers[i] = thread(dataReader, i + 1);

	for (int i = 0; i < countWriter; i++)
		writers[i] = thread(dataWriter, i + 1);

	for (int i = 0; i < countWriter; ++i)
		writers[i].join();

	for (int i = 0; i < countReader; ++i)
		readers[i].join();

	sem_destroy(&semaphore);
	return 0;
}