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

// Heap size for each test case.
static int HEAP_SIZE = 10000;

// Part 1
struct Block {
    int block_size; // # of bytes in the data section
    struct Block *next_block; // in C, you have to use struct Block  as the type
};

// Part 2: Size of our minimum memory blocks.
static int OVERHEAD_SIZE = sizeof(struct Block);

// Part 3: Size of a void pointer.
static int VOID_POINTER_SIZE = sizeof(void*);

// Part 4: Create a global void* that maintains the position of the free free block.
struct Block* free_head;

// Part 5: Create an initializer that grabs a block of memory and point free_head to the block.
void my_initialize_heap(int size) {
    printf("\n=== MY_INITIALIZE_HEAP OF SIZE: %d ===\n", size);
    free_head = malloc(size);
    printf("Free Head Location: %d\n", (int)free_head);
    free_head -> block_size = size - OVERHEAD_SIZE;
    printf("Total Empty Memory: %d\n", free_head -> block_size);
    free_head -> next_block = NULL;
    printf("Init Block next pointer: %p\n", free_head -> next_block);
}

// Part 6: Create a function void* my_alloc(int size), which fills an allocation
// request of size bytes and returns a pointer to the data portion of the block used to satisfy the request.
void* my_alloc(int size) {
    printf("\n=== MY_ALLOC OF SIZE: %d ===\n", size);
    // Size must be greater than or equal to 0 to allocate.
    if (size < 0)
        return NULL;
    
    // The current block starts at the free_head
    struct Block* previous_block = NULL;
    struct Block* current_block = free_head;
    bool is_allocated = false;
    
    // Go through the chain to find a first-fit block
    do {
        // Check if it fits at all.
        if (current_block -> block_size >= size) {
            
            // Check if the current block size is a multiple of VOID_POINTER_SIZE.
            int remainder = size % VOID_POINTER_SIZE;
            
            // If not, add enough to make it a multiple of VOID_POINTER_SIZE.
            int mutate = size;
            if(remainder != 0) {
                mutate += VOID_POINTER_SIZE - remainder;
            }
            
            // Minimum splitting size requirement
            int min_split_size = mutate + OVERHEAD_SIZE + VOID_POINTER_SIZE;
            
            // Splitting
            if(current_block -> block_size >= min_split_size) {
                printf("Splitting...\n");
                
                // Make the new block, update all pointers.
                struct Block* new_block = (struct Block*)((char*)current_block + OVERHEAD_SIZE + mutate );
                new_block -> block_size = (current_block -> block_size) - size - OVERHEAD_SIZE;
                new_block -> next_block = current_block -> next_block;
                // Current at head
                if(current_block == free_head) {
                    printf("Head update...\n");
                    free_head = new_block;
                }
                // Current in Chain
                else {
                    printf("Chain update...\n");
                    previous_block -> next_block = new_block;
                }
                // Update current block
                current_block -> block_size = size;
                current_block -> next_block = NULL;
            }
            
            // Not splitting
            else {
                printf("Not Splitting...\n");
                // Current at head
                if(current_block == free_head) {
                    printf("Head update...\n");
                    free_head = current_block -> next_block;
                }
                // Current in Chain
                else {
                    printf("Chain update...\n");
                    previous_block -> next_block = current_block -> next_block;
                }
                // Update current block
                current_block -> block_size = size;
                current_block -> next_block = NULL;
            }
            // Break the while
            is_allocated = true;
        }
        else {
            previous_block = current_block;
            current_block = current_block -> next_block;
        }
        
    } while (current_block != NULL && !is_allocated);
    return current_block;
}

// Create a function void my_free(void *data), which deallocates a
// value that was allocated on the heap.
// The pointer will be to the data portion of a block;
// move backwards in memory to find the block's overhead information,
// and then link it into the free list.
void my_free(void* data) {
    printf("\n=== MY_FREE ===\n");
    struct Block* free_block = data;
    printf("Free block location: %d\n", (int)free_block);
    free_block -> next_block = free_head;
    free_head = free_block;
}

bool test_1() {
    // Arrange
    my_initialize_heap(HEAP_SIZE);
    bool is_equal = false;
    
    // Act
    void* test_block_1 = my_alloc(sizeof(int));
    printf("Block 1 Location: %d\n", (int)test_block_1);
    my_free(test_block_1);
    void* test_block_2 = my_alloc(sizeof(int));
    printf("Block 2 Location: %d\n", (int)test_block_2);
    
    // Assert
    if(test_block_1 == test_block_2) {
        is_equal = true;
    }
    return is_equal;
}

bool test_2() {
    // Arrange
    my_initialize_heap(HEAP_SIZE);
    bool is_equal = false;
    int distance = VOID_POINTER_SIZE + OVERHEAD_SIZE;
    
    // Act
    void* test_block_1 = my_alloc(sizeof(int));
    printf("Block 1 Location: %d\n", (int)test_block_1);
    void* test_block_2 = my_alloc(sizeof(int));
    printf("Block 2 Location: %d\n", (int)test_block_2);
    
    // Assert
    if(test_block_2 - test_block_1 == distance) {
        is_equal = true;
    }
    return is_equal;
}

bool test_3() {
    // Arrange
    my_initialize_heap(HEAP_SIZE);
    bool is_equal = false;
    int distance = VOID_POINTER_SIZE + OVERHEAD_SIZE;

    // Act
    void* test_block_1 = my_alloc(sizeof(int));
    printf("Test Block 1 Location: %d\n", (int)test_block_1);
    void* test_block_2 = my_alloc(sizeof(int));
    printf("Test Block 2 Location: %d\n", (int)test_block_2);
    void* test_block_3 = my_alloc(sizeof(int));
    printf("Test Block 3 Location: %d\n", (int)test_block_3);
    my_free(test_block_2);
    void* test_block_4 = my_alloc(sizeof(double));
    printf("Test Block 4 Location: %d\n", (int)test_block_4);
    void* test_block_5 = my_alloc(sizeof(char));
    printf("Test Block 5 Location: %d\n", (int)test_block_5);
    
    // Assert
    if((test_block_2 - test_block_1 == distance) &&
       (test_block_3 - test_block_2 == distance) &&
       (test_block_4 - test_block_3 == distance) &&
       (test_block_5 == test_block_2)) {
        is_equal = true;
    }

    return is_equal;
}

bool test_4() {
    // Arrange
    my_initialize_heap(HEAP_SIZE);
    bool is_equal = false;
    int distance = VOID_POINTER_SIZE + OVERHEAD_SIZE;
    
    // Act
    void* test_block_1 = my_alloc(sizeof(char));
    printf("Test Block 1 Location: %d\n", (int)test_block_1);
    void* test_block_2 = my_alloc(sizeof(int));
    printf("Test Block 2 Location: %d\n", (int)test_block_2);
    
    // Assert
    if(test_block_2 - test_block_1 == distance) {
        is_equal = true;
    }
    return is_equal;
}

bool test_5() {
    // Arrange
    my_initialize_heap(HEAP_SIZE);
    bool is_equal = false;
    int distance = sizeof(int[100]) + OVERHEAD_SIZE;
    
    // Act
    void* test_block_1 = my_alloc(sizeof(int[100]));
    void* temp_block_1 = test_block_1;
    printf("Test Block 1 Location: %d\n", (int)test_block_1);
    void* test_block_2 = my_alloc(sizeof(int));
    printf("Test Block 2 Location: %d\n", (int)test_block_2);
    my_free(test_block_1);
    printf("Test Block 2 Location: %d\n", (int)test_block_2);
    
    // Assert
    if(test_block_2 - test_block_1 == distance) {
        is_equal = true;
    }
    if(temp_block_1 != test_block_1) {
        is_equal = false;
    }
    
    return is_equal;
}

// Main Method
int main(int argc, const char * argv[]) {
    printf("**************** RUN MAIN ***************\n");
    printf("Block Size: %d\n", OVERHEAD_SIZE);
    printf("Void Pointer Size: %d\n", VOID_POINTER_SIZE);
    printf("\n************** Test Case 1 **************\n");
    printf("\nTest 1 Result: %d\n", test_1());
    printf("\n************** Test Case 2 **************\n");
    printf("\nTest 2 Result: %d\n", test_2());
    printf("\n************** Test Case 3 **************\n");
    printf("\nTest 3 Result: %d\n", test_3());
    printf("\n************** Test Case 4 **************\n");
    printf("\nTest 4 Result: %d\n", test_4());
    printf("\n************** Test Case 5 **************\n");
    printf("\nTest 5 Result: %d\n", test_5());
    return 0;
}
