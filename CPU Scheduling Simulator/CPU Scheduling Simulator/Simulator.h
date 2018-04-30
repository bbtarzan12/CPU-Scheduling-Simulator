#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum ScheduleType { FCFS, N_SJF, P_SJF, N_Prirority, P_Priority, RR };

struct process
{
	int ID;
	int CPUBurstTime;
	int IOBurstTime;
	int ArrivalTime;
	int Priority;
} typedef Process;

Process* ProcessList;
int Size;

void Init(int size);
void CreateProcess(int size);
void Config();
void Schedule();
void FCFSSchedule();
void Evaluation();

Process* CreateTempList();