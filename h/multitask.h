#pragma once
#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 4096

typedef enum {
    TASK_CPU,
    TASK_GPU
} task_type_t;

typedef void (*task_func_t)(void*);

typedef struct {
    uint32_t* stack;
    uint32_t* stack_top;
    task_func_t func;
    void* arg;
    uint32_t regs[8];
    uint32_t eip;
    int active;
    task_type_t type;
    uint32_t* page_directory; // <--- własny katalog stron (CR3)
} task_t;

extern task_t tasks[MAX_TASKS];
extern int current_task;
extern uint32_t* clone_kernel_page_directory();
extern void switch_page_directory(uint32_t* pd);
task_t* t;
int create_task(task_func_t func, void* arg, task_type_t type){
    t->page_directory = clone_kernel_page_directory();
};
void task_yield();
void task_start();
void context_switch(int next_task);