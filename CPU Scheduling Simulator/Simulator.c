#include "Simulator.h"

void main(int argc)
{
	if (argc == 1)
	{
		printf("Enter the number of processes : ");
		scanf("%d", &argc);
	}
	Init(argc);
	//DebugInit();
	Evaluation();
}

void Evaluation()
{
	NodePtr GanttChart = Update(RR, FALSE, 4);
	DrawVerticalGanttChart(GanttChart);
	DrawNodeInformation(TerminatedQueue);
	free(GanttChart);
}

void DebugInit()
{
	//									id cbt at p
	InsertProcess(&JobQueue, NewProcess(1, 5, 0, 1));
	InsertProcess(&JobQueue, NewProcess(2, 3, 1, 2));
	InsertProcess(&JobQueue, NewProcess(3, 8, 2, 3));
	InsertProcess(&JobQueue, NewProcess(3, 6, 3, 4));
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
		process->ID = ID_I ? i : randID;
		process->CPUBurstTime = rand() % 20 + 1;
		process->IOBurstTime = rand() % 5 + 1;
		process->ArrivalTime = ARRIVALTIME_I ? i : rand() % 20 + 1;
		process->Priority = numbers[i];
		process->CPURemaningTime = process->CPUBurstTime;
		process->IORemaningTime = 0;
		process->WaitingTime = 0;
		process->TurnaroundTime = 0;
		InsertProcess(&JobQueue, process);
		randID += rand() % 30 + 1;
		free(process);
	}
	free(numbers);
}

NodePtr Update(AlgorithmType type, bool preemptive, int timeQuantum)
{
	DebugNode(JobQueue);
	int time = 0;
	TimeConsumed = 0;
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

ProcessPtr Schedule(AlgorithmType type, bool preemptive, int timeQuantum)
{
	switch (type)
	{
		case FCFS:
			return FCFSAlgorithm();
		case SJF:
			return SJFAlgorithm(preemptive);
		case P:
			return PriorityAlgorithm(preemptive);
		case RR:
			return RRAlgorithm(timeQuantum);
		default:
			PrintError("Invalid AlgorithmType");
			return NULL;
	}
}

ProcessPtr Simulate(int time, AlgorithmType type, bool preemptive, int timeQuantum)
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

	if (PrevProcess != RunningProcess)
	{
		TimeConsumed = 0;
	}

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
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process;
	if (Process != NULL)
	{
		if (RunningProcess != NULL)
		{
			return RunningProcess;
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

ProcessPtr SJFAlgorithm(bool preemtive)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process;
	if (Process != NULL)
	{
		NodePtr temp = ReadyQueue;
		while (temp != NULL)
		{

			if (temp->Process->CPURemaningTime <= Process->CPURemaningTime)
			{
				if (temp->Process->CPURemaningTime == Process->CPURemaningTime)
				{
					if (temp->Process->ArrivalTime <= Process->ArrivalTime)
						Process = temp->Process;
				}
				else
				{
					Process = temp->Process;
				}
			}
			temp = temp->Next;
		}

		if (RunningProcess != NULL)
		{
			if (preemtive)
			{

				if (RunningProcess->CPURemaningTime >= Process->CPURemaningTime)
				{
					if (RunningProcess->CPURemaningTime == Process->CPURemaningTime)
					{
						if (RunningProcess->ArrivalTime <= Process->ArrivalTime)
							return RunningProcess;
					}

					InsertProcess(&ReadyQueue, RunningProcess);
					RunningProcess = NULL;
					DeleteProcess(&ReadyQueue, Process);
					return Process;
				}
				return RunningProcess;
			}
			else
			{
				return RunningProcess;
			}
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

ProcessPtr PriorityAlgorithm(bool preemtive)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process;
	if (Process != NULL)
	{
		NodePtr temp = ReadyQueue;
		while (temp != NULL)
		{

			if (temp->Process->Priority <= Process->Priority)
			{
				if (temp->Process->Priority == Process->Priority)
				{
					if (temp->Process->ArrivalTime <= Process->ArrivalTime)
						Process = temp->Process;
				}
				else
				{
					Process = temp->Process;
				}
			}
			temp = temp->Next;
		}

		if (RunningProcess != NULL)
		{
			if (preemtive)
			{

				if (RunningProcess->Priority >= Process->Priority)
				{
					if (RunningProcess->Priority == Process->Priority)
					{
						if (RunningProcess->ArrivalTime <= Process->ArrivalTime)
							return RunningProcess;
					}

					InsertProcess(&ReadyQueue, RunningProcess);
					RunningProcess = NULL;
					DeleteProcess(&ReadyQueue, Process);
					return Process;
				}
				return RunningProcess;
			}
			else
			{
				return RunningProcess;
			}
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

ProcessPtr RRAlgorithm(int timeQuantum)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process;
	if (Process != NULL)
	{
		if (RunningProcess != NULL)
		{
			if (TimeConsumed >= timeQuantum)
			{
				InsertProcess(&ReadyQueue, RunningProcess);
				RunningProcess = NULL;
				DeleteProcess(&ReadyQueue, Process);
				return Process;
			}
			else
			{
				return RunningProcess;
			}
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
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
	TimeConsumed++;

	if (RANDOM_IO && rand() % 5 == 0)
	{
		RunningProcess->IORemaningTime += RunningProcess->IOBurstTime;
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
