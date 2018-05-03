#include "Helper.h"

struct process
{
	int ID;
	int CPUBurstTime;
	int IOBurstTime;
	int ArrivalTime;
	int Priority;
} typedef Process;


struct node
{
	Process* Process;
	struct node *Next;
} typedef Node;

enum GetProcessTypeEnum { ArrivalTime, Priority } typedef GetProcessType;


Node* ReadyQueue;

void InsertProcess(Node** head, Process* process);
void DebugNode(Node* head);
void DeleteProcess(Node** head, Process* process);
Process* GetProcess(Process* list, GetProcessType type);
Process* GetProcessByArrivalTime(Process* list);
Process* GetProcessByPriority(Process* list);