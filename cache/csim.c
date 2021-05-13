/*
 * author: Wei Xinpeng
 * student ID: 519021910888 
 */
#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void usage() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(0);
}

int getSet(uint64_t address, int setBits, int blockBits) {
    uint64_t add = address >> blockBits;
    int set = 0;
    for (int i = setBits - 1; i >= 0; i--) {
        if ((add >> i) & 1) set = set * 2 + 1;
        else set = set * 2;
    }
    return set;
}

typedef struct Node {
    struct Node * next;
    int lineIndex;
} LRU_node;

void insert(LRU_node * head, int index) {
    LRU_node * new_node = (LRU_node *)calloc(1, sizeof(LRU_node));
    new_node -> lineIndex = index;
    new_node -> next = head -> next;
    head -> next = new_node;
}

int getOldest(LRU_node * head) {
    LRU_node * p = head -> next, * p_prev = head;
    int result;
    while(p -> next != NULL) {
        p_prev = p;
        p = p -> next;
    }
    p_prev -> next = NULL;
    result = p -> lineIndex;
    free(p);
    return result;
}

void freeList(LRU_node * head) {
    while(head -> next != NULL) {
        LRU_node * tmp = head -> next;
        head -> next = tmp -> next;
        free(tmp);
    }
}
void hit(LRU_node * head, int index) {
    LRU_node * p = head -> next, * p_prev = head;
    while (p -> lineIndex != index) {
        p_prev = p;
        p = p -> next;
    }
    p_prev -> next = p -> next;
    p -> next = head -> next;
    head -> next = p;
}

int main(int argc, char **argv)
{
    opterr = 0;//getopt不打印额外错误信息
    char para;
    int verbose = 0;
    int setBits = 0;
    int sets = 0;
    int blockBits = 0;
    int linePerSet = 0;
    char route[50];
    uint64_t ** cache;
    LRU_node ** header;

    /* results */
    int hits = 0, misses = 0, evictions = 0;
    while ((para = getopt(argc, argv, "hvs:E:b:t:")) != EOF) {
        switch (para) {
            case 'h':{
                usage();
            }
            case 'v':{
                verbose = 1;
                break;
            }
            case 's':{
                setBits = atoi(optarg);
                break;
            }
            case 'E':{
                linePerSet = atoi(optarg);
                break;
            }
            case 'b':{
                blockBits = atoi(optarg);
                break;
            }
            case 't':{
                memcpy(route, optarg, strlen(optarg));
                break;
            }
            case '?':{
                printf("./csim: invalid option -- '%c'\n", optopt);
                usage();
            }
        }
    }
    if (setBits == 0 || blockBits == 0 || linePerSet == 0) {
        printf("./csim: Missing required command line argument\n");
        usage();
    }
    FILE *trace = fopen(route, "r");
    if (trace == NULL) {
        printf("%s: No such file or directory\n", route);
        exit(0);
    }

    sets = 1 << setBits;

    cache = (uint64_t **)calloc(sets, sizeof(uint64_t*));
    for (int i = 0; i < sets; i++) {
        cache[i] = (uint64_t *)calloc(linePerSet, sizeof(uint64_t));
    }
    header = (LRU_node **)calloc(sets, sizeof(LRU_node*));
    for (int i = 0; i < sets; i++) {
        header[i] = (LRU_node *)calloc(1, sizeof(LRU_node));
        header[i] -> next = NULL;
    }
    
    char buff[50];
    while (fgets(buff, 50, trace) != NULL) {
        *strchr(buff, '\n') = '\0';
        char *p = buff;
        if (*p == 'I')continue;//ignore instrucion load
        p++;
        char memAccess = *p;
        p++;
        uint64_t address = strtoul(p, NULL, 16);
        switch (memAccess) {
            case 'L':
            case 'S':{
                int i, j = -1;//j records an unwriten cache line
                int set = getSet(address, setBits, blockBits);
                for (i = 0; i < linePerSet; i++) {
                    if (cache[set][i] & 1) {
                        if ((address >> (setBits + blockBits)) == (cache[set][i] >> 1)) break;
                    }
                    else j = i;
                }
                if (i == linePerSet) {//not found
                    misses += 1;
                    strcat(buff, " miss");
                    if (j >= 0) {
                        cache[set][j] = ((address >> (setBits + blockBits)) << 1) + 1;
                        insert(header[set], j);
                    }
                    else {
                        evictions++;
                        strcat(buff, " eviction");
                        int oldest = getOldest(header[set]);
                        insert(header[set], oldest);
                        cache[set][oldest] = ((address >> (setBits + blockBits)) << 1) + 1;
                    }
                }
                else {
                    hits += 1;
                    strcat(buff, " hit");
                    hit(header[set], i);
                }
                if (verbose) printf("%s\n", buff);
                break;
            }
            case 'M':{
                int i, j = -1;//j records an unwriten cache line
                int set = getSet(address, setBits, blockBits);
                for (i = 0; i < linePerSet; i++) {
                    if (cache[set][i] & 1) {
                        if ( (address >> (setBits + blockBits)) == (cache[set][i] >> 1) ) break;
                    }
                    else j = i;
                }
                if (i == linePerSet) {//not found
                    misses += 1;
                    hits++;//after load, store must hit
                    strcat(buff, " miss");
                    if (j >= 0) {
                        cache[set][j] = ((address >> (setBits + blockBits)) << 1) + 1;
                        insert(header[set], j);
                    }
                    else {
                        evictions++;
                        strcat(buff, " eviction");
                        int oldest = getOldest(header[set]);
                        insert(header[set], oldest);
                        cache[set][oldest] = ((address >> (setBits + blockBits)) << 1) + 1;
                    }
                    strcat(buff, " hit");
                }
                else {
                    hits += 2;
                    hit(header[set], i);
                    strcat(buff, " hit hit");
                }
                if (verbose) printf("%s\n", buff);
                break;
            }
        }
    }
    
    for (int i = 0; i < sets; i++) {
        free(cache[i]);
        freeList(header[i]);
    }
    free(cache);
    free(header);

    printSummary(hits, misses, evictions);
    return 0;
}
