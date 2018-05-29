#include "Process.h"

// 프로세스 생성을 위한 함수
ProcessPtr NewProcess(int id, int cbt, int at, int p)
{
	ProcessPtr process = (ProcessPtr) malloc(sizeof(Process));
	process->ID = id;
	process->CPUBurstTime = cbt;
	process->ArrivalTime = at;
	process->Priority = p;
	process->CPURemaningTime = cbt;
	process->IORemaningTime = 0;
	process->WaitingTime = 0;
	process->TurnaroundTime = 0;
	return process;
}

// 큐에 프로세스를 넣는 함수
void InsertProcess(NodePtr* head, ProcessPtr process)
{
	NodePtr NewNode = (NodePtr)malloc(sizeof(Node));
	NodePtr Temp = *head;
	
	NewNode->Process = process;
	NewNode->Next = NULL;

	if (Temp == NULL)
	{
		*head = NewNode;
		return;
	}

	while (Temp->Next != NULL)
	{
		Temp = Temp->Next;
	}
	Temp->Next = NewNode;
}

// 간트차트 그리는 함수
void DrawVerticalGanttChart(NodePtr head)
{
	NodePtr temp = head;
	ProcessPtr prev = -1;
	int time = 0;

	while (temp != NULL)
	{
		if (temp->Process != prev)
		{
			if (prev == -1)
			{
				printf("┌────────┐ 0\n");
			}
			else
			{
				printf("├────────┤ %d\n", time);
			}

			if (temp->Process == NULL)
			{
				printf("│  IDLE  │\n");
			}
			else
			{
				printf("│  %3d   │\n", temp->Process->ID);
			}
		}
		else
		{
			printf("│        │\n");
		}
		time++;
		prev = temp->Process;
		temp = temp->Next;
	}
	printf("└────────┘ %d\n", time);
}

// 모든 프로세스들의 Waiting Time과 Trunaround Time 출력과 Average 정보도 출력
void DrawNodeInformation(NodePtr head)
{
	NodePtr Temp = head;
	int count = 0;
	float AverageWaitingTime = 0;
	float AverageTurnaroundTime = 0;
	printf("┌─────┬──────────────┬─────────────────┐\n");
	printf("│ I D │ Waiting Time │ Trunaround Time │\n");
	printf("├─────┼──────────────┼─────────────────┤\n");
	while (Temp != NULL)
	{
		ProcessPtr process = Temp->Process;
		if (process == NULL)
		{
			Temp = Temp->Next;
			continue;
		}
		AverageWaitingTime += process->WaitingTime;
		AverageTurnaroundTime += process->TurnaroundTime;
		count++;
		printf("│%5d│%14d│%17d│\n", process->ID, process->WaitingTime, process->TurnaroundTime);
		Temp = Temp->Next;
		if (Temp != NULL)
			printf("├─────┼──────────────┤─────────────────┤\n");
	}
	printf("└─────┴──────────────┴─────────────────┘\n");

	AverageWaitingTime = AverageWaitingTime / count;
	AverageTurnaroundTime = AverageTurnaroundTime / count;

	printf("┌──────────────────────┬─────────────────────────┐\n");
	printf("│ Average Waiting Time │ Average Trunaround Time │\n");
	printf("├──────────────────────┼─────────────────────────┤\n");
	printf("│%22.1f│%25.1f│\n", AverageWaitingTime, AverageTurnaroundTime);
	printf("└──────────────────────┴─────────────────────────┘\n");
}

// 디버그를위해 큐의 모든 프로세스 정보를 출력
void DebugNode(NodePtr head)
{
	NodePtr Temp = head;
	printf("┌─────┬─────────────┬────────────────┬───────────────┬──────────┬──────────────┬─────────────────┐\n");
	printf("│ I D │ Arival Time │ CPU Burst Time │ IO Burst Time │ Priority │ Waiting Time │ Trunaround Time │\n");
	printf("├─────┼─────────────┼────────────────┼───────────────┼──────────┼──────────────┤─────────────────┤\n");
	while (Temp != NULL)
	{
		ProcessPtr process = Temp->Process;
		if (process == NULL)
		{
			//PrintError("Process is NULL");
			Temp = Temp->Next;
			continue;
		}
		printf("│%5d│%13d│%16d│%15d│%10d│%14d│%17d│\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority, process->WaitingTime, process->TurnaroundTime);
		Temp = Temp->Next;
		if(Temp != NULL)
			printf("├─────┼─────────────┼────────────────┼───────────────┼──────────┼──────────────┤─────────────────┤\n");
	}
	printf("└─────┴─────────────┴────────────────┴───────────────┴──────────┴──────────────┴─────────────────┘\n");
}

// 디버그를 위해 프로세스의 정보를 출력
void DebugProcess(ProcessPtr process)
{
	if (process == NULL)
		return;
	printf("┌─────┬─────────────┬────────────────┬──────────┬──────────────┬─────────────────┐\n");
	printf("│ I D │ Arival Time │ CPU Burst Time │ Priority │ Waiting Time │ Trunaround Time │\n");
	printf("├─────┼─────────────┼────────────────┼──────────┼──────────────┤─────────────────┤\n");
	printf("│%5d│%13d│%16d│%10d│%14d│%17d│\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->Priority, process->WaitingTime, process->TurnaroundTime);
	printf("└─────┴─────────────┴────────────────┴──────────┴──────────────┴─────────────────┘\n");
}

// 큐에서 프로세스를 삭제하는 함수
void DeleteProcess(NodePtr* head, ProcessPtr process)
{
	NodePtr Temp = *head;
	NodePtr Find = NULL;

	if ((*head)->Process == process)
	{
		if ((*head)->Next != NULL)
		{
			*head = (*head)->Next;
		}
		else
		{
			*head = NULL;
		}

		free(Temp);
		Temp = NULL;
		return;
	}

	while (Temp->Next != NULL)
	{
		if (Temp->Next->Process == process)
		{
			Find = Temp->Next;
			Temp->Next = Temp->Next->Next;
			free(Find);
			Find = NULL;
			return;
		}
		Temp = Temp->Next;
	}

	PrintWarning("Can not find the process");
}

// from 큐에서 to 큐로 process를 이동시키는 함수
void MoveProcess(NodePtr* from, NodePtr* to, ProcessPtr process)
{
	DeleteProcess(from, process);
	InsertProcess(to, process);
}

// 큐의 프로세스 수를 계산하는 함수
int GetNodeLength(NodePtr head)
{
	if (head == NULL)
		return 0;

	return 1 + GetNodeLength(head->Next);
}

// 도착시간이나 우선순위에 따라 프로세스를 찾아 가져오는 함수
ProcessPtr GetProcess(NodePtr head, GetProcessType type, int time)
{
	ProcessPtr (*Func) (NodePtr, ProcessPtr, int);
	switch (type)
	{
		case ARRIVALTIME:
			Func = GetProcessByArrivalTime;
			break;
		case PRIORITY:
			Func = GetProcessByPriority;
			break;
		default:
			PrintError("Invalid GetProcessType");
			return NULL;
	}
	return Func(head, NULL, time);
}

// 도착시간에 따라 프로세스를 가져오는 함수
ProcessPtr GetProcessByArrivalTime(NodePtr head, ProcessPtr process, int time)
{
	if (head == NULL)
	{
		if (process == NULL)
			return NULL;
		else
			return process;
	}

	if (head->Process->ArrivalTime <= time)
	{
		if (process == NULL || head->Process->ArrivalTime < process->ArrivalTime)
			process = head->Process;
		if (head->Process->ArrivalTime == process->ArrivalTime)
			process = head->Process->ID < process->ID ? head->Process : process;
	}
	return GetProcessByArrivalTime(head->Next, process, time);
}

// 우선순위에 따라 프로세스를 가져오는 함수
ProcessPtr GetProcessByPriority(NodePtr head, ProcessPtr process, int time)
{
	if (head == NULL)
	{
		if (process == NULL)
			return NULL;
		else
			return process;
	}

	if (head->Process->ArrivalTime <= time)
	{
		if (process == NULL || head->Process->Priority < process->Priority)
			process = head->Process;
		if (head->Process->ArrivalTime == process->ArrivalTime)
			process = head->Process->ID < process->ID ? head->Process : process;
	}
	
	return GetProcessByPriority(head->Next, process, time);
}