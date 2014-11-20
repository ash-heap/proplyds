#include "dlist.h"
#include <stdlib.h>
#include <assert.h>




struct DList {
    struct DListNode* head;
    struct DListNode* tail;
};




struct DListNode {
    struct DListNode* prev;
    struct DListNode* next;
    void* data;
};




struct DList* dlistNew()
{
    struct DList* dlist = malloc(sizeof(struct DList));
    dlist->head = NULL;
    dlist->tail = NULL;
    return dlist;
}




struct DList* dlistCopy(struct DList* const dlist)
{
    assert(dlist != NULL);

    struct DList* newDlist = dlistNew();
    struct DListNode* node = dlist->head;
    while (node != NULL) {
        dlistInsertAtTail(newDlist, node->data);
        node = node->next;
    }

    return newDlist;
}




void dlistDelete(struct DList* const dlist)
{
    assert(dlist != NULL);

    // Delete nodes.
    struct DListNode* node     = dlist->head;
    struct DListNode* nextNode;
    while (node != NULL) {
        nextNode = node->next;
        dlistNodeDelete(node);
        node = nextNode;
    }

    // Delete the metadata structure.
    free(dlist);
}




struct DListNode* dlistHead(struct DList* const dlist)
{
    assert(dlist != NULL);

    return dlist->head;
}




struct DListNode* dlistTail(struct DList* const dlist)
{
    assert(dlist != NULL);

    return dlist->tail;
}




struct DListNode* dlistNodePrev(struct DListNode* node)
{
    return node->prev;
}




struct DListNode* dlistNodeNext(struct DListNode* node)
{
    return node->next;
}




void dlistCons(struct DList* const dlist, void* data)
{
    assert(dlist != NULL);

    // Prepend new node.
    dlist->head = dlistNodeNew(NULL, dlist->head, data);

    // Set previous field of next node to the new node.
    if (dlist->head->next != NULL)
        dlist->head->next->prev = dlist->head;

    // Fix tail pointer if this is the only node.
    if (dlist->tail == NULL)
        dlist->tail = dlist->head;
}




void dlistCat(struct DList* const dlist0, struct DList* const dlist1)
{
    assert(dlist0 != NULL);
    assert(dlist1 != NULL);

    // Handle the special case of the first list being empty.
    if (dlist0->head == NULL) {
        dlist0->head = dlist1->head;
        dlist0->tail = dlist1->tail;

    // Only perform the concatenation if the second list is not empty.
    } else if (dlist1->head != NULL) {
        dlist0->tail->next = dlist1->head;
        dlist1->head->prev = dlist0->tail;
        dlist0->tail = dlist1->tail;
    }

    // Free the second list.
    free(dlist1);
}




void dlistInsertAtHead(struct DList* const dlist, void* data)
{
    dlistCons(dlist, data);
}




void dlistInsertAtTail(struct DList* const dlist, void* data)
{
    assert(dlist != NULL);

    // Append new node.
    dlist->tail = dlistNodeNew(dlist->tail, NULL, data);

    // Set next field of previous node to the new node.
    if (dlist->tail->prev != NULL)
        dlist->tail->prev->next = dlist->tail;

    // Fix head pointer if this is the only node.
    if (dlist->head == NULL)
        dlist->head = dlist->tail;
}




void dlistInsertAfter(struct DList* const dlist,
                      struct DListNode* const node,
                      void* data)
{
    assert(dlist != NULL);
    assert(node != NULL);

    // Make new node.
    struct DListNode* newNode = dlistNodeNew(node, node->next, data);

    // Connect to previous node.
    node->next = newNode;

    // Connect to next node.
    if (newNode->next != NULL)
        newNode->next->prev = newNode;

    // Deal with special case of node being the tail node of the list.
    if (dlist->tail == node)
        dlist->tail = newNode;
}




void dlistInsertBefore(struct DList* const dlist,
                       struct DListNode* const node,
                       void* data)
{
    assert(dlist != NULL);
    assert(node != NULL);

    // Make new node.
    struct DListNode* newNode = dlistNodeNew(node->prev, node, data);

    // Connect to previous node.
    if (newNode->prev != NULL)
        newNode->prev->next = newNode;

    // Connect to next node.
    node->prev = newNode;

    // Deal with special case of node being the head node of the list.
    if (dlist->head == node)
        dlist->head = newNode;
}




void dlistDeleteNode(struct DList* const dlist, struct DListNode* const node)
{
    assert(dlist != NULL);
    assert(node != NULL);

    // Deal with special case of this being the head of the list.
    if (dlist->head == node)
        dlist->head = node->next;

    // Deal with special case of this being the tail of the list.
    if (dlist->tail == node)
        dlist->tail = node->prev;

    // Link forward the previous node.
    if (node->prev != NULL)
        node->prev->next = node->next;

    // Link back the next node.
    if (node->next != NULL)
        node->next->prev = node->prev;

    // Delete the node.
    dlistNodeDelete(node);
}




struct DListNode* dlistNodeNew(struct DListNode* const prev,
                               struct DListNode* const next,
                               void* data)
{
    struct DListNode* node = malloc(sizeof(struct DListNode));
    node->prev = prev;
    node->next = next;
    node->data = data;
    return node;
}




void dlistNodeDelete(struct DListNode* const node)
{
    free(node);
}




void* dlistNodeData(struct DListNode* const node)
{
    return node->data;
}
