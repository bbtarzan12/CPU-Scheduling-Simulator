#include "Simulator.h"

void main(int argc)
{
	if (argc == 1)
	{
		printf("Enter the number of processes : ");
		scanf("%d", &argc);
	}
	Init(argc);
	FCFSSchedule();
}

void Init(int size)
{
	Size = size;
	CreateProcess(size);
}

void CreateProcess(int size)
{
	int* numbers = (int*)malloc(sizeof(int) * size);
	srand(time(NULL));

	int randID = rand() % 10 + 1;

	for (int i = 0; i < size; i++)
	{
		numbers[i] = i;
	}

	for (int i = 0; i < size; i++)
	{
		int n, temp;
		n = rand() % size;
		temp = numbers[n];
		numbers[n] = numbers[i];
		numbers[i] = temp;
	}

	for (int i = 0; i < size; i++)
	{
		Process* process = (Process*)malloc(sizeof(Process));
		process->ID = randID;
		process->CPUBurstTime = rand() % 20 + 1;
		process->IOBurstTime = rand() % 20 + 1;
		process->ArrivalTime = rand() % 20 + 1;
		process->Priority = numbers[i];
		InsertProcess(&ReadyQueue, process);
		randID += rand() % 30 + 1;
	}
	DebugNode(ReadyQueue);
}

void FCFSSchedule()
{
	while (TRUE)
	{

	}
}