#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <queue>
#include <cstring>
#include <algorithm>        //sort()
#include <iomanip>

using namespace std;

struct processRR {
	int processID;             //process id
	int arrivalTime;           //time at which the process arrives
	int burstTime;             //time it takes for the process to be executed 
	int waitingTime;           //time the processes is spent waiting to be executed
	int turnAroundTime;        //time from when the process enters it's ready state until it is fully executed
	int responseTime;          //time at which the process responds to being executed
	int startTime;             //time at which the process will start running
	int completionTime;        //time at which the process would've been completed
};

class Node {
private:
	int id = 0;
	float arrival_time = 0, burst_time = 0, turnaround_time = 0, waiting_time = 0, priority = 0;
	Node* link_next;
	Node* head = NULL;//pointer to start of the list (left)
	Node* tail = NULL;//pointer to end of the list(right)
public:
	void addNode(float, float, float, float);//add data to each nod, running once adds one node
	void sortNode();//sort ALL NODES by order of priority
	void printList();//calculate and print things like waiting time, turnaround time and the averages

};
typedef Node* nodePtr; //alias used for Node* to simplify the use of pointers

//Part a
void FirstComeFirstServe();
void ShortestJobFirst();
void PriorityNonPreemptive();
void RoundRobin();
void RunAll();
//part b
void FirstInFirstOut();
void LeastRecentlyUsed();
bool compareSecond(processRR, processRR);
bool compareFirst(processRR, processRR);
void LeastRecentlyUsed(vector<int>& p, int frame, int process);
void FirstInFirstOut(vector<int>& p, int frame, int process);
void Optimal(vector<int>& p, int frame, int process);
//Part c
//@total is a int vector used to store the requests queue to a max of 10
//@front is the starting position given by the used
//@amount is the amount of requests
void DiskFirstComeFirstServe(vector<int>& total, int front, int amount);
void ShortestSeekFirst(vector<int>& total, int front, int amount);
void Scan(vector<int>& total, int front, int amount);
void c_Scan(vector<int>& total, int front, int amount);
//====================================================================================================//
int main() {
	//option selection by user
	int category, sub_category;
	cout << "Category selection menu: ";
	cout << "\n1) Processes\n2) Memory\n3) Disks" << endl;
	cin >> category;
	while (cin.fail()) {//validar que el input sea un entero
		cout << "Error. You need to enter a valid integer.\n" << endl;
		cin.clear(); //limpia el error
		cin.ignore(100000, '\n'); //ignora y se mueve a la siguiente linea 
		cin >> category;
	}

	switch (category) {
		//CPU:
	case 1:
	{
		cout << "Select: \n1) FCFS \n2) SJF \n3) Priority \n4) Round Robin\n5) Run all Process Algorithms\n" << endl;
		cin >> sub_category;

		if (sub_category == 1) {
			FirstComeFirstServe();
		}
		else if (sub_category == 2) {
			ShortestJobFirst();
		}
		else if (sub_category == 3) {
			PriorityNonPreemptive();
		}
		else if (sub_category == 4) {
			RoundRobin();
		}
		else if (sub_category == 5) {
			RunAll();
		}
		else {
			return 0;
		}
		break;
	}
	case 2:
	{
		int frame;          //to define how many frames will be use
		int process;        //to define the amount of processes that the user will enter
		cout << "------Memory SCHEDULING------\n";
		cout << "LRU \t OPTIMAL \t FIFO \t";
		//user will enter the amount of frames
		cout << "Enter the number of frames you wish to use.\n(MAX number of frames allowed is 20): ";
		cin >> frame;
		cout << "\n";
		//if the user enters a number of frames that exceeds 20 or is less than 0, the user will be prompted to enter another number
		while (frame > 20) {
			cout << "Error! \nPlease enter a number equal or lesser than 20 and more than zero.\n";
			cin >> frame;
		}
		//input validation for any non integer entered
		while (cin.fail()) {
			cout << "Error. You need to enter a valid integer.\n" << endl;
			cin.clear();
			cin.ignore(100000, '\n');
			cin >> frame;
		}
		//user will enter the amount of processes 
		cout << "Enter the number number of processes.: ";
		cin >> process;

		//input validation for any non integer entered
		while (cin.fail()) {
			cout << "Error. You need to enter a valid integer.\n" << endl;
			cin.clear();
			cin.ignore(100000, '\n');
			cin >> process;
		}
		vector <int> temp(process);
		cout << "\nEnter the chain of processes: ";
		for (int x = 0; x < process; x++) {
			cin >> temp[x];
		}
		LeastRecentlyUsed(temp, frame, process);
		FirstInFirstOut(temp, frame, process);
		Optimal(temp, frame, process);
		break;
	}
	case 3:
	{
		cout << "------DISK SCHEDULING------\n";
		cout << "FCFS \t SSF \t SCAN \t C-SCAN\n";
		int request_queue, initial_position, request;
		cout << "Enter the request queue. Max is 10 numbers.";
		cin >> request_queue;
		cout << "Enter the desired initial position: ";
		cin >> initial_position;
		vector <int> vectRequests(request_queue);
		if (request_queue < 0) {
			cout << "Error";
		}
		else {
			for (int i = 0; i < request_queue; i++) {
				cout << "Enter requests #" << i << ":";
				cin >> request;
				vectRequests[i] = request;
			}
		}

		DiskFirstComeFirstServe(vectRequests, initial_position, request_queue);
		cout << "\n\n\n";
		ShortestSeekFirst(vectRequests, initial_position, request_queue);
		cout << "\n\n\n";
		Scan(vectRequests, initial_position, request_queue);
		cout << "\n\n\n";
		c_Scan(vectRequests, initial_position, request_queue);
		break;
	}
	}
}
//Funciones//
//============================================================================================================//
void FirstComeFirstServe() {
	Node fcfs{};
	int process;
	float process_id = 0, arrival_time = 0, burst_time = 0;
	cout << "Enter number of process";
	cin >> process;
	//grab and inset data in each node
	//for every iteration a node is added to the list
	for (int i = 1; i <= process; i++) {
		cout << "\nEnter Process id: ";
		cin >> process_id;
		cout << "\nEnter Arrival Time: ";
		cin >> arrival_time;
		cout << "\nEnter Burst Time: ";
		cin >> burst_time;
		// last parameter is priority which is null in this case, for the sake of function reusability in other algorithms 
		fcfs.addNode(process_id, arrival_time, burst_time, 1);
	}

	fcfs.sortNode();
	cout << "\t\t\tFCFS Scheduling\n";
	fcfs.printList();
}
void ShortestJobFirst() {
	Node sjf{};
	int process;
	float process_id = 0, arrival_time = 0, burst_time = 0;
	cout << "Enter number of process";
	cin >> process;
	//grab and inset data in each node
	//for every iteration a node is added to the list
	for (int i = 1; i <= process; i++) {
		cout << "\nEnter Process id: ";
		cin >> process_id;
		cout << "\nEnter Arrival Time: ";
		cin >> arrival_time;
		cout << "\nEnter Burst Time: ";
		cin >> burst_time;
		sjf.addNode(process_id, arrival_time, burst_time, NULL);
	}

	sjf.sortNode();

	cout << "\tSJF Scheduling\n";
	sjf.printList();
}
void PriorityNonPreemptive() {
	Node prt{};
	int process;
	float process_id = 0, arrival_time = 0, burst_time = 0, priority = 0;
	cout << "Enter number of process";
	cin >> process;
	//grab and inset data in each node
	//for every iteration a node is added to the list
	for (int i = 1; i <= process; i++) {
		cout << "\nEnter Process id: ";
		cin >> process_id;
		cout << "\nEnter Arrival Time: ";
		cin >> arrival_time;
		cout << "\nEnter Burst Time: ";
		cin >> burst_time;
		cout << "\nEnter Priority: ";
		cin >> priority;
		prt.addNode(process_id, arrival_time, burst_time, priority);
	}

	prt.sortNode(); //sort by priority given
	cout << "\tPriority Non-Preemptive Scheduling\n";
	prt.printList(); //calculates and prints
}
void RoundRobin() {

	int index;                              //index variable
	int processU;                           //# of processes entered by the user 
	int quantumU;                           //# of the quantum variable entered by the user
	float averageWaitingTime;               //average amount of time all the processes spent waiting to be executed
	float averageTurnaroundTime;            //average amount of time all the processes spent from comming to the ready state to their completition
	int totalWaitingTime = 0;               //total amount of time every individual process spent waiting to be executed
	int totalTurnAroundTime = 0;            //total amount of time every individual process spent from comming to the ready state to their completition
	int totalResponseTime = 0;              //amount of time a process takes to respond 
	int totalIdleTime = 0;                  //time a process spends after being executed and it is not yet finished and it exits the execution state
	struct processRR p[10];                 //maximum amount of processes is 10
	int burstRemaining[50];                //max amount of burst time is 50     


	//sets the output to 2 decimal places
	cout << setprecision(2) << fixed;

	//user enters the amount processes 
	cout << "Enter the number of processes (Maximum amount of processes that can be entered is 10): ";
	cin >> processU;
	//input validation for any numbers greater than 10
	while (processU > 10) {
		cout << "\nError! You need to enter a number lesser than 10/\n";
		cin >> processU;
	}
	//input validation for any non integer entered
	while (cin.fail()) {
		cout << "Error. You need to enter a valid integer.\n" << endl;
		cin.clear();
		cin.ignore(100000, '\n');
		cin >> processU;
	}
	cout << "Enter time quantum: ";
	cin >> quantumU;
	//input validation for any non integer entered
	while (cin.fail()) {
		cout << "Error. You need to enter a valid integer.\n" << endl;
		cin.clear();
		cin.ignore(100000, '\n');
		cin >> quantumU;
	}
	//User enters the arrival time and the burst time for each individual process created 
	for (int i = 0; i < processU; i++) {
		cout << "Enter arrival time of process " << i + 1 << ": ";
		cin >> p[i].arrivalTime;
		cout << "Enter burst time of process " << i + 1 << ": ";
		cin >> p[i].burstTime;
		burstRemaining[i] = p[i].burstTime;
		p[i].processID = i + 1;
		cout << endl;
	}
	//sort
	sort(p, p + processU, compareFirst);

	queue<int> q;
	int currentTime = 0;
	q.push(0);
	int completed = 0;
	int mark[100];
	memset(mark, 0, sizeof(mark));
	mark[0] = 1;
	//while the amount of processes completed is not equal to 0 keep executing the processes
	while (completed != processU) {
		index = q.front();
		q.pop();

		if (burstRemaining[index] == p[index].burstTime) {
			p[index].startTime = max(currentTime, p[index].arrivalTime);
			totalIdleTime += p[index].startTime - currentTime;
			currentTime = p[index].startTime;
		}

		if (burstRemaining[index] - quantumU > 0) {
			burstRemaining[index] -= quantumU;
			currentTime += quantumU;
		}
		else {
			currentTime += burstRemaining[index];
			burstRemaining[index] = 0;
			completed++;

			p[index].completionTime = currentTime;
			p[index].turnAroundTime = p[index].completionTime - p[index].arrivalTime;
			p[index].waitingTime = p[index].turnAroundTime - p[index].burstTime;
			p[index].responseTime = p[index].startTime - p[index].arrivalTime;

			totalTurnAroundTime += p[index].turnAroundTime;
			totalWaitingTime += p[index].waitingTime;
			totalResponseTime += p[index].responseTime;
		}

		for (int x = 1; x < processU
			; x++) {
			if (burstRemaining[x] > 0 && p[x].arrivalTime <= currentTime && mark[x] == 0) {
				q.push(x);
				mark[x] = 1;
			}
		}
		if (burstRemaining[index] > 0) {
			q.push(index);
		}

		if (q.empty()) {
			for (int x = 1; x < processU; x++) {
				if (burstRemaining[x] > 0) {
					q.push(x);
					mark[x] = 1;
					break;
				}
			}
		}


	}
	//defining the Average Turn Around Time by using the totalTurnAround time varible which sums all the TAT of all processes and dividing it by the amount of processes
	averageTurnaroundTime = (float)totalTurnAroundTime / processU;
	//defining the Average Waiting Time by using the totalWaiting time variable which sums all the WT of all processes and dividing it by the amount of processes
	averageWaitingTime = (float)totalWaitingTime / processU;
	//sorting the processes 
	sort(p, p + processU, compareSecond);
	cout << "\t\t\t Round Robin Scheduling";
	// Round Robin algorithm output
	cout << "\nProcess #\t" << "Arrival Time\t" << "Burst Time\t" << "Turn Around Time\t" << "Waiting Time\t\n";

	for (int x = 0; x < processU; x++) {
		cout << p[x].processID << "\t\t" << p[x].arrivalTime << "\t\t" << p[x].burstTime << "\t\t" << p[x].turnAroundTime << "\t\t\t" << p[x].waitingTime << "\n";
	}
	//Average Tunr Around Time output
	cout << "Average Turnaround Time = " << averageTurnaroundTime << "\n";
	//Average Waiting Time output
	cout << "Average Waiting Time = " << averageWaitingTime << "\n";

}
void RunAll() {
	Node fcfs{}, sjf{}, prt{};
	int process, quantumU, burstRemaining[100], index, totalWaitingTime = 0, totalTurnAroundTime = 0, totalResponseTime = 0, totalIdleTime = 0;
	float process_id = 0, arrival_time = 0, burst_time = 0, priority = 0, averageWaitingTime, averageTurnaroundTime;
	struct processRR p[10];
	cout << setprecision(2) << fixed;
	cout << "Enter number of process";
	cin >> process;
	while (process > 10 || process < 1) {
		cout << "\nError! You need to enter a number in range 1-10/\n";
		cin >> process;
	}
	while (cin.fail()) {
		cout << "Error. You need to enter a valid integer.\n" << endl;
		cin.clear();
		cin.ignore(100000, '\n');
		cin >> process;
	}
	//grab and set data in each node
	//for every iteration a node is added to the list
	for (int i = 0; i < process; i++) {
		cout << "\nEnter Process id: ";
		cin >> process_id;
		p[i].processID = process_id;
		cout << "\nEnter Arrival Time: ";
		cin >> arrival_time;
		p[i].arrivalTime = arrival_time;
		cout << "\nEnter Burst Time: ";
		cin >> burst_time;
		p[i].burstTime = burst_time;
		burstRemaining[i] = p[i].burstTime;
		cout << "\n Enter priority: ";
		cin >> priority;
		// last parameter is priority which is null in this case, for the sake of function reusability in other algorithms 
		fcfs.addNode(process_id, arrival_time, burst_time, 1);
		sjf.addNode(process_id, arrival_time, burst_time, NULL);
		prt.addNode(process_id, arrival_time, burst_time, priority);
	}
	cout << "Enter time quantum: ";
	cin >> quantumU;
	while (cin.fail()) {
		cout << "Error. You need to enter a valid integer.\n" << endl;
		cin.clear();
		cin.ignore(100000, '\n');
		cin >> quantumU;
	}

	fcfs.sortNode();
	sjf.sortNode();
	prt.sortNode();
	cout << "FCFS Scheduling";
	fcfs.printList();
	cout << "\nSJF Scheduling";
	sjf.printList();
	cout << "\nPriority Scheduling";
	prt.printList();
	//round robin
	//sort
	sort(p, p + process, compareFirst);

	queue<int> q;
	int currentTime = 0;
	q.push(0);
	int completed = 0;
	int mark[100];
	memset(mark, 0, sizeof(mark));
	mark[0] = 1;
	//while the amount of processes completed is not equal to 0 keep executing the processes
	while (completed != process) {
		index = q.front();
		q.pop();

		if (burstRemaining[index] == p[index].burstTime) {
			p[index].startTime = max(currentTime, p[index].arrivalTime);
			totalIdleTime += p[index].startTime - currentTime;
			currentTime = p[index].startTime;
		}

		if (burstRemaining[index] - quantumU > 0) {
			burstRemaining[index] -= quantumU;
			currentTime += quantumU;
		}
		else {
			currentTime += burstRemaining[index];
			burstRemaining[index] = 0;
			completed++;

			p[index].completionTime = currentTime;
			p[index].turnAroundTime = p[index].completionTime - p[index].arrivalTime;
			p[index].waitingTime = p[index].turnAroundTime - p[index].burstTime;
			p[index].responseTime = p[index].startTime - p[index].arrivalTime;

			totalTurnAroundTime += p[index].turnAroundTime;
			totalWaitingTime += p[index].waitingTime;
			totalResponseTime += p[index].responseTime;
		}

		for (int x = 1; x < process; x++) {
			if (burstRemaining[x] > 0 && p[x].arrivalTime <= currentTime && mark[x] == 0) {
				q.push(x);
				mark[x] = 1;
			}
		}
		if (burstRemaining[index] > 0) {
			q.push(index);
		}

		if (q.empty()) {
			for (int x = 1; x < process; x++) {
				if (burstRemaining[x] > 0) {
					q.push(x);
					mark[x] = 1;
					break;
				}
			}
		}


	}
	//defining the Average Turn Around Time by using the totalTurnAround time varible which sums all the TAT of all processes and dividing it by the amount of processes
	averageTurnaroundTime = (float)totalTurnAroundTime / process;
	//defining the Average Waiting Time by using the totalWaiting time variable which sums all the WT of all processes and dividing it by the amount of processes
	averageWaitingTime = (float)totalWaitingTime / process;
	//sorting the processes 
	sort(p, p + process, compareSecond);
	cout << "\t\t\t Round Robin Scheduling";
	// Round Robin algorithm output
	cout << "\nProcess #\t" << "Arrival Time\t" << "Burst Time\t" << "Turn Around Time\t" << "Waiting Time\t\n";

	for (int x = 0; x < process; x++) {
		cout << p[x].processID << "\t\t" << p[x].arrivalTime << "\t\t" << p[x].burstTime << "\t\t" << p[x].turnAroundTime << "\t\t\t" << p[x].waitingTime << "\n";
	}
	//Average Tunr Around Time output
	cout << "Average Turnaround Time = " << averageTurnaroundTime << "\n";
	//Average Waiting Time output
	cout << "Average Waiting Time = " << averageWaitingTime << "\n";

}
void Node::addNode(float id, float arrival_time, float burst_time, float priority) {
	//create and populate the node
	nodePtr new_node = new Node;
	new_node->id = id;
	new_node->arrival_time = arrival_time;
	new_node->burst_time = burst_time;
	new_node->priority = priority;
	new_node->link_next = NULL;

	//if the list is empty populate the list
	if (head == NULL) {
		head = new_node;
		tail = new_node;
	}
	else {
		tail->link_next = new_node;
		tail = new_node;
	}
}
void Node::sortNode() {

	nodePtr current = head;
	nodePtr index = NULL;
	nodePtr temp_node = new Node;

	if (head == NULL) {
		return;
	}
	else {
		while (current != NULL) {
			index = current->link_next;
			while (index != NULL) {
				if (current->arrival_time > index->arrival_time) {//if the order of the list, order in this case is arrival time 

					//save all the current data in a temporal node
					temp_node->id = current->id;
					temp_node->arrival_time = current->arrival_time;
					temp_node->burst_time = current->burst_time;
					temp_node->priority = current->priority;

					//move data from next node the current node
					current->id = index->id;
					current->arrival_time = index->arrival_time;
					current->burst_time = index->burst_time;
					current->priority = index->priority;

					//save the data from temporal node to the next node
					index->id = temp_node->id;
					index->arrival_time = temp_node->arrival_time;
					index->burst_time = temp_node->burst_time;
					index->priority = temp_node->priority;
				}
				//If the arrival time is the same, then it will sort by priority (used by priority non preemptive)
				else if (current->arrival_time == index->arrival_time && current->priority > index->priority) {
					//save all the current data in a temporal node
					temp_node->id = current->id;
					temp_node->arrival_time = current->arrival_time;
					temp_node->burst_time = current->burst_time;
					temp_node->priority = current->priority;
					//move data from next node the current node
					current->id = index->id;
					current->arrival_time = index->arrival_time;
					current->burst_time = index->burst_time;
					current->priority = index->priority;
					//save the data from temporal node to the next node
					index->id = temp_node->id;
					index->arrival_time = temp_node->arrival_time;
					index->burst_time = temp_node->burst_time;
					index->priority = temp_node->priority;
				}
				//if the arrival time IS THE SAME and priority is null, it will sort by SHORTEST burst time
				//priority for shortest job first will be null because in the calling function it is assigned 
				else if (current->arrival_time == index->arrival_time && current->burst_time > index->burst_time && current->priority == NULL && index->priority == NULL) {
					temp_node->id = current->id;
					temp_node->arrival_time = current->arrival_time;
					temp_node->burst_time = current->burst_time;
					temp_node->priority = current->priority;
					//move data from next node the current node
					current->id = index->id;
					current->arrival_time = index->arrival_time;
					current->burst_time = index->burst_time;
					current->priority = index->priority;
					//save the data from temporal node to the next node
					index->id = temp_node->id;
					index->arrival_time = temp_node->arrival_time;
					index->burst_time = temp_node->burst_time;
					index->priority = temp_node->priority;
				}
				index = index->link_next;
			}
			current = current->link_next;
		}
	}

	delete temp_node;
}
void Node::printList() {
	float count = 0, average_waiting = 0, average_turnaround = 0, burst, temp = 0, idle_time = 0, exit_time = 0;
	if (head == NULL) {
		cout << "List is empty";
	}
	cout << "\nProcess ID\t" << "Arrival Time\t" << "Burst Time\t" << "Waiting Time\t" << "Turnaround Time\t\n";
	//iterate throught the list until its empty
	while (head != NULL) {
		count++;
		//if the arrival is higher than the exit time of previous processes there will be idle time
		if (head->arrival_time > exit_time) {
			head->turnaround_time = head->burst_time;
			idle_time += (arrival_time - exit_time);
			exit_time += idle_time + head->burst_time;
		}
		else {
			exit_time += head->burst_time;
			head->turnaround_time = exit_time - head->arrival_time;
		}
		if (count == 1) {
			head->waiting_time = 0;

		}
		else if (count > 1) {
			;

			head->waiting_time = head->turnaround_time - head->burst_time;
		}
		//if the waiting time is less than 0 then there was idle time in between processes, hence why waiting time falls back to 0 
		if (head->waiting_time < 0) {
			head->waiting_time = 0;
		}
		cout << head->id << "\t\t\t" << head->arrival_time << "\t\t\t" << head->burst_time << "\t\t" << head->waiting_time << "\t\t\t" << head->turnaround_time << endl;
		//store and add all the waiting times and turnaround times
		average_waiting = average_waiting + head->waiting_time;
		average_turnaround = average_turnaround + head->turnaround_time;
		//point to next node before exiting loop
		head = head->link_next;
	}

	cout << "\nAverage Waiting Time is: " << average_waiting / count;
	cout << "\nAverage Turnaround Time is: " << average_turnaround / count;
	if (idle_time > 0) {
		cout << "\nIdle time was: " << idle_time;
	}
	cout << "\n\n";
}
void DiskFirstComeFirstServe(vector<int>& total, int front, int amount) {
	int sum = 0;

	int temp = front;
	cout << "\nFCFS DISK SCHEDULING ALGORITHM\n";
	cout << "Requests : \t Number of tracks traversed :" << endl;
	cout << temp;
	for (int i = 0; i < amount; i++) {
		cout << "\n" << total[i] << "\t\t\t" << abs(total[i] - temp);
		// calculating the difference for the head movement

		sum += abs(total[i] - temp);
		// head is now at the next I/O request
		temp = total[i];
	}
	cout << '\n';
	cout << "Total head movements = " << sum << '\n';
}
void ShortestSeekFirst(vector<int>& total, int front, int amount) {
	int i;
	int sum = 0;

	//I/O requests and array b will store the output
	vector <int> out;

	//creating a map to store the count of each element
	//in the array total.
	map <int, int> element_count;


	for (i = 0; i < total.size(); i++) {
		element_count[total[i]]++;
	}

	int temp = front;
	int element;
	out.push_back(front);
	int count = 0;
	while (count < amount) {
		//initially taking diff to be very large.
		int diff = 999999;
		//traversing in map to find the least difference
		for (auto q : element_count) {
			if (abs(q.first - temp) < diff) {
				element = q.first;
				diff = abs(q.first - temp);
			}
		}
		//deleting the element that has the least difference from the map
		element_count[element]--;
		if (element_count[element] == 0) {
			element_count.erase(element);
		}
		//adding that element to our output array.
		out.push_back(element);
		temp = element;
		count++;
	}
	//printing the movements chain
	cout << "\nSSF DISK SCHEDULING ALGORITHM\n";
	cout << "Requests : \t Number of tracks traversed :" << endl;
	cout << out[0];
	temp = out[0];
	for (i = 1; i < out.size(); i++) {
		cout << "\n" << out[i] << "\t\t\t" << abs(out[i] - temp);
		sum += abs(out[i] - temp);
		temp = out[i];
	}
	//print the total movements
	cout << '\n';
	cout << "Total head movements = " << sum << '\n';

}
void Scan(vector<int>& total, int front, int amount) {
	int i;
	int sum = 0;
	//creating two arrays, array a will store the input
	//I/O requests and array b will store the output
	vector <int> input(amount), output;
	input = total;
	int temp = front;

	//add the vector values  
	input.push_back(front);
	input.push_back(0);
	//sort values in ascending order
	sort(input.begin(), input.end());

	for (i = 0; i < input.size(); i++) {
		if (front == input[i])
			break;
	}
	int k = i;
	//sorts in descending order from initial position
	//then from 0 in ascending order
	if (k < input.size()) {
		for (i = k; i >= 0; i--) {
			output.push_back(input[i]);
		}
		for (i = k + 1; i < input.size(); i++) {
			output.push_back(input[i]);
		}
	}

	//print the chain of movements done
	cout << "\nSCAN DISK SCHEDULING ALGORITHM\n";
	cout << "Requests : \t Number of tracks traversed :" << endl;
	temp = output[0];
	cout << temp;
	for (i = 1; i < output.size(); i++) {
		cout << "\n" << output[i] << "\t\t\t" << abs(output[i] - temp);
		sum += abs(output[i] - temp);
		temp = output[i];
	}
	//print the total movements
	cout << '\n';
	cout << "Total head movements = " << sum << '\n';
}
void c_Scan(vector<int>& total, int front, int amount) {
	int i = 0, k = 0, sum = 0;

	//I/O requests and array b will store the output
	vector <int> out;
	int temp = front;
	//add initial position to the vector
	total.push_back(front);
	total.push_back(0);
	//sort the vector in ascending order
	sort(total.begin(), total.end());
	for (i = 0; i < total.size(); i++) {
		if (front == total[i])
			break;
	}
	k = i;
	//sort the vector from initial in ascending order
	//then from  0 in ascending again
	if (k < total.size()) {
		for (i = k; i <= total.size(); i++) {
			out.push_back(total[i]);
		}
		for (i = 1; i < k; i++) {
			out.push_back(total[i]);
		}
	}

	//adds every head movement 
	cout << "\nC-SCAN DISK SCHEDULING ALGORITHM\n";
	cout << "Requests : \t Number of tracks traversed :" << endl;
	temp = out[0];
	cout << temp;
	for (i = 1; i < out.size(); i++) {
		cout << "\n" << out[i] << "\t\t\t" << abs(out[i] - temp);
		sum += abs(out[i] - temp);
		temp = out[i];
	}
	cout << '\n';
	cout << "Total head movements = " << sum << '\n';
}
bool compareFirst(processRR p1, processRR p2) {
	//gives us which process arrived first
	return p1.arrivalTime < p2.arrivalTime;
}
bool compareSecond(processRR p1, processRR  p2) {
	return p1.processID < p2.processID;
}
void LeastRecentlyUsed(vector<int>& temp, int frame, int process) {
	//variables of integer type      
	int x, y, z;          //integer variables to be used in loops
	int counter = 0;      //counter variable initialized at zero(0)


	vector<int> count(process);
	//for loop for user to enter all the processes 
	//a = is the vector of vectors with different number of elements in different rows
	vector<vector<int>> a(frame);

	for (x = 0; x < frame; x++) {
		a[x] = vector<int>(process, -1);
	}
	//creating a map
	//the key for the map used is an integer and the value type is an integer and the map name is m
	map <int, int> m;

	for (x = 0; x < process; x++) {
		//c = is the vector of pairs with different number of pairs that store to values mapped to each other
		vector<pair<int, int>> c;
		//declaring q as an auto varible

		for (auto q : m) {
			//inserting an element at the end of the vector 
			//q.second gives us the value, while q.first gives us the key to access the map
			c.push_back({ q.second,q.first });
		}
		//sorting the frames
		sort(c.begin(), c.end());
		//check if the program ran correctly
		bool check = false;

		//implementing the LRU
		for (y = 0; y < frame; y++) {
			if (a[y][x] == temp[x]) {
				counter++;
				count[x] = 1;
				m[temp[x]] = 1;
				check = true;
				break;
			}


			if (a[y][x] == -1) {
				for (z = x; z < process; z++)
					a[y][z] = temp[x];
				m[temp[x]]++;
				check = true;
				break;
			}
		}

		//in case there is an error while executing the program
		if (y == frame || check == false) {
			for (y = 0; y < frame; y++) {
				if (a[y][x] == c[c.size() - 1].second) {
					//delete both vectors
					m.erase(a[y][x]);
					for (z = x; z < process; z++)
						a[y][z] = temp[x];
					m[temp[x]]++;
					break;
				}
			}
		}


		for (auto q : m) {
			if (q.first != temp[x]) {
				m[q.first]++;
			}
		}
	}
	//out put for the # of processes
	cout << "\nLeast Recently Used\n";


	//output for # of page faults caused by the algorithm
	cout << "\n";
	cout << '\n' << "The number of PAGE FAULTS caused by the algorithm is: " << process - counter << "\n";

}
void FirstInFirstOut(vector<int>& temp, int frame, int process) {
	//variables of integer type
	int x, y, z;      //integer variables to be used in loops 
	int counter = 0;  //counter variable initialized at zero(0)

	//user will enter the amount of frames

	//user will enter the number of processes
	vector<int> count(process);
	//for loop to enter al the processes 
	//a = is the vector of vectors with different number of elements in different rows
	vector<vector<int>> a(frame);

	for (x = 0; x < frame; x++) {
		a[x] = vector<int>(process, -1);
	}
	//creating a map 
	//the key for the map used is an integer and the value type is an integer and the map name is m
	map <int, int> m;

	for (x = 0; x < process; x++) {
		vector<pair<int, int>> c;
		for (auto q : m) {
			//inserting an element at the end of the vector 
			//q.second gives us the value, while q.first gives us the key to access the map
			c.push_back({ q.second,q.first });
		}
		//sorting the frames
		sort(c.begin(), c.end());
		//bool variable to check the program
		bool check = false;
		//implementing FIFO
		for (y = 0; y < frame; y++) {
			if (a[y][x] == temp[x]) {
				counter++;
				count[x] = 1;
				m[temp[x]]++;
				check = true;
				break;
			}

			if (a[y][x] == -1) {
				for (z = x; z < process; z++)
					a[y][z] = temp[x];
				m[temp[x]]++;
				check = true;
				break;
			}
		}
		//in case there is an error while running the program 
		if (y == frame || check == false) {
			for (y = 0; y < frame; y++) {
				if (a[y][x] == c[c.size() - 1].second) {
					//delete both vectors
					m.erase(a[y][x]);
					for (z = x; z < process; z++)
						a[y][z] = temp[x];
					m[temp[x]]++;
					break;
				}
			}
		}

		for (auto q : m) {
			if (q.first != temp[x]) {
				m[q.first]++;
			}
		}
	}

	cout << "First In Fisrt Out\n";
	cout << "\n";
	//output for the # of pagefaults caused by the algorithm
	cout << "\nThe number of PAGE FAULTS cause by the alogrithm is: " << process - counter << "\n";

}
void Optimal(vector<int>& p, int frame, int process) {

	//integer type variables 
	int x, y, z;                  //variables used for loops 

	vector<vector<int>> a(frame, vector<int>(process, -1));
	//creating a map
	//the key for the map used is an integer and the value type is an integer and the map name is m
	map <int, int> m;

	for (x = 0; x < process; x++) {
		vector<int> op;
		vector<pair<int, int>> c;
		for (auto q : m) {
			c.push_back({ q.second,q.first });
		}
		for (int q = x + 1; q < process; q++) {
			for (y = 0; y < frame; y++) {
				if (a[y][x] == p[q]) {
					op.push_back(p[q]);
				}
			}
		}
		//sort from begining 
		sort(op.begin(), op.end());
		//removes an element position from the vector
		op.erase(unique(op.begin(), op.end()), op.end());
		bool test = true;
		if (op.size() == frame) {
			test = false;
		}
		//sort from begining
		sort(c.begin(), c.end());
		bool check = false;
		for (y = 0; y < frame; y++) {
			if (a[y][x] == p[x]) {
				m[p[x]]++;
				check = true;
				break;
			}
			//while z keeps being less than the number of processes keep going through the map
			if (a[y][x] == -1) {
				for (z = x; z < process; z++)
					a[y][z] = p[x];
				m[p[x]]++;
				check = true;
				break;
			}
		}
		//when y is equal to the number of frames stop printing more frames until we get the necessary amount
		if (y == frame || check == false) {
			for (y = 0; y < frame; y++) {
				if (test == true) {
					if (a[y][x] == c[c.size() - 1].second) {
						m.erase(a[y][x]);
						for (z = x; z < process; z++)
							a[y][z] = p[x];
						m[p[x]]++;
						break;
					}
				}
				//if user enters an incorrect value delete it and request it again
				else if (test == false) {
					if (a[y][x] == op[op.size() - 1]) {
						m.erase(a[y][x]);
						for (z = x; z < process; z++)
							a[y][z] = p[x];
						m[p[x]]++;
						break;
					}
				}
			}
		}
		for (auto q : m) {
			if (q.first != p[x]) {
				m[q.first]++;
			}
		}
	}
	//keep going through all the processes with a vector called count
	int counter = 0;
	vector<int> count(process);
	for (x = 1; x < process; x++) {
		for (y = 0; y < frame; y++) {
			if (p[x] == a[y][x - 1]) {
				counter++;
				count[x] = 1;
				break;
			}
		}
	}
	//output for processes
	cout << "\nOptimal Page Replacement\n";

	//output for the amount of page faults caused by the algorithm
	cout << "\n";
	cout << "\nThe amount of page faults caused by the algorithm is: " << process - counter << "\n";

}