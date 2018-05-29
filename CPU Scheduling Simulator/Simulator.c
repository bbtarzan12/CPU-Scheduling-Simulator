#include "Simulator.h"

void main(int argc)
{
	system("clear");
	if (argc == 1)
	{
		printf("Enter the number of processes : ");
		scanf("%d", &argc);
	}

	printf("Active random IO? : ");
	scanf("%d", &RANDOMIO);

	Init(argc);

	printf("\n############# FCFS #############\n");
	Evaluation(FCFS, FALSE);
	printf("\n############# Non-Preemptive SJF #############\n");
	Evaluation(SJF, FALSE);
	printf("\n############# Preemptive SJF #############\n");
	Evaluation(SJF, TRUE);
	printf("\n############# Non-Preemptive Priority #############\n");
	Evaluation(PRIORITY, FALSE);
	printf("\n############# Preemptive Priority #############\n");
	Evaluation(PRIORITY, TRUE);
	printf("\n############# Round Robin #############\n");
	Evaluation(RR, TRUE);
	
}

// 실제로 Simulation을 하고 성능 평가를 하는 함수
void Evaluation(AlgorithmType type, bool preeptive)
{
	NodePtr tempQueue = originalQueue;
	while (tempQueue != NULL)
	{
		tempQueue->Process->CPURemaningTime = tempQueue->Process->CPUBurstTime;
		tempQueue->Process->IORemaningTime = 0;
		tempQueue->Process->WaitingTime = 0;
		tempQueue->Process->TurnaroundTime = 0;
		InsertProcess(&JobQueue, tempQueue->Process);
		tempQueue = tempQueue->Next;
	}
	NodePtr GanttChart = Update(type, preeptive, 4);
	DrawVerticalGanttChart(GanttChart);
	DrawNodeInformation(TerminatedQueue);
	free(TerminatedQueue);
	TerminatedQueue = NULL; // Queue의 모든 노드를 비워줘야하지만 시간이 없기때문에 그냥 막나가버리기~
	free(GanttChart);
	GanttChart = NULL;
}
 
// 디버그를 위해 임의로 프로세스를 JobQueue에 할당
void DebugInit()
{
	//     								id cbt at p
	InsertProcess(&JobQueue, NewProcess(1, 5, 0, 1));
	InsertProcess(&JobQueue, NewProcess(2, 3, 1, 2));
	InsertProcess(&JobQueue, NewProcess(3, 8, 2, 3));
	InsertProcess(&JobQueue, NewProcess(3, 6, 3, 4));
}

// Simulation할 프로세스를 Size 수로 임의로 생성
void Init(int size)
{
	srand(time(NULL));
	CreateProcess(size);
}

// Process 생성 함수
void CreateProcess(int size)
{
	int* numbers = (int*)malloc(sizeof(int) * size);

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
		ProcessPtr process = (ProcessPtr)malloc(sizeof(Process));
		process->ID = ID_I ? i : randID;
		process->CPUBurstTime = rand() % 20 + 1;
		process->IOBurstTime = rand() % 5 + 1;
		process->ArrivalTime = ARRIVALTIME_I ? i : rand() % 20 + 1;
		process->Priority = numbers[i];
		process->CPURemaningTime = process->CPUBurstTime;
		process->IORemaningTime = 0;
		process->WaitingTime = 0;
		process->TurnaroundTime = 0;
		InsertProcess(&originalQueue, process);
		randID += rand() % 30 + 1;
	}
	free(numbers);
}

// 간트차트 생성을 위해 시간체크와 Simulate를 한다.
NodePtr Update(AlgorithmType type, bool preemptive, int timeQuantum)
{
	// 시뮬레이션 시작 전에 Job 큐의 노드들 출력
	DebugNode(JobQueue);
	int time = 0;
	TimeConsumed = 0;
	NodePtr GanttNode = NULL;
	while (TRUE)
	{
		if (JobQueue == NULL && ReadyQueue == NULL && WaitingQueue == NULL && RunningProcess == NULL) // job, ready, waiting, running이 모두 null이면 아마 시뮬레이션이 끝났다는 뜻
			break;
		ProcessPtr process = Simulate(time, type, preemptive, timeQuantum); // 시뮬레이션을 하자
		InsertProcess(&GanttNode, process); // 시뮬레이션이 끝나면 간트차트를 그리기 위해 프로세스를 간트노드에 추가
		time++;
	}
	return GanttNode;
}

// AlgorithmType에 맞게 Process를 가져오는 함수
ProcessPtr Schedule(AlgorithmType type, bool preemptive, int timeQuantum)
{
	switch (type)
	{
		case FCFS:
			return FCFSAlgorithm();
		case SJF:
			return SJFAlgorithm(preemptive);
		case P:
			return PriorityAlgorithm(preemptive);
		case RR:
			return RRAlgorithm(timeQuantum); // TimeQuantum은 고정 4
		default:
			PrintError("Invalid AlgorithmType");
			return NULL;
	}
}

// 주어진 시간에 맞춰 실제로 Simulation을 하는 함수
ProcessPtr Simulate(int time, AlgorithmType type, bool preemptive, int timeQuantum)
{
	SimulateTime++;
	while (TRUE)
	{
		ProcessPtr Temp;
		if (type == PRIORITY)
			Temp = GetProcess(JobQueue, PRIORITY, time);
		else
			Temp = GetProcess(JobQueue, ARRIVALTIME, time);
		if (Temp == NULL)
			break;
		MoveProcess(&JobQueue, &ReadyQueue, Temp); // 도착한 프로세스를 Ready큐에서 Job큐로 이동
	}

	ProcessPtr PrevProcess = RunningProcess; // Schedule 이전 프로세스를 캡쳐해서 프로세스가 달라지는지 확인
	RunningProcess = Schedule(type, preemptive, timeQuantum); // Job큐의 프로세스들을 처리

	if (PrevProcess != RunningProcess)
	{
		TimeConsumed = 0;
	}

	WaitAllProcess(ReadyQueue); // Ready큐에 있는 프로세스들 처리
	PerformIOOperation(WaitingQueue); // Waiting큐에 있는 프로세스들 처리(IO)
	if (RunningProcess != NULL)
	{
		return ExecuteRunningProcess(); // Running 프로세스를 처리
	}
	else
	{
		return NULL; // 간트차트 Idle
	}
}

ProcessPtr FCFSAlgorithm()
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process; // 일단 레디큐에서 받아오고
	if (Process != NULL)
	{
		if (RunningProcess != NULL) // 그런데 실행중인 프로세스가 있다면
		{
			return RunningProcess; // 실행중인 프로세스를 계속 실행하자
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process); // 실행중인 프로세스가 없으면 이제 요놈을 실행
			return Process;
		}
	}
	else
	{
		return RunningProcess; // 레디큐가 비어있으면 실행중인 프로세스를 계속 실행
	}
}

ProcessPtr SJFAlgorithm(bool preemtive)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process; // 레디큐에서 받아오고
	if (Process != NULL)
	{
		NodePtr temp = ReadyQueue;
		while (temp != NULL)
		{
			// Ready큐를 순회하면서 CPU 남은시간이 가장 짧은 프로세스를 찾는다
			if (temp->Process->CPURemaningTime <= Process->CPURemaningTime) // SJF이므로 CPU 남은시간이 짧은 놈을 찾자
			{
				if (temp->Process->CPURemaningTime == Process->CPURemaningTime) // 만약 CPU남은시간이 같으면
				{
					if (temp->Process->ArrivalTime <= Process->ArrivalTime) // 도착한 시간으로 비교
						Process = temp->Process;
				}
				else
				{
					Process = temp->Process;
				}
			}
			temp = temp->Next;
		}

		if (RunningProcess != NULL) // 실행중인 프로세스가 있으면
		{
			if (preemtive) // Preemtive이면 확인을 해야지
			{

				if (RunningProcess->CPURemaningTime >= Process->CPURemaningTime) // 실행중인게 CPU 남은시간이 더 길까?
				{
					if (RunningProcess->CPURemaningTime == Process->CPURemaningTime)
					{
						if (RunningProcess->ArrivalTime <= Process->ArrivalTime)
							return RunningProcess; // CPU 남은시간이 같은데 도착시간으로 비교하니 현재 실행중인게 더 짧아서 실행중인걸 계속 실행
					}

					InsertProcess(&ReadyQueue, RunningProcess); // 실행중인게 더 길면 실행중인걸 레디큐로 옮기고
					RunningProcess = NULL;
					DeleteProcess(&ReadyQueue, Process); // 요놈을 실행
					return Process;
				}
				return RunningProcess;
			}
			else // Non-Preemtive면 실행중인거 계속 실행
			{
				return RunningProcess;
			}
		}
		else
		{	
			// 실행중인 프로세스가 없으면 바로 요놈을 실행하자
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

ProcessPtr PriorityAlgorithm(bool preemtive)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process; // 레디큐에서 가져오자
	if (Process != NULL)
	{
		NodePtr temp = ReadyQueue;
		while (temp != NULL)
		{

			if (temp->Process->Priority <= Process->Priority) // Priority가 가장 높은애를 찾는다.
			{
				if (temp->Process->Priority == Process->Priority)
				{
					if (temp->Process->ArrivalTime <= Process->ArrivalTime) // 만약 Priority가 같으면 도착시간으로 비교
						Process = temp->Process;
				}
				else
				{
					Process = temp->Process;
				}
			}
			temp = temp->Next;
		}

		if (RunningProcess != NULL) // 실행중인 프로세스가 있다면
		{
			if (preemtive) // Preemtive 일때만 가능하지
			{
				if (RunningProcess->Priority >= Process->Priority) // 실행중인 프로세스가 Priority가 더 낮으면
				{
					if (RunningProcess->Priority == Process->Priority)
					{
						if (RunningProcess->ArrivalTime <= Process->ArrivalTime) // 같을때는 도착시간으로 비교
							return RunningProcess;
					}

					InsertProcess(&ReadyQueue, RunningProcess); // 실행중인걸 Ready 큐로 옮기자
					RunningProcess = NULL;
					DeleteProcess(&ReadyQueue, Process); // 요놈을 이제 실행하자
					return Process;
				}
				return RunningProcess;
			}
			else
			{
				return RunningProcess;
			}
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

ProcessPtr RRAlgorithm(int timeQuantum)
{
	ProcessPtr Process = ReadyQueue == NULL ? NULL : ReadyQueue->Process; // 레디큐에서 가지고오자
	if (Process != NULL)
	{
		if (RunningProcess != NULL)
		{
			if (TimeConsumed >= timeQuantum) // 현재 실행하고있는 프로세스의 실행한 시간이 TimeQuantum 이상이면 다 소비했다
			{
				InsertProcess(&ReadyQueue, RunningProcess); // 이제 소비를 했으니 Ready 큐로 돌아가야지
				RunningProcess = NULL;
				DeleteProcess(&ReadyQueue, Process);
				return Process;
			}
			else
			{
				return RunningProcess;
			}
		}
		else
		{
			DeleteProcess(&ReadyQueue, Process);
			return Process;
		}
	}
	else
	{
		return RunningProcess;
	}
}

// Ready큐에서 기다리고있는 프로세스들 처리
void WaitAllProcess(NodePtr head)
{
	if (head == NULL)
		return;

	ProcessPtr Process = head->Process;
	Process->TurnaroundTime++;
	Process->WaitingTime++;

	WaitAllProcess(head->Next);
}

// Waiting큐에서 기다리고있는 프로세스들 처리
void PerformIOOperation(NodePtr head)
{
	if (head == NULL)
		return;

	ProcessPtr Process = head->Process;
	Process->TurnaroundTime++;
	Process->IORemaningTime--;

	if (Process->IORemaningTime <= 0)
	{
		head = (head->Next != NULL && head->Next->Next != NULL) ? head->Next->Next : NULL;
		MoveProcess(&WaitingQueue, &ReadyQueue, Process); // IO 남은시간이 0이면 이제 다시 Ready큐로 이동
		if(head != NULL)
			PerformIOOperation(head); // 에러체크때문에 넣어봄
	}
	else
	{
		PerformIOOperation(head->Next);
	}
}

// 현재 실행중인 프로세스 처리
ProcessPtr ExecuteRunningProcess()
{
	ProcessPtr temp = RunningProcess;
	RunningProcess->CPURemaningTime--;
	RunningProcess->TurnaroundTime++;
	TimeConsumed++;

	if (RANDOMIO && rand() % 5 == 0) // 랜덤으로 IO처리
	{
		printf("Random IO (ID : %d)\n", RunningProcess->ID);
		RunningProcess->IORemaningTime += RunningProcess->IOBurstTime; // IO가 발생하면 IORemainingTime이 0에서 BurstTime이 됨
	}

	if (RunningProcess->CPURemaningTime <= 0)
	{
		InsertProcess(&TerminatedQueue, RunningProcess); // CPURemaining 시간이 0이면 처리가 끝난 프로세스이므로 TerminatedQueue로 이동
		RunningProcess = NULL;
		return temp;
	}
	else if (RunningProcess->IORemaningTime > 0)
	{
		InsertProcess(&WaitingQueue, RunningProcess); // IO 남은 시간이 1 이상이면 IO를 처리해야하니 Waiting 큐로 이동
		RunningProcess = NULL;
		return temp;
	}
	return RunningProcess;
}
