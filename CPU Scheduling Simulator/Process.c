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
	printf("忙式式式式成式式式式式式式式式式式式式成式式式式式式式式式式式式式式式式成式式式式式式式式式式式式式式式式成式式式式式式式式式式式忖\n");
	printf("弛 I D弛  Arival Time弛  CPU Burst Time弛  I/O Burst Time弛  Priority 弛\n");
	printf("戍式式式式托式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式扣\n");
	while (Temp != NULL)
	{
		Process* process = Temp->Process;
		if (process == NULL)
		{
			PrintError("Process is NULL");
		}
		printf("弛%4d弛\t%11d弛\t%12d弛\t%13d弛\t%8d 弛\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
		Temp = Temp->Next;
		if(Temp != NULL)
			printf("戍式式式式托式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式扣\n");
	}
	printf("戌式式式式扛式式式式式式式式式式式式式扛式式式式式式式式式式式式式式式式扛式式式式式式式式式式式式式式式式扛式式式式式式式式式式式戎\n");
}

void DebugProcess(Process* process)
{
	printf("忙式式式式成式式式式式式式式式式式式式成式式式式式式式式式式式式式式式式成式式式式式式式式式式式式式式式式成式式式式式式式式式式式忖\n");
	printf("弛 I D弛  Arival Time弛  CPU Burst Time弛  I/O Burst Time弛  Priority 弛\n");
	printf("戍式式式式托式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式式式式式式托式式式式式式式式式式式扣\n");
	printf("弛%4d弛\t%11d弛\t%12d弛\t%13d弛\t%8d 弛\n", process->ID, process->ArrivalTime, process->CPUBurstTime, process->IOBurstTime, process->Priority);
	printf("戌式式式式扛式式式式式式式式式式式式式扛式式式式式式式式式式式式式式式式扛式式式式式式式式式式式式式式式式扛式式式式式式式式式式式戎\n");
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
	Process* (*Func) (Node*, Process*);
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
	return Func(head, NULL);
}

Process* GetProcessByArrivalTime(Node* head, Process* process)
{
	if (head == NULL)
	{
		if (process == NULL)
			return NULL;
		else
			return process;
	}

	if (process == NULL || head->Process->ArrivalTime < process->ArrivalTime)
		process = head->Process;
	if (head->Process->ArrivalTime == process->ArrivalTime)
		process = head->Process->ID < process->ID ? head->Process : process;
	return GetProcessByArrivalTime(head->Next, process);
}

Process* GetProcessByPriority(Node* head, Process* process)
{
	if (head == NULL)
	{
		if (process == NULL)
			return NULL;
		else
			return process;
	}
	if (process == NULL || head->Process->Priority < process->Priority)
		process = head->Process;
	if (head->Process->ArrivalTime == process->ArrivalTime)
		process = head->Process->ID < process->ID ? head->Process : process;
	
	return GetProcessByPriority(head->Next, process);
}

