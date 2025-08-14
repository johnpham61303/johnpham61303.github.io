/**
 * Author: John Pham
 * Date: 10/13/2024
 * Section: 597
 * email: jtp210001@tamu.edu
 * project 2 - binary trees
 * mytest.cpp
 * test file for the dtree class
 */

#include <iostream>
#include "dtree.h"

using namespace std;

//test destructor for memory leaks 
void testDestructor() {
    DTree* tree = new DTree();

    tree->insert(Account("John1", 1, false, "badge1", "online"));
    tree->insert(Account("John2", 2, true, "badge2", "offline"));
    tree->insert(Account("John3", 3, false, "badge3", "online"));
    tree->insert(Account("John4", 4, true, "badge4", "offline"));
    tree->insert(Account("John5", 5, false, "badge5", "online"));
    tree->insert(Account("John6", 6, true, "badge6", "offline"));
    tree->insert(Account("John7", 7, false, "badge7", "online"));
    tree->insert(Account("John8", 8, true, "badge8", "offline"));
    tree->insert(Account("John9", 9, false, "badge9", "online"));
    tree->insert(Account("John10", 10, true, "badge10", "offline"));

    tree->printAccounts();
    tree->dump(tree->getRoot());
    delete tree;
}

//test deep copy
void testDeepCopy() {
    DTree* tree = new DTree();

    tree->insert(Account("John1", 1, false, "badge1", "online"));
    tree->insert(Account("John2", 2, true, "badge2", "offline"));
    tree->insert(Account("John3", 3, false, "badge3", "online"));
    tree->insert(Account("John4", 4, true, "badge4", "offline"));
    tree->insert(Account("John5", 5, false, "badge5", "online"));
    tree->insert(Account("John6", 6, true, "badge6", "offline"));
    tree->insert(Account("John7", 7, false, "badge7", "online"));
    tree->insert(Account("John8", 8, true, "badge8", "offline"));
    tree->insert(Account("John9", 9, false, "badge9", "online"));
    tree->insert(Account("John10", 10, true, "badge10", "offline"));

    tree->printAccounts();

    DTree* newTree = tree;

    newTree->printAccounts();

    tree->dump(tree->getRoot());
    newTree->dump(newTree->getRoot());
    delete tree;
    delete newTree;

}
//test update functions and imbalance / rebalance
void testUpdate() {

    DTree tree = DTree();

    tree.insert(Account("John1", 1, false, "badge1", "online"));
    tree.insert(Account("John2", 2, true, "badge2", "offline"));
    tree.insert(Account("John3", 3, false, "badge3", "online"));
    tree.insert(Account("John4", 4, true, "badge4", "offline"));
    tree.insert(Account("John5", 5, false, "badge5", "online"));
    tree.insert(Account("John6", 6, true, "badge6", "offline"));
    tree.insert(Account("John7", 7, false, "badge7", "online"));
    tree.insert(Account("John8", 8, true, "badge8", "offline"));
    tree.insert(Account("John9", 9, false, "badge9", "online"));
    tree.insert(Account("John10", 10, true, "badge10", "offline"));

    DNode* removed;
    tree.remove(7,removed);
    tree.remove(8,removed);
    tree.remove(9, removed);
    tree.remove(10,removed);

    tree.updateSize(tree.getRoot());
    tree.updateNumVacant(tree.getRoot());

    tree.printAccounts();

    DNode* root = tree.getRoot();
    cout << tree.getRoot()->getSize() << endl;
    cout << tree.getRoot()->getNumVacant() << endl;

    if (tree.checkImbalance(root)) {
        tree.rebalance(root);
    }
}


int main() {
    testDestructor();
    testDeepCopy();
    testUpdate();
}