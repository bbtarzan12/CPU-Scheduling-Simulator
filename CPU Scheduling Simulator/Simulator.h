#include "Process.h"

enum ScheduleTypeEnum { FCFS, N_SJF, P_SJF, N_Prirority, P_Priority, RR } typedef ScheduleType;

void Init(int size);
void CreateProcess(int size);
void Schedule();
void FCFSSchedule();
void Evaluation();