#include <iostream>
#include "utree.h"
#include "dtree.h"

void testUTree() {
    UTree tree;

    // Test insertion
    std::cout << "Testing insertion..." << std::endl;
    tree.insert(Account("user1", 1234, false, "badge1", "status1"));
    tree.insert(Account("user2", 2345, false, "badge2", "status2"));
    tree.insert(Account("user3", 3456, false, "badge3", "status3"));
    tree.insert(Account("user1", 5678, false, "badge4", "status4"));

    // Test retrieval
    std::cout << "Testing retrieval..." << std::endl;
    UNode* user1Node = tree.retrieve("user1");
    if (user1Node) {
        std::cout << "Retrieved UNode for 'user1'." << std::endl;
    } else {
        std::cout << "Failed to retrieve UNode for 'user1'." << std::endl;
    }

    // Test retrieveUser
    std::cout << "Testing retrieveUser..." << std::endl;
    DNode* user1Account = tree.retrieveUser("user1", 1234);
    if (user1Account) {
        std::cout << "Retrieved account for 'user1' with discriminator 1234." << std::endl;
    } else {
        std::cout << "Failed to retrieve account for 'user1' with discriminator 1234." << std::endl;
    }

    // Test numUsers
    std::cout << "Testing numUsers..." << std::endl;
    int numUser1 = tree.numUsers("user1");
    std::cout << "Number of users with username 'user1': " << numUser1 << std::endl;

    // Test removal
    std::cout << "Testing removal..." << std::endl;
    DNode* removed = nullptr;
    if (tree.removeUser("user1", 1234, removed)) {
        std::cout << "Removed user1 with discriminator 1234." << std::endl;
    } else {
        std::cout << "Failed to remove user1 with discriminator 1234." << std::endl;
    }

    // Test if UNode is removed after all accounts are vacant
    tree.removeUser("user1", 5678, removed);
    if (tree.retrieve("user1") == nullptr) {
        std::cout << "UNode for 'user1' is removed after all discriminators are removed." << std::endl;
    } else {
        std::cout << "UNode for 'user1' is not removed correctly." << std::endl;
    }

    // Test printUsers
    std::cout << "Testing printUsers..." << std::endl;
    tree.printUsers();

    // Test dump
    std::cout << "Testing dump..." << std::endl;
    tree.dump();
    std::cout << std::endl;

    // Test clear
    std::cout << "Testing clear..." << std::endl;
    tree.clear();
    if (tree.getRoot() == nullptr) {
        std::cout << "Tree cleared successfully." << std::endl;
    } else {
        std::cout << "Failed to clear the tree." << std::endl;
    }
}

int main() {
    testUTree();

    // Run valgrind to check for memory leaks
    // Command: valgrind --leak-check=full --track-origins=yes ./test_program

    return 0;
}