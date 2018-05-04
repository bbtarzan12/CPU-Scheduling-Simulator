#include "Process.h"

enum AlgorithmTypeEnum { FCFS, N_SJF, P_SJF, N_Prirority, P_Priority, RR } typedef AlgorithmType;

void Init(int size);
void CreateProcess(int size);
NodePtr Update(AlgorithmType type, int preemptive, int timeQuantum);
ProcessPtr Schedule(AlgorithmType type, int preemptive, int timeQuantum);
ProcessPtr Simulate(int time, AlgorithmType type, int preemptive, int timeQuantum );
void Evaluation(Node* Terminated);

void WaitAllProcess(NodePtr head);
void PerformIOOperation(NodePtr head);
ProcessPtr ExecuteRunningProcess();

ProcessPtr FCFSAlgorithm();

NodePtr JobQueue;
NodePtr ReadyQueue;
ProcessPtr RunningProcess;
NodePtr WaitingQueue;
NodePtr TerminatedQueue;

int SimulateTime = -1;