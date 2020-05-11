#include <string>
#include <iostream> 
#include <sstream>
#include <cstdlib>

using namespace std;

const int MAXCHILDREN = 10;
const int MAXPERSONS = 99;
const int MAXFAMILIES = 99;
const int MATRIXS = 98;

struct family;

//struct to hold a person object
struct person {
    int personNum = 0;
    family *parent = NULL;
    family *marriage = NULL;
}; //person

//struct to hold a family object
struct family{
    int familyNum = 0;;
    person *husband = NULL;
    person *wife = NULL;
    person *children[MAXCHILDREN];
}; //family

//Node for the queue
struct node{
    int num;
    //1 if Person 2 if Family
    int PorF;
    int backNum;
    node *backN;
    node *next;
}; //node

//Queue to handle the BFS
struct queue{
    node *front;
    node *rear;
}; //queue

//Make a new node
node *makeNode(int num, int PorF, int backNum, node* backN, node* next){
    node *answer = new node;
    answer->num = num;
    answer->PorF = PorF;
    answer->backNum = backNum;
    answer->next = next;
    answer->backN = backN;
    return(answer);
} //makeNode()

//Make a new empty queue
queue makeEmptyQueue() {
    queue answer;
    answer.front = answer.rear = makeNode(0, 0, 0, NULL, NULL);
    return answer;
} //makeEmptyQueue()

//Checks to see if the queue is empty
bool isEmptyQueue(queue &theQueue){
    return(theQueue.front == NULL);
} //isEmptyQueue()

//inserts node into the Queue
void insertQueue(queue &theQueue, int PorF, int backNum, int num, node* backN){
    node *newNode = makeNode(num, PorF, backNum, backN, NULL);
    if(isEmptyQueue(theQueue)){
        theQueue.rear = newNode;
        theQueue.front = newNode;
    }
    else{
        theQueue.rear->next = newNode;
        theQueue.rear = newNode;
    }
} //insertQueue()

//deletes a node from the end of the queue
node* deleteQueue(queue &theQueue){
    node *oldNode = new node();
    if(isEmptyQueue(theQueue)){
        oldNode = NULL;
        return oldNode;
    }

    oldNode = theQueue.front;
    theQueue.front = oldNode->next;
    return oldNode;        
} //deleteQueue()

//Adds a person to the adj matrix and to the person and family arrays
void addPerson(person people[], family fams[], int adjMatrixPF[MATRIXS][MATRIXS], int adjMatrixFP[MATRIXS][MATRIXS]){
    int personNum, parentNum, marriageNum;
    string parentS, marriageS;

    cin >> personNum >> parentS >> parentNum >> marriageS >> marriageNum;

    if(parentNum != 0){
        people[personNum].parent = &fams[parentNum];
        //add to adj matrix
        adjMatrixPF[personNum-1][parentNum-1] = 1;
    }

    if(marriageNum != 0){
        people[personNum].marriage = &fams[marriageNum];
        adjMatrixPF[personNum-1][marriageNum-1] = 2;
    }
    people[personNum].personNum = personNum;
    
    //print the person
    cout << "Person " << personNum << " has parents " << parentNum << " and is married in family " << marriageNum << "." << endl;
} //addPerson()

//Adds a family to the adj matrix as well as the fams and person arrays
void addFamily(person people[], family fams[], int adjMatrixPF[MATRIXS][MATRIXS], int adjMatrixFP[MATRIXS][MATRIXS]){
    int familyNum, husbandNum, wifeNum;
    int childCnt = 0;
    string husbandS, wifeS, childS;
    string childrenLine; 
    int childNumsArr[MAXCHILDREN];

    cin >> familyNum >> husbandS >> husbandNum >> wifeS >> wifeNum;
    if(husbandNum != 0){
        fams[familyNum].husband = &people[husbandNum];
        //add to adj matrix
        adjMatrixFP[husbandNum-1][familyNum-1] = 3;
    }
    if(wifeNum != 0){
        fams[familyNum].wife = &people[wifeNum];
        adjMatrixFP[wifeNum-1][familyNum-1] = 4;
    }

    fams[familyNum].familyNum = familyNum;
    
    //hand taking in the children
    getline(cin, childrenLine);
    stringstream iss(childrenLine);
    string intermediate;
    int childNum;

    while(getline(iss, intermediate, ' ')){
        if(intermediate != "Child"){
            childNum = atoi(intermediate.c_str());
            if(childNum != 0){
                fams[familyNum].children[childCnt] = &people[childNum];
                adjMatrixFP[childNum-1][familyNum-1] = 5;
                childNumsArr[childCnt] = childNum;
                childCnt++;
            }
        }
    }
    
    //Print the family
    cout << "Family " << familyNum << " has husband " << husbandNum << ", wife " << wifeNum << ", and children";

    for(int i = 0; i < childCnt; i++){
        cout << " " << childNumsArr[i];
    }
    cout << "." << endl;
} //addFamily()

//Runs the verify commands to make sure the arrays are formatted correctly
void verifyPeople(person people[], family fams[], int adjMatrixPF[MATRIXS][MATRIXS], int adjMatrixFP[MATRIXS][MATRIXS]){

    int isFine = 0;

    for(int i = 0; i < MAXFAMILIES; i++){
        for(int j = 0; j < MAXPERSONS; j++){
            if(adjMatrixPF[j][i] == 1){
                if(adjMatrixFP[j][i] != 5){
                    cout << "Person " << j+1 << " points to parent family " << i+1 << " but there is no back-pointer." << endl;
                    isFine = 1;
                }
            }
            else if(adjMatrixPF[j][i] == 2){
                if(adjMatrixFP[j][i] != 3 && adjMatrixFP[j][i] != 4){
                    cout << "Person " << j+1 << " points to marriage family " << i+1 << " but there is no back-pointer." << endl;
                    isFine = 1;
                }
            }
        }
    }
    for(int k = 0; k < MAXFAMILIES; k++){
        for(int l = 0; l < MAXPERSONS; l++){
            if(adjMatrixFP[l][k] == 3){
                if(adjMatrixPF[l][k] != 2){
                    cout << "Family " << k+1 << " points to husband person " << l+1 << " but there is no back-pointer." << endl;
                    isFine = 1;
                }
            }
            else if(adjMatrixFP[l][k] == 4){
                if(adjMatrixPF[l][k] != 2){
                    cout << "Family " << k+1 << " points to wife person " << l+1 << " but there is no back-pointer." << endl;
                    isFine = 1;
                }
            }
            else if(adjMatrixFP[l][k] == 5){
                if(adjMatrixPF[l][k] != 1){
                    cout << "Family " << k+1 << " points to child person " << l+1 << " but there is no back-pointer." << endl;
                    isFine = 1;
                }
            }
        }
    }
    if(isFine == 0){
        cout << "The data is consistant" << endl;
    }
}  //verifyPeople()

//This runs a BFS search on the fams and people arrays to find the shortest distance between 2 points
void relate(person people[], family fams[], int adjMatrixPF[MATRIXS][MATRIXS], int adjMatrixFP[MATRIXS][MATRIXS]){
    int num1 = 0;
    int num2 = 0;

    cin >> num1 >> num2;

    bool visitedP[MAXPERSONS] = {false};
    bool visitedF[MAXFAMILIES] = {false};
    bool endFound = false;

    queue workQueue = makeEmptyQueue();
    visitedP[num2] = true;
    insertQueue(workQueue, 1, 0, num2, NULL);
    node* place = makeNode(0,0,0, NULL, NULL);
    node* endNode = makeNode(0,0,0, NULL, NULL);
    while((!isEmptyQueue(workQueue)) && (endFound == false)){
        place = deleteQueue(workQueue);

        //If the node we are looking at is a person
        if(place->PorF == 1){
            if(place->num == num1){
                endNode = place;
                endFound = true;
                break;
            }
            if(people[place->num].parent != NULL){
                if((people[place->num].parent->familyNum != 0) && (visitedF[(people[place->num].parent->familyNum)-1] == false)){
                    visitedF[(people[place->num].parent->familyNum)-1] = true;
                    insertQueue(workQueue, 2, place->num, people[place->num].parent->familyNum, place);
                }
            }
            if(people[place->num].marriage != NULL){
                if((people[place->num].marriage->familyNum != 0) && (visitedF[(people[place->num].marriage->familyNum)-1] == false)){
                    visitedF[(people[place->num].marriage->familyNum)-1] = true;
                    insertQueue(workQueue, 2, place->num, people[place->num].marriage->familyNum, place);
                }
            }

        }
        //If the node we are looking at is a family
        else if(place->PorF == 2){
            if(fams[place->num].husband != NULL){
                if((fams[place->num].husband->personNum != 0) && (visitedP[(fams[place->num].husband->personNum)-1] == false)){
                    visitedP[(fams[place->num].husband->personNum)-1] = true;
                    insertQueue(workQueue, 1, place->num, fams[place->num].husband->personNum, place);
                }
            }
            if(fams[place->num].wife != NULL){
                if((fams[place->num].wife->personNum != 0) && (visitedP[(fams[place->num].wife->personNum)-1] == false)){
                    visitedP[(fams[place->num].wife->personNum)-1] = true;
                    insertQueue(workQueue, 1, place->num, fams[place->num].wife->personNum, place);
                }
            }
            for(int i = 0; i < MAXCHILDREN; i++){
               if(fams[place->num].children[i] != NULL){
                    if((fams[place->num].children[i]->personNum != 0) && (visitedP[(fams[place->num].children[i]->personNum)-1] == false)){
                        visitedP[(fams[place->num].children[i]->personNum)-1] = true;
                        insertQueue(workQueue, 1, place->num, fams[place->num].children[i]->personNum, place);
                    }
                }
            }
        }
    }
    //print the backtrack
    node* currentN = makeNode(0,0,0, NULL, NULL);

    if(endNode->backN != NULL){
        cout << "person " << endNode->num;
    }
    else{
        cout << "Finding the Shortest Path failed" << endl;
    }

    while(endNode->backN != NULL){
        currentN = endNode->backN;
        cout << " -> family " << currentN->num;
        endNode = currentN->backN;
        cout << " -> person " << endNode->num;
    }
    cout << endl;

} //relate()
    

int main() {
    string command;

    person people[MAXPERSONS+1];
    family fams[MAXFAMILIES+1];

    int adjMatrixPF[MATRIXS][MATRIXS] = {0};
    int adjMatrixFP[MATRIXS][MATRIXS] = {0};

    while(!(cin >> command).eof()){
        if(command == "Person"){
            addPerson(people, fams, adjMatrixPF, adjMatrixFP);
        }
        else if(command == "Family"){
            addFamily(people, fams, adjMatrixPF, adjMatrixFP);        
        }
        else if(command == "Verify"){
            verifyPeople(people, fams, adjMatrixPF, adjMatrixFP);
        }
        else if(command == "Relate"){
            relate(people, fams, adjMatrixPF, adjMatrixFP);        
        }
    }
    return 0;
} //Main()

