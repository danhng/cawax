//
// @author Danh Thanh Nguyen <d.t.nguyen@newcastle.ac.uk>
//

#include "cawax_datastructs.h"
#include "cawax.h"
#include <stdio.h>
#include <stdlib.h>

void * getComponent(Sample * sample, int componentIndex) {
	if (!sample) {
		fprintf(stderr, "Calling getComponent needs a valid Sample pointer. %p given.\n", sample);
		return NULL;
	}

	if (componentIndex & CINDEX_X) {
		return &(sample->x);
	}
	if (componentIndex & CINDEX_Y) {
		return &(sample->y);
	}
	if (componentIndex & CINDEX_Z) {
		return &(sample->z);
	}
	if (componentIndex & CINDEX_RMQ) {
		return &(sample->rmq);
	}
	if (componentIndex & CINDEX_ORDER) {
		return &(sample->order);
	}
	if (componentIndex & CINDEX_TIME) {
		return &(sample->time);
	}
	

	fprintf(stderr, "Calling getComponent needs a valid componentIndex. %x given.\n", componentIndex);
	return NULL;
}

Node * jump(const Node * depart, int step) {
	if (!depart) {
		fprintf(stderr, "Calling jump requires a non-null Node ptr, %p given. \n", depart);
		return ERROR_PTR_OUTPUT;
	}
	char forw = step >= 0;
	step = abs(step);
	int count = 0;
	Node * current = depart;
	while (count++ < step && current) {
		current = forw ? current->next : (step == 0) ? current : current->prev;
	}
	// count now should be 1 greater than step
	if (count < step) {
		printf("Calling jump on %p step %d: Cannot reach the destination. Stopped at step %d.\n", depart, step, count);
	}
	//printf("count is: %d\n", count);
	return current;
}


LinkedList * makeList() {
	LinkedList * list;
	int s = sizeof(Node *) + sizeof(long) ;
	list = (LinkedList *) malloc(s);
	//printf("Allocating %d bytes to a new LinkedList at %p\n", s, list);

	list->head = NULL;
	list->count = 0;
	return list;
}

Node * makeNode(Sample sample)
{
	Node * node;
	int s = sizeof sample + sizeof node * 2;
	node = (Node *) malloc(s);
	//printf("Allocating %d bytes to a new Node at %p\n", s, node);

	node->sample = sample;

	node->next = NULL;
	node->prev = NULL;

	return node;
}

/**
 * Get the node pointer at the specified index in the list
 *  - Return a null pointer if the index arg exceeds the size of the list.
 */
Node * get(node_index index, LinkedList * list) {
	//printf("Calling get at index %d of list %p size %d\n", index, list, list->count);
	int count = 0;
	if (index < 0 || index >= list->count) {
		printf("err: Illegal index: %d when accessing list %p of size %d, max index: %d\n", index, list, list->count, list->count -1 );
		return NULL;
	}
	return jump(list->head, index);
}

/**
* Add a sample to the end a signal list
*  - Update pre tail node
*  - Create new node for the new sample
*  - Link new node to the list
*/
LinkedList * add(Sample sample, LinkedList *list) {
	return addI(list->count, sample, list);
}

/**
 * Add a sample to the specified index of the list
 *  - Create the node for the new sample
 *  - Update the neighbor nodes as to the newly created node
 * Return: the list with the sample added at the exact index specified in the list
 */
LinkedList * addI(node_index index, Sample sample, LinkedList *list) {
	//printf("Calling addI with index: %d, list count: %d\n", index, list->count);
    if (index > list->count) {
        printf("Error: index to add %d exceeds size of linked list at %p. List modification omitted.\n", index, list);
        return list;
    }

	// allocate a new node
	Node * newNode = makeNode(sample);
	// old node at index not available?
	// first node?
	if (list->count == 0) {
		//printf("List %p: add case: first node.\n", list);
		list->head = newNode;
		list->count++;
		return list;
	}
	// append ?
	if (index == list->count) {
		//printf("List %p: add case: append.\n", list);
		Node * tail = get(list->count - 1, list);
		//update newNode
		newNode->prev = tail;
		newNode->next = NULL;
		//update involved node
		tail->next = newNode;
		//update the head of the list and count
		list->count++;
		return list;
	}
	// old node at index available?
	Node * old = get(index, list);
	//printf("getting old node at index %d of list %p with %d nodes: %p\n", index, list, list->count, old);
	// new head ?
	if (index == 0) {
		//printf("List %p: add case: new head.\n", list);
		//update newNode
		newNode->prev = NULL;
		newNode->next = old;
		//update involved node
		old->prev = newNode;
		//update the head of the list and count
		list->head = newNode;
		list->count++;
		return list;
	}
	// sandwich?
	//printf("List %p: add case: sandwich\n", list);
	//get involved nodes;
	Node * prev_old = old->prev;
	// update the new node 
	newNode->prev = prev_old;
	newNode->next = old;
	// update the old node
	old->prev = newNode;
	// update the prev old node
	prev_old->next = newNode;
	// update the count
	list->count++;

	return list;
}

acc * getSubSeries(dataDimension d, node_index start, node_index end, LinkedList * list)
{
	if (end >= list->count) {
		printf("warn: specfied end index %d out of bound %d in list %p. reset to tail.", end, list->count, list);
		end = list->count - 1;
	}
	if (start > end) {
		printf("err: specfied start index %d greater than end index %d in list %p. abort.", start, end, list);
		exit(EXIT_FAILURE);
	}

	//TODO free
	unsigned int size = end - start + 2;
	unsigned int count = 0;
	acc * sub = (acc *) calloc(sizeof(acc), size); // inclusive on both ends + 100 terminated
	Node * current = get(start, list);
	
	while (count < (size-1) && current) {
		sub[count] = (d == Y ? current->sample.y : d == Z ? current->sample.z : d == X ? current->sample.x : current->sample.rmq);
		count++;
		current = current->next;
	}
	sub[count] = TERMINATION_VALUE; // 0x1111 g should be unreachable 
	return sub;
}

void freeLinkedList(LinkedList * list)
{
	if (!list)
		return;
	emptyLinkedList(list);
	free(list);
	return;
}

LinkedList * emptyLinkedList(LinkedList * list)
{
	if (!list)
		return list;
	
	if (!(list->head)) {
		printf("Warn: emptying list %p with NULL head.", list);
		return list;
	}

	// free the nodes starting from the tail to ensure list consistency in the event of failure.
	Node * current = get(list->count - 1, list);
	while (current = current->prev) {
		free(current->next);
		list->count--;
	}

	// reset list fields
	list->head = NULL;
	list->count = 0;

	return list;
}

int toStringSample(Sample * sample)
{
	char * time = (char *)malloc(20);
	printf("%-30s/%-5d/x: %2.3f, y: %2.3f, z: %2.3f, rmq: %2.3f\n", cawaxInternalTimeToString(sample->time, time), sample->order, sample->x, sample->y, sample->z, sample->rmq);
	free(time);
	return 0;
}

int toStringNode(Node * node)
{
	Sample sample = node->sample;
	char * time = (char *)malloc(15);
	printf("--------------------------------------------------\n");
	printf("Node %p / Prev: %p / Next: %p \nSample:  %-15s / %-5d / x: %2.3f, y: %2.3f, z: %2.3f, rmq: %2.3f\n", 
		node, node->prev, node->next, cawaxInternalTimeToString(sample.time, time), sample.order, sample.x, sample.y, sample.z, sample.rmq);
	printf("--------------------------------------------------\n");
	free(time);
	return 0;
}

int toStringList(LinkedList* list) {
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("LinkedList at %p: %d nodes, head %p\n", list, list->count, list->head);
	if (list->count == 0) {
		printf("Empty\n");
	}
	else {
		Node * current = list->head;
		while (current) {
			toStringNode(current);
			current = current->next;
		}
	}
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	return 0;
}






