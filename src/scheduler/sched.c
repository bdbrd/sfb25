#include <stdio.h>
#include <string.h>
#include <SFB25.h>
#include "../hal/smp.h"
#include <error.h>
#include "../mm/kmalloc.h"
#include "sched.h"

extern context *save_context();

extern void switch_context(context *old, context *new);

#define QUANTUM_US 10000

int next_pid = 1;

void idle_task(){
    kprintf("Hello world from bruhd task!\n");
}

void test_task(){
    kprintf("Hello world from scheduled task!\n");
    return;
}

/* Setup a process structure */
proc *alloc_process(void){
    proc *proc_list = get_cpu_struct()->process_list;

    for(uint64_t i = 0; i < PROC_MAX; i++){
        if(proc_list[i].state == UNUSED){
            proc *p = &proc_list[i];

            p->state = READY;
            kprintf("pstate = 0x{x}\n", READY);
            kprintf("actual: 0x{x}\n", p->state);
            p->kstack = kmalloc(INITIAL_STACK_SIZE);

            p->pid = next_pid++;

            memset(&p->context, 0, sizeof(context));

            p->context.rbp = (uint64_t)p->kstack;
            p->context.rsp = (uint64_t)p->context.rbp + INITIAL_STACK_SIZE;
            
            kprintf("actua2l: 0x{x}\n", p->state);

            return p;
        }
    }

    kprintf("Couldn't find free process!");

}
kstatus add_task(uint64_t *entry){
    proc *proc = alloc_process();

    if (proc == NULL) {
        klog(LOG_ERROR, __func__, "proc == null!");
        kkill();
    }

    proc->context.rip = (uint64_t)entry;

    return KERNEL_STATUS_SUCCESS;
}

void scheduler_init(){
    if(!get_cpu_struct_initialized()){
        kprintf("sched: cpu struct not initialized!");
        kkill();
    }

    cpu_state *state = get_cpu_struct();

    if(state->current_process != NULL){
        kprintf("sched: scheduler on CPU {d} already initialized!\n", state->lapic_id);
        kkill();
    }

    kprintf("hi1\n");
    proc *proc_list = state->process_list;

    memset(proc_list, 0, sizeof(proc) * 512);

    int pid = add_task((uint64_t*)idle_task);
    add_task((uint64_t*)test_task);

    for(;;){
        for(int i = 0; i < PROC_MAX; i++){
            if(proc_list[i].state == READY){

                kprintf("Hell yeah");
                
                context old_state = state->current_process->context;

                state->current_process = &proc_list[i];
                state->current_process->state = RUNNING;

                switch_context(&old_state, &state->current_process->context);

            }
        }
    }
}

void scheduler_tick(){
    cpu_state *state = get_cpu_struct();
    proc *proc_list = state->process_list;

    switch_context(&state->current_process->context, &state->scheduler_context);


}