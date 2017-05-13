/**
 * Red-Black Tree, part 2 | A (roughly) self-balancing binary tree
 * [NOTE: Algorithm inspired by Wikipedia page on RBTs, RBT visualization inspired by a Stack Overflow response]
 * @author Harish Palani
 * @version May 2017
 */
#include <iostream>
#include <sstream>
#include <vector>
#include "RBT.h"

using namespace std;

int main() {
    cout << "Red-Black Tree | Insertion & Deletion" << "\n" << "Created by Harish Palani, April 2017" << "\n" << endl;
    RBT rbt;
    
    // Get input list
    cout << "Enter your list, separated by spaces:" << endl;
    string input;
    getline(cin, input);
    stringstream ss(input);
    
    // Load input list into vector
    vector<int> list;
    int i;
    while (ss >> i) {
        list.push_back(i);
        if (ss.peek() == ' ') {
            ss.ignore();
        }
    }
    
    // Insert vector contents into RBT
    for (i = 0; i < list.size(); i++) {
        rbt.insert(list.at(i));
    }
    
    while (true) {
        int n;
        char response;
        cout << "\n1 | Print out tree\n2 | Insert a number\n3 | Search for a number\n4 | Delete a number\nq | quit" << endl;
        cin >> response;
        
        if (response == '1') {
            rbt.print();
        } else if (response == '2') {
            cout << "Number to insert: ";
            cin >> n;
            cout << endl;
            rbt.insert(n);
            rbt.print();
        } else if (response == '3') {
            cout << "Number to find: ";
            cin >> n;
            cout << endl;
            if (rbt.find(n)) {
                cout << n << " is in the tree." << "\n"  << endl;
            } else {
                cout << n << " is NOT in the tree." << "\n" << endl;
            }
            rbt.print();
        } else if (response == '4') {
            cout << "Number to delete: ";
            cin >> n;
            cout << endl;
            rbt.remove(n);
            rbt.print();
        } else {
            break;
        }
    }
    
    return 0;
}