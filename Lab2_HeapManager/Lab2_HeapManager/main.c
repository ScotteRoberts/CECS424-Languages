//
//  main.c
//  Lab2_HeapManager
//
//  Created by Scott Roberts on 2/23/18.
//  Copyright © 2018 Scott Roberts. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

// Part 1
struct Block {
    int block_size; // # of bytes in the data section
    struct Block *next_block; // in C, you have to use  struct Block  as the type
};

static int HEAP_SIZE = 1000;

// Part 2: Size of our minimum memory blocks.
static int BLOCK_SIZE = sizeof(struct Block);

// Part 3: Size of a void pointer.
static int VOID_POINTER_SIZE = sizeof(void*);

// Part 4: Create a global void* that maintains the position of the free free block.
struct Block* free_head;

// Part 5: Create an initializer that grabs a block of memory and point free_head to the block.
void my_initialize_heap(int size) {
    free_head = malloc(size);
    printf("Free Head Location: %d\n", (int)free_head);
    free_head -> block_size = size - BLOCK_SIZE;
    printf("Total Empty Memory: %d\n", free_head -> block_size);
    free_head -> next_block = NULL;
    printf("Init Block next pointer: %p\n", free_head -> next_block);
}

// Part 6: Create a function void* my_alloc(int size), which  lls an allocation
// request of size bytes and returns a pointer to the data portion of the block used to satisfy the request.
void* my_alloc(int size) {
    return NULL;
}

// Main Method
int main(int argc, const char * argv[]) {
    printf("Block Size: %d\n", BLOCK_SIZE);
    printf("Void Pointer Size: %d\n", VOID_POINTER_SIZE);
    my_initialize_heap(HEAP_SIZE);
    return 0;
}


