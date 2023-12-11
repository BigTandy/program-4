
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
	Node* next;
	Node* priv;
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

	/*
		This condition checks that the return code from `fscanf` is not either EOF or 0
		It does this by doing an `or` operation on `ret == EOF` and `ret == 0` if either become true the whole statement does,
			which means that we hit either EOF or 0 args filled
		And since we want to search and fill as long as that does -not- happen, we invert the whole statement

		We also check that we are within the size of the buffer, as to not overflow it
	*/


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



	/*
	ret = fscanf(src, "%d", &current);

	while ((!(ret == EOF || ret == 0)) && (i < slen)) {
		ret = fscanf(src, "%d", &current);
		scores[i] = current;
		i++;
	}
	*/

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
	FILE* quiz = fopen("C:\\Users\\Phoebe\\OneDrive - UMass Lowell\\Program 4 Format Score Record\\quiz.txt", "r");

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
	recordList->r = {
		"ROOT", "ROOT", {0}, 0.0
	};
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



	dumpList(recordList);

	FILE* out = fopen("average.txt", "w");

	if (out == NULL) {
		fprintf(stderr, "Error opening file, quiting");
		return 1;
	}

	outputRecords(recordList, stdout);

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
			fprintf(outSrc, "%4d ", curr->r.Scores[i]);
		}
		fprintf(outSrc, "%10.2lf\n", curr->r.Average);
	}


}




