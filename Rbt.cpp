/*
  Author: Shengdong Li
  Date: 5/11/20
  Red Black Tree Deletion: A really cancer project that includes the search function to search for nodes in the tree and also the delete function to delete a number from the tree.
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include "Node.h"

void getInput(char* in); //Gets console input, stores into char* in
void read(Node* &root, char* in, int type); //Gets file input, stores into char* in, sequentially adds into rbt
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
int getInputType(char* in); //Asks user for whether they want file input or input from the console
void search(Node* root, char* in); //Asks user to enter a number, and returns whether it is in the tree, or the number of occurences
bool checkOccurence(Node* current, int tosearch); //Returns true if the number is in the tree and false if it is not
void remove(Node* &current, char* in, Node* &root); //Asks user to enter a number, and that number in the tree
Node* findRemove(Node* &past, Node* &current, int toDelete);
void swapNode(Node* &N, Node* &C); //Tries to swap N with C
Node* singleNodeDeletion(Node* &N); //Tries to find C of N, see if either N or C are red, and executes the appropriate cases, otherwise goes into the actual delete cases
//Delete cases 1-6 done with huge help from wikipedia red black tree article "https://en.wikipedia.org/wiki/Red%E2%80%93black_tree"
void deleteCase1(Node* &N); //Case that C is the new root
void deleteCase2(Node* &N); //Case that S is red
void deleteCase3(Node* &N); //Case that P, S, and S's children are black.
void deleteCase4(Node* &N); // S and S's children are black, P is red.
void deleteCase5(Node* &N); // S is black, S's left child is red, S's right child is black, and N is the left child of its parent
void deleteCase6(Node* &N); // S is black, S's right child is red, and N is the left child of its parent P

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
    read(root, in, getInputType(in));

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
          if(root == NULL){
            cout << "The tree is empty!" << endl;
          }else{
            print(root, 0);
          }
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
        case 6:
          //Search
          search(root, in);
          break;
        case 7:
          //Remove
          remove(root, in, root);
          break;
      }
    }
    //Prompt user if they want to start the program over
    cout << "Would you like to create another tree? (y/n)" << endl;
    while(true){
      getInput(in);
      in[0] = toupper(in[0]);
      if(strcmp(in, "Y") == 0){
        //Empty the tree
        while(root!=NULL){
          Node* temp = findRemove(root, root, root->getValue());
          if(temp == NULL || temp->getValue() != -1){
            root = temp;
          }else{
            break;
          }
        }
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
    cout << "No input detected...please enter something" << endl;
  }
}

//This function asks for file input name and sets the in variable equal to it
void read(Node* &root, char* in, int type){
  //New file strem
  ifstream stream;
  if(type == 1){
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
  }else{
    //Get console input
    cout << "Please enter your input then." << endl;
    getInput(in);
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
    }else if(strcmp(in, "SEARCH") == 0){
      return 6;
    }else if(strcmp(in, "REMOVE") == 0){
      return 7;
    }
    cout << "\"" << in << "\" is not recognized as a command. Please type \"help\" for a list of commands!" << endl;
  }
}

//Calls find() to insert node, and also corrects the root after rotations if needed
void insert(Node* &root, int toAdd){
  //Actually inserts stuff
  find(root, toAdd);

  //Corrects the root after rotations
  while(root->getParent() != NULL){
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
  cout << "\n----------\nEnter \"add\" to add a number to the tree,\n\"print\" to print the tree,\n\"search\" to check if a number is in the tree,\n\"remove\" to remove a number from the tree, or \n\"quit\" to exit the modding tree phase.\nAlso note that yellow = black in this tree, this was chosen for better readability.\nType \"help\" again to reprint this list.\n----------\n" << endl;
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

//A function that asks the user whether they would like to input their data through files or a tree
int getInputType(char* in){
  cout << "Please enter (1) for file input, or (2) for console input." << endl;
  while(true){
    getInput(in);
    //If they chose file input return 1
    if(strcmp(in, (char*)"1") == 0){
      return 1;
      //If they chose console input return 2
    }else if(strcmp(in, (char*)"2") == 0){
      return 2;
    }
    //Otherwise prompt them to choose again
    cout << "Please enter (1) or (2)" << endl;
  }
}

//Searches for if a number is in the tree
bool checkOccurence(Node* current, int toSearch){
  int inQuestion = current->getValue();
  Node* left = current->getLeft();
  Node* right = current->getRight();
  //If node is less than search number
  if(toSearch < inQuestion){
    //Check if there is a left node
    if(left != NULL){
      return checkOccurence(left, toSearch);
    }else{
      return false;
    }
  }
  //If node is equal to search number
  if(toSearch == inQuestion){
    return true;
  }

  //If node is greater than search number
  if(toSearch > inQuestion){
    if(right != NULL){
      return checkOccurence(right, toSearch);
    }else{
      return false;
    }
  }
  //Should never happen?
  return false;
}

//Asks user to enter a number, and returns the number of occurences in the tree
void search(Node* root, char* in){
  //Prompt user to input number to be searched
  cout << "Please enter the number to be searched" << endl;
  //Store into in
  getInput(in);
  //Convert in to int and check occurences
  if(checkOccurence(root, atoi(in))){
    cout << "\"" << in << "\"" << " is present in this tree." << endl;
  }else{
    cout << "Could not find any occurences of \"" << in << "\" in this tree." << endl;
  }
}

//Asks user to enter a number, and deletes that number in the tree/
void remove(Node* &current, char* in, Node* &root){
  //First we need to ask user for the number to remove
  cout << "Please enter the number to remove" << endl;
  //Store into in
  getInput(in);
  //Store the returning root in temp
  Node* temp = findRemove(current, current, atoi(in));
  //If temp is NULL or a match was found
  if(temp == NULL || temp->getValue() != -1){
    //Set root equal to the new temp
    root = temp;
    cout << "Successfully deleted \"" << in << "\" from the tree." << endl;
  }else{
    //Otherwise notify the user that nothing happened
    cout << "Did not delete any instances of \""<< in << "\" in the RBT because the number was not found." << endl;
  }
}

//Tries to execute regular rbt delete until the node in question becomes a node with 1 or less children, until which the node is deleted and put through deletion cases.
Node* findRemove(Node* &past, Node* &current, int toDelete){
  //Let's think through this logically.
  //If the current node is null that means the number has not been found
  if(current == NULL){
    //Return filler node with meaning that a node has not been found
	  return new Node(-1);
  }
  //First we actually have to find the number to delete.
  int inQuestion = current->getValue();
  Node* left = current->getLeft();
  Node* right = current->getRight();
  //If node is less than search number
  if(toDelete < inQuestion){
    //Recurse left
    return findRemove(current, left, toDelete);
  }



  /* UPDATE: Here's the rundown for what we need to add for RBT
   *
   * 1. First of all, RBT deletion DOES happen. Like how could it not? In the end, we're deleting, and correcting, so don't worry about rewriting the current deletion code that is already working.
   *
   * 2. Instead, we'll have to look at each case below on an individual basis and *decide on the N and it's respective child that we'd like to return...*
   *
   * 3. Then tie it into the Wikipedia Red Black Tree code, which utilizes replace node (swapping the location of child and parent) and drives right into delete one child.
   */



  //If node is equal to search number
  //A lot of stuff here can probably be cleaned up but has been left in here for *compatability* purposes
  if(toDelete == inQuestion){
    //There are three possible cases
    //One is that there are no children
    if(left == NULL && right == NULL){
      //RBT CHECK: Q: What does dealing with the root entail in this case?
      //RBT CHECK: A: Dealing with the root would mean we would
      //RBT CHECK: Concerns: We might have to add returns here later, since we are going straight into the replace nodes function. This is fine right now though
      //If we're dealing with the root node
      if(past == current){
        delete current;
        current = NULL;
        return NULL;
      }
      //RBT CHECK: Q: Is deleting the node good in this case?
      //RBT CHECK: A: Deleting the node is probably ok in this case, but we need to record the $x$ and potential $c$ values first, to return those.

      //RBT ADDED CODE
      //Node* n = current;
      //Node* c = NULL;
      //END OF RBT ADDED CODE

      //RBT CHECK: Q: Do we actually delete anything?
      //RBT CHECK: A: No, we have to return to the swap function. The swap function takes care of the below code.

      //RBT REMOVED CODE
      ////Then we have to find which child the current deleted node is
      //if(past->getLeft() == current){
      //  past->setLeft(NULL);
      //}else{
      //  past->setRight(NULL);
      //}
      ////Then get rid of the node
      //delete current;
      //current = NULL;
      //return;
      //END OF RBT REMOVED CODE

      //RBT ADDED CODE
      return singleNodeDeletion(current);

      //END OF RBT ADDED CODE
    }
    //One is that there is one child
    //First we have to figure out if the current node is the right or left node
    //We just have to swap the parent with the child
    //If there are left children
    if((left != NULL && right == NULL)){
      //If we're dealing with the root case
      if(past == current){
        //RBT CHECK: Q: Can we freely delete the root as in BST?
        //RBT CHECK: A: No, because we have to account for recolor. Best to return N and C

        //RBT REMOVED CODE
        ////Set the root to the left
        //delete current;
        //current = left;
        //return;
        //END OF RBT REMOVED CODE

        //RBT ADDED CODE
        //Node* n = current;
        //Node* c = left;
        return singleNodeDeletion(current);
        //END OF RBT ADDED CODE
      }

      //RBT CHECK: Q: Do we need to swap rn?
      //RBT CHECK: A: NO, just return N and C!

      //RBT REMOVED CODE

      ////Check if our current is the right or left child of the past
      //if(past->getRight() == current){
      //  //If it's the right, then swap the parent's right with current left
      //  past->setRight(left);
      //  delete current;
      //  current = NULL;
      //}else{
      //  //If it's the left, then swap the parent's right with current left
      //  past->setLeft(left);
      //  delete current;
      //  current = NULL;
      //}
      //return;

      //RBT ADDED CODE
      //Node* n = current;
      //Node* c = left;
      //return new Node*[2]{current, left};
      return singleNodeDeletion(current);
      //END OF RBT ADDED CODE
     
      //If there are right children
    }else if((left == NULL && right != NULL)){
      //If we're dealing with the root case
      if(past == current){
        //RBT REMOVED CODE
        ////Set the root to the right
        //delete current;
        //current = right;
        //return;
        //END OF RBT REMOVED CODE
         
        //RBT ADDED CODE
        //Node* n = current;
        //Node* c = right;
        //return new Node*[2]{current, right};
        //cout << "current is: " << current->getValue() << endl;
        return singleNodeDeletion(current);
        //END OF RBT ADDED CODE
      }
      //RBT REMOVED CODE
      ////Check if our current is the right or left child of the past
      //if(past->getRight() == current){
      //  //If it's the right, then swap the parent's right with current right
      //  past->setRight(right);
      //  delete current;
      //  current = NULL;
      //}else{
      //  //If it's the left, then swap the parent's right with current right
      //  past->setLeft(right);
      //  delete current;
      //  current = NULL;
      //}
      //return;
      //END OF RBT REMOVED CODE

      //RBT ADDED CODE
      //Node* n = current;
      //Node* c = right;
      //return new Node*[2]{current, right};
      return singleNodeDeletion(current);
      //END OF RBT ADDED CODE
       
      //If there are two children
    }else{
      //Then we'll first need to find the next "smallest" or "biggest" node.
      //We'll implement the smallest this time. This means the the next node to the left, and the farthest to the right

      //Start at the left node
      Node* parent = left;
      Node* child = left;
      //Keep going until the very right is reached, keeping track of the parent
      while(child->getRight() != NULL){
        //Parent is child, and child is next right
        parent = child;
        child = child->getRight();
      }

      //RBT CHECK: Q: Do we need to actually swap nodes?
      //RBT CHECK: A: No, we don't actually swap nodes, we can just swap values.




      //RBT REMOVED CODE
      ////So first the parent and child relationship must be broken off
      //parent->setRight(NULL);
      ////Even if there are no children, this should be fine.
      ////Next, we have to replace the current with the child.
      //END OF RBT REMOVED CODE

      //RBT REMOVED CODE - I don't think root case matters in this context
      ////Be wary of root case!
      //if(past == current){
      //  //Then we have to ensure the original root's right and left child is preserved, so tie the current child to that right child and left child
      //  child->setRight(current->getRight());
      //  //However, what if we have a case where we're tying the next left to the node itself? Well, just check it then!
      //  if(current->getLeft() != child){
      //    child->setLeft(current->getLeft());
      //  }
      //  delete current;
      //  current = child;
      //  return;
      //}
      //END OF RBT REMOVED CODE

      //RBT ADDED CODE

      //Let's just swap the values of the current with parent first
      current->setValue(child->getValue());

      //END OF RBT ADDED CODE

      //RBT ADDED CODE

      //Now we can just return the Node and child to be swapped. It's gotta be the non nill node if possible, tho

      //I'm pretty sure that here right is guranteed to be nil, so let's just pick the left
      //********* MAY BE BUGGED **********/
      //return new Node*[2]{child, child->getLeft()};
      return singleNodeDeletion(child);

      //END OF RBT ADDED CODE





      //RBT REMOVED CODE




      ////If it's not a root case, then much of the same thing occurs, except we have to take into account the "past" node
      ////We have to figure out if the "current" node is the right or left of it's parent
      ////If the current is to the right, just keep that in mind when we're swapping
      //if(past->getRight() == current){
      //  //We still have to ensure that the original root's right and left child are preserved
      //  child->setRight(current->getRight());
      //  if(current->getLeft() != child){
      //    child->setLeft(current->getLeft());
      //  }
      //  //Set the current equal to the child
      //  delete current;
      //  current = child;
      //  //Then tie in the past to the current
      //  past->setRight(child);
      //  return;
      //  //If the current is to the left, just keep that in mind also
      //}else{
      //  //We still have to ensure that the original root's right and left child are preserved
      //  child->setRight(current->getRight());
      //  if(current->getLeft() != child){
      //    child->setLeft(current->getLeft());
      //  }
      //  //Set the current equal to the child
      //  delete current;
      //  current = child;
      //  //Then tie in the past to the current
      //  past->setLeft(child);
      //  return;
      //}



      //END OF RBT REMOVED CODE
    }
  }

  //If node is greater than search number
  if(toDelete > inQuestion){
      return findRemove(current, right, toDelete);
  }
  //RBT ADDED CODE
  //HOPEFULLY THIS NEVER HAPPENS, I HOPE THIS DOESN'T SCREW UP ANYTHING THX
  return NULL;
  //END OF RBT ADDED CODE
}

//This function takes in N and C and attempts to replace them, making NIL nodes black filler nodes with -1 value
void swapNode(Node* &N, Node* &C) {
  //Make sure that
  //I guess if N is NULL we have nothing to do
  if(N == NULL){
    return;
  }
  //Then if C is NULL, we'll just make a dummy black node for now
  if(C == NULL){
    //Make its value -1, an invalid value
    C = new Node(-1);
    //Then set its color to black
    C->setColor(true);
  }
  //Set the child's parent to N's current parent
  C->setParent(N->getParent());
  //Consider root case
  if(N->getParent() == NULL){
  //cout << "Swapnode, Root case" << endl;
  N = C;
  //If N is the left child of its parent
  }else if (N == N->getParent()->getLeft()) {
  //cout << "Swapnode, N is the left child of its parent" << endl;
    //Set the parent's new left to C
    N->getParent()->setLeft(C);
  //If N is the right child of its parent
  } else {
  //cout << "Swapnode, N is the right child of its parent" << endl;
    //Set the parent's new right to C
    N->getParent()->setRight(C);
  }
}

//Tries to see if either N or C are red, and executes the appropriate cases, otherwise goes into the actual delete cases
Node* singleNodeDeletion(Node* &N){
  //Again, if N is NULL get out
  if(N == NULL){
    return NULL;
  }

  //cout << "N is:" << N->getValue() << endl;

  //cout << N->getValue() << endl;
  bool NColor = N->getColor();

  //Try to grab the non nil node as child
  Node* C;
  if(N->getRight() == NULL){
    C = N->getLeft();
  }else{
    C = N->getRight();
  }

  bool CColor;
  if(C != NULL){
    CColor = C->getColor();
  }else if(C == NULL){
    CColor = true;
  }

  //Swap the N with C!
  swapNode(N, C);
  //cout << "N is(after swap):" << N->getValue() << endl;
  //cout << "C is:" << C->getValue() << endl;
  //If N's color was black
  if (NColor == true) {
    //If Child's color was red
    if (CColor == false) {
      //Just set child's color to black
      C->setColor(true);
    } else {
      //cout << "Calling delete case 1" << endl;
      deleteCase1(C);
    }
  }

  //This is a really important snippet of code.
  //Since C is always attached to something ether as a nil black node or a node with a value, you can always use it to trace back to the root
  //Define temp as C
  Node* temp = C;
  //While the parent is not null
  while(temp->getParent()!=NULL){
    //Keep going up. This should result in the root
    temp = temp->getParent();
  }

  //This cleans up the black filler nodes with -1 value!
  if(C->getValue() == -1){
    //Make sure that parent isn't NULL
    if(C->getParent() != NULL){
      if (C == C->getParent()->getLeft()) {
        //Set the parent's new left to NULL
        C->getParent()->setLeft(NULL);
        //If C is the right child of its parent
      } else {
        //Set the parent's new right to NULL
        C->getParent()->setRight(NULL);
      }
    }
  }

  //delete N;
  return temp;
}

//WITH HELP FROM WIKIPEDIA
//If N is the new root then no need to keep correcting
void deleteCase1(Node* &N){
  //cout << "N is the new root." << endl;
  if(N->getParent() != NULL){
      //cout << "Calling delete case 2" << endl;
    deleteCase2(N);
  }
}

//WITH HELP FROM WIKIPEDIA
//"If the sibling is red then swap parent and sibling colors and rotate through the parent, turning S into N's grandparent."
void deleteCase2(Node* &N){
  Node* S = N->getSibling();

  //If sibling's color is red
  if(S->getColor() == false){
    //cout << "Sibling's color is red" << endl;
    //Swap colors
    N->getParent()->setColor(false);
    S->setColor(true);
    //Rotate to turn S into N's grandparent
    if(N == N->getParent()->getLeft()){
      N->getParent()->rotateLeft();
    }else{
      N->getParent()->rotateRight();
    }
  }
  //cout << "Calling delete case 3" << endl;
  deleteCase3(N);
}

//Case that P, S, and S's children are black.
//MIGHT have to consider NULL cases for children???
void deleteCase3(Node* &N){
  Node* S = N->getSibling();

  //If Child's parent, Sibling and it's children are all black
  if((N->getParent()->getColor() == true) && (S->getColor() == true) && (S->getLeft()->getColor() == true) && (S->getRight()->getColor() == true)){
    //cout << "Child's parent, sibling, and it's children are all black" << endl;
    //Repaint S red
    S->setColor(false);
    //Rebalance the tree starting from the first case
    Node* P = N->getParent();
    //cout << "Calling delete case 2" << endl;
    deleteCase1(P);
  }else{
    //cout << "Calling delete case 4" << endl;
    deleteCase4(N);
  }
}

// S and S's children are black, P is red.
void deleteCase4(Node* &N){
  Node* S = N->getSibling();

  //If sibling and its children are black and p is red
  if((N->getParent()->getColor() == false) && (S->getColor() == true) && (S->getLeft()->getColor() == true) && (S->getRight()->getColor() == true)){
    //cout << "Sibling and its children are black, p is red" << endl;
    //Exchange S and P colors
    S->setColor(false);
    N->getParent()->setColor(true);
  }else{
    //Otherwise go onto case 5
    //cout << "Calling delete case 5" << endl;
    deleteCase5(N);
  }
}

// S is black, S's left child is red, S's right child is black, and N is the left child of its parent
void deleteCase5(Node* &N){
  Node* S = N->getSibling();

  //cout << "case 5, siblings value is:" << S->getValue() << endl;
  //cout << "case 5, sibling's color is:" << S->getColor() << endl;
  //cout << "case 5, N's parent is " << N->getParent()->getValue() << endl;
  //cout << "case 5, sibling's left child is: " << S->getLeft()->getColor() << endl;
  //cout << "case 5, sibling's right child is: " << S->getRight()->getColor() << endl;
  if(N==N->getParent()->getLeft()){
    //cout << "deleteCase 5, N is the left child of parent" << endl;
  }else if(N==N->getParent()->getRight()){
    //cout << "deleteCase 5, N is the right child of parent" << endl;
  }

  //If S is black
  if(S->getColor() == true){
    //cout << "S color is black" << endl;
    //...and it's left child is red and right child is black, and N is the left child of its parent,
    //cout << "deleteCase5, N's parent is:" << N->getParent()->getValue() << endl;
    //cout << "deleteCase5, N's parent's left is:" << N->getParent()->getLeft()->getValue() << endl;
    //cout << "deleteCase5, N's parent's right is:" << N->getParent()->getRight()->getValue() << endl;
    if((N == N->getParent()->getLeft()) && (S->getRight()->getColor() == true) && (S->getLeft()->getColor() == false)){
      //cout << "S is black, S left is red, S right is black, and N left child of parent" << endl;
      //Swap S and new P colors
      S->setColor(false);
      S->getLeft()->setColor(true);
      //Then rotate right so that S's left child becomes the parent of S and S's right child is stil the child of S
      S->rotateRight();
      //... or it's right child is black and right child is red, and N is the right child of its parent
    }else if((N == N->getParent()->getRight()) && (S->getLeft()->getColor() == true) && (S->getRight()->getColor() == false)){
      //cout << "S is black, S left is red, S right is black, N left child of parent" << endl;
      //Swap S and new P colors
      S->setColor(false);
      S->getRight()->setColor(true);
      //Then rotate left so that S's right child becomes the parent of S and S's left child is still the child of S
      S->rotateLeft();
    }
  }
  //cout << "Calling delete case 6" << endl;
  deleteCase6(N);
}

// S is black, S's right child is red, and N is the left child of its parent P
void deleteCase6(Node* &N){
  Node* S = N->getSibling();

  //Swap P and S colors
  S->setColor(N->getParent()->getColor());
  //cout << "DELETECASE6: S's COLOR IS" << S->getColor() << "." << endl;
  N->getParent()->setColor(true);

  //If N is the left child of P
  if (N == N->getParent()->getLeft()){
    //cout << "DELETECASE6: N IS THE LEFT CHILD OF PARENT" << endl;
    //Make S's right child color black
    S->getRight()->setColor(true);
    //Rotate left through N's parent so that P's parent is S
    N->getParent()->rotateLeft();
    //If N is the right child of P
  } else {
    //cout << "DELETECASE6: N IS THE RIGHT CHILD OF PARENT" << endl;
    //Make S's left child color black
    S->getLeft()->setColor(true);
    //Rotate right through N's parent so that P's parent is S
    N->getParent()->rotateRight();
  }
}
