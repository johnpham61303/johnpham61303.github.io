#include "buffer.h"

Buffer::Buffer(int capacity){
    // **************************************
    // Implement the alternative constructor
    // No need to set m_next to nullptr, 
    // it will be handled by linked list,
    // because it is used by linked list
    // **************************************

    //initialize m_count
    m_count = 0;

    //initialize m_start
    m_start = 0;

    //initialize m_end
    m_end = 0;

    //if capacity is less than 1, buffer is set to the nullptr
    if (capacity < 1){
        m_capacity = 0;
        m_buffer = nullptr;
    }

    //else allocate memory for the array of size capacity
    else{
        m_capacity = capacity;
        m_buffer = new int[capacity];
        for (int i = 0; i < m_capacity; i++){
            m_buffer[i] = 0;
        }
    }
}

void Buffer::clear(){
    // ***********************************
    // Implement clear() function
    // No need to set m_next to nullptr, 
    // it will be handled by linked list,
    // because it is used by linked list
    // ***********************************

    //check if buffer is a nullptr
    if (m_buffer == nullptr){
        return;
    }
    
    //delete the array
    delete[] m_buffer;

    //initialize m_capacity
    m_capacity = 0;

    //initialize m_count
    m_count = 0;

    //initialize m_start
    m_start = 0;

    //initialize m_end
    m_end = 0;

    //set buffer to nullptr
    m_buffer = nullptr;

}

Buffer::~Buffer(){
    clear();
}

int Buffer::count(){return m_count;}

int Buffer::capacity(){return m_capacity;}

bool Buffer::full(){
    // **************************
    // Implement full() function
    // **************************

    //if the number of items in the array is equal to the capacity, return true
    if (m_count == m_capacity ){
        return true;
    }

    //otherwise return false
    else{
        return false;
    }
}

bool Buffer::empty(){
    // **************************
    // Implement empty() function
    // **************************

    //if the number of items in the array is 0, then retrn true
    if (m_count == 0){
        return true;
    }

    //otherwise return false
    else{
        return false;
    }

}

void Buffer::enqueue(int data){
    // ********************************
    // Implement enqueue(...) function
    // ********************************

    //if the array is full, throw an error
    if (full()){
        throw std::overflow_error("No space for enqueue! Creating a new buffer...");
    }
    
    //put data at the end of the array
    m_buffer[m_end] = data;

    //update m_end and if it's at the last index, we wrap around to the front
    m_end = (m_end + 1) % m_capacity;

    //update m_count
    m_count++;
}

int Buffer::dequeue(){
    // *****************************
    // Implement dequeue() function
    // *****************************

    if(empty()){
        throw std::underflow_error("Nothing to dequeue!");
    }
    int data = m_buffer[m_start];
    m_start = (m_start + 1) % m_capacity;
    m_count--;
    return data;
}

Buffer::Buffer(const Buffer & rhs){
    // *******************************
    // Implement the copy constructor
    // *******************************

    //check if the original buffer is invalid
    if (rhs.m_capacity < 1){
        m_buffer = new int[0];
        m_capacity = 0;
    }
    else{
        m_capacity = rhs.m_capacity;
        m_buffer = new int[m_capacity];
        for (int i = 0; i < m_capacity; i++){
            m_buffer[i] = rhs.m_buffer[i];
        }
    }
    //copy count from rhs
    m_count = rhs.m_count;

    //copy start from rhs
    m_start = rhs.m_start;

    //copy end from rhs
    m_end = rhs.m_end;


}

const Buffer & Buffer::operator=(const Buffer & rhs){
    // ******************************
    // Implement assignment operator
    // ******************************

    //check for self assignment
    if (this == &rhs){
        return *this;
    }

    //delete current buffer
    delete[] m_buffer;

    //copy capacity
    m_capacity = rhs.m_capacity;

    //copy start
    m_start = rhs.m_start;

    //copy end
    m_end = rhs.m_end;

    //copy count
    m_count = rhs.m_count;

    //check if rhs buffer is nullptr
    if (rhs.m_buffer == nullptr) {
        m_buffer = nullptr;
    }

    //else copy each element from rhs array
    else{
        m_buffer = new int[m_capacity];
        for (int i = 0; i < m_capacity; i++){
            m_buffer[i] = rhs.m_buffer[i];
        }
    }
    return *this;
}

void Buffer::dump(){
    int start = m_start;
    int counter = 0;
    if (!empty()) {
        while ( counter < m_count) {
            start = (start + 1) % m_capacity;
            counter++;
        }
    }
}