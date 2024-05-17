#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "linked_list.h"

// initialize head
node *head = NULL;

// sequence counter
int seq_ctr = 0;

// read line line_no of the file fname. returns NULL if the line does not exist in the file or if an error occured when reading the file.
char* read_line(char* fname, int line_no) {
	// TODO
	FILE* file;
	char* line = (char*) malloc(LINE_SIZE * sizeof(char));
	file = fopen(fname, "r");

	if(file == NULL) {
		fprintf(stderr, "Error opening file %s using fopen()", fname);
		return NULL;
	}

	for(int i = 0; i < line_no; i++) { // Skip over all lines until we reaach the line we want to read
		if(fgets(line, LINE_SIZE, file) == NULL) { // Was there an error in reading the next line?

			if(feof(file)) { // If end of file reached, close file and return NULL to indicate line does not exist
				fclose(file);
				return NULL;
			}
			else { // Error in reading using fgets() due to error other than line not existing
				fprintf(stderr, "Error in reading file %s using fgets()", fname);
				fclose(file);
				return NULL;
			}
		}
	}

	// Read line line_no
	if(fgets(line, LINE_SIZE, file) != NULL) {
		fclose(file);
		return line;
	}
	else {
		if(feof(file)) { // line does not exist
			fclose(file);
			return NULL;
		}
		else { // error in reading using fgets()
			fprintf(stderr, "Error reading line number %d using fgets()", line_no);
			fclose(file);
			return NULL;
		}
	}
} 

//traverse the linked list
void traversal(node *head)
{
	if (head == NULL) {
		return;
	}

	node *n = head;

	while (n != NULL) {
		printf("%d, %d, %s", n->seq_no, n->line_no, n->content);
		n = n->next;
	}
}
// insert the node into the linked list. when inserted, maintains sorted property of linked list sorted in ascending order of line numbers
void insert(node **phead, node *newnode)
{
	// Checking for valid node or empty linked list
	if (newnode == NULL) {
		return;
	}

	newnode->seq_no = seq_ctr;
	seq_ctr++;

	if (*phead == NULL) {
		*phead = newnode;
		return;
	}

	node *n = *phead;

	if(newnode->line_no < n->line_no) { // insert new node at the start of the linked list
		newnode->next = n;
		*phead = newnode;
		return;
	}

	// Iterate over linked list until reaching the last node then add last node
	while (n->next != NULL) {
		if ((newnode->line_no > n->line_no) && (newnode->line_no < n->next->line_no)) {
			node *temp = n->next;
			n->next = newnode;
			newnode->next = temp;
			return;
		} else {
			n = n->next;
		}
	}

	n->next = newnode;
}

//create a new node structure
node* create_node(int line_no, char *line)
{
	// Allocate memory for the node
	node *n = malloc(sizeof(node));

	// Populate the fields of the node with the corresponding values
	n->seq_no = seq_ctr;
	n->line_no = line_no;
	n->content = line;
	n->next = NULL;

	return n;
}

// deallocate the linked list whose head node corresponds to global variable node* head
void deallocate_linked_list() {
	node* curr_node = head;
	while(curr_node != NULL) {
		node* next_node = curr_node->next;
		free(curr_node->content);
		free(curr_node);
		curr_node = next_node;
	}
}
