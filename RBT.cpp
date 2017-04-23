#include "RBT.h"

using namespace std;

RBT::RBT() {
    root = new Node();
}

RBT::~RBT() {
    delete root;
}

// Visualize RBT
void RBT::print() { // credit: Stack Overflow
    int levels = countLevels(root, 0);
    int nodes[(int)(pow(2, levels)) - 1];
    int* ptr = nodes;
    memset(nodes, 0, sizeof(nodes));
    populate(ptr, 0, root);
    
    int index = 0;
    for (int l = 1; l <= levels; l++) {
        
        for (int i = 0; i < pow(2, levels-l)-1; i++) {
            cout << ' ';
        }
        
        for (int n = 0; n < pow(2, l-1); n++) {
            // Set color | red or black
            if (nodes[index] > 0) {
                cout << nodes[index] << 'B';
            } else if (nodes[index] < 0) {
                cout << -nodes[index] << 'R';
            } else {
                cout << ' ';
            }
            
            index++;
            
            for (int i = 0; i < pow(2, levels - l + 1) - 1; i++) {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// Insert number into RBT (wrapper method for ease of use)
void RBT::insert(int data) {
    insert(insertFirst(root, data));
}

// Main insertion algorithm
void RBT::insert(Node* node) {
    if (node->mParent == 0) {
        node->setColor('b');
        return;
    } else if (node->mParent->mBlack) { // Parent is black
        return;
    } else if (node->getUncle()->isRed()) { // Parent is red, Uncle is red
        // SET || Parent: black / Uncle: black / Grandparent: red
        node->mParent->setColor('b');
        node->getUncle()->setColor('b');
        node->getGrandparent()->setColor('r');
        
        // Recursive iteration upwards, w/ grandparent
        insert(node->getGrandparent());
        
        return;
    }
    
    // Parent is red, Uncle is black | Current node is right child, Parent is left child
    if (node->isChild('r') && node->mParent->isChild('l')) {
        // Slide the current node up to its parent locale
        Node* bygone = node->mParent;
        *parentPtrGenerator(bygone) = node;
        node->mParent = node->getGrandparent();
        bygone->setRight(node->mLeft);
        node->setLeft(bygone);
        node = node->mLeft;
    }
    
    // Parent is red, Uncle is black | Current node is left child, Parent is right child
    if (node->isChild('l') && node->mParent->isChild('r')) {
        // Slide the current node up to its parent locale
        Node* bygone = node->mParent;
        *parentPtrGenerator(bygone) = node;
        node->mParent = node->getGrandparent();
        bygone->setLeft(node->mLeft);
        node->setRight(bygone);
        node = node->mRight;
    }
    
    node->getGrandparent()->setColor('r');
    node->mParent->setColor('b');
    
    if (node->isChild('l')) { // If left child, shift parent to the right
        Node* parent = node->mParent;
        Node* bygone = parent->mParent;
        *parentPtrGenerator(bygone) = parent;
        parent->mParent = parent->getGrandparent();
        bygone->setLeft(parent->mLeft);
        parent->setRight(bygone);
    } else { // If right child, shift parent to the left
        Node* parent = node->mParent;
        Node* bygone = parent->mParent;
        *parentPtrGenerator(bygone) = parent;
        parent->mParent = parent->getGrandparent();
        bygone->setRight(parent->mLeft);
        parent->setLeft(bygone);
    }
}

// Helper methods //
// Visualization
void RBT::populate(int* &list, int i, Node* node) { // credit: Stack Overflow
    if(node == 0) { return; }
    list[i] = node->mBlack ? node->mData : -node->mData;
    populate(list, i*2+1, node->mLeft);
    populate(list, i*2+2, node->mRight);
}

int RBT::countLevels(Node* root, int level = 0) { // credit: Stack Overflow
    if (root == 0) { return level; }
    return max(countLevels(root->mLeft, level + 1), countLevels(root->mRight, level + 1));
}

// RBT functions
// Initiates RBT insertion process
Node* RBT::insertFirst(Node* child, int data) {
    if (child->mLeft == 0 && child->mLeft == 0) {
        child->mData = data;
        child->setColor('r');
        child->setLeft(new Node());
        child->setRight(new Node());
        
        return child;
    }
    
    if (data < child->mData) {
        return insertFirst(child->mLeft, data);
    } else {
        return insertFirst(child->mRight, data);
    }
}

// Generates a parent pointer
Node** RBT::parentPtrGenerator(Node* child) {
    Node** ptr;
    if(child->mParent == 0) { ptr = &root; }
    else if (child == child->mParent->mLeft) { ptr = &(child->mParent->mLeft); }
    else { ptr = &(child->mParent->mRight); }
    return ptr;
}