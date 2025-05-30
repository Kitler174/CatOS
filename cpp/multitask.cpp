#include "../h/multitask.h"
#include <stdlib.h>
#include <string.h>

task_t tasks[MAX_TASKS];
int num_tasks = 0;
int current_task = 0;

extern void context_switch_asm(uint32_t* old_regs, uint32_t* new_regs, uint32_t* old_eip, uint32_t new_eip);

int create_task(task_func_t func, void* arg, task_type_t type) {
    if (num_tasks >= MAX_TASKS) return -1;
    task_t* t = &tasks[num_tasks];

    t->stack = (uint32_t*)malloc(STACK_SIZE);
    t->stack_top = t->stack + (STACK_SIZE/sizeof(uint32_t));
    memset(t->stack, 0, STACK_SIZE);
    t->func = func;
    t->arg = arg;
    t->active = 1;
    t->type = type;

    // Ustawienie "fałszywego" stosu: pierwsze wywołanie taska wraca do task_exit
    uint32_t* sp = t->stack_top;
    *--sp = (uint32_t)t->arg;           // argument
    *--sp = (uint32_t)task_exit;        // "ret" po skończeniu
    t->regs[3] = (uint32_t)sp;          // ESP
    t->eip = (uint32_t)t->func;         // EIP

    num_tasks++;
    return num_tasks - 1;
}

// Zakończenie taska (nie wraca)
void task_exit() {
    tasks[current_task].active = 0;
    task_yield();
    while (1); // nie wraca
}

// Przełączenie do następnego aktywnego taska (round-robin)
void task_yield() {
    int next = (current_task + 1) % num_tasks;
    while (!tasks[next].active) next = (next + 1) % num_tasks;
    if (next != current_task)
        context_switch(next);
}

// Rozpoczęcie multitaskingu
void task_start() {
    current_task = 0;
    context_switch(current_task);
}

// Przełączenie kontekstu
void context_switch(int next_task) {
    static uint32_t* old_eip = NULL;
    int prev = current_task;
    current_task = next_task;
    context_switch_asm(tasks[prev].regs, tasks[next_task].regs, &tasks[prev].eip, tasks[next_task].eip);
}