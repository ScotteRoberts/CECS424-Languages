//
//  main.c
//  Lab2_HeapManager
//
//  Created by Scott Roberts on 2/23/18.
//  Copyright © 2018 Scott Roberts. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    printf("********* MY_INITIALIZE_HEAP OF SIZE: %d **********\n", size);
    free_head = malloc(size);
    printf("Free Head Location: %d\n", (int)free_head);
    printf("Free Head Size Location: %d\n", (int)&(free_head -> block_size));
    printf("Free Head Next_Block Location: %d\n", (int)&(free_head -> next_block));
    free_head -> block_size = size - BLOCK_SIZE;
    printf("Total Empty Memory: %d\n", free_head -> block_size);
    free_head -> next_block = NULL;
    printf("Init Block next pointer: %p\n\n", free_head -> next_block);
}

// Part 6: Create a function void* my_alloc(int size), which fills an allocation
// request of size bytes and returns a pointer to the data portion of the block used to satisfy the request.
void* my_alloc(int size) {
    printf("********* MY_ALLOC OF SIZE: %d **********\n", size);
    // Size must be greater than or equal to 0 to allocate.
    if (size < 0)
        return NULL;
    
    // The current block starts at the free_head
    struct Block* current_block = free_head;
    bool isAllocated = false;
    
    // Go through the chain to find a first-fit block
    do {
        // Print checks
        printf("Current Block Size: %d\n", current_block -> block_size);
        if (current_block -> next_block != NULL)
            printf("Next Block size: %d\n", current_block -> next_block -> block_size);
        
        // Check if it fits at all.
        if (current_block -> block_size >= size) {
            // Split the block
            if(current_block -> block_size >= (size + BLOCK_SIZE + VOID_POINTER_SIZE)) {
                printf("Splitting Block...\n");
                // Make the new block, update all pointers and update current block size.
                struct Block new_block = {.block_size = (current_block -> block_size) - size - BLOCK_SIZE,
                    .next_block = current_block -> next_block};
                free_head = &new_block;
                current_block -> block_size = size;
                current_block -> next_block = NULL;
                printf("New Block Size: %d\n", new_block.block_size);
                isAllocated = true;
            }
            // Allocate the current block without splitting
            else {
                // Update pointers and current block size
                free_head = current_block -> next_block;
                current_block -> block_size = size;
                current_block -> next_block = NULL;
                isAllocated = true;
            }
        }
        else {
            current_block = current_block -> next_block;
        }
        
    } while (current_block != NULL && !isAllocated);
    return current_block;
}




void* test_my_alloc(int size) {
    struct Block new_block = {.block_size = size};
    int* data_pointer = &(new_block.block_size);
    return data_pointer;
}

// Main Method
int main(int argc, const char * argv[]) {
    printf("************** RUN MAIN ***************\n");
    printf("Block Size: %d\n", BLOCK_SIZE);
    printf("Void Pointer Size: %d\n\n", VOID_POINTER_SIZE);
    my_initialize_heap(HEAP_SIZE);
    my_alloc(64);
    return 0;
}


