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

//Getters for sibling
Node* Node::getSibling(){
  //First get parent
  Node* p = this->getParent();
  //If NULL
  if(p == NULL){
    //Return NULL
    return NULL;
  }else{
    //Otherwise return the parent's child that is not this node
    if(p->getLeft()==this){
      return p->getRight();
    }else{
      return p->getLeft();
    }
  }
}

//Getters for grandparent
Node* Node::getGrandParent(){
  //Parent's parent
  return this->getParent()->getParent();
}

//Getters for uncle
Node* Node::getUncle(){
  return this->getParent()->getSibling();
}


//Getters and setters for color,
//FALSE IS RED, TRUE IS BLACK, 0 IS FALSE, 1 IS TRUE
bool Node::getColor(){
  if(this == NULL){
    std::cout << "hey this is null haha" << std::endl;
    return true;
  }
  return color;
}

void Node::setColor(bool newColor){
  color = newColor;
}

//Not needed for now, might be needed later
void Node::reverseColor(){
  color = !color; 
}

//Left Rotate, PIVOT IS RIGHT
void Node::rotateLeft(){
  //Remember the pivot's left subtree
  Node* leftSubtree = NULL;
  //CHANGED AS OF 5/11 FROM LEFT TO RIGHT
  if(right!=NULL){
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
    if(leftSubtree != NULL){
      leftSubtree->setParent(this);
    }
  }else{
    //The root case
    //Set the pivot's parent to NULL
    right->setParent(NULL);
    //Set the root's parent to be the pivot (right)
    right->setLeft(this);
    if(parent!=NULL){
    std::cout << "Parent value before setting parent to right is: " << parent->getValue() << std::endl;
    }
    parent = right;
    if(parent!=NULL){
    std::cout << "Parent value after setting parent to right is: " << parent->getValue() << std::endl;
    }
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    if(right!=NULL){
    std::cout << "Right value before setting is: " << right->getValue() << std::endl;
    }
    right = leftSubtree;
    if(right!=NULL){
    std::cout << "Right value after setting is: " << right->getValue() << std::endl;
    }
    leftSubtree->setParent(this);
    if(leftSubtree != NULL){
      leftSubtree->setParent(this);
    }
  }
}

//Right Rotate 
void Node::rotateRight(){
  //Remember the pivot's left subtree
  Node* rightSubtree = NULL;
  //CHANGED AS OF 5/11 FROM LEFT TO RIGHT
  if(left!=NULL){
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
    rightSubtree->setParent(this);
    if(rightSubtree != NULL){
      rightSubtree->setParent(this);
    }
  }else{
    //The root case
    //Set the pivot's parent to NULL
    left->setParent(NULL);
    //Set the root's parent to be the pivot (left)
    left->setRight(this);
    if(parent!=NULL){
      std::cout << "Parent value before setting parent to left is: " << parent->getValue() << std::endl;
    }
    parent = left;
    if(parent!=NULL){
      std::cout << "Parent value after setting parent to left is: " << parent->getValue() << std::endl;
    }
    //Then finally, the pivot's left subtree becomes the old root's new right subtree
    if(left!=NULL){

    std::cout << "Left value before setting is: " << left->getValue() << std::endl;
    }
    left = rightSubtree;
    if(left!=NULL){
    std::cout << "Left value before setting is: " << left->getValue() << std::endl;
    }
    rightSubtree->setParent(this);
    if(rightSubtree != NULL){
      rightSubtree->setParent(this);
    }
  }
}
