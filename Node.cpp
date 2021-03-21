#include<iostream>
using namespace std;

/**
@Node class represents a process in Operating System
It contains a value which is also a process id
It also contains a reference to it's next and previous sibling processes
It also contains priority which is between 0 - 10
*/
class Node
{
public:
	Node(int value, Node* nextptr = NULL, Node* prevptr = NULL, int currentpriority = 0){
		
		// if priority is not in range, then raise an exception
		if (currentpriority < 0 || currentpriority > 10){
			throw "Invalid priority value. It should be or in between 0 and 10";
		}
		
		this -> value = value;
		this -> next = nextptr;
		this -> prev = prevptr;
		this -> priority = currentpriority;
	}
	
	// @Returns value or process id
	int getVal(void){
		return this -> value;
	}

	// @Returns pointer to next Node
	Node* getNext(void){
		return this -> next;
	}

	// @Returns pointer to previous Node
	Node* getPrev(void){
		return this -> prev;
	}

	// set value or process id
	void setVal(int value){
		this -> value = value;
	}
	
	// set pointer to previous Node
	void setPrev(Node* prevptr){
		this -> prev = prevptr;
	}

	// set pointer to next Node
	void setNext(Node* nextptr){
		this -> next = nextptr;
	}

	// @Returns priority of current node/process
	int getPriority(void){
		return this -> priority;
	}

	// set priority of current node/process
	void setPriority(int priority){
		
		// if priority is not in range, then raise an exception
		if (priority < 0 || priority > 10){
			throw "Invalid priority value. It should be or in between 0 and 10";
		}
		this -> priority = priority;
	}

private:
	Node* next;
	Node* prev;
	int priority;
	int value;
};

/**
* @Stack class represents a stack of Node
* It contains a pointer to top Node
*/
class Stack
{
public:
	Stack(void){
		top = NULL;
	}
	
	// it deletes the pointers to each node in the stack
	~Stack(void){
		while (top != NULL){
			Node *temp = top;
			top = top -> getNext();
			delete temp;
		}
	}

	// push a node with a value into the stack
	void Push(int value){
		
		// creating a new node with stack's current top as it's next pointer node
		Node *newNode = new Node(value, top, NULL, 0);
		
		// if stack is not empty, then set top previous pointer to new node
		if (top != NULL){
			top -> setPrev(newNode);
		}
		
		// update top pointer to new node
		top = newNode;
	}

	/**
	* @Returns node reference to top element in stack
	* and removes the top element from the stack
	* It also raises an exception if stack is empty
	*/
	Node* NodePop(void){
		
		// raise an exception if stack is empty
		if (top == NULL){
			throw "Illegal Operation Exception. Stack is empty!";
		}
		
		// pop out a top node pointer and returns it
		Node* temp = top;
		top = top -> getNext();
		return temp;
	}

	/**
	* @Returns value of top node reference in stack
	* and removes the top element from the stack
	* It also raises an exception if stack is empty
	*/
	int Pop(void){
		
		// raise an exception if stack is empty
		if (top == NULL){
			throw "Illegal Operation Exception. Stack is empty!";
		}
		
		// pop out a top node pointer and returns its value
		Node* temp = top;
		top = top -> getNext();
		return temp -> getVal();
	}

private:

	Node* top;
};

/**
* @Queue class represents a queue of Node
* It contains a pointer to back node of queue
* It contains a pointer to front node of queue
*/
class Queue
{
public:
	Queue(void){
		back = NULL;
		front = NULL;
	}

	// it deletes the pointers to each node in the queue
	~Queue(void){
		
		while (back != NULL){
			Node *temp = back;
			delete temp;
			back = back -> getNext();
		}
	}

	/**
	* insert a new node in queue
	* @param i represents value/id 
	* @param priority represents the priority of process
	*/
	void Enqueue(int i, int priority = 0){
		
		// creating a new node with back pointer as new node's next pointer
		Node *newNode = new Node(i, back, NULL, priority);
		
		// if queue is not empty, set back previous pointer to new node
		if (back != NULL){
			back -> setPrev(newNode);
		}
		else{
			// sets front as new node
			front = newNode;
		}
		
		// updates back pointer with new node
		back = newNode;
	}
	
	/**
	* dequeue a node from queue
	* @Returns the id of node from queue
	* It also raises an exception if queue is empty
	*/
	int Dequeue(void){
		
		// raise an exception if queue is empty
		if (front == NULL){
			throw "Illegal Operation Exception. Queue is empty!";
		}
		
		// dequeue a node from queue and returns it's value
		Node* dequeNode = NodeDequeue();
		return dequeNode -> getVal();
	}

protected:

	Node* back;
	Node* front;

private:

	virtual Node* NodeDequeue(void){
		Node *temp = front;
		front = front -> getPrev();
		return temp;
	}
};

/**
* @Scheduler class extends the Queue class
* It schedules process based on priority
* It also prevents blocking of low priority processes
*/
class Scheduler : public Queue
{
	
public:
	
	/**
	* insert a new node in queue
	* @param i represents value/id 
	* @param priority represents the priority of process
	*/
	void Enqueue(int i, int priority = 0){
		
		// creating new node with front and back references as null
		Node *newNode = new Node(i, NULL, NULL, priority);
		
		// adding first item in the list
		if (front == NULL && back == NULL){
			back = newNode;
			front = newNode;
		}
		else{ // adding other items in the list
			Node *current = back;
			while (current != NULL){
				if (priority > current -> getPriority()){
					
					// increasing priority by 1 of less priority processes to prevent blocking
					current -> setPriority( current -> getPriority() + 1);
					
					current = current -> getNext();
					continue;
				}
				break;
			}
			
			// if priority is greater than every other process priority
			// then insert it in the start of queue
			if (current == NULL){
				front -> setNext(newNode);
				newNode -> setPrev(front);
				front = newNode;
			}
			else {
				// otherwise insert node in the middle of the list
				
				// setting current node previous as new node previous
				newNode -> setPrev(current -> getPrev());
				// setting new node as current's previous node's next
				if (current -> getPrev() != NULL){
					current -> getPrev() -> setNext(newNode);
				}
				else{
					back = newNode;
				}
				// setting current as new node next
				newNode -> setNext(current);
				// setting new node as current previous
				current -> setPrev(newNode);
			}
		}
		
	}
private:

	/**
	* dequeue a node pointer from queue
	*/
	Node* NodeDequeue(void){
		Node *temp = front;
		front = front -> getPrev();
		return temp;
	}
	
};




int main(){
	
	// Stack testing is start
//	Stack stack;
//	
//	stack.Push(5);
//	stack.Push(4);
//	stack.Push(10);
//	stack.Push(2);
//	stack.Push(1);
//	
//	cout << stack.Pop()<<endl;
//	cout << stack.NodePop() -> getVal()<<endl;
//	cout << stack.NodePop() -> getVal()<<endl;
//	cout << stack.Pop()<<endl;
//	cout << stack.Pop()<<endl;
//	
//	try {
//      cout << stack.Pop()<<endl;
//   } catch (const char* msg) {
//     cerr << msg << endl;
//   }

	// Stack testing is end
	
	// Queueu testing is start
//	Queue queue;
//	
//	queue.Enqueue(1, 9);
//	queue.Enqueue(2, 7);
//	queue.Enqueue(3, 8);
//	queue.Enqueue(4, 10);
//	queue.Enqueue(5, 4);
//	
//	try{
//		cout << queue.Dequeue() << endl;
//		cout << queue.Dequeue() << endl;
//		cout << queue.Dequeue() << endl;
//		cout << queue.Dequeue() << endl;
//		cout << queue.Dequeue() << endl;
//		cout << queue.Dequeue() << endl;
//	}
//	catch (const char* msg) {
//    	cerr << msg << endl;
//   	}
//	
	// Queue testing is end
	
	// Scheduler testing is start
	Scheduler scheduler;
	
	scheduler.Enqueue(1, 9);
	scheduler.Enqueue(2, 7);
	scheduler.Enqueue(3, 8);
	scheduler.Enqueue(4, 10);
	scheduler.Enqueue(5, 4);
	
	try{
		cout << scheduler.Dequeue() << endl;
		cout << scheduler.Dequeue() << endl;
		cout << scheduler.Dequeue() << endl;
		cout << scheduler.Dequeue() << endl;
		cout << scheduler.Dequeue() << endl;
		cout << scheduler.Dequeue() << endl;
	}
	catch (const char* msg) {
    	cerr << msg << endl;
   	}
	
	// Scheduler testing is end
	
	return 0;
}
