#include <iostream>
#include <cstring>
#include <fstream>
#include "Node.h"

void getInput(char* in); //Gets console input, stores into char* in
void read(Node* &root, char* in); //Gets file input, stores into char* in, sequentially adds into rbt
int getAction(char* in); //Gets input and returns a specific number corresponding to an action
void insert(Node* &root, int toAdd); //Insert int into tree 
void find(Node* &current, int toAdd); //Actually finds the place to insert node 
void correct(Node* &current); //Corrects added node to satisfy RBT properties
void correctCase1(Node* &current); //Root case
void correctCase2(Node* &current); //Parent is black
void correctCase3(Node* &current); //Parent is red (so it's not the root) and Uncle is red
void correctCase4(Node* &current); //Parent is red and Uncle is black, triangle case 
void correctCase4Step2(Node* &current); //Paren is red and Uncle is black, line case
void add(Node* &current, char* in); //Enter a number, which gets insert()ed into tree
void print(Node* current, int depth); //Prints the tree
void help(); //Prints list of commands

using namespace std;

//Main program
int main(){
  //Variable that stores user input
  char inArr[999];
  char* in = &inArr[0];

  //Root node of the red black tree
  Node* root = NULL;

  //Program loop
  bool running = true;
  while(running){
    //Ask user for their file name input, and input into tree
    read(root, in);

    cout << "Now entering the modding tree phase! Enter \"help\" for a list of commands." << endl;
    //Editing tree loop
    bool moddingTree = true;
    while(moddingTree){
      //Ask the user what they want to do. 
      switch(getAction(in)){
      case 2:
        //Add
        add(root, in);
        break;
      case 3:
        //Print
        print(root, 0);
        //Reset text color after print
        cout << "\033[0m\n" << endl;
        break;
      case 4:
        //Print list of commands
        help();
        break;
      case 5:
        //Quit
        cout << "Thank you for choosing bubbles!" << endl;
        moddingTree = false;
        break;
      }
    }
    //Prompt user if they want to start the program over
    cout << "Would you like to create another tree? (y/n)" << endl;
    while(true){
      getInput(in);
      in[0] = toupper(in[0]);
      if(strcmp(in, "Y") == 0){
        //Reset variables
        root = NULL;
        break;
      }else if(strcmp(in, "N") == 0){
        running = !running;
        break;
      }
      cout << "Please enter (y/n)!" << endl;
    }
  }
  return 0;
}

//This function takes in user input and sets the char* in variable equal to it
void getInput(char* in){
  while(true){
    cin.getline(in, 999);
    cin.clear();
    if(strcmp(in, "") != 0){
      break;
    }
    cout << "Please enter something..." << endl;
  }
}

//This function asks for file input name and sets the in variable equal to it
void read(Node* &root, char* in){
  //New file strem
  ifstream stream;
  while(true){
    cout << "Please enter the name of your file." << endl;
    getInput(in);
    //Open file
    stream.open(in);
    //Make sure that the file name is correct
    if(stream.good()){
      //If so, read and put into in
      stream.getline(in, 999);
      break;
    }
    //Otherwise do over
    cout << "File not found. Please make sure your file name is correct." << endl;
  }
  //Split input, convert into int, then call insertion
  char* buffer = new char[4];
  int counter = 0;
  int inLen = strlen(in);
  for(int a = 0; a <= inLen; ++a){
    if(in[a] == ' ' || a == inLen){
      //Pass into insert function
      insert(root, atoi(buffer));
      //Then reset counter and buffer
      buffer = new char[4];
      counter = 0;
      continue;
    }
    buffer[counter++] = in[a];
  }
}

//This function gets input using getInput() and strcmps to return a number to be used in the modding tree phase
int getAction(char* in){
  //Prompt user for a command
  cout << "Please enter a command." << endl;
  while(true){
    //Get input
    getInput(in);
    //Set everything to caps
    int inLen = strlen(in);
    for(int a = 0; a < inLen; ++a){
      in[a] = toupper(in[a]);
    }
    //Return a number corresponding to the command, or notify user of incorrect input.
    if(strcmp(in, "ADD") == 0){
      return 2;
    }else if(strcmp(in, "PRINT") == 0){
      return 3;
    }else if(strcmp(in, "HELP") == 0){
      return 4;
    }else if(strcmp(in, "QUIT") == 0){
      return 5;
    }
    cout << "\"" << in << "\" is not recognized as a command. Please type \"help\" for a list of commands!" << endl;
  }
}

//Calls find() to insert node, and also corrects the root after rotations if needed
void insert(Node* &root, int toAdd){
  //Actually inserts stuff
  find(root, toAdd);

  //Corrects the root after rotations
  if(root->getParent() != NULL){
    root = root->getParent();
  }
}

//Function that uses recursion to traverse the red black tree to find the right place to insert a node
void find(Node* &current, int toAdd){
  //Root case
  if(current == NULL){
    current = new Node(toAdd);
    correct(current);
    return;
  }
  //If number to add is greater than current node
  if(toAdd > current->getValue()){
    Node* right = current->getRight();
    //If there is a right subtree
    if(right != NULL){
      //Recurse
      insert(right, toAdd);
    }else{ //Otherwise, just add
      current->setRight(new Node(toAdd));
      right = current->getRight();
      //Set parent 
      right->setParent(current);
      //Might need to call correct here
      correct(right);
    }
  }else{ //If number to add is less than or equal to current node
    Node* left = current->getLeft();
    //If there is a left subtree
    if(left != NULL){
      insert(left, toAdd);
    }else{ //Otherwise, just recurse
      current->setLeft(new Node(toAdd));
      left = current->getLeft();
      //Set parent 
      left->setParent(current);
      //Might need to call correct here
      correct(left);
    }
  }
}


//Tries to repair the tree after the insertion, inspiration for correct cases from wikipedia: "https://en.wikipedia.org/wiki/Red%E2%80%93black_tree"
void correct(Node* &current){
  //New nodes are always inserted as red (false)

  if(current->getParent() == NULL){
    //This is the root case
    correctCase1(current);
  }else if(current->getParent()->getColor()){
    //This is when the parent is black
    correctCase2(current);
  }else if(current->getUncle() != NULL && current->getUncle()->getColor() == false){
    //This is when the uncle is red
    correctCase3(current);
  }else{
    //This is when the uncle is black
    correctCase4(current);
  }
  return;
}

//This function takes user input, converts it to a number, and calls insert
void add(Node* &root, char* in){
  cout << "Ok, please enter the number that you would like to add." << endl;
  getInput(in);
  insert(root, atoi(in));
  cout << "\"" << atoi(in) << "\" has been added to the tree." << endl;
}

//Tries to print all nodes in the red black tree, with color, sideways, using inorder traversal
void print(Node* current, int depth){
  //If null, just leave!
  if(current == NULL){
    return;
  }
  //Recurse to the the right
  print(current->getRight(), depth+1);
  //Print current node
  for(int a = 0; a < depth; ++a){
    cout << "    ";
  }
  //Set corresponding color
  if(current->getColor()){
    //Black (but actually gold lol)
    cout << "\033[1;33m";
  }else{
    //Red (false)
    cout << "\033[1;31m";
  }
  //Print node and reset color
  cout << current->getValue() << "\n" << endl;
  //Recurse to the left
  print(current->getLeft(), depth+1);
}

//Prints a list of all commands and their functions
void help(){
  cout << "\n----------\nEnter \"add\" to add a number to the tree,\n\"print\" to print the tree,\nor \"quit\" to exit the modding tree phase.\nType \"help\" again to reprint this list.\n----------\n" << endl;
}

//Root case
void correctCase1(Node* &current){
  //cout << current->getValue() << " is a ";
  //cout << "root case! (change color and we're done!)" << endl;
  //Color black
  current->setColor(true);
}
//Parent is black
void correctCase2(Node* &current){
  //cout << current->getValue() << " is a ";
  //cout << "Parent is black! (we're done!)" << endl;
  return;
}
//Parent is red (so it's not the root) and Uncle is red
void correctCase3(Node* &current){
  Node* grandParent = current->getGrandParent();
  //cout << current->getValue() << " is a ";
  //cout << "Parent and uncle are red! (Need to recolor and recurse)" << endl;
  //Change parent and uncle to black
  current->getParent()->setColor(true);
  current->getUncle()->setColor(true);
  //Change grandparent to red
  grandParent->setColor(false);
  //Recursively call on grandparent
  correct(grandParent);
}
//Parent is red and Uncle is black (triangle and line)
void correctCase4(Node* &current){
  //cout << current->getValue() << " is a ";
  //cout << "Parent is red, uncle is black! (Need to find out if it's the triangle or line case, then rotate)" << endl;

  Node* parent = current->getParent();
  Node* grandParent = current->getGrandParent();
  //First check if it is a triangle (left side)
  if(grandParent->getLeft() == parent && parent->getRight() == current){
    //cout << "Triangle, We're rotating left" << endl;
    //Then we need to do a left rotation through the parent
    parent->rotateLeft();
    current = current->getLeft();
    //Then check if it is a triangle (right side)
  }else if(grandParent->getRight() == parent && parent->getLeft() == current){
    //cout << "Triangle, We're rotating right" << endl;
    //Then we need to do a right rotation through the parent
    parent->rotateRight();
    current = current->getRight();
  }

  //Then see if we need to consider the line case and rotate the grandparent
  Node* t = current;
  correctCase4Step2(t);
}

void correctCase4Step2(Node* &current){
  Node* parent = current->getParent();
  Node* grandParent = current->getGrandParent();
  //Then we need to check if it is a line (left side)
  if(grandParent->getLeft() == parent && parent->getLeft() == current){
    //cout << "Line, We're rotating right" << endl;
    //Then we need to rotate the grandparent, in the opposite direction (in this case, right)
    grandParent->rotateRight();
  }
  //Then check if it is a line (right side)
  else if(grandParent->getRight() == parent && parent->getRight() == current){
    //cout << "Line, We're rotating left" << endl;
    //Then we need to rotate the grandparent, in the opposite direction (in this case, left)
    grandParent->rotateLeft();
  }
  //Recolor everything, swap parent and grandparent colors
  parent->setColor(true);
  grandParent->setColor(false);
}
