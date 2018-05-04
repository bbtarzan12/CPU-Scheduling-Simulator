#include "Simulator.h"

void main(int argc)
{
	if (argc == 1)
	{
		printf("Enter the number of processes : ");
		scanf("%d", &argc);
	}
	Init(argc);
	NodePtr GanttChart = Update(FCFS, 0, 0);
	DrawGanttChart(GanttChart);
	DrawNodeInformation(TerminatedQueue);
}

void Init(int size)
{
	CreateProcess(size);
}

void CreateProcess(int size)
{
	int* numbers = (int*)malloc(sizeof(int) * size);
	srand(time(NULL));

	int randID = rand() % 10 + 1;

	for (int i = 0; i < size; i++)
	{
		numbers[i] = i;
	}

	for (int i = 0; i < size; i++)
	{
		int n, temp;
		n = rand() % size;
		temp = numbers[n];
		numbers[n] = numbers[i];
		numbers[i] = temp;
	}

	for (int i = 0; i < size; i++)
	{
		ProcessPtr process = (ProcessPtr)malloc(sizeof(Process));
		process->ID = randID;
		process->CPUBurstTime = rand() % 20 + 1;
		process->ArrivalTime = rand() % 20 + 1;
		process->Priority = numbers[i];
		process->CPURemaningTime = process->CPUBurstTime;
		process->IORemaningTime = 0;
		process->WaitingTime = 0;
		process->TurnaroundTime = 0;
		InsertProcess(&JobQueue, process);
		randID += rand() % 30 + 1;
	}
}

NodePtr Update(AlgorithmType type, int preemptive, int timeQuantum)
{
	DebugNode(JobQueue);
	int time = 0;
	NodePtr GanttNode = NULL;
	while (TRUE)
	{
		if (JobQueue == NULL && ReadyQueue == NULL && WaitingQueue == NULL && RunningProcess == NULL)
			break;
		ProcessPtr process = Simulate(time, type, preemptive, timeQuantum);
		InsertProcess(&GanttNode, process);
		time++;
	}
	return GanttNode;
}

ProcessPtr Schedule(AlgorithmType type, int preemptive, int timeQuantum)
{
	switch (type)
	{
		case FCFS:
			return FCFSAlgorithm();
		default:
			PrintError("Invalid AlgorithmType");
			return NULL;
	}
}

ProcessPtr Simulate(int time, AlgorithmType type, int preemptive, int timeQuantum)
{
	SimulateTime++;
	while (TRUE)
	{
		ProcessPtr Temp = GetProcess(JobQueue, ARRIVALTIME, time);
		if (Temp == NULL)
			break;
		MoveProcess(&JobQueue, &ReadyQueue, Temp);
	}

	ProcessPtr PrevProcess = RunningProcess;
	RunningProcess = Schedule(type, preemptive, timeQuantum);

	WaitAllProcess(ReadyQueue);
	PerformIOOperation(WaitingQueue);
	if (RunningProcess != NULL)
	{
		return ExecuteRunningProcess();
	}
	else
	{
		return NULL;
	}
}

ProcessPtr FCFSAlgorithm()
{
	ProcessPtr process = ReadyQueue == NULL ? NULL : ReadyQueue->Process;
	if (process != NULL)
	{
		if (RunningProcess != NULL)
		{
			return RunningProcess;
		}
		else
		{
			DeleteProcess(&ReadyQueue, process);
			return process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

void WaitAllProcess(NodePtr head)
{
	if (head == NULL)
		return;

	ProcessPtr Process = head->Process;
	Process->TurnaroundTime++;
	Process->WaitingTime++;

	WaitAllProcess(head->Next);
}

void PerformIOOperation(NodePtr head)
{
	if (head == NULL)
		return;

	ProcessPtr Process = head->Process;
	Process->TurnaroundTime++;
	Process->IORemaningTime--;

	if (Process->IORemaningTime <= 0)
	{
		head = (head->Next != NULL && head->Next->Next != NULL) ? head->Next->Next : NULL;
		MoveProcess(&WaitingQueue, &ReadyQueue, Process);
		if(head != NULL)
			PerformIOOperation(head);
	}
	else
	{
		PerformIOOperation(head->Next);
	}
}

ProcessPtr ExecuteRunningProcess()
{
	ProcessPtr temp = RunningProcess;
	RunningProcess->CPURemaningTime--;
	RunningProcess->TurnaroundTime++;

	if (RANDOM_IO && rand() % 5 == 0)
	{
		RunningProcess->IORemaningTime += rand() % 10;
	}

	if (RunningProcess->CPURemaningTime <= 0)
	{
		InsertProcess(&TerminatedQueue, RunningProcess);
		RunningProcess = NULL;
		return temp;
	}
	else if (RunningProcess->IORemaningTime > 0)
	{
		InsertProcess(&WaitingQueue, RunningProcess);
		RunningProcess = NULL;
		return temp;
	}
	return RunningProcess;
}
