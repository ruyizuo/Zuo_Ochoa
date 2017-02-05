/* ------------------------------------------------------------------------
 phase1.c
 Skeleton file for Phase 1. These routines are very incomplete and are
 intended to give you a starting point. Feel free to use this or not.
 ------------------------------------------------------------------------ */

#include <stddef.h>
#include "usloss.h"
#include "phase1.h"
#include "p3stubs.c"

/* -------------------------- Globals ------------------------------------- */

typedef struct PCB {
    USLOSS_Context       context;
    int                  (*startFunc)(void *);  /* Starting function */
    void                 *startArg;             /* Arg to starting function */
    int                  PID;
    int                  priority;
    int                  tag;
    int                  stacksize;
    int                  *stack;
    int                  status;
    
} PCB;


struct Node{
    PCB data;
    struct Node *next;
}*front = NULL,*rear = NULL;


/* the process table */
PCB procTable[P1_MAXPROC];


/* current process ID */
int pid = -1;
/* size of the process queue */
int queueSize =0;


/* number of processes */

int numProcs = 0;

static int sentinel(void *arg);
static void launch(void);



/* -------------------------- Functions ----------------------------------- */
//queue structure
void insert(int PID)
{
    struct Node *newNode;
    newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = procTable[PID];
	queueSize++; //always update the queue size
    newNode -> next = NULL;
    if(front == NULL)
        front = rear = newNode;
    else{
        rear -> next = newNode;
        rear = newNode;
    }
}



/* ------------------------------------------------------------------------
 Name - dispatcher
 Purpose - runs the highest priority runnable process
 Parameters - none
 Returns - nothing
 Side Effects - runs a process
 ----------------------------------------------------------------------- */


void dispatcher()
{
int i =0;
bool processChanged = false;  //this boolean will be set to true IF we found a process with higher priority to run
int currentHighestPriority = pid; //set global pid to be the highest priority
int positionInProcTable = 	0;  //this var holds the position in which a process is in the ProcTable
PCB highestPriorityProcess;

  while(front != NULL){
	if(front.data.priority < currentHighestPriority){
		currentHighestPriority = front.data.PID;	//save priority value
		positionInProcTable = i;			//save position in table
		highestPriorityProcess = front.data.priority;		//save PCB
		processChanged = true;
	}
    }

  if(processChanged == true){  //we found a higher priority process
	//need to delete it from the queue

	}


}
/* ------------------------------------------------------------------------
 Name - startup
 Purpose - Initializes semaphores, process lists and interrupt vector.
 Start up sentinel process and the P2_Startup process.
 Parameters - none, called by USLOSS
 Returns - nothing
 Side Effects - lots, starts the whole thing
 ----------------------------------------------------------------------- */
void startup(int argc, char **argv)
{
    
    /* initialize the process table here */
    int i =0;
    for(i=0; i<P1_MAXPROC; i++){
        procTable[i].PID = -1;
        
    }
    
    /* Initialize the Ready list, Blocked list, etc. here */
    
    /* Initialize the interrupt vector here */
    
    /* Initialize the semaphores here */
    
    /* startup a sentinel process */
    /* HINT: you don't want any forked processes to run until startup is finished.
     * You'll need to do something  to prevent that from happening.
     * Otherwise your sentinel will start running as soon as you fork it and
     * it will call P1_Halt because it is the only running process.
     */
    P1_Fork("sentinel", sentinel, NULL, USLOSS_MIN_STACK, 6, 0);
    
    /* start the P2_Startup process */
    P1_Fork("P2_Startup", P2_Startup, NULL, 4 * USLOSS_MIN_STACK, 1, 0);
    
    dispatcher();
    
    /* Should never get here (sentinel will call USLOSS_Halt) */
    
    return;
} /* End of startup */

/* ------------------------------------------------------------------------
 Name - finish
 Purpose - Required by USLOSS
 Parameters - none
 Returns - nothing
 Side Effects - none
 ----------------------------------------------------------------------- */
void finish(int argc, char **argv)
{
    USLOSS_Console("Goodbye.\n");
} /* End of finish */

/* ------------------------------------------------------------------------
 Name - P1_Fork
 Purpose - Gets a new process from the process table and initializes
 information of the process.  Updates information in the
 parent process to reflect this child process creation.
 Parameters - the process procedure address, the size of the stack and
 the priority to be assigned to the child process.
 Returns - the process id of the created child or an error code.
 Side Effects - ReadyList is changed, procTable is changed, Current
 process information changed
 ------------------------------------------------------------------------ */
// Use USLOSS_PSR_CURRENT_MODE & USLOSS_PsrGet to check kernel mode
int P1_Fork(char *name, int (*f)(void *), void *arg, int stacksize, int priority, int tag)
{
    int i = 0;
    int result;
    //check if in kernel mode
    if(!(USLOSS_PSR_CURRENT_MODE & USLOSS_psrGet()){
        //throw some sort of error because it was not in kernel mode
    }
       
       //check if the given process has an invalid priorty
       if(priority > 6 ||priority < 1){
           //throw some sorort of error because priority os put of bounds
       }
       
       for (i = 0; i < P1_MAXPROC ; i++) {
           if(procTable[i].PID == -1){
               break;
           }
       }
       int newPid = i;
       //if no more processes, return -1.
       if(newPid >= P1_MAXPROC){
           result = -1;
      }else{
       /* newPid = pid of empty PCB here */
       procTable[newPid].startFunc = f;
       procTable[newPid].startArg = arg;
       procTable[newPid].PID = newPid;
       procTable[newPid].priority = priority;
       procTable[newPid].stacksize = stacksize;
       procTable[newPid].status = 0; /* process is runnable */
       // more stuff here, e.g. allocate stack, page table, initialize context, etc.
       P3_AllocatePageTable(newPid);
       //Assume stack is integer type
       procTable[newPid].stack = (int*)malloc(stacksize * sizeof(int));
       //Not sure initialize context in Fork or dispatcher
       USLOSS_ContextSwitch(NULL,procTable[newPid].context);

       if (priority < procTable[pid].priority) {
           insert(newPid);
           dispatcher();
       }
       
       if(tag != 0 && tag != 1){  //if tage is invalid, return -4
           result = -4;
           
       }else if(stacksize < USLOSS_MIN_STACK){ //if stacksize is less than USLOSS_MIN_STACK, return -2
           result = -2;
           //if no more processes, return -1. But I dont know what means no more processes, does that mean the
           //process table is full?
       }else{
           result = newPid;
       }
           
       }
       return newPid;
       } /* End of fork */
       
    /* ------------------------------------------------------------------------
     Name - launch
     Purpose - Dummy function to enable interrupts and launch a given process
     upon startup.
     Parameters - none
     Returns - nothing
     Side Effects - enable interrupts
     ------------------------------------------------------------------------ */
       void launch(void)
    {
        int  rc;
        int  status;
        status = USLOSS_PsrSet(USLOSS_PsrGet() | USLOSS_PSR_CURRENT_INT);
        if (status != 0) {
            USLOSS_Console("USLOSS_PsrSet failed: %d\n", status);
            USLOSS_Halt(1);
        }
        rc = procTable[pid].startFunc(procTable[pid].startArg);
        /* quit if we ever come back */
        P1_Quit(rc);
    } /* End of launch */
       
    /* ------------------------------------------------------------------------
     Name - P1_Quit
     Purpose - Causes the process to quit and wait for its parent to call P1_Join.
     Parameters - quit status
     Returns - nothing
     Side Effects - the currently running process quits
     ------------------------------------------------------------------------ */
       void P1_Quit(int status) {
           // Do something here.
       }
       
    /* ------------------------------------------------------------------------
     Name - P1_GetState
     Purpose - gets the state of the process
     Parameters - process PID
     Returns - process state
     Side Effects - none
     ------------------------------------------------------------------------ */
       int P1_GetState(int PID) {
           return procTable[PID].PID;
       }
       
    /* ------------------------------------------------------------------------
     Name - sentinel
     Purpose - The purpose of the sentinel routine is two-fold.  One
     responsibility is to keep the system going when all other
     processes are blocked.  The other is to detect and report
     simple deadlock states.
     Parameters - none
     Returns - nothing
     Side Effects -  if system is in deadlock, print appropriate error
     and halt.
     ----------------------------------------------------------------------- */
       int sentinel (void *notused)
    {
        while (numProcs > 1)
        {
            /* Check for deadlock here */
            USLOSS_WaitInt();
        }
        USLOSS_Halt(0);
        /* Never gets here. */
        return 0;
    } /* End of sentinel */
