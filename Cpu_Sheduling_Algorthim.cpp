#include <iostream>
#include<string>
#include<fstream>
#include <vector>
#include<algorithm>
#include<iomanip>
#include <climits>

using namespace std;

struct PCB //process control block structure 
{
	int pId;
	int arrivalTime;
	int burstTime;
	int turnAroundTime;
	int waitingTime;
	int remainingTime;
	int finishTime;
	int startTime;
};


void FirstComeFirstServer(PCB proceses[], int contextSwitch, int count)
{
	cout << setfill('*') << setw(70) << "First Come First Serve Algorthim " << setfill('*') << setw(30) << '*';
	

	int currentTime = 0;

	for (int i = 0; i < count; i++) 
	{
		currentTime = max(proceses[i].arrivalTime, currentTime); // Account for process arrival
		proceses[i].startTime = currentTime;
		proceses[i].finishTime = currentTime + proceses[i].burstTime;
		currentTime = proceses[i].finishTime + contextSwitch; // Add context switch time
	}

	for (int i = 0; i < count; i++) {
		proceses[i].turnAroundTime = proceses[i].finishTime - proceses[i].arrivalTime;
		proceses[i].waitingTime = proceses[i].turnAroundTime - proceses[i].burstTime;
	}
}

void RoundRobin(PCB process[], int quantum, int contextSwitch, int num) {

	cout << setfill('*') << setw(60) << "RoundRoubin algorithm" << setfill('*') << setw(30) << '*';

	for (int i = 0; i < num; i++) {
		process[i].remainingTime = process[i].burstTime;	// Initialize remaining times, first execution status, and first start times
		process[i].startTime = -1;  // Indicates that the process hasn't started yet
	}

	int currentTime = 0;
	bool allDone = false;

	
	while (!allDone) {
		allDone = true;

		for (int i = 0; i < num; i++) {
			if (process[i].remainingTime > 0) {  // Process has not finished
				allDone = false;

				if (process[i].startTime == -1)
				{  // If this is the first time it starts
					process[i].startTime = currentTime;
				}

				if (process[i].remainingTime > quantum) {
					currentTime += quantum;  
					process[i].remainingTime -= quantum;  // Decrease remaining time
					currentTime += contextSwitch;  // Context switch time
				}
				else {
					currentTime += process[i].remainingTime;  // Remaining burst time
					process[i].remainingTime = 0;  // Process is completed
					process[i].finishTime = currentTime;  // Record finish time
					currentTime += contextSwitch;  // Context switch time
				}
			}
		}
	}

	for (int i = 0; i < num; i++) {
		process[i].turnAroundTime = process[i].finishTime - process[i].arrivalTime;  // Turnaround time
		process[i].waitingTime = process[i].turnAroundTime - process[i].burstTime;  // Waiting time
	}
}

void ShortestRemainingTime(PCB process[], int contextSwitch, int num) 
{
	cout << setfill('*') << setw(70) << "Shortest Remaining Time algorithm" << setfill('*') << setw(30) << '*';

	for (int i = 0; i < num; i++) {
		process[i].remainingTime = process[i].burstTime;
		process[i].startTime = -1;  // Initialize start time
	}

	int complete = 0;  // Number of completed processes
	int currentTime = 0;  // Current simulation time
	int previousProcess = -1;  // Keeps track of the previous process 

	// Loop until all processes are complete
	while (complete < num) {
		int shortest = -1;  // Index of the process with the shortest remaining time
		int minRemainingTime = 100000;  

		// Find the process with the shortest remaining time among those that have arrived
		for (int i = 0; i < num; i++) {
			if (process[i].arrivalTime <= currentTime &&
				process[i].remainingTime > 0 &&
				process[i].remainingTime < minRemainingTime) {
				minRemainingTime = process[i].remainingTime;
				shortest = i;
			}
		}

		if (shortest == -1) {  
			currentTime++;
			continue;
		}

		// If the running process changes, add context switch time
		if (previousProcess != -1 && previousProcess != shortest) {
			currentTime += contextSwitch;  // Add context switch time
		}

		// If the process is running for the first time, set its start time
		if (process[shortest].startTime == -1) {
			process[shortest].startTime = currentTime;
		}

		// Process one unit of time
		process[shortest].remainingTime--;
		currentTime++;

		if (process[shortest].remainingTime == 0) {  // Process has finished
			complete++;
			process[shortest].finishTime = currentTime;  // Record finish time
			process[shortest].waitingTime = process[shortest].finishTime - process[shortest].burstTime - process[shortest].arrivalTime;

		

			process[shortest].turnAroundTime = process[shortest].waitingTime + process[shortest].burstTime;  // Turnaround time
		}

		previousProcess = shortest;
	}
}
void readFile(string& fileLocation, PCB proccess[], int& contextSwitch, int& quantuim, int& count) {
	ifstream fin;
	count = 0;
	fin.open(fileLocation);
	if (!fin)
	{
		cout << "file is not exist";
		exit(1);
	}
	else {
		fin >> contextSwitch >> quantuim;
		while (!fin.eof())
		{
			count++;
			fin >> proccess[count - 1].arrivalTime;
			fin >> proccess[count - 1].burstTime;
			proccess[count - 1].pId = count;

		}

	}
	fin.close();

}

void printProcess(PCB process[], int count, int Q, int CS, int choice)
{
	cout << "quantim " << Q << " context switch " << CS << endl;
	cout << setfill('-') << setw(100) << "-";
	cout << endl;
	cout << "| Process ID\t Arrival Time\tburst time\tfinish time \t turn around time \t waiting time |" << endl;
	float avgturnaround = 0;
	for (int i = 0; i < count; i++)
	{
		avgturnaround += process[i].turnAroundTime;
	}
	avgturnaround /= count;
	float avgwaitingtime = 0;
	for (int i = 0; i < count; i++)
	{
		avgwaitingtime += process[i].waitingTime;
	}
	avgwaitingtime /= count;

	for (int i = 0; i < count; i++)
	{
		cout << setfill(' ') << '|' << setw(5) << process[i].pId << "\t\t" << setw(7) << process[i].arrivalTime << "\t\t" << setw(7) << process[i].burstTime << "\t\t" << setw(7) << process[i].finishTime << "\t\t" << setw(7) << process[i].turnAroundTime << "\t\t\t" << setw(8) << process[i].waitingTime << endl;

	}
	int totalBusyTime = 0;
	for (int i = 0; i < count; i++) {
		totalBusyTime += process[i].burstTime;
	}
	
	int minStartTime = 10000; 
	int maxFinishTime = 0;      

	for (int i = 0; i < count; i++) {
		if (process[i].startTime < minStartTime) { 
			minStartTime = process[i].startTime; // Determine earliest start
		}
		if (process[i].finishTime > maxFinishTime) {
			maxFinishTime = process[i].finishTime; // Determine latest finish
		}
	}

	
	int totalTime = maxFinishTime - minStartTime; 


	// Calculate CPU utilization
	double cpuUtilization = ((double)totalBusyTime / totalTime) * 100; // CPU utilization as a percentage



	//cout <<setw(60) << " average   " << avgwaitingtime << endl;
	cout << setw(70) << "average " << avgturnaround << endl;
	cout << setw(95) << "average " << avgwaitingtime << endl;
	cout << "CPU Utilization: " <<setprecision(4) << cpuUtilization << "%" << endl;
	cout << setfill('-') << setw(100) << "-";
	cout << endl;

}

void GantChart(PCB process[], int count) {
	cout << endl;
	for (int i = 0; i < count; i++)
	{
		cout << "P" << process[i].pId << " start time is " << process[i].startTime << " finish time " << process[i].finishTime << endl;
	}
	cout << endl;
}

int main()
{

	string loc = "C:\\Users\\ttc\\Documents\\MohdSweity\\visual studio application\\OS\\process_data.txt";
	int contextSwitch, quantim, numberProcess;
	
	int choice;
	do
	{
		//system("cls");
		PCB* Process = new PCB[100];
		readFile(loc, Process, contextSwitch, quantim, numberProcess);

		cout << "CPU sheduling simultor  " << endl;
		cout << "1.First Come First Server \n2.RoundRobin \n3.ShortestRemainingTime\n4. exis \nchoose your algorithm ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			system("cls");

			FirstComeFirstServer(Process, contextSwitch, numberProcess);
			GantChart(Process, numberProcess);
			printProcess(Process, numberProcess, quantim, contextSwitch, choice);
			delete[] Process;
			break;
		case 2:
			system("cls");

			RoundRobin(Process, quantim, contextSwitch, numberProcess);
			GantChart(Process, numberProcess);
			printProcess(Process, numberProcess, quantim, contextSwitch, choice);
			delete[] Process;

			break;
		case 3:
			system("cls");

			ShortestRemainingTime(Process, contextSwitch, numberProcess);
			GantChart(Process, numberProcess);
			printProcess(Process, numberProcess, quantim, contextSwitch, choice);
			delete[] Process;

			break;
		case 4:
			delete[] Process;
			cout << "the program closed " << endl;
			return 0;

		default:
			system("cls");

			break;
		}


	} while (true);



	return 0;

}