#ifndef RBT_H
#define RBT_H

#include <cmath>
#include <cstring>
#include <iostream>

struct Node {
    int mData;
    bool mBlack;
    
    Node* mLeft;
    Node* mRight;
    Node* mParent;
    
    // Node constructor w/ initialization list
    Node(int data, bool black = false) : mLeft(0), mRight(0), mParent(0) {
        mData = data;
        mBlack = black;
    }
    
    // Default node constructor, also w/ initialization list
    Node() : mLeft(0), mRight(0), mParent(0) {
        mData = 0;
        mBlack = true;
    }
    
    // Determines if node is sentinel
    bool isSentinel() { return mLeft == 0 && mRight == 0; }
    
    // Create a sentinel node
    void setSentinel() {
        deleteSubtrees();
        mLeft = 0;
        mRight = 0;
        mData = 0;
        setColor('b');
    }
    
    // Gets grandparent node
    Node* getGrandparent() {
        if (mParent == 0) { return 0; }
        return mParent->mParent;
    }
    
    // Gets uncle node
    Node* getUncle() {
        Node* grandparent = getGrandparent();
        if (grandparent == 0) { return 0; }
        if (grandparent->mLeft == mParent) { return grandparent->mRight; }
        return grandparent->mLeft;
    }
    
    // Gets sibling node
    Node* getSibling() {
        if (mParent == 0) { return 0; }
        if (mParent->mLeft == this) { return mParent->mRight; } 
        return mParent->mLeft;
    }
    
    // Paints node either red or black
    void setColor(char color) { // 'r' = RED | 'b' = BLACK
        if (color == 'r') { mBlack = false; }
        else { mBlack = true; }
    }
    
    // Sets left node (mLeft)
    void setLeft(Node* node) {
        mLeft = node;
        if (node != 0) { node->mParent = this; }
    }
    
    // Sets right node (mRight)
    void setRight(Node* node) {
        mRight = node;
        if (node != 0) { node->mParent = this; }
    }
    
    // Checks if node is painted red
    bool isRed() { return !mBlack; }
    
    // Checks if node is child in the specified direction
    bool isChild(char direction) {
        if (direction = 'l') {
            return mParent->mLeft == this;
        } else {
            return mParent->mRight == this;
        }
    }
    
    // Deletes all subtrees
    void deleteSubtrees() {
        if (mLeft != 0) { mLeft->deleteSubtrees(); }
        if (mRight != 0) { mRight->deleteSubtrees(); }
        delete mLeft;
        delete mRight;
    }
};

class RBT {
    public:
        RBT();
        ~RBT();
        void print();          // Option #1
        void insert(int data); // Option #2
        bool find(int data); // Option #3
        bool remove(int data); // Option #4
    
    private:
        Node* root;
        Node* insertFirst(Node* child, int data);
        void insert(Node* node);
        Node* search(Node* root, int data);
        void populate(int* &list, int i, Node* node);
        int countLevels(Node* root, int level);
        Node** parentPtrGenerator(Node* child);
        void balance(Node* node);
        void removeTarget(Node* target);
};

#endif