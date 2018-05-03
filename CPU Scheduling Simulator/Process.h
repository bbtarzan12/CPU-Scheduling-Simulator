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

enum GetProcessTypeEnum { ARRIVALTIME, PRIORITY } typedef GetProcessType;

void InsertProcess(Node** head, Process* process);
void DeleteProcess(Node** head, Process* process);
int GetNodeLength(Node* head);

void DebugNode(Node* head);
void DebugProcess(Process* process);

Process* GetProcess(Node* head, GetProcessType type);
Process* GetProcessByArrivalTime(Node* head, Process* process);
Process* GetProcessByPriority(Node* head, Process* process);