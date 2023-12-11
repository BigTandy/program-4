/*
 Computing I -- COMP.1010 Honor Statement
 
 The practice of good ethical behavior is essential for maintaining
 good order in the classroom, providing an enriching learning
 experience for students, and as training as a practicing computing
 professional upon graduation.  This practice is manifested in the
 University’s Academic Integrity policy. Students are expected to
 strictly avoid academic dishonesty and adhere to the Academic
 Integrity policy as outlined in the course catalog.  Violations
 will be dealt with as outlined therein.
 
 All programming assignments in this class are to be done by the
 student alone.  No outside help is permitted except the instructor
 and approved tutors.
 
 I certify that the work submitted with this assignment is mine and
 was generated in a manner consistent with this document, the
 course academic policy on the course website on Blackboard, and
 the UMass Lowell academic code.
 
 Date: Dec, 10, 2023
 Name: Bud Patterson
 */



/***********************************************
Author: Bud Patterson
Date: Dec 10, 2023
Purpose: Compute averages for quiz scores for a class
Sources of Help: Stackoverflow, and wikipedia
Difference: By using functions, you are able to make the program more modular, easier to read, easier to modify, and easy to debug
Time Spent: Around 3 hours
***********************************************/

/*
Computing I -- COMP.1010 Honor Statement
The practice of good ethical behavior is essential for maintaining good order
in the classroom, providing an enriching learning experience for students, and
as training as a practicing computing professional upon graduation. This
practice is manifested in the University’s Academic Integrity policy. Students
are expected to strictly avoid academic dishonesty and adhere to the Academic
Integrity policy as outlined in the course catalog. Violations will be dealt
with as outlined therein.
All programming assignments in this class are to be done by the student alone.
No outside help is permitted except the instructor and approved tutors.
I certify that the work submitted with this assignment is mine and was generated
in a manner consistent with this document, the course academic policy on the
course website on Blackboard, and the UMass Lowell academic code.
Date: Dec 10, 2023
Name: Bud Patterson
*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//Student record
typedef struct Record {
	char First[11];
	char Last[11];
	int Scores[10];
	double Average;
} Record;


//Linked list node, for arbitrary number of records
typedef struct Node {
	Record r;
	struct Node* next;
	struct Node* priv;
} Node;

/*
	The reason I am using linked lists, and parsing and storing the data before outputting is for keeping the code simple,
	and easily modifiable (incase requirements change, or if I have to modify the program, along with easier debugging)

	I feel that keeping the parsing of the data and the outputing of the processed data uncoupled is best practice,
		it keeps the code readable, modular, easily modifiable, and easily debuggable as the control flow and data flow is simple
*/






int append(Node* root, char First[], char Last[], int Scores[], double Average) {

	Node* currentNode = root;

	//Find the last node in the list
	
	while (currentNode->next != NULL) {
		currentNode = currentNode->next;
	}
	


	//Create a new Node
	Node* NewNode = (Node*)malloc(sizeof(Node));

	if (NewNode == NULL) {
		// If allocating memory failed, let calling function know
		fprintf(stderr, "Error opening file, quiting");
		return 1;
	}


	//Code to initalize arrays in the new record
	for (int i = 0; i < 11; i++) {
		NewNode->r.First[i] = First[i];
		NewNode->r.Last[i] = Last[i];
		if (i < 10) {
			NewNode->r.Scores[i] = Scores[i];
		}
	}
	NewNode->r.Average = Average;

	//Setting up forward and back links for the new node and the current node
	currentNode->next = NewNode;
	NewNode->priv = currentNode;
	NewNode->next = NULL;

	

	return 0;
}


/*
	This function is used for debugging purposes, it dumps the contents of the entire list to stdout 
*/
void dumpList(Node* node) {


	int i;

	for (Node* curr = node; curr != NULL; curr = curr->next) {
		printf("First: %s\n", curr->r.First);
		printf("Last: %s\n", curr->r.Last);
		printf("Scores: ");
		for (i = 0; i < 10; i++) {
			printf("%d ", curr->r.Scores[i]);
		}
		printf("\nAverage: %.2lf\n\n", curr->r.Average);
	} 

	return;
}


void extract_name(FILE* src, char first[], char last[], int flen, int llen) {



	for (int k = 0; k < 11; k++) {
		first[k] = '\0';
		last[k] = '\0';
	}


	/*
	This loop iterates over the first name and assigns it to the `first` array until it either hits a space or hits the string length
	*/
	int i = 0;
	char current;
	do {
		fscanf(src, "%c", &current);
		first[i] = current;
		i++;
	} while (current != ' ' && i < flen - 1);
	first[i - 1] = '\0';


	/*
	This loop iterates over the last name and assigns it to the `first` array until it either hits a space or hits the string length
	*/
	i = 0;
	do {
		fscanf(src, "%c", &current);
		last[i] = current;
		i++;
	} while ((current != ' ' && current != '\n') && i < llen - 1);
	last[i - 1] = '\0';


	return;
}


void extract_scores(FILE* src, int scores[], int slen) {

	//Zero out the scores array
	for (int j = 0; j < slen; j++) {
		scores[j] = 0;
	}


	int i = 0;
	int current, ret; //Current score, and `fscanf` return code



	for (i = 0; i < 10; i++) {
		ret = fscanf(src, "%d", &current);
		if (ret == EOF || ret == 0) {
			return;
		}
		scores[i] = current;
	}

	//This is just to consume the newline char at the end of the line, it causes issues
	char throwaway;
	fscanf(src, "%c", &throwaway);




	return;
}



double compute_average(int scores[], int slen) {

	int sum = 0;

	for (int i = 0; i < 10 && i < slen; i++) {
		sum += scores[i];
	}

	return (double)sum / 10.0;
}



void outputRecords(Node* root, FILE* outSrc);



int main(int argc, char* argv[]) {


	//Open input file
	FILE* quiz = fopen("quiz.txt", "r");

	if (quiz == NULL) {
		fprintf(stderr, "Error opening file, quiting");
		return 1;
	}


	Node* recordList = (Node*)malloc(sizeof(Node));

	if (recordList == NULL) {
		fprintf(stderr, "Heap Error, quitting");
		return 2;
	}

	//Init root node
	//recordList->r 
	
	char rootName[] = "ROOT";
	
	for (int k = 0; k < 10; k++) {
	   recordList->r.First[k] = rootName[k];
	   recordList->r.Last[k] = rootName[k];
	   recordList->r.Scores[k] = 0;
	}
	recordList->r.Average = 0;
	
	recordList->priv = NULL;
	recordList->next = NULL;


	char FirstTemp[11], LastTemp[11];
	int ScoreTemp[10];

	int appendReturnCode;

	do {

		//Grab the names and scores from the file
		extract_name(quiz, FirstTemp, LastTemp, 11, 11);
		extract_scores(quiz, ScoreTemp, 10);


		//Append new record to the end of the list
		appendReturnCode = append(
			recordList, FirstTemp, LastTemp, ScoreTemp, compute_average(ScoreTemp, 10)
		);

		if (appendReturnCode != 0) {
			fprintf(stderr, "Heap Error, quitting");
			return 2;
		}




	} while (!feof(quiz));
	// Conditional checks that we are not at `EOF` yet


	//Close the file as we are done with it
	fclose(quiz);



	//dumpList(recordList);

	FILE* out = fopen("average.txt", "w");

	if (out == NULL) {
		fprintf(stderr, "Error opening file, quiting");
		return 1;
	}

	outputRecords(recordList, out);

	fclose(out);


	return 0;
}





void outputRecords(Node* root, FILE* outSrc) {


	int i;

	char name[25];


	for (Node* curr = root; curr != NULL; curr = curr->next) {

		if (curr == root) {
			continue;
		}

		sprintf(name, "%s, %s", curr->r.Last, curr->r.First);

		fprintf(outSrc, "%-20s", name);

		for (i = 0; i < 10; i++) {
			fprintf(outSrc, "%4d", curr->r.Scores[i]);
		}
		fprintf(outSrc, "%10.2lf\n", curr->r.Average);
	}


}
