// RQueue: 

#include "rqueue.h"
#include <vector>
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

RQueue::RQueue(prifn_t priFn)
{
  //set variables
  _heap = nullptr;
  _size = 0;
  priority = priFn;

}

RQueue::~RQueue()
{
  //clear the heap
  clear();
}

RQueue::RQueue(const RQueue& rhs)
{

  //copy the priority function over
  priority = rhs.priority;
  //use a helper function to recursively copy the heap
  _size = rhs._size;
  //call helper function to recursively copy the heap
  _heap = copyHeap(rhs._heap);
}

Node* RQueue::copyHeap(Node* node) {
  if (node == nullptr) {
    return nullptr;
  }

  //create a new node with the same student
  Node* newNode = new Node(node->_student);

  //recursively copy the left and right subtrees
  newNode->_left = copyHeap(node->_left);
  newNode->_right = copyHeap(node->_right);

  return newNode;
}


RQueue& RQueue::operator=(const RQueue& rhs)
{

  //check for self-assignment
  if (this == &rhs) {
    return *this;
  }

  //free existing resources
  clear();

  //copy the priority function over
  priority = rhs.priority;

  //copy the size
  _size = rhs._size;

  //use a helper function to recursively copy the heap
  _heap = copyHeap(rhs._heap);
  return *this;
}

void RQueue::insertStudent(const Student& input) {
  // record the previous heap
  RQueue prev = RQueue(priority);
  prev._heap = _heap;
  prev._size = _size;
  _heap = new Node(input);
  _size = 1;
  // Merge the new node with the previous heap
  mergeWithQueue(prev);
}

Node* RQueue::merge(Node* h1, Node* h2) {
    // If one of the heaps is empty, return the other
    if (h1 == nullptr) return h2;
    if (h2 == nullptr) return h1;

    // Ensure h1 has the smaller root according to the priority function
    if (priority(h1->_student) > priority(h2->_student)) {
        std::swap(h1, h2);
    }

    // Merge h1's right child with h2 and swap the children of h1
    h1->_right = merge(h1->_right, h2);
    std::swap(h1->_left, h1->_right);

    return h1;
}

Student RQueue::getNextStudent() {

  if (_heap == nullptr) {
    throw domain_error("Queue is empty");
  }

  //the root of the heap contains the highest priority student
  Student highestPriorityStudent = _heap->_student;

  //merge the left and right subtrees
  Node* oldRoot = _heap;
  _heap = merge(_heap->_left, _heap->_right);

  //delete the old root node
  delete oldRoot;

  //decrement the size of the heap
  _size--;

  return highestPriorityStudent;
}


void RQueue::mergeWithQueue(RQueue& rhs) {

  //check for self-merging
  if (this == &rhs) {
    throw domain_error("Cannot merge a queue with itself");
  }

  //check if the priority functions are the same
  if (priority != rhs.priority) {
    throw domain_error("Cannot merge queues with different priority functions");
  }

  //merge the two heaps
  _heap = merge(_heap, rhs._heap);

  //update the size of the host queue
  _size += rhs._size;

  //clear the rhs queue
  rhs._heap = nullptr;
  rhs._size = 0;
}

void RQueue::clear() {

  clear(_heap);
  _heap = nullptr;
  _size = 0;
}

void RQueue::clear(Node* node) {
  if (node != nullptr) {
    clear(node->_left);
    clear(node->_right);
    delete node;
  }
}

int RQueue::numStudents() const
{
  return _size;
}

void RQueue::printStudentQueue() const {
  std::cout << "Contents of the student queue:" << std::endl;
  printPreorder(_heap);
}

void RQueue::printPreorder(Node* node) const {
  if (node != nullptr) {
    // Print the current student with its priority
    std::cout << "[" << priority(node->_student) << "] " << node->_student << std::endl;

    // Recursively print the left and right subtrees
    printPreorder(node->_left);
    printPreorder(node->_right);
  }
}

prifn_t RQueue::getPriorityFn() const {
  return priority;
}

void RQueue::setPriorityFn(prifn_t priFn) {
    // Extract all elements from the current heap into a temporary container
    std::vector<Student> tempStudents;
    while (_heap) {
        tempStudents.push_back(getNextStudent());
    }

    // Update the priority function
    priority = priFn;

    // Rebuild the heap
    for (const auto& student : tempStudents) {
        insertStudent(student);  // Reinsert using the new priority
    }
}

void RQueue::collectStudents(Node* node, std::vector<Student>& students) const {
    if (node != nullptr) {
        students.push_back(node->_student);
        collectStudents(node->_left, students);
        collectStudents(node->_right, students);
    }
}


// for debugging
void RQueue::dump() const
{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr() 
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}