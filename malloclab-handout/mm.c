/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Yoon So Young",
    /* First member's email address */
    "lovelife@kaist.ac.kr",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define GETSIZE(size) ((size) & (~1))

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//I implemented an explicit list. I managed free nodes by implementing a doubly linked list.
struct Node {
    int size;//size of the node.
    struct Node* prev;//points to the previous free node.
    struct Node* next;//points to next free node.
};
struct Node * head;
void insertnode(struct Node *node){//insert a new node into the "FRONT" of the free list.
    node->prev = NULL;
    node->next = head;
    if (head != NULL) {
        head->prev= node;
    }
    head = node;
}
void removenode(struct Node *node){//remove a node that was currently in the list of free nodes.
    struct Node* prevnode=(node->prev);
    struct Node* nextnode=(node->next);
    if (prevnode==NULL){//when the node was head.
        if (nextnode==NULL){//when there was nothing in the list, just set the head as NULL and return.
            head=NULL;
            return;
        }
        //when the node was head, nextnode becomes head.
        nextnode->prev=NULL;
        head=nextnode;
        return;
        }
    if (node->next==NULL){//when the node was at the end of the list, set the prevnode to become the end of the list.
        prevnode->next=NULL;
        return;
    }
    //when it's none of the cases above, link between prevnode and nextnode.
    prevnode->next = node->next;
    nextnode->prev = prevnode;
}
struct Node * findleft(struct Node * node){//find the adjacent left node.
    struct Node * leftnode;
    leftnode = (struct Node *)((char *)node -(*(int*)((char*)node-4)& ~1));
    return leftnode;
}
struct Node * findright(struct Node * node){//find the adjacent right node.
    struct Node * rightnode;
    rightnode = (struct Node *)((char *)node + (*(int *)node & ~1));//*(int*)node denotes the size of the node, and (char *)node denotes the starting address of node.
    return rightnode;
}
void writefooter(struct Node * node){//as there is "size"at the first 4 byte of the node, write the size again at the end of the node in order to easily find out the left and right node.
    *(int *)((char *)node + GETSIZE(*(int *)node) - 4) = node->size;    
}
/* mm_check: checks the consistency of heap.
*/
void mm_check(){
    struct Node * firstnode;
    int fail1,fail2,fail3;
    struct Node * current;
    current=head;//set current initially to head.
    fail1=0;//becomes 1 when any block in th free list is not marked as free.;
    while (!(current==NULL)){//while current is not head or last one
        if((current->next)!=NULL){
            if ((((findright(current)->size&1)==0)
                && (findright(current)->size))
                ||((findleft(current)->size&1)==0
                && (findleft(current)->size))){//means that there is contiguous free blocks that somehow escaped coalescing.
                fail2=1;
            if ((((current->next)->size)&1)!=0){
                fail1=1;
                break;
            }
            current=current->next;}
            if ((current->next)==NULL){
                break;}
    }
    if (fail1==1){
        printf("there is an error in the free list. not every block in the free list is marked as free.\n");
    }
    if (fail2==1){
        printf("there is contiguous free blocks that somehow escaped coalescing.\n");
    }
    }
    firstnode=(struct Node *)((char *)mem_heap_lo()+4);
    while ((long *)findright(firstnode)<=(long *)((char *)mem_heap_hi()-4)){
        if ((int)firstnode>=(int)findright(firstnode)){//the address of findright(firstnode) have to be bigger than the address of firstnode. if not, there is a possibility that allocated blocks have overlapped.
            fail3=1;
        }
        firstnode=findright(firstnode);
}
    if (fail3==1){
        printf("error. allocated blocks may have overlapped.\n");
    }
    return;
}
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    void* p = mem_sbrk(8);
    *(int *)p = 0;
    *((int *)p + 1) = 0;//set the first 8 byte to 0.
    head = NULL;//set head as NULL.
    return 0;
   
}
/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{   
   // round up to the square number of 2 to increase efficiency.
   if (size == 448)
       size = 512;
   if (size == 112)
        size = 128;
//make size allgnment (into multiple of 8).
    size = ALIGN(size) + 8;
    if (size<24)
        size=24;
    struct Node * current;
    current = head;//initial current is head.
    int init; 
    init=0;
    //FIRST FIT MODEL
    while (!(current == NULL)) {//until we reach the end of the free lists. or, get rid of the cases where there is no availiable free list.
        if(current->size >= size) {//when we find a node where the size is bigger than need-to-be-allocated size, we set that current to the node to be allocated.
            init = 1;
            break;
        }
        current = current->next;//go to the next node until we find a node that has size bigger than the need-to-be-allocated size.
    }
    if((current == NULL) && (init == 0)){//when head is null. or there is nothing that matches this.
        struct Node * newnode;
        newnode=(struct Node *)((int *)mem_sbrk(size) - 1);//allocate a "new" space.
        newnode->size = size | 1;//make the new node allocated bit to 1.
        writefooter(newnode);
        *(int *)((char*)newnode + (newnode->size & ~1))= 0;
        return (void *)((int *)newnode+1);
    }
    removenode(current);
    if (((current->size) - size) >= 24){//when there is still more than 24 bytes left after allocating into a free list, make a new free node.
        struct Node * newnode;
        newnode = (struct Node *)((char *)current + size);
        newnode->size = (current->size - size) & (~1);
        writefooter(newnode);
        insertnode(newnode);
        current->size = current->size - newnode->size;
    }
    current->size = current->size | 1;//set the current allocated bit to 1.
    writefooter(current);
    return (void *)((int *)current + 1);
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{   
    struct Node * leftnode;
    struct Node * rightnode;
    struct Node * node = (struct Node *)((int *)ptr-1);
    int coal = 0;
    //1. change the last bit of the header from 1(allocated) to 0(free).
    node->size = (node->size)&(~1);
    writefooter(node);//insert the footer.

    //2. if there is a free block at the adjacent left or right, coalesce it.
    leftnode = findleft(node);
    rightnode = findright(node);
    if (((leftnode->size)&1) == 0 && leftnode != node) {//if leftone was a free block, just increase the size of leftfreeblock. don't execute it when the node was at the very left.
        coal = 1;
        leftnode->size = leftnode->size + node->size;//add up the size.
        writefooter(leftnode);
        node = leftnode;
    }
    if ((((rightnode->size)&1)==0) && (rightnode->size)){//when the right one was a free block, and was not the very last node, coalasce it.
        node->size = (node->size) + (rightnode->size);
        writefooter(node);
        removenode(rightnode);
    }
    if (coal == 0) {//when we didn't coalasce with the left block, insert node.
        insertnode(node);
    }
    return;
}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    int newsize;
    int oldsize;
    struct Node * target;
    struct Node * newfreenode;

    target=(struct Node *)((char *)ptr - 4);//points at the current node, just at the starting of the header.
    oldsize = (target->size)&(~1);
    newsize = ALIGN(size)+8;
    struct Node * rightnode;
    /* DESCRIPTION
    first if= when realloc size is smaller than the original.
    second if= when left node was a free node, and was able to realloc it there.
    third if= when right node was a free node, and was able to realloc just by coalascing with the original.
    last one=when none of these was possible.
    
    */
    if (oldsize >= newsize)//when the realloc size is smaller than the old size.
    {   if ((oldsize-newsize)>=24)//when the difference is bigger than 24 bytes, we can make a new free list.
        {
            target->size=newsize+1;//change the size of allocated chunk to new size and set the "allocated" bit.
            writefooter(target);//write the footer.
            newfreenode = findright(target);//get the starting address of new free list.
            newfreenode->size = (oldsize-newsize)&(~1);//write the new free node's size as the difference of oldsize/newsize. and set the allocation bit to zero.
            writefooter(newfreenode);//write footer to the new free node.
            insertnode(newfreenode);//add this free node to the list of free nodes.
        }
        //if the difference between smaller size and oldsize is less than 24, we can't make a new free node, so just allocate as it is.
        return ptr;
    }
    //BELOW is activated when the realloc size is bigger than the old size.
    rightnode = findright(target);
    if ((((rightnode->size)&1)==0) && (newsize<=((rightnode->size)+(target->size))))
    {  
        removenode(rightnode);//first remove the node from free list to allocate information there.
        int difference;
        if((difference = (rightnode->size)+oldsize-newsize)>=24)//if the amount of unused byte after reallocating is bigger than 24, we can make a new free node.
        {   newfreenode = (struct Node *)((char *)target+newsize);
            newfreenode->size=difference;
            writefooter(newfreenode);
            insertnode(newfreenode);
            target->size=newsize+1;//set the new allocated(in this case, leftnode) size to newsize and set the allocation bit to 1.
        }
    
        else{//when the left node was a free list but we can't make a new free list using the unused byte, we just set newsize as the sum of leftnodesize and targetsize, which will be bigger than the actual "newsize".
            target->size = target->size + rightnode->size;//Here also, target->size is already set to 1, so need not add one.
        }
        writefooter(target);
        return ptr;
    }
    else if (rightnode->size == 0) {//when rightnode reaches the end,
        int diff = newsize - oldsize;
        mem_sbrk(diff);
        target->size = newsize | 1;
        writefooter(target);

        *(int *)findright(target) = 0;
        return ptr;
    }
//if none of the adjacent one was a free node, malloc it, copy the data, and free it.
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - 4);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
