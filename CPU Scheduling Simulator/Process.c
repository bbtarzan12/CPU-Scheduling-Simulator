#include "Process.h"

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
				printf("�������������������� 0\n");
			}
			else
			{
				printf("�������������������� %d\n", time);
			}

			if (temp->Process == NULL)
			{
				printf("��  IDLE  ��\n");
			}
			else
			{
				printf("��  %3d   ��\n", temp->Process->ID);
			}
		}
		else
		{
			printf("��        ��\n");
		}
		time++;
		prev = temp->Process;
		temp = temp->Next;
	}
	printf("�������������������� %d\n", time);
}

void DrawNodeInformation(NodePtr head)
{
	NodePtr Temp = head;
	int count = 0;
	float AverageWaitingTime = 0;
	float AverageTurnaroundTime = 0;
	printf("��������������������������������������������������������������������������������\n");
	printf("�� I D �� Waiting Time �� Trunaround Time ��\n");
	printf("��������������������������������������������������������������������������������\n");
	while (Temp != NULL)
	{
		ProcessPtr process = Temp->Process;
		if (process == NULL)
		{
			//PrintError("Process is NULL");
			Temp = Temp->Next;
			continue;
		}
		AverageWaitingTime += process->WaitingTime;
		AverageTurnaroundTime += process->TurnaroundTime;
		count++;
		printf("��%5d��%14d��%17d��\n", process->ID, process->WaitingTime, process->TurnaroundTime);
		Temp = Temp->Next;
		if (Temp != NULL)
			printf("��������������������������������������������������������������������������������\n");
	}
	printf("��������������������������������������������������������������������������������\n");

	AverageWaitingTime = AverageWaitingTime / count;
	AverageTurnaroundTime = AverageTurnaroundTime / count;

	printf("����������������������������������������������������������������������������������������������������\n");
	printf("�� Average Waiting Time �� Average Trunaround Time ��\n");
	printf("����������������������������������������������������������������������������������������������������\n");
	printf("��%22.1f��%25.1f��\n", AverageWaitingTime, AverageTurnaroundTime);
	printf("����������������������������������������������������������������������������������������������������\n");
}

void DebugNode(NodePtr head)
{
	NodePtr Temp = head;
	printf("����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� I D �� Arival Time �� CPU Burst Time �� IO Burst Time �� Priority �� Waiting Time �� Trunaround Time ��\n");
	printf("����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	while (Temp != NULL)
	{
		ProcessPtr process = Temp->Process;
		if (process == NULL)
		{
			//PrintError("Process is NULL");
			Temp = Temp->Next;
			continue;
		}
		printf("��%5d��%13d��%16d��%15d��%10d��%14d��%17d��\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority, process->WaitingTime, process->TurnaroundTime);
		Temp = Temp->Next;
		if(Temp != NULL)
			printf("����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	}
	printf("����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
}

void DebugProcess(ProcessPtr process)
{
	if (process == NULL)
		return;
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� I D �� Arival Time �� CPU Burst Time �� Priority �� Waiting Time �� Trunaround Time ��\n");
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("��%5d��%13d��%16d��%10d��%14d��%17d��\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->Priority, process->WaitingTime, process->TurnaroundTime);
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
}

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
		return;
	}

	while (Temp->Next != NULL)
	{
		if (Temp->Next->Process == process)
		{
			Find = Temp->Next;
			Temp->Next = Temp->Next->Next;
			free(Find);
			return;
		}
		Temp = Temp->Next;
	}

	PrintWarning("Can not find the process");
}

void MoveProcess(NodePtr* from, NodePtr* to, ProcessPtr process)
{
	DeleteProcess(from, process);
	InsertProcess(to, process);
}

int GetNodeLength(NodePtr head)
{
	if (head == NULL)
		return 0;

	return 1 + GetNodeLength(head->Next);
}

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