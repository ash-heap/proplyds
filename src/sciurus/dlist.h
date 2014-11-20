#ifndef SCIURUS_DLIST_H
#define SCIURUS_DLIST_H


#ifdef __cplusplus
extern "C" {
#endif




struct DList;

struct DListNode;




struct DList* dlistNew();

struct DList* dlistCopy(struct DList* const dlist);

void dlistDelete(struct DList* const dlist);




struct DListNode* dlistHead(struct DList* const dlist);

struct DListNode* dlistTail(struct DList* const dlist);

struct DListNode* dlistNodePrev(struct DListNode* node);

struct DListNode* dlistNodeNext(struct DListNode* node);




void dlistCons(struct DList* const dlist, void* data);

void dlistCat(struct DList* const dlist0, struct DList* const dlist1);




void dlistInsertAtHead(struct DList* const dlist, void* data);

void dlistInsertAtTail(struct DList* const dlist, void* data);

void dlistInsertAfter(struct DList* const dlist,
                      struct DListNode* const node,
                      void* data);

void dlistInsertBefore(struct DList* const dlist,
                       struct DListNode* const node,
                       void* data);

void dlistDeleteNode(struct DList* const dlist, struct DListNode* const node);




struct DListNode* dlistNodeNew(struct DListNode* const prev,
                               struct DListNode* const next,
                               void* data);

void dlistNodeDelete(struct DListNode* const node);




void* dlistNodeData(struct DListNode* const node);




#ifdef __cplusplus
}
#endif


#endif
