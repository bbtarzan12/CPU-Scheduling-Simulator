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

	printf("<Debug> ");
	while (Temp != NULL)
	{
		if (Temp->Process == NULL)
		{
			PrintError("Process is NULL");
		}
		printf("%d > ", Temp->Process->ID);
		Temp = Temp->Next;
	}
	printf("End\n");
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

Process * GetProcess(Process* list, GetProcessType type)
{
	Process* (*Func) (Process*);
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
	return Func(list);
}

Process* GetProcessByArrivalTime(Process* list)
{
	return NULL;
}

Process* GetProcessByPriority(Process* list)
{
	return NULL;
}

