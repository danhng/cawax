/**
 * @author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
 * Newcastle University - School of Computing Science
 */

#include <time.h>
#include "omapi.h"
#include <stdio.h>

#ifndef SIMULATED_DATASTRUCTS_H
#define SIMULATED_DATASTRUCTS_H

typedef double acc;
typedef unsigned long sample_th;
typedef unsigned long node_index;

enum dimension {X, Y, Z};

/**
 * Represent a sample obtained from the device
 */
struct Sample {
    acc x;
    acc y;
    acc z;
    OM_DATETIME time;
    sample_th order; // order as in time domain
};

struct Node {
    Sample sample;
    Sample *next;
    Sample *prev;
};

struct LinkedList {
    long count;
    Sample *head;
};
// operations on linked list

/**
 * Get the node pointer at the specified index in the list
 *  - Return a null pointer if the index arg exceeds the size of the list.
 */
Node *get(node_index index, LinkedList);

/**
 * Add a sample to the end a signal list
 *  - Update pre tail node
 *  - Create new node for the new sample
 *  - Link new node to the list
 */
LinkedList *add(Sample sample, LinkedList *list);

/**
 * Add a sample to the specified index of the list
 *  - Create the node for the new sample
 *  - Update the neighbor nodes as to the newly created node
 */
LinkedList *add(node_index index, Sample sample, LinkedList *list);

/**
 * Remove (unlink) a sample from the end of the signal list based on the time and the sample order
 *  - Update the node before the last node
 *  - Free the last node
 */
LinkedList *remove(LinkedList *list);

/**
 * Get the time domain series for a specific dimension from {start} (inclusive) to (inclusive) {end}
 *  - If {start} = 0 then we start from the beginning
 *  - If {end} = 0 then we end at the tail (count - 1)
 *
 */
acc *getSubSeries(dimension d, node_index start, node_index end, LinkedList *list);

/**
 * toString functions
 *  - Store the formatted string representation the struct to the buffer in {buf}
 */
int toString(char *buf, Sample *sample);
int toString(char *buf, Node *node);
int toString(char *buf, LinkedList *list);


#endif //SIMULATED_DATASTRUCTS_H
