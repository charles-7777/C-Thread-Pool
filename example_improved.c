/* 
 * Improved example for C-Thread-Pool
 * 
 * This example demonstrates:
 * - Thread pool creation and management
 * - Task distribution across threads
 * - Work load balancing
 * - Thread identification using friendly IDs
 * 
 */

#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include "thpool.h"

// Global counter for demonstration
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
int total_tasks_completed = 0;

void task(void *arg){
	int task_num = (int)(uintptr_t)arg;
	pthread_t thread_id = pthread_self();
	
	// Simulate some work
	usleep(10000); // 10ms delay to make execution more observable
	
	pthread_mutex_lock(&counter_mutex);
	total_tasks_completed++;
	printf("Task %d completed by thread #%u (Total completed: %d)\n", 
	       task_num, (unsigned int)thread_id, total_tasks_completed);
	pthread_mutex_unlock(&counter_mutex);
}

void heavy_task(void *arg){
	int task_num = (int)(uintptr_t)arg;
	pthread_t thread_id = pthread_self();
	
	// Simulate heavy work
	usleep(100000); // 100ms delay
	
	pthread_mutex_lock(&counter_mutex);
	total_tasks_completed++;
	printf("Heavy task %d completed by thread #%u (Total completed: %d)\n", 
	       task_num, (unsigned int)thread_id, total_tasks_completed);
	pthread_mutex_unlock(&counter_mutex);
}

int main(){
	
	puts("=== C-Thread-Pool Improved Example ===\n");
	
	// Test 1: Basic thread pool with light tasks
	puts("--- Test 1: Light Tasks (40 tasks, 4 threads) ---");
	threadpool thpool1 = thpool_init(4);
	
	total_tasks_completed = 0;
	for (int i = 0; i < 40; i++){
		thpool_add_work(thpool1, task, (void*)(uintptr_t)i);
	}
	
	printf("Added 40 light tasks to threadpool\n");
	printf("Working threads: %d\n", thpool_num_threads_working(thpool1));
	
	thpool_wait(thpool1);
	printf("All light tasks completed. Total: %d\n\n", total_tasks_completed);
	
	thpool_destroy(thpool1);
	
	// Test 2: Thread pool with heavy tasks
	puts("--- Test 2: Heavy Tasks (20 tasks, 4 threads) ---");
	threadpool thpool2 = thpool_init(4);
	
	total_tasks_completed = 0;
	for (int i = 0; i < 20; i++){
		thpool_add_work(thpool2, heavy_task, (void*)(uintptr_t)i);
	}
	
	printf("Added 20 heavy tasks to threadpool\n");
	printf("Working threads: %d\n", thpool_num_threads_working(thpool2));
	
	thpool_wait(thpool2);
	printf("All heavy tasks completed. Total: %d\n\n", total_tasks_completed);
	
	thpool_destroy(thpool2);
	
	// Test 3: Different thread pool sizes
	puts("--- Test 3: Thread Pool Size Comparison ---");
	int pool_sizes[] = {1, 2, 4, 8};
	int num_pools = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
	
	for (int i = 0; i < num_pools; i++){
		int pool_size = pool_sizes[i];
		printf("Testing with %d thread(s)... ", pool_size);
		
		threadpool thpool = thpool_init(pool_size);
		total_tasks_completed = 0;
		
		for (int j = 0; j < 16; j++){
			thpool_add_work(thpool, task, (void*)(uintptr_t)j);
		}
		
		thpool_wait(thpool);
		printf("Completed %d tasks\n", total_tasks_completed);
		
		thpool_destroy(thpool);
	}
	
	puts("\n=== All tests completed successfully ===");
	
	return 0;
}
