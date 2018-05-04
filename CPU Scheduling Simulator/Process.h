#include "Helper.h"

struct process
{
	int ID;
	int CPUBurstTime;
	int IOBurstTime;
	int ArrivalTime;
	int Priority;
	int CPURemaningTime;
	int IORemaningTime;
	int WaitingTime;
	int TurnaroundTime;
} typedef Process;

typedef Process* ProcessPtr;

struct node
{
	ProcessPtr Process;
	struct node *Next;
} typedef Node;

enum GetProcessTypeEnum { ARRIVALTIME, PRIORITY } typedef GetProcessType;

void InsertProcess(Node** head, ProcessPtr process);
void DeleteProcess(Node** head, ProcessPtr process);
void MoveProcess(Node** from, Node** to, ProcessPtr process);
int GetNodeLength(Node* head);

void DrawGanttChart(Node* head);
void DebugNode(Node* head);
void DebugProcess(ProcessPtr process);

ProcessPtr GetProcess(Node* head, GetProcessType type, int time);
ProcessPtr GetProcessByArrivalTime(Node* head, ProcessPtr process, int time);
ProcessPtr GetProcessByPriority(Node* head, ProcessPtr process, int time);
