#include "RBT.h"

using namespace std;

RBT::RBT() {
    root = new Node();
}

RBT::~RBT() {
    root->deleteSubtrees();
    delete root;
}

void RBT::insert(int num) { insert(insertFirst(root, num)); }

void RBT::insert(Node * inserted) {
    if (inserted->parent == 0) {
        inserted->paintBlack();
        return;
    } else if (inserted->parent->black) { // Parent is black
        return;
    } else if (inserted->uncle()->isRed()) { // Parent is red, Uncle is red
        // SET || Parent: black / Uncle: black / Grandparent: red        
        inserted->parent->paintBlack();
        inserted->uncle()->paintBlack();
        inserted->grandparent()->paintRed();
        
        // Recursive iteration upwards, w/ grandparent
        insert(inserted->grandparent());
        
        return;
    }
    
    // Parent is red, Uncle is black | Current node is right child, Parent is left child
    if (inserted->isRightChild() && inserted->parent->isLeftChild()) {
        // Slide the current node up to its parent locale
        leftRotation(inserted);
        inserted = inserted->left;
    } 
    
    // Parent is red, Uncle is black | Current node is left child, Parent is right child
    if (inserted->isLeftChild() && inserted->parent->isRightChild()) {
        // Slide the current node up to its parent locale
        rightRotation(inserted);
        inserted = inserted->right;
    }
    
    inserted->grandparent()->paintRed();
    inserted->parent->paintBlack();
    
    if (inserted->isLeftChild()) { // If left child, shift parent to the right
        rightRotation(inserted->parent);
    } else { // If right child, shift parent to the left
        leftRotation(inserted->parent);
    }
}

bool RBT::remove(int num) {
    Node* toRemove = search(root, num);
    
    // Node with desired data point not found
    if (toRemove == 0) { return false; }
    
    // If neither child is null, replace target with next & delete next
    if (!toRemove->left->isSentinel() && !toRemove->right->isSentinel()) {
        Node* successor = toRemove->right;
        while (!successor->left->isSentinel()) { successor = successor->left; }
        toRemove->value = successor->value;
        toRemove = successor;
    }
    removeSingleNode(toRemove);
    return true;
}

void RBT::removeSingleNode(Node* toRemove) {
    if (toRemove->isRed()) {
        toRemove->makeSentinel();
        return;
    }
    
    Node* child = toRemove->nonSentinelChild();
    if (child->isRed()) { // toRemove black, child red.
        replaceParentOf(child);
        child->paintBlack();
        return;
    }
    
    // Node target must replace its former parent
    toRemove->makeSentinel();
    rebalance(toRemove);
}

void RBT::rebalance(Node* node) {
    // If node is the root
    if (node->parent == 0) { return; }
    
    Node* sibling = node->sibling();
    if(sibling->isRed()) { // Node's parent is black
        // Reverse parent and sibling colors
        node->parent->paintRed();
        sibling->paintBlack();
        
        // Rotate sibling into parent's place
        if (node->isLeftChild()) { leftRotation(sibling); }
        else { rightRotation(sibling); }
    }
    
    sibling = node->sibling();
    // If parent is black, set sibling to red & balance
    if (node->parent->black && sibling->black && sibling->left->black && sibling->right->black) {
        sibling->paintRed();
        rebalance(node->parent);
        return;
    }
    
    if (node->parent->isRed() && sibling->black && sibling->left->black && sibling->right->black) {
        sibling->paintRed();
        node->parent->paintBlack();
        return;
    }
    
    if (node->isLeftChild() && sibling->right->black) {
        sibling->left->paintBlack();
        sibling->paintRed();
        rightRotation(sibling->left);
    } else if (node->isRightChild() && sibling->left->black) {
        sibling->right->paintBlack();
        sibling->paintRed();
        leftRotation(sibling->right);
    }
    sibling = node->sibling();
    sibling->black = node->parent->black;
    node->parent->paintBlack();
    
    if(node->isLeftChild()) {
        sibling->right->paintBlack();
        leftRotation(sibling);
    } else {
        sibling->left->paintBlack();
        rightRotation(sibling);
    }
}

void RBT::print() { // credit: Stack Overflow
    int numLevels = getNumLevels(root, 0);
    int nodes[(int)(pow(2, numLevels)) - 1];
    int* arrPtr = nodes;
    memset(nodes, 0, sizeof(nodes));
    populateArray(arrPtr, 0, root);
    
    int index = 0;
    for (int l = 1; l <= numLevels; l++) {
        for (int i = 0; i < pow(2, numLevels-l)-1; i++) { cout << ' '; }
        for (int n = 0; n < pow(2, l-1); n++) {
            if (nodes[index] > 0) {
                cout << nodes[index] << 'B';
            } else if (nodes[index] < 0) { 
                cout << -nodes[index] << 'R'; 
            } else { 
                cout << ' '; 
            }
            index++;
            
            for (int i = 0; i < pow(2, numLevels - l + 1) - 1; i++) {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

// Initiates RBT insertion process
Node* RBT::insertFirst(Node * child, int num) {
    if(child->isSentinel()) {
        child->value = num;
        child->paintRed();
        child->addSentinelLeafs();
        return child;
    }
    
    if(num < child->value) { return insertFirst(child->left, num); } 
    else { return insertFirst(child->right, num); }
}

int RBT::getNumLevels(Node* currentRoot, int level = 0) {
    if (currentRoot == 0) { return level; }
    return max(getNumLevels(currentRoot->left, level + 1), getNumLevels(currentRoot->right, level + 1));
}

void RBT::populateArray(int *& array, int index, Node* node) {
    if(node == 0) { return; }
    array[index] = node->black ? node->value : -node->value;
    populateArray(array, index*2+1, node->left);
    populateArray(array, index*2+2, node->right);
}

Node* RBT::search(Node * currentRoot, int num) {
    if(currentRoot->isSentinel()) { return 0; }
    if(currentRoot->value == num) { return currentRoot; }
    else if(num > currentRoot->value) { return search(currentRoot->right, num); }
    else { return search(currentRoot->left, num); }
}

bool RBT::find(int num) {
    return search(root, num) != 0;
}

void RBT::leftRotation(Node* formerChild) {
    Node* formerParent = formerChild->parent;
    *parentPtrTo(formerParent) = formerChild;
    formerChild->parent = formerChild->grandparent();
    formerParent->setRight(formerChild->left);
    formerChild->setLeft(formerParent);
}

void RBT::rightRotation(Node* formerChild) {
    Node* formerParent = formerChild->parent;
    *parentPtrTo(formerParent) = formerChild;
    formerChild->parent = formerChild->grandparent();
    formerParent->setLeft(formerChild->right);
    formerChild->setRight(formerParent);
}

// Generates a parent pointer
Node** RBT::parentPtrTo(Node* child) {
    Node** ptr;
    if (child->parent == 0) {
        ptr = &root;
    } else if (child == child->parent->left) {
        ptr = &(child->parent->left);
    } else {
        ptr = &(child->parent->right);
    }
    return ptr;
}

void RBT::replaceParentOf(Node* child) {
    Node* parent = child->parent;
    Node** childPtr = parentPtrTo(child);
    child->parent = child->grandparent();
    (*parentPtrTo(parent)) = child;
    *childPtr = 0;
    parent->deleteSubtrees();
    delete parent;
}