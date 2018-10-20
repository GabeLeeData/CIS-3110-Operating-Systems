#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void FCFS_state(bool dCheck, bool vCheck);
void RR_state (bool dCheck, bool vCheck, int qNum);
void d_display(int sthread, int numP, int arriv, int Proc_Total, int t_Io, int TimeTotal, int t_adj);
void f_display(double TimeAvg, int fTime, int fProc, int totalCount, int F_IoT, int F_Cpu);
void v_display(int arriv, int sthread, int numP, int runTime, int TimeTotal);
int process (int Counter, int temp_time, int *ar);
int RR_process (int timer, int qNum);
double util_cpu (int fTime, int F_IoT, int F_Cpu, int fProc);


int main (int argc, char *argv[]) {

  int state=0;
  int qNum = 0;
  bool dCheck = false;
  bool vCheck = false;


  //Check to see what arguments are entered

  for (int i=0; i < argc; i++)
  {

    //Check to see if -d was included
    if (strcmp(argv[i], "-d") == 0) dCheck = true;

    //Check to see if -v was included
    else if (strcmp(argv[i], "-v") == 0) vCheck = true;

    //Check to see if we need to use RoundRobin or FCFS
    else if (strcmp(argv[i], "-r") == 0) state = 1;

    // Initalize the quantnum number
    else qNum = atoi(argv[i]);

  }
  // Which output to use RR or Fcfs
  if (state == 0)
  {
    FCFS_state(dCheck, vCheck);
  }

  if (state == 1)
  {
    RR_state(dCheck, vCheck, qNum);
  }


}


//Round Robin mode
void RR_state (bool dCheck, bool vCheck, int qNum)
{
  int nProcRR = 0;
  int threadSwRR = 0;
  int procSwRR = 0;
  int fProcRR = 0;
  int fTimeRR = 0;
  int numPRR = 0;
  int threadNumRR = 0;
  int temp1 = 0;
  int totalCountRR = 0;
  int sthreadRR = 0;
  int arrivRR = 0;
  int nCpuRR = 0;
  int temp2 = 0;
  int configRR = 0;
  int TimeCpuRR = 0;
  int TimeIoRR = 0;
  int config2RR = 0;
  int TimeCpu2RR = 0;
  int t_IoRR = 0;
  int t_CpuRR = 0;
  int F_IoTRR = 0;
  int F_CpuRR = 0;
  int Proc_TotalRR = 0;
  int TimeTotalRR =0;
  int t_adjRR = 0;
  int TimeAvgRR = 0;
  int runTimeRR = 0;
  int TempCpu[100];
  int TempIo[100];
  int TotalCpuRR[100];
  int TotalIoRR[100];


  printf("\n\n Round Robin ( Quantum = %d time units): \n\n", qNum);


  //Read in the Structure
  fscanf(stdin, "%d %d %d", &nProcRR, &threadSwRR, &procSwRR);

  fProcRR = nProcRR * procSwRR;
  //Global time
  fTimeRR = fTimeRR + fProcRR;

  //Read in the rest of the process

  for (int x = 0; x < nProcRR; x++)
  {

    fscanf(stdin, "%d %d", &numPRR, &threadNumRR);

    temp1 = (threadSwRR * threadNumRR);
    totalCountRR = totalCountRR + threadNumRR;
    fTimeRR = fTimeRR + temp1;
    fProcRR = fProcRR + temp1;

    for (int t = 0; t < threadNumRR; t++)
    {

      //Reading in
      fscanf(stdin, "%d %d %d", &sthreadRR, &arrivRR, &nCpuRR);
      temp2 = nCpuRR -1;

      //Reading in threads
      for (int l = 0; l < temp2; l ++)
      {
        //Round robin Implementation
        fscanf (stdin, "%d %d %d", &configRR, &TimeCpuRR, &TimeIoRR);

        for (int p = 0; p < temp2; p ++)
        {
          //Quantum number checking - Round Robbin
          TempCpu[p] = TimeCpuRR;
          TempCpu[p] = RR_process(TempCpu[p], qNum);
          TotalCpuRR[l] =  TempCpu[p];


          TempIo[p] = TimeIoRR;
          TempIo[p] = RR_process(TempIo[p] , qNum);
          TotalIoRR[l] = TempIo[p];

        }
      }

      fscanf(stdin, "%d %d", &config2RR, &TimeCpu2RR);

      //Reset the Cpu and Io Time after each thread
      TotalCpuRR[0] = TimeCpu2RR;
      t_CpuRR = 0;
      t_IoRR = 0;


      //Adds to Global Clock for Total time and Total Cpu time
      F_IoTRR = process (F_IoTRR, temp2, TotalIoRR);
      F_CpuRR = process (F_CpuRR, nCpuRR, TotalCpuRR);

      //Counts the temporary during a single thread for time and cpu bursts
      t_IoRR  = process (t_IoRR, temp2, TotalIoRR);
      t_CpuRR = process (t_CpuRR, nCpuRR, TotalCpuRR);

      Proc_TotalRR = t_IoRR + t_CpuRR;

      //Calculating final total time (Global Time)
      TimeTotalRR = TimeTotalRR + Proc_TotalRR + threadSwRR;
      if (sthreadRR ==1) TimeTotalRR = TimeTotalRR + procSwRR;

      //Calculating average time
      t_adjRR = TimeTotalRR - arrivRR;
      TimeAvgRR = TimeAvgRR + t_adjRR;

      //display for Verbose mode
      if (vCheck)
      {
        runTimeRR = Proc_TotalRR - t_IoRR;
        v_display(arrivRR, sthreadRR, numPRR, runTimeRR, TimeTotalRR);
        printf("\n");
      }

      //display for detailed detailed mode

      if (dCheck) d_display(sthreadRR, numPRR, arrivRR, Proc_TotalRR, t_IoRR, TimeTotalRR, t_adjRR);

    }

  }
  //final Display
  f_display(TimeAvgRR, fTimeRR, fProcRR, totalCountRR, F_IoTRR, F_CpuRR);

}

//Round robin quant number proccessing
int RR_process (int timer, int qNum)
{

  int checker = timer - qNum;
  int realval = 0;
  if (checker >= 0)
  {
    timer = timer - qNum;
    realval = timer;
  }
  else
  {
    realval = timer;
  }

  return realval;

}

void FCFS_state(bool dCheck, bool vCheck)
{

  int nProc = 0;
  int threadSw = 0;
  int procSw = 0;
  int fTime = 0;
  int fProc = 0;
  int count = 0;
  int totalCount = 0;
  int numP = 0;
  int threadNum = 0;
  int temp1 = 0;
  int sthread = 0;
  int arriv = 0;
  int nCpu = 0;
  int temp2 = 0;
  int config = 0;
  int TimeCpu = 0;
  int TimeIo = 0;
  int TotalIo[100];
  int TotalCpu[100];
  int t_Io = 0;
  int F_IoT = 0;
  int F_Cpu = 0;
  int t_Cpu = 0;
  int Proc_Total = 0;
  int TimeTotal = 0;
  int t_adj = 0;
  int config2 = 0;
  int TimeCpu2 = 0;
  int runTime = 0;
  double TimeAvg = 0;


  //Read in the Structure of the textfile
  printf("\n FCFS Mode\n");
  fscanf(stdin, "%d %d %d", &nProc, &threadSw, &procSw);

  //Starting the Count
  fProc = nProc * procSw;

  //Global Time
  fTime = fTime + fProc;

  for (int x =0; x <nProc; x++)
  {

    //Reading in files and adding towards the global clock
    fscanf(stdin, "%d %d", &numP, &threadNum);

    temp1 = (threadSw * threadNum);
    fTime = fTime + temp1;
    fProc = fProc + temp1;
    totalCount = totalCount + threadNum;


    for (int o = 0; o <threadNum; o++)
    {

      //Reading in
      fscanf(stdin, "%d %d %d", &sthread, &arriv, &nCpu);
      // last process has no io time so loop till second last
      temp2 = nCpu - 1;

      for (int l = 0; l < temp2; l++)
      {
        fscanf (stdin, "%d %d %d", &config, &TimeCpu, &TimeIo);
        //Adding up Cpu
        TotalCpu[l] = TimeCpu;
        //Adding up Io
        TotalIo[l] = TimeIo;
      }

      fscanf(stdin, "%d %d", &config2, &TimeCpu2);


      //Reset the Cpu and Io Time after each thread
      TotalCpu[0] = TimeCpu2;
      t_Cpu = 0;
      t_Io = 0;


      //Adds to Global Clock for Total time and total Cpu Bursts
      F_IoT = process (F_IoT, temp2, TotalIo);
      F_Cpu = process (F_Cpu, nCpu, TotalCpu);

      //Counts the temporary during a single process for time and cpu bursts
      t_Io  = process (t_Io, temp2, TotalIo);
      t_Cpu = process (t_Cpu, nCpu, TotalCpu);

      Proc_Total = t_Io + t_Cpu;

      //Calculating final  total time (Global Time)
      TimeTotal = TimeTotal + Proc_Total + threadSw;
      if (sthread ==1) TimeTotal = TimeTotal + procSw;

      //Calculating average time
      t_adj = TimeTotal - arriv;
      TimeAvg= TimeAvg + t_adj;

      //display for verbose mode
      if (vCheck)
      {
        runTime = Proc_Total-t_Io;
        v_display(arriv, sthread, numP, runTime, TimeTotal);
        printf("\n");
      }

      //display for detailed mode
      if (dCheck) d_display(sthread, numP, arriv, Proc_Total, t_Io, TimeTotal, t_adj);

    }
  }
  //final display
  f_display(TimeAvg, fTime, fProc, totalCount, F_IoT, F_Cpu);

}

//Verbose Display
void v_display(int arriv, int sthread, int numP, int runTime, int TimeTotal)
{

  printf("\n At time %d: Thread %d of Process %d moves from new to ready \n", arriv, sthread, numP);
  printf("\n At time %d: Thread %d of Process %d moves from ready to running \n", runTime, sthread, numP);
  printf("\n At time %d: Thread %d of Process %d moves from running to terminated \n", TimeTotal, sthread, numP);

}

//Detailed Display
void d_display(int sthread, int numP, int arriv, int Proc_Total, int t_Io, int TimeTotal, int t_adj)
{

  int temp = 0;
  temp = Proc_Total - t_Io;
  printf("\n\nThread %d of Process %d: \n\n", sthread, numP);
  printf("Arrival Time: %d \n", arriv);
  printf("Service Time: %d units \n", temp);
  printf("I/O Time: %d units \n", t_Io);
  printf("Turnaround Time: %d units \n", t_adj);
  printf("Finish Time: %d units \n\n", TimeTotal);

}

//Displaying Final Results
void f_display(double TimeAvg, int fTime, int fProc, int totalCount, int F_IoT, int F_Cpu)
{
  double TuAvg = TimeAvg / totalCount;
  fTime = fTime + F_IoT + F_Cpu;
  double cpu = util_cpu(fTime, F_IoT, F_Cpu, fProc);
  printf("\n\nTotal Time required is %d units \n", fTime);
  printf("Average Turnaround Time is %.2lf time units \n", TuAvg);
  printf("CPU Utilization is %.2lf \n", cpu);
}

//Calculates the Cpu utilization
double util_cpu (int fTime, int F_IoT, int F_Cpu, int fProc)
{
  double cpu = 0;
  cpu = (fTime - fProc);
  cpu = (cpu / fTime);
  cpu = cpu * 100;

  return cpu;
}


// Adds up the total of CpuTime or IoTime
int process (int Counter, int temp_time, int *ar)
{

    for (int l =0; l <temp_time; l++)
    {
      Counter = Counter + ar[l];
    }

    return Counter;

}
