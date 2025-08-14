/**
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */


/**
 * root getter
 */
DNode*& DTree::getRoot() {
    return _root;
}

DTree::~DTree() {
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
    if (this != &rhs) {
        clear();
        _root = copyTree(rhs._root);
    }
    return *this;
}
DNode* DTree::copyTree(DNode* node) {
    if (node == nullptr) {
        return nullptr;
    }
    //create a new node
    DNode* newNode = new DNode(node->getAccount());
    //recursively copy left and right children
    newNode->_left = copyTree(node->_left);
    newNode->_right = copyTree(node->_right);
    //set member variables
    newNode->_size = node->getSize();
    newNode->_numVacant = node->getNumVacant();
    newNode->_vacant = node->isVacant();



    return newNode;
}

/**
 * Dynamically allocates a new DNode in the tree. 
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {
    if (retrieve(newAcct.getDiscriminator()) != nullptr) {
        return false;
    }
    return insertHelper(newAcct, _root);
}

bool DTree::insertHelper(Account newAcct, DNode*& node) {
    bool canInsert = false;
    if (node == nullptr) {
        node = new DNode(newAcct);
        return true;
    }
    else if (node->_vacant) {
        bool lExists = node->_left != nullptr;
        bool rExists = node->_right != nullptr;
        int disc = node->getDiscriminator();
        bool validPlacement = false;

        //check if the disc is between existing children
        if (lExists && rExists) {
            validPlacement = (disc > node->_left->getAccount().getDiscriminator() && disc < node->_right->getAccount().getDiscriminator());
        }
        else if (lExists) {
            validPlacement = (disc > node->_left->getAccount().getDiscriminator());
        }
        else {
            validPlacement = true;
        }

        //update discriminator if it doesn't exist
        if (validPlacement && retrieve(newAcct.getDiscriminator()) == nullptr) {
            node->_vacant = false;
            node->_numVacant--;
            node->_account = newAcct;
            return true;
        }
    }
    //inserting to the left
    if (newAcct.getDiscriminator() < node->getAccount().getDiscriminator()) {
        if (insertHelper(newAcct, node->_left)) {
            updateSize(node);
            updateNumVacant(node);
            if (checkImbalance(node)) {
                rebalance(node);
            }
            canInsert = true;
        }
    }
    //inserting to the right
    else if (newAcct.getDiscriminator() > node->getAccount().getDiscriminator()) {
        if (insertHelper(newAcct, node->_right)) {
            updateSize(node);
            updateNumVacant(node);
            if (checkImbalance(node)) {
                rebalance(node);
            }
            canInsert = true;
        }
    }
    else {
        return false;
    }
    if (canInsert) {
        updateSize(_root);
        updateNumVacant(_root);
        if(checkImbalance(_root)) {
            rebalance(_root);
        }
        return true;
    }
    return false;
}
/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode*& removed) {
    //error handling
    if (disc == INVALID_DISC) {
        return false;
    }
    if (disc < MIN_DISC || disc > MAX_DISC){
        return false;
    }
    if (_root == nullptr) {
        return false;
    }
    if(retrieve(disc) == nullptr) {
        return false;
    }

    DNode* parent = nullptr;
    DNode* current = _root;
    //try to find the node thats going to be removed
    while (current != nullptr && current->_account.getDiscriminator() != disc) {
        parent = current;
        if (disc < current->_account.getDiscriminator()) {
            current = current->_left;
        }
        else {
            current = current->_right;
        }
    }

    //if node was not found:
    if (current == nullptr) {
        return false;
    }

    //if found then make the current node as vacant
    current->_vacant = true;
    removed = current;

    //update the size and numvacant of parents
    while (parent != nullptr) {
        updateSize(parent);
        updateNumVacant(parent);
        if (parent->_account.getDiscriminator() > disc) {
            parent = parent->_left;
        }
        else {
            parent = parent->_right;
        }
    }

    return true;
}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
    return retrieveHelper(disc, _root);
}

DNode* DTree::retrieveHelper(int disc, DNode* node) {
    //check if the node is nullptr
    if (node == nullptr) {
        return nullptr;
    }

    //start traversing the tree
    if (disc < node->getAccount().getDiscriminator()) {
        return retrieveHelper(disc, node->_left);
    }
    else if (disc > node->getAccount().getDiscriminator()) {
        return retrieveHelper(disc, node->_right);
    }
    else {
        if (node->isVacant()) {
            return nullptr;
        }
        else {
            return node;
        }
    }
}
/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear() {
    clear(_root);
    _root = nullptr;
}

//helper for clear function
void DTree::clear(DNode* node) {
    if (node == nullptr) {
        delete node;
        return;
    }
    //clear in-order
    clear(node->_left);
    clear(node->_right);
    delete node;
}

/**
 * Prints all accounts' details within the DTree.
 */
void DTree::printAccounts() const {
    printAccountsHelper(_root);
}
//printaccounts helper function
void DTree::printAccountsHelper(DNode* node) const {
    if (node == nullptr) {
        return;
    }

    //print in order
    printAccountsHelper(node->_left);
    if (!node->isVacant()) {
        cout << node->getAccount() << endl;
    }
    printAccountsHelper(node->_right);
}
/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
    return _root->getSize() - _root->getNumVacant();
}

/**
 * Updates the size of a node based on the imedaite children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* node) {
    node->_size = updateSizeHelper(node);
}
int DTree::updateSizeHelper(DNode* node) {
    if (node == nullptr) {
        return 0;
    } 
    else {
        return 1 + updateSizeHelper(node->_left) + updateSizeHelper(node->_right);
    }
}

/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode* node) {
    if (node == nullptr) {
        return;
    }
    if (node->_vacant) {
        node->_numVacant = 1;
    }
    else {
        node->_numVacant = 0;
    }
    if (node->_left != nullptr) {
        node->_numVacant += node->_left->getNumVacant();
    }
    if (node->_right != nullptr) {
        node->_numVacant += node->_right->getNumVacant();
    }
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode* node) {
    if (node == nullptr) {
        return false;
    }
    //if 2 childs
    if (node->_left != nullptr && node->_right != nullptr) {
        if (node->_left->getSize() >= 4 || node->_right->getSize() >= 4) {
            if (node->_left->getSize() >= 1.5 * node->_right->getSize() || node->_right->getSize() >= 1.5 * node->_left->getSize()) {
                return true;
            }
        }
    }
    //if only right child
    if (node->_left == nullptr && node->_right != nullptr) {
        if (node->_right->getSize() >= 4) {
            return true;
        }
    }
    //if only left child
    if (node->_left != nullptr && node->_right == nullptr) {
        if (node->_left->getSize() >= 4){ 
            return true;
        }
    }
    return false;

}

//----------------
/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (pass by reference).
 * @param node DNode root of the subtree to balance
 */
void DTree::rebalance(DNode*& node) {
    if (node == nullptr) {
        return;
    }
    //first create an aray of all nodes
    int size = node->getSize() - node->getNumVacant();
    DNode* array = new DNode[size];
    //starting index
    int index = 0;
    //create the array with the helper
    createArrayHelper(node, array, index);

    //create the new balanced tree
    DNode* newRoot = rebalanceHelper(array, 0, size - 1);
    //delete the node and set the node equal to the newly balanced root
    clear(node);
    node = newRoot;
    //free array memory
    delete[] array;
}

//helper for creating the array
void DTree::createArrayHelper(DNode* node, DNode* array, int& index) {
    if (node == nullptr) {
        return;
    }

    //create in-order
    createArrayHelper(node->_left, array, index);
    if (!node->isVacant()) {
        array[index] = *node;
        index++;
    }
    createArrayHelper(node->_right, array, index);
}

//helper for rebalancing
DNode* DTree::rebalanceHelper(DNode* array, int start, int end) {
    //stop recursion if start > end
    if (start > end) {
        return nullptr;
    }
    //start from the middle
    int mid = (start + end) / 2;

    DNode* newNode = new DNode(array[mid].getAccount());
    newNode->_left = rebalanceHelper(array,start,mid-1);
    newNode->_right = rebalanceHelper(array, mid+1, end);
    
    //update size and numvacant
    updateSize(newNode);
    updateNumVacant(newNode);
    return newNode;
}
// -- OR --

/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
//DNode* DTree::rebalance(DNode*& node) {
    
//}
//----------------

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() << 
            "\n\tDiscriminator: " << acct.getDiscriminator() <<
            "\n\tNitro: " << acct.hasNitro() << 
            "\n\tBadge: " << acct.getBadge() << 
            "\n\tStatus: " << acct.getStatus();
    return sout;
}