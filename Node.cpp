#include <iostream>
#include "Node.h"

//Constructor
Node::Node(int newValue){
  value = newValue;

  left = NULL;
  right = NULL;
  parent = NULL;
  color = false;
}

//Destructor
Node::~Node(){
  //Nothing to do here cause we stopped using int*
}

//Getters and setters for value
//need to test if passing by value works
int Node::getValue(){
  return value;
}

void Node::setValue(int newValue){
  value = newValue;
}

//Getters and setters for left node
Node* Node::getLeft(){
  return left;
}

void Node::setLeft(Node* newLeft){
  left = newLeft;
}

//Getters and setters for right node
Node* Node::getRight(){
  return right;
}

void Node::setRight(Node* newRight){
  right = newRight;
}


//Getters and setters for parent node
Node* Node::getParent(){
  if(this != NULL){
    return parent;
  }else{
    return NULL;
  }
}

void Node::setParent(Node* newParent){
  parent = newParent;
}

//Getters for grandparent
Node* Node::getGrandParent(){
  return this->getParent()->getParent();
}

//Getters for uncle
//This is pretty scuffed. In RBT deletion we'll swap from this to parent get sibling
Node* Node::getUncle(){
  Node* grandParent = this->getGrandParent();
  //If the grandparent isn't null
  if(grandParent != NULL){
    //Return the opposite of what isn't the parent of parent
    if(grandParent->getLeft() == parent){
      return grandParent->getRight();
    }else{
      return grandParent->getLeft();
    }
  }
  return NULL;
}


//Getters and setters for color
bool Node::getColor(){
  return color;
}

void Node::setColor(bool newColor){
  color = newColor;
}

//Not needed for now, might be needed later
void Node::reverseColor(){
  color = !color; 
}

//Left Rotate
void Node::rotateLeft(){
  //Remember the pivot's left subtree
  Node* leftSubtree = NULL;
  if(left!=NULL){
    leftSubtree = right->getLeft();
  }
  //To connect up the parent, first make sure that it's not the root
  if(parent != NULL){
    //If the root was the left subtree
    if(parent->getLeft() == this){
      //Set the parent's left to the pivot
      parent->setLeft(right);
      //If the root was the right subtree
    }else{
      //Set the parent's right to the pivot
      parent->setRight(right);
    }
    //Set the pivot's parent to the parent
    right->setParent(parent);
    //Set the root's parent to be the pivot (right)
    right->setLeft(this);
    parent = right;
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    right = leftSubtree;
  }else{
    //The root case
    //Set the pivot's parent to NULL
    right->setParent(NULL);
    //Set the root's parent to be the pivot (right)
    right->setLeft(this);
    parent = right;
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    right = leftSubtree;
  }
}

//Right Rotate 
void Node::rotateRight(){
  //Remember the pivot's left subtree
  Node* rightSubtree = NULL;
  if(right!=NULL){
    //The right subtree is the pivot's right
    rightSubtree = left->getRight();
  }
  //To connect up the parent, first make sure that it's not the root
  if(parent != NULL){
    //If the root was the left subtree
    if(parent->getLeft() == this){
      //Set the parent's left to the pivot
      parent->setLeft(left);
      //If the root was the right subtree
    }else{
      //Set the parent's right to the pivot
      parent->setRight(left);
    }
    //Set the pivot's parent to the parent
    left->setParent(parent);
    //Set the root's parent to be the pivot (left)
    left->setRight(this);
    parent = left;
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    left = rightSubtree;
  }else{
    //The root case
    //Set the pivot's parent to NULL
    left->setParent(NULL);
    //Set the root's parent to be the pivot (left)
    left->setRight(this);
    parent = left;
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    left = rightSubtree;
  }
}
