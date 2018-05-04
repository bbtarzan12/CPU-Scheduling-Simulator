#include "Process.h"

void InsertProcess(Node** head, ProcessPtr process)
{
	Node* NewNode = (Node*)malloc(sizeof(Node));
	Node* Temp = *head;
	
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

void DrawGanttChart(Node* head)
{
	Node* temp = head;
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

void DebugNode(Node* head)
{
	Node* Temp = head;
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� I D��  Arival Time��  CPU Burst Time��  I/O Burst Time��  Priority ��\n");
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
	while (Temp != NULL)
	{
		ProcessPtr process = Temp->Process;
		if (process == NULL)
		{
			//PrintError("Process is NULL");
			Temp = Temp->Next;
			continue;
		}
		printf("��%4d��\t%11d��\t%12d��\t%13d��\t%8d ��\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
		Temp = Temp->Next;
		if(Temp != NULL)
			printf("������������������������������������������������������������������������������������������������������������������������������������\n");
	}
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
}

void DebugProcess(ProcessPtr process)
{
	if (process == NULL)
		return;
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� I D��  Arival Time��  CPU Burst Time��  I/O Burst Time��  Priority ��\n");
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("��%4d��\t%11d��\t%12d��\t%13d��\t%8d ��\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
	printf("������������������������������������������������������������������������������������������������������������������������������������\n");
}

void DeleteProcess(Node** head, ProcessPtr process)
{
	Node* Temp = *head;
	Node* Find = NULL;

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

void MoveProcess(Node** from, Node** to, ProcessPtr process)
{
	DeleteProcess(from, process);
	InsertProcess(to, process);
}

int GetNodeLength(Node* head)
{
	if (head == NULL)
		return 0;

	return 1 + GetNodeLength(head->Next);
}

ProcessPtr GetProcess(Node* head, GetProcessType type, int time)
{
	ProcessPtr (*Func) (Node*, ProcessPtr, int);
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


ProcessPtr GetProcessByArrivalTime(Node* head, ProcessPtr process, int time)
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

ProcessPtr GetProcessByPriority(Node* head, ProcessPtr process, int time)
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

