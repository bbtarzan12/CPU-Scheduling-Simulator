#include "Process.h"

void InsertProcess(Node** head, Process* process)
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


void DebugNode(Node* head)
{
	Node* Temp = head;

	printf("┌────┬─────────────┬────────────────┬────────────────┬───────────┐\n");
	printf("│ I D│  Arival Time│  CPU Burst Time│  I/O Burst Time│  Priority │\n");
	printf("├────┼─────────────┼────────────────┼────────────────┼───────────┤\n");
	while (Temp != NULL)
	{
		Process* process = Temp->Process;
		if (process == NULL)
		{
			PrintError("Process is NULL");
		}
		printf("│%4d│\t%11d│\t%12d│\t%13d│\t%8d │\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
		Temp = Temp->Next;
		if(Temp != NULL)
			printf("├────┼─────────────┼────────────────┼────────────────┼───────────┤\n");
	}
	printf("└────┴─────────────┴────────────────┴────────────────┴───────────┘\n");
}

void DebugProcess(Process* process)
{
	printf("┌────┬─────────────┬────────────────┬────────────────┬───────────┐\n");
	printf("│ I D│  Arival Time│  CPU Burst Time│  I/O Burst Time│  Priority │\n");
	printf("├────┼─────────────┼────────────────┼────────────────┼───────────┤\n");
	printf("│%4d│\t%11d│\t%12d│\t%13d│\t%8d │\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
	printf("└────┴─────────────┴────────────────┴────────────────┴───────────┘\n");
}

void DeleteProcess(Node** head, Process* process)
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

int GetNodeLength(Node* head)
{
	if (head == NULL)
		return 0;

	return 1 + GetNodeLength(head->Next);
}

Process* GetProcess(Node* head, GetProcessType type)
{
	Process* (*Func) (Node*, int);
	switch (type)
	{
		case ArrivalTime:
			Func = GetProcessByArrivalTime;
			break;
		case Priority:
			Func = GetProcessByPriority;
			break;
		default:
			break;
	}
	return Func(head, -1);
}

Process* GetProcessByArrivalTime(Node* head, int time)
{
	if (head == NULL)
	{
		if (time == -1)
			return NULL;
		else
			return head->Process;
	}

	if (time == -1 || head->Process->ArrivalTime < time)
		time = head->Process->ArrivalTime;

	return GetProcessByArrivalTime(head->Next, time);
}

Process* GetProcessByPriority(Node* head, int priority)
{
	if (head == NULL)
	{
		if (priority == -1)
			return NULL;
		else
			return head->Process;
	}

	if (priority == -1 || head->Process->Priority < priority)
		priority = head->Process->Priority;

	return GetProcessByArrivalTime(head->Next, priority);
}

