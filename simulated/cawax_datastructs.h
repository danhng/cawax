/**
 * @author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
 * Newcastle University - School of Computing Science
 */

#include <time.h>
#include "omapi.h"
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

#ifndef SIMULATED_DATASTRUCTS_H
#define SIMULATED_DATASTRUCTS_H

typedef double acc;
typedef unsigned long sample_th;
typedef unsigned long node_index;

typedef enum {X, Y, Z} dimension;

/**
 * Represent a sample obtained from the device
 */
typedef struct sample {
    acc x;
    acc y;
    acc z;
    OM_DATETIME time;
    sample_th order; // order as in time domain
} Sample;

typedef struct node {
    Sample sample;
    struct node *next;
    struct node *prev;
} Node;

typedef struct linkedList {
    long count;
    Node *head;
} LinkedList;
// operations on linked list

/*
Make a new Node for a sample 
Return: Pointer to the newly created node
*/
Node * makeNode(Sample sample);

/*
Make a new list with NULL head and 0 count
Return: Pointer to the newly created list
*/
LinkedList * makeList();

/**
 * Get the node pointer at the specified index in the list
 *  - Return a null pointer if the index arg exceeds the size of the list.
 */
Node *get(node_index index, LinkedList * list);

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
LinkedList *addI(node_index index, Sample sample, LinkedList *list);

/**
 * Remove (unlink) a sample from the end of the signal list based on the time and the sample order
 *  - Update the node before the last node
 *  - Free the last node
 */
LinkedList * removeI(LinkedList *list);

/**
 * Get the time domain series for a specific dimension from {start} (inclusive) to (inclusive) {end}
 *  - If {start} = 0 then we start from the beginning
 *  - If {end} out of bound (>= list count) then {end} is tail (count - 1)
Return a sub series array terminated by 100
 *
 */
acc *getSubSeries(dimension d, node_index start, node_index end, LinkedList *list);

/**
 * toString functions
 *  - print out the string representation of the target struct
 */
int toStringSample(Sample *sample);
int toStringNode(Node *node);
int toStringList(LinkedList *list);


#endif //SIMULATED_DATASTRUCTS_H
