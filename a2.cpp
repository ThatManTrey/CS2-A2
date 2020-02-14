//a2.cpp
//calculates factorial
//stores in linked list, then linked stack
//displays factorial, number of digits, process times
//in terminal and A2.out

#include<iostream>
#include<iomanip>	//to optimize display
#include<fstream>	//for outfile
#include<ctime>	//for time obervation

using namespace std;

//node structure for lists
typedef struct node *node_ptr;
struct node
{
	double data;
	node_ptr next;
};

//calculates factorial
//Pre: calculation adt is initialized, factorial entered
//Post: stores 8 digit chunks of calculation in linked list,
//headed by node_ptr calculate
void calculate(node_ptr calculation, const int &factorial);

//reverses linked list headed by node_ptr calculation
//node_ptr number points to new head
//counts nodes to find number of digits
//Pre: calculation determined
//Post: number full with nodes to display sequentially,
//number of digits in number calculated
void reverse(node_ptr &calculation, node_ptr &number, int &digits);

//prints number stored in linked stack number, 8 digit chunks
//displays factorial calculated, digits, time for each process:
//calculation, reversing, display
//stores all output in outfile
//Pre: outfile opened, number contains number, factorial entered, times calculated
//Post: number list is empty, output contains calculation w/ stats
void print(ofstream &outfile, node_ptr &number, const int &factorial, const int &digits, const float &calc_time, const float &rev_time);

//main program
int main()
{
	//open A2.out for storing data
	ofstream outfile;
	outfile.open("A2.out");
	if (outfile.fail())
	{
		cout << "Error opening file.\n";
		exit(1);
	}
	
	//prompt, user enters factorial to be calculated
	int factorial;
	cout << "Welcome to my badass factorial calculator.\n";
	cout << "Enter 'N' to be calculated in 'N!': ";
	cin >> factorial;
	
	//program terminates if factorial is negative
	while(factorial >= 0)
	{
		int digits = 0;	//digit number var
		node_ptr calculation = new node; //head for linked list	
		calculation->data = 1;	//initialize list for 0!=1!=1
		calculation->next = NULL;
		node_ptr number = new node;	//linked stack pointer

		cout << "\nBeginning calculation:\n";
		clock_t calc_start = clock();	//start calc time
		calculate(calculation, factorial);	//store each 8 digits of factorial!
											//in nodes of adt headed by calculation
		float calc_time = static_cast<float>(clock()) - static_cast<float>(calc_start);
		//end calc time, store in calc_time
	
		cout << "\nReversing:\n";
		clock_t rev_start = clock();	//start reverse time
		reverse(calculation, number, digits);	//number is now accessible via number linked stack
												//store number of digits
		float rev_time = static_cast<float>(clock()) - static_cast<float>(rev_start);
		//end reverse time, store in rev_time

		cout << "\nPrinting:\n";
		print(outfile, number, factorial, digits, calc_time, rev_time);
		//print each node in number, along with other data

		//prompt for next calculation
		cout << "\nEnter 'N' to be calculated in 'N!'\n";
		cin >> factorial;
		//nodes deallocated at end of loop
	}
	
	cout << "\nThanks for tryin' dis out homie.\n\n";
	
	outfile.close();	//close outfile

	return 0;
}

void calculate(node_ptr calculation, const int &factorial)
{
	unsigned long long int result = 0;	//stores result after individual computations
	// max = 18,446,744,073,709,551,615
	int carry;

	int max = 1, current = 0;	//max = number of nodes in adt, current node
	double base = 100000000;	//refers to max of each node: 8 digits

	node_ptr temp = new node;	//temp node to traverse adt
	//starts calculation at 2!, bypasses need for 0!=1!
	for (int x = 2; x <= factorial; x++)
	{
		current = 0;	//resets current index
		carry = 0;	//reset carry
		temp = calculation;	//assign temp to adt head
		//while current < max
		while (current < max)
		{	
			result = temp->data * x + carry;
			//result = node * current_factorial + previous carry value
			temp->data = result % static_cast<int>(base);	//node = result mod base
			carry = static_cast<int>(result / base);	//carry for next = result div base

			//for debugging, procedural observation
			//cout << fixed << setprecision(0) << "Next list node(x, k, index, carry, result, temp->data): "
			//	<< x << " " << k << " " << index << " " << carry << " " << result << " " << temp->data << endl;
			//system("pause");

			//if adt is full, and there is more to calculate, allocate new space
			if (carry > 0 && temp->next == NULL)
			{
				max++;	//max++
				//initialize new node
				node_ptr newnode = new node;
				newnode->data = 0;
				newnode->next = NULL;
				temp->next = newnode;	//connect to adt
			}
			current++;	//next index
			temp = temp->next;	//next node
		}
	}
}

void reverse(node_ptr &calculation, node_ptr &number, int &digits)
{
	digits = 0;	//initialize to 0

	//if calculation has one node
	if (calculation == NULL)
	{
		digits = 8;	//max digits = 8
		number = calculation;	//list and stack point to same node
	}
	//multiple nodes
	else
	{
		node_ptr prev = NULL, next = NULL, current = calculation;
		//while current node isn't null
		while (current != NULL)
		{
			//pointers become reversed, point backwards through list
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
			digits += 8;	//each node has 8 digits max
		}
		number = prev;	//stack pointer = last node of linked list
	}

	long compare = 10000000;	//base
	double firstnode = number->data;	//top or leading digits
	//while first node is less than comparison
	while (firstnode < compare)
	{
		//for each digit less than 8 the first node is,
		//subtract 1 from digits to compensate
		digits--;
		compare /= 10;	//adjust comparison
	}
}

void print(ofstream &outfile, node_ptr &number, const int &factorial, const int &digits, const float &calc_time, const float &rev_time)
{
	bool firstline = true;	//to take care of leading zeros
	int characters = 13;	//characters initialized to include first cout string
	
	//prompt for calculation
	cout << "Calculation: ";
	outfile << "Calculation: ";

	clock_t display_start = clock();	//start timer for display
	
	//recount digits in first node
	int top = static_cast<int>(number->data);
	do {
		++characters;	//add to char counter
		top /= 10;
	} while (top);	//while remainder isn't 0
	
	//output first node, sets cout to fixed notation, no decimals
	cout << fixed << setprecision(0) << number->data;
	outfile << fixed << setprecision(0) << number->data;
	//pop top node
	node_ptr firstptr = number;
	number = number->next;
	delete firstptr;

	//while stack isn't empty
	while (number != NULL)
	{
		//if not first line and line has 80 char
		if ((!firstline) && (characters == 80))
		{
			//end the line
			cout << endl;
			outfile << endl;
			characters = 0;	//resets char counter
		}
		//if first line and line is b/w 72 and 80 characters
		else if ((firstline) && characters >= 80 - 8)
		{
			//end the line
			cout << endl;
			outfile << endl;
			characters = 0;	//resets char counter
			firstline = false;	//never again first line
		}

		//if top node is zero
		if (number->data == 0)
		{
			//print 8 0s for node
			cout << "00000000";
			outfile << "00000000";
			characters += 8;
			/*
			//rest of digits are 0s
			int rest = digits - count;	//rest = total - used digits
			//for rest of digits, output 0
			for(int i = 0; i < rest; i++)
			{
				// if 80 char, end line, reset char counter
				if(characters == 80)
				{
					cout << endl;
					outfile << endl;
					characters = 0;
				}
				cout << 0;
				outfile << 0;
				characters++;
			}
			*/
			node_ptr zero = number;
			number = number->next;
			delete zero;
		}
		else
		{
			int base = 10000000;
			//while top node is less than base digits
			//and if whole number is more than one node's worth of digits
			while ((number->data < base) && (digits > 8))
			{
				//add zero to output
				cout << "0";
				base /= 10;
			}
			//output node
			cout << number->data;
			outfile << number->data;
			//add 8 to counter
			characters += 8;
			//pop number's top node
			node_ptr temp = number;
			number = number->next;
			delete temp;
		}
	}
	float display_time = static_cast<float>(clock()) - display_start;	//end display timer
	//terminal out
	cout << endl;	//end number
	cout << "Factorial computed: " << factorial << "!" << endl;
	cout << "Total digits: " << digits << endl;
	cout << "Calculation time: " << (calc_time / CLOCKS_PER_SEC) << " seconds" << endl;
	cout << "Reversing time: " << (rev_time / CLOCKS_PER_SEC) << " seconds" << endl;
	cout << "Display time: " << (display_time / CLOCKS_PER_SEC) << " seconds" << endl;
	//outfile out
	outfile << endl;
	outfile << "Factorial computed: " << factorial << "!" << endl;
	outfile << "Total digits: " << digits << endl;
	outfile << "Calculation time: " << (calc_time / CLOCKS_PER_SEC) << " seconds" << endl;
	outfile << "Reversing time: " << (rev_time / CLOCKS_PER_SEC) << " seconds" << endl;
	outfile << "Display time: " << (display_time / CLOCKS_PER_SEC) << " seconds" << endl << endl;
}