/**
 * Project 2 - Binary Trees
 * UserTree.h
 * An interface for the UTree class.
 */

#pragma once

#include "dtree.h"
#include <fstream>
#include <sstream>

#define DEFAULT_HEIGHT 0

class Grader;   /* For grading purposes */
class Tester;   /* Forward declaration for testing class */

class UNode {
    friend class Grader;
    friend class Tester;
    friend class UTree;
public:
    UNode() {
        _dtree = new DTree();
        _height = DEFAULT_HEIGHT;
        _left = nullptr;
        _right = nullptr;
    }

    ~UNode() {
        delete _dtree;
        _dtree = nullptr;
    }

    /* Getters */
    DTree*& getDTree() {return _dtree;}
    int getHeight() const {return _height;}
    string getUsername() const {return _dtree->getUsername();}
    UNode* getLeft() const {return _left;}
    UNode* getRight() const {return _right;}

private:
    DTree* _dtree;
    int _height;
    UNode* _left;
    UNode* _right;

    /* IMPLEMENT (optional): Additional helper functions */

};

class UTree {
    friend class Grader;
    friend class Tester;

public:
    UTree():_root(nullptr){}

    /* IMPLEMENT: destructor */
    ~UTree();

    /* IMPLEMENT: Basic operations */

    void loadData(string infile, bool append = true);
    bool insert(Account newAcct);
    bool removeUser(string username, int disc, DNode*& removed);
    UNode* retrieve(string username);
    DNode* retrieveUser(string username, int disc);
    int numUsers(string username);
    void clear();
    void printUsers() const;
    void dump() const {dump(_root);}
    void dump(UNode* node) const;


    /* IMPLEMENT: "Helper" functions */
    
    void updateHeight(UNode* node);
    int checkImbalance(UNode* node);
    //----------------
    void rebalance(UNode*& node);
    // -- OR --
    // UNode* rebalance(UNode* node);
    //----------------

    //clear helper function
    void clear(UNode* node);

    //insert helper function
    bool insert(UNode*& node, Account newAcct);

    //removerUser helper function
    bool removeUser(UNode*& node, string username, int disc, DNode*& removed);

    //removeNode helper function
    void removeUNode(UNode*& node, const string& username);

    //retrieve helper function
    UNode* retrieve(UNode* node, string username);

    //printUsers helper function
    void printUsers(UNode* node) const;

    //rotateleft for rebalance
    void rotateLeft(UNode*& node);

    //rotateright for rebalance
    void rotateRight(UNode*& node);

    UNode* getRoot() const {return _root;}

private:
    UNode* _root;

    /* IMPLEMENT (optional): any additional helper functions here! */
};
