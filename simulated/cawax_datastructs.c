//
// @author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>.
//

#include "cawax_datastructs.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Get the node pointer at the specified index in the list
 *  - Return a null pointer if the index arg exceeds the size of the list.
 */
Node * get(node_index index, LinkedList * list) {
    return list->head[index];

}

/**
 * Add a sample to the end a signal list
 *  - Update pre tail node
 *  - Create new node for the new sample
 *  - Link new node to the list
 */
LinkedList * add(Sample sample, LinkedList *list) {

    Node * tail = get(list->count - 1, list);

    Node * new;
    // allocate some memory to the new size
    new = (Node *) malloc(sizeof sample, sizeof node * 2 );
    // update the content of the new node
    new->sample = sample;
    new->next = NULL;
    new->prev = tail;

    // update the next of the current tail
    if (!tail->next)
        printf("Warning: tail of linked list %x is not NULL. Overriding...", tail->next);
    tail->next = new;

    // increment the count
    count++;
}


/**
 * Add a sample to the specified index of the list
 *  - Create the node for the new sample
 *  - Update the neighbor nodes as to the newly created node
 */
LinkedList *add(node_index index, Sample sample, LinkedList *list) {

    if (index >= list->count) {
        printf("Error: index to add %d exceeds size of linked list at %x", index, list);
        return;
    }
    Node * old = get(index, list);
    Node * prev_old = get()
}


