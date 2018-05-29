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

typedef Node* NodePtr;

enum GetProcessTypeEnum { ARRIVALTIME, PRIORITY } typedef GetProcessType;

ProcessPtr NewProcess(int id, int cbt, int at, int p);
void InsertProcess(NodePtr* head, ProcessPtr process);
void DeleteProcess(NodePtr* head, ProcessPtr process);
void MoveProcess(NodePtr* from, NodePtr* to, ProcessPtr process);
int GetNodeLength(NodePtr head);


void DrawVerticalGanttChart(NodePtr head);
void DrawNodeInformation(NodePtr head);
void DebugNode(NodePtr head);
void DebugProcess(ProcessPtr process);

ProcessPtr GetProcess(NodePtr head, GetProcessType type, int time);
ProcessPtr GetProcessByArrivalTime(NodePtr head, ProcessPtr process, int time);
ProcessPtr GetProcessByPriority(NodePtr head, ProcessPtr process, int time);
