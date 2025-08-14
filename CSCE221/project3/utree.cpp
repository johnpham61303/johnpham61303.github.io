/**
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
    clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    return insert(_root, newAcct);
}

//helper function for insert
bool UTree::insert(UNode*& node, Account newAcct) {
    if (node == nullptr) {
        // Create a new UNode if not found
        node = new UNode();
        node->getDTree()->insert(newAcct);
        updateHeight(node);
        return true;
    }

    string username = newAcct.getUsername();
    if (username < node->getUsername()) {
        // Traverse left
        if (!insert(node->_left, newAcct)) {
            return false;
        }
    } else if (username > node->getUsername()) {
        // Traverse right
        if (!insert(node->_right, newAcct)) {
            return false;
        }
    } else {
        // Username matches, insert into DTree
        if (!node->getDTree()->insert(newAcct)) {
            return false; // Account already exists
        }
    }

    // Update height and check for imbalance
    updateHeight(node);
    if (checkImbalance(node)) {
        rebalance(node);
    }

    return true;
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
    UNode* node = retrieve(username);
    if (node == nullptr) {
        return false; // Username not found
    }

    // Remove the account from the DTree
    if (!node->getDTree()->remove(disc, removed)) {
        return false; // Account not found
    }

    // Check if the DTree is now empty
    if (node->getDTree()->getNumUsers() == 0) {
        // Remove the UNode from the UTree
        removeUNode(_root, username);
    }

    return true;
}

void UTree::removeUNode(UNode*& node, const string& username) {
    if (node == nullptr) return;

    if (username < node->getUsername()) {
        removeUNode(node->_left, username);
    } else if (username > node->getUsername()) {
        removeUNode(node->_right, username);
    } else {
        // Node to be deleted found
        if (node->_left == nullptr) {
            UNode* temp = node;
            node = node->_right;
            delete temp;
        } else if (node->_right == nullptr) {
            UNode* temp = node;
            node = node->_left;
            delete temp;
        } else {
            // Node with two children
            UNode* largest = node->_left;
            while (largest->_right != nullptr) {
                largest = largest->_right;
            }
            node->_dtree = largest->_dtree;
            removeUNode(node->_left, largest->getUsername());
        }
    }

    if (node != nullptr) {
        updateHeight(node);
        if (checkImbalance(node)) {
            rebalance(node);
        }
    }
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    return retrieve(_root, username);
}

//helper function for retrieve
UNode* UTree::retrieve(UNode* node, string username) {
    if (node == nullptr) {
        return nullptr; // Base case: node not found
    }

    if (username < node->getUsername()) {
        // Traverse left
        return retrieve(node->_left, username);
    } else if (username > node->getUsername()) {
        // Traverse right
        return retrieve(node->_right, username);
    } else {
        // Username matches
        return node;
    }
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    // First, retrieve the UNode with the matching username
    UNode* userNode = retrieve(username);
    if (userNode == nullptr) {
        return nullptr; // Username not found
    }

    // Now, retrieve the DNode with the matching discriminator from the DTree
    DNode* userAccount = userNode->getDTree()->retrieve(disc);
    return userAccount; // This will be nullptr if the discriminator is not found
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    UNode* node = retrieve(username);
    if (node != nullptr) {
        return node->getDTree()->getNumUsers();
    }
    else {
        return 0;
    }
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    clear(_root);
    _root = nullptr;
}
//clear helper function
void UTree::clear(UNode* node) {
    if (node == nullptr) {
        delete node;
        return;
    }
    else {
        clear(node->_left);
        clear(node->_right);
        delete node;
    }
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    printUsers(_root);
}

void UTree::printUsers(UNode* node) const {
    if (node == nullptr) {
        return; // Base case: if the node is null, do nothing
    }

    // Traverse the left subtree
    printUsers(node->_left);

    // Print accounts in the current node's DTree
    node->getDTree()->printAccounts();

    // Traverse the right subtree
    printUsers(node->_right);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    if (node == nullptr) {
        return; // If the node is null, there's nothing to update
    }

    // Calculate the height of the left and right children
    int leftHeight = (node->_left != nullptr) ? node->_left->getHeight() : -1;
    int rightHeight = (node->_right != nullptr) ? node->_right->getHeight() : -1;

    // The height of the current node is the maximum height of its children plus one
    node->_height = std::max(leftHeight, rightHeight) + 1;
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
    if (node == nullptr) {
        return 0; // If the node is null, there's no imbalance
    }

    // Calculate the height of the left and right children
    int leftHeight = (node->_left != nullptr) ? node->_left->getHeight() : -1;
    int rightHeight = (node->_right != nullptr) ? node->_right->getHeight() : -1;

    // Calculate the balance factor
    int balanceFactor = leftHeight - rightHeight;

    // Return the balance factor
    return balanceFactor;
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode*& node) {
    int balanceFactor = checkImbalance(node);

    // Left heavy
    if (balanceFactor > 1) {
        if (checkImbalance(node->_left) < 0) {
            // Left-Right case
            rotateLeft(node->_left);
        }
        // Left-Left case
        rotateRight(node);
    }
    // Right heavy
    else if (balanceFactor < -1) {
        if (checkImbalance(node->_right) > 0) {
            // Right-Left case
            rotateRight(node->_right);
        }
        // Right-Right case
        rotateLeft(node);
    }
}

void UTree::rotateLeft(UNode*& node) {
    UNode* newRoot = node->_right;
    node->_right = newRoot->_left;
    newRoot->_left = node;
    updateHeight(node);
    updateHeight(newRoot);
    node = newRoot;
}

void UTree::rotateRight(UNode*& node) {
    UNode* newRoot = node->_left;
    node->_left = newRoot->_right;
    newRoot->_right = node;
    updateHeight(node);
    updateHeight(newRoot);
    node = newRoot;
}


/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UNode* UTree::rebalance(UNode* node) {

//}
//----------------
