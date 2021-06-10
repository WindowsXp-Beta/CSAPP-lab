/*
 * author: 魏新鹏
 * studentID: 519021910888
 * MY dynamic allocator
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"
#define DEBUGx
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* MY macro and global var define begin */
#define WSIZE 4
#define DSIZE 8
size_t CHUNKSIZE;//get from getpagesize()

#define MAX(x, y) ((x)>(y)?(x):(y))
#define PACK(size, alloc) ((size)|(alloc))

#define GET(p) (*(unsigned int*)(p)) //read a word(4 byte) at address p
#define PUT(p, val) (*(unsigned int*)(p) = (val))
#define PUT_LONG(p, val) (*(unsigned long*)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))

#define SUCC(bp) (char*)(bp)
#define PREV(bp) ((char*)(bp) + DSIZE)
#define GET_SUCC(bp) (*(unsigned long*)SUCC(bp))//unsigned long is equal to void*
#define GET_PREV(bp) (*(unsigned long*)PREV(bp))

static char* heap_listp;

void* freeList_head;
#define FREE_LIST_NUM 9

inline void printList(void ** p) {
    void * iterator = *p;
    while(iterator) {
        size_t size = GET_SIZE(HDRP(iterator));
        size_t alloc = GET_ALLOC(HDRP(iterator));
        printf("block at %lu, size is %lu, alloc is %lu succ is %lu prev is %lu\n", iterator, size, alloc, GET_SUCC(iterator), GET_PREV(iterator));
        iterator = GET_SUCC(iterator);
    }
}

inline void heapChecker() {
    for(int i = 0; i < FREE_LIST_NUM; i++) {
        printf("Print List %d\n", i);
        printList(freeList_head + i * 8);
    }
}

inline int get_index(size_t size) {
    for(int i = 12; i >= 5; i--) {
        if((size >> i) > 0) {
            return i - 4;
        }
    }
    return 0;
}

static void insert_list(size_t size, void* bp) {
    int index = get_index(size);
    void* start = (*(unsigned long*)(freeList_head + index * 8));
    // while(start) {
    //     if(GET_SIZE(HDRP(start)) >= size) break;
    //     start = GET_SUCC(start); 
    // }
    // PUT_LONG(SUCC(bp), start);//它的NEXT为原先首元素
    // PUT_LONG(PREV(bp), freeList_head + index * 8);//它的PREV设为head

    // if(old_first) PUT_LONG(PREV(old_first), bp);//原先首元素的PREV
    // PUT_LONG(freeList_head + index * 8, bp);//head指向它
    
    char *pre = NULL;
    while ((start != NULL) && (size > GET_SIZE(HDRP(start)))) {
        pre = start;
        start = GET_SUCC(start);
    }// i should be first node size >= input size
    if (start == NULL && pre == NULL) {//empty list
        PUT_LONG(freeList_head + index * 8, bp);//head指向它
        PUT_LONG(PREV(bp), NULL);
        PUT_LONG(SUCC(bp), NULL);
    } else if (start == NULL && pre != NULL) {// add the last
        PUT_LONG(PREV(bp), pre);
        PUT_LONG(SUCC(bp), NULL);
        PUT_LONG(SUCC(pre), bp);
    } else if (pre == NULL) {// add the first
        PUT_LONG(freeList_head + index * 8, bp);//head指向它
        PUT_LONG(PREV(start), bp);
        PUT_LONG(SUCC(bp), start);
        PUT_LONG(PREV(bp), NULL);
    } else {
        PUT_LONG(PREV(bp), pre);
        PUT_LONG(SUCC(bp), start);
        PUT_LONG(PREV(start), bp);
        PUT_LONG(SUCC(pre), bp);
    }
}

// inline 
static void remove_list(void* bp) {
    #ifdef DEBUG
    printf("remove %lu\n", bp);
    heapChecker();
    fflush(stdout);
    #endif
    size_t size = GET_SIZE(HDRP(bp));
    int index = get_index(size);
    if(GET_PREV(bp) == NULL) {
        PUT_LONG(freeList_head + index * 8, GET_SUCC(bp));
        if(GET_SUCC(bp) != NULL) PUT_LONG(PREV(GET_SUCC(bp)), NULL);
    }
    else if(GET_SUCC(bp) == NULL) {
        PUT_LONG(SUCC(GET_PREV(bp)), NULL);
    }
    else {
        PUT_LONG(SUCC(GET_PREV(bp)), GET_SUCC(bp));
        PUT_LONG(PREV(GET_SUCC(bp)), GET_PREV(bp));
    }
    // void* succ = GET_SUCC(bp);
    // void* prev = GET_PREV(bp);
    // if(succ) PUT_LONG(PREV(succ), prev);//if next not NULL
    // PUT_LONG(SUCC(prev), succ);
}

static void* coalesce(void* bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if(prev_alloc && next_alloc) {//前后都已分配
        insert_list(size, bp);
        return bp;
    }
    else if(prev_alloc && !next_alloc) {//前已分配，后未分配
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        remove_list(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert_list(size, bp);
        return bp;
    }
    else if(!prev_alloc && next_alloc) {//前未分配，后已分配
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        remove_list(PREV_BLKP(bp));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        insert_list(size, PREV_BLKP(bp));
        return PREV_BLKP(bp);
    }
    else {//前后都未分配
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        remove_list(PREV_BLKP(bp));
        remove_list(NEXT_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        insert_list(size, PREV_BLKP(bp));
        return PREV_BLKP(bp);
    }
}

static void* extend_heap(size_t words) {
    char* bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1) return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return coalesce(bp);
}
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    CHUNKSIZE = mem_pagesize();
    if((heap_listp = mem_sbrk(FREE_LIST_NUM * DSIZE + DSIZE + DSIZE)) == (void*)-1)
        return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + FREE_LIST_NUM * DSIZE + WSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + FREE_LIST_NUM * DSIZE + DSIZE, PACK(DSIZE, 1));
    PUT(heap_listp + FREE_LIST_NUM * DSIZE + DSIZE + WSIZE, PACK(0, 1));


    freeList_head = heap_listp + WSIZE;
    for(int i = 0; i < FREE_LIST_NUM; i++) {
        PUT_LONG(freeList_head + i * 8, NULL);
    }
    insert_list(CHUNKSIZE, heap_listp + FREE_LIST_NUM * DSIZE + DSIZE + DSIZE);
    //begin extend
    void* bp;
    size_t words = CHUNKSIZE/WSIZE, size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1) return -1;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return 0;
}

void* find_fit(size_t size) {
    void* head_ptr;
    int index = get_index(size);
    for(int i = index; i < 9; i++) {
        head_ptr = *(unsigned long*)(freeList_head + i * 8);
        while(head_ptr != NULL) {
            if( GET_SIZE(HDRP(head_ptr)) >= size) {
                return head_ptr;
            }
            head_ptr = GET_SUCC(head_ptr);
        }
    }
    return NULL;
}

void* place(void* bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));
    size_t left = csize - asize;
    remove_list(bp);
    if(left < 24) {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        return bp;
    }
    else if(left <= 96) {//need split
    // else{
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(left, 0));
        PUT(FTRP(bp), PACK(left, 0));
        insert_list(left, bp);
        return PREV_BLKP(bp);
    }
    else {
        PUT(HDRP(bp), PACK(left, 0));
        PUT(FTRP(bp), PACK(left, 0));
        insert_list(left, bp);
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        #ifdef DEBUG
        heapChecker();
        fflush(stdout);
        #endif
        return bp;
    }
}

/* 
 * 
 * 
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;
    if(size <= 0) return NULL;

    if(size <= DSIZE) asize = DSIZE + DSIZE;
    else asize = ALIGN(size + DSIZE);//size + header + footer

    if((bp = find_fit(asize)) != NULL) {
        bp = place(bp, asize);
        return bp;
    } else {
        extendsize = MAX(asize, CHUNKSIZE);
        if((bp = extend_heap(extendsize/WSIZE)) == NULL) {
            return NULL;
        }
        bp = place(bp, asize);
        return bp;
    }
}

/*
 * 
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/*
 * 
 * 
 * 
 */
void *mm_realloc(void *ptr, size_t size)
{
    if(ptr == NULL) return mm_malloc(size);
    if(size == 0) {
        mm_free(ptr);
        return ptr;
    }

    size_t asize, old_size = GET_SIZE(HDRP(ptr));
    if(size <= DSIZE) asize = DSIZE + DSIZE;
    else asize = ALIGN(size + DSIZE);//size + header + footer

    if(asize <= old_size) {
        return ptr;
    }
    else { //asize > old_size
        size_t lack = asize - old_size;
        size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
        size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(ptr)));
        size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        if(!next_size) {//如果到堆底了
            // size_t extendSize = MAX(lack, CHUNKSIZE);
            size_t extendSize = CHUNKSIZE;
            extend_heap(extendSize/WSIZE);
            old_size += extendSize;
            remove_list(NEXT_BLKP(ptr));
            PUT(HDRP(ptr), PACK(old_size, 1));
            PUT(FTRP(ptr), PACK(old_size, 1));
            return ptr;
        }
        if(!next_alloc && prev_alloc) {//如果后面的块空闲
            old_size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
            if(old_size >= asize) {//合并这两块，无需memcpy
                remove_list(NEXT_BLKP(ptr));
                PUT(HDRP(ptr), PACK(old_size, 1));
                PUT(FTRP(ptr), PACK(old_size, 1));
                return ptr;
            }
        }
        else if(!prev_alloc && next_alloc) {
            old_size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
            if(old_size >= asize) {
                void * newPtr = PREV_BLKP(ptr);
                remove_list(newPtr);
                memcpy(newPtr, ptr, GET_SIZE(HDRP(ptr)));
                PUT(HDRP(newPtr), PACK(old_size, 1));
                PUT(FTRP(newPtr), PACK(old_size, 1));
                return newPtr;
            }
        }
        else if(!prev_alloc && !next_alloc) {
            old_size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
            if(old_size >= asize) {
                void * newPtr = PREV_BLKP(ptr);
                remove_list(NEXT_BLKP(ptr));
                remove_list(newPtr);
                memcpy(newPtr, ptr, GET_SIZE(HDRP(ptr)));
                PUT(HDRP(newPtr), PACK(old_size, 1));
                PUT(FTRP(newPtr), PACK(old_size, 1));
                return newPtr;
            }
        }
        else {
            void* new_ptr = mm_malloc(size);
            memcpy(new_ptr, ptr, old_size - DSIZE);
            mm_free(ptr);
            return new_ptr;
        }
    }
}