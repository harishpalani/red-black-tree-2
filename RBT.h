#ifndef RBT_H
#define RBT_H

#include <cmath>
#include <cstring>
#include <iostream>

struct Node {
  int value;
  bool black;
  
  Node* left;
  Node* right; 
  Node* parent;
  
  Node(int _value, bool _black = false) : left(0), right(0), parent(0) {
      value = _value;
      black = _black;
  }
  
  Node() : left(0), right(0), parent(0) {
      value = 0;
      black = true;
  }
  
  ~Node(){}
  
  bool isSentinel() { return left == 0 && right == 0; }
  bool isRed() { return !black; }
  void paintBlack() { black = true; }
  void paintRed() { black = false; }
  void makeSentinel() {
      deleteSubtrees();
      left = 0;
      right = 0;
      paintBlack();
      value = 0;
  }
  
  void addSentinelLeafs() {
      setLeft(new Node());
      setRight(new Node());
  }
  
  Node* grandparent() {
      return parent == 0 ? 0 : parent->parent;
  }
  
  Node* uncle() {
      Node* gp = grandparent();
      if (gp == 0) { return 0; }
      return gp->left == parent ? gp->right : gp->left;
  }
  
  Node* sibling() {
      if (parent == 0) { return 0; }
      return parent->left == this ? parent->right : parent->left;
  }
  
  void setLeft(Node* node) {
      left = node;
      if (node != 0) { node->parent = this; }
  }
  
  void setRight(Node* node){
      right = node;
      if (node != 0) { node->parent = this; }
  }
  
  Node* nonSentinelChild() {
      return left->isSentinel() ? right : left;
  }
  
  bool isLeftChild() {
      return parent->left == this;
  }
  
  bool isRightChild() {
      return parent->right == this;
  }
  
  void deleteSubtrees() {
      if (left != 0) { left->deleteSubtrees(); }
      if (right != 0) { right->deleteSubtrees(); }
      delete left;
      delete right;
  }
};

class RBT {
    public:
        RBT();
        ~RBT();
        void print();
        void insert(int num);
        bool find(int num);
        bool remove(int num);
        
    private:
        Node* root;
        Node* insertFirst(Node* node, int num);
        void insert(Node* inserted);
        Node* search(Node * node, int num);
        void populateArray(int* &array, int index, Node* node);
        int getNumLevels(Node* root, int level);
        void removeSingleNode(Node* toRemove);
        void leftRotation(Node* formerChild);
        void rightRotation(Node* formerChild);
        Node** parentPtrTo(Node* child);
        void replaceParentOf(Node* child);
        void rebalance(Node* node);
};

#endif