#include "Process.h"

enum AlgorithmTypeEnum { FCFS, N_SJF, P_SJF, N_Prirority, P_Priority, RR } typedef AlgorithmType;

void Init(int size);
void CreateProcess(int size);
ProcessPtr Schedule(AlgorithmType type, int preemptive, int timeQuantum);
ProcessPtr Simulate(int time, AlgorithmType type, int preemptive, int timeQuantum );
void Evaluation();

void WaitAllProcess(Node* head);
void PerformIOOperation(Node* head);
ProcessPtr ExecuteRunningProcess();

ProcessPtr FCFSAlgorithm();

Node* JobQueue;
Node* ReadyQueue;
ProcessPtr RunningProcess;
Node* WaitingQueue;
Node* TerminatedQueue;

int SimulateTime = -1;