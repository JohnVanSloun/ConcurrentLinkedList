#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"

//global line-number counter to be sync'ed.
int line_ctr = 0;

char* filename = NULL;

//initialization of mutex locks
pthread_mutex_t line_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t seq_lock = PTHREAD_MUTEX_INITIALIZER;
//at least two mutex locks should be used

//start of the threads
void* process_file(void *param)
{
	while (1) {
		// Get next line and increment line_ctr
		pthread_mutex_lock(&line_lock);
		int line_num = line_ctr;
		line_ctr++;
		pthread_mutex_unlock(&line_lock);

		char* line = read_line(filename, line_num);

		if (line == NULL) {
			break;
		}

		// Create new node
		node *new_node = create_node(line_num, line);

		// Insert node into list
		pthread_mutex_lock(&list_lock);
		insert(&head, new_node);
		pthread_mutex_unlock(&list_lock);
	}
}

int main(int argc, char* argv[])
{
	FILE* in_ptr;
	int threads;

	if (argc != 3) {
		printf("Incorrect arg count. Enter file name and thread count\n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	threads = atoi(argv[2]);
	if (threads < 1 || threads > 16) {
		printf("Incorrect number of threads. Enter between 1 and 16\n");
		exit(EXIT_FAILURE);
	}

	// TODO: create threads and join them, pthread_create & pthread_join

	pthread_t* tid = (pthread_t*) malloc(threads * sizeof(pthread_t)); // array to store tid's
	if(tid == NULL) {
		perror("Error in allocating memory for tid array using malloc()");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < threads; i++) {
		int retval = pthread_create(&tid[i], NULL, process_file, NULL);
		if(retval != 0) {
			perror("Error in creating a thread for process_file() function using pthread_create()");
			free(tid);
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < threads; i++) {
		int retval = pthread_join(tid[i], NULL);
		if(retval != 0) {
			perror("Error in waiting for a thread executing process_file() using pthread_join()");
			free(tid);
			exit(EXIT_FAILURE);
		}
	}
	free(tid);
	traversal(head);
	deallocate_linked_list();

	return 0;
}
