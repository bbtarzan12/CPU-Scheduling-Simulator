#include "Process.h"

enum AlgorithmTypeEnum { FCFS, SJF, P, RR } typedef AlgorithmType;

void DebugInit();
void Init(int size);
void CreateProcess(int size);
NodePtr Update(AlgorithmType type, bool preemptive, int timeQuantum);
ProcessPtr Schedule(AlgorithmType type, bool preemptive, int timeQuantum);
ProcessPtr Simulate(int time, AlgorithmType type, bool preemptive, int timeQuantum );
void Evaluation();

void WaitAllProcess(NodePtr head);
void PerformIOOperation(NodePtr head);
ProcessPtr ExecuteRunningProcess();

ProcessPtr FCFSAlgorithm();
ProcessPtr SJFAlgorithm(bool preemtive);
ProcessPtr PriorityAlgorithm(bool preemtive);
ProcessPtr RRAlgorithm(int timeQuantum);

NodePtr JobQueue;
NodePtr ReadyQueue;
ProcessPtr RunningProcess;
NodePtr WaitingQueue;
NodePtr TerminatedQueue;

int SimulateTime = -1;
int TimeConsumed = 0;