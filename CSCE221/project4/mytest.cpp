// mytest.cpp: Test suite for the RQueue class.

#include "rqueue.h"
#include <iostream>
using namespace std;

// Priority functions
int priorityFn1(const Student &student);
int priorityFn2(const Student &student);

int main() {
    // Test 1: Create a queue and insert students
    cout << "Test 1: Insert students into the queue" << endl;
    RQueue queue(priorityFn1);
    Student student1("Alice", 2, YEAR1, CS, MINORITY);
    Student student2("Bob", 3, YEAR2, ENG, MAJORITY);
    Student student3("Charlie", 1, YEAR3, SCI, MINORITY);

    queue.insertStudent(student1);
    queue.insertStudent(student2);
    queue.insertStudent(student3);

    queue.dump();
    cout << endl;

    // Test 2: Extract students in priority order
    cout << "Test 2: Extract students in priority order" << endl;
    while (queue.numStudents() > 0) {
        Student student = queue.getNextStudent();
        cout << "[" << priorityFn1(student) << "] " << student << endl;
    }
    cout << endl;

    // Test 3: Change priority function and reinsert students
    cout << "Test 3: Change priority function and reinsert students" << endl;
    queue.insertStudent(student1);
    queue.insertStudent(student2);
    queue.insertStudent(student3);

    queue.setPriorityFn(priorityFn2);
    queue.dump();
    cout << endl;

    // Test 4: Merge two queues
    cout << "Test 4: Merge two queues" << endl;
    RQueue queue2(priorityFn1);
    Student student4("David", 4, YEAR4, CS, MAJORITY);
    queue2.insertStudent(student4);

    queue.mergeWithQueue(queue2);
    queue.dump();
    cout << endl;

    // Test 5: Clear the queue
    cout << "Test 5: Clear the queue" << endl;
    queue.clear();
    queue.dump();
    cout << endl;

    return 0;
}

int priorityFn1(const Student &student) {
    return student.getYear() + student.getMajor() + student.getGroup();
}

int priorityFn2(const Student &student) {
    return student.getPriority();
}