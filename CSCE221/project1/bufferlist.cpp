#include "bufferlist.h"
BufferList::BufferList(int minBufCapacity){
    // **************************************
    // Implement the alternative constructor
    // **************************************

    //if minbufcapacity is less than 1, the constructor creates a buffer with size default_min_capacity
    if(minBufCapacity < 1){
        m_minBufCapacity = DEFAULT_MIN_CAPACITY;
    }
    else {
        m_minBufCapacity = minBufCapacity;
    }
    m_cursor = new Buffer(m_minBufCapacity);
    m_cursor->m_next = m_cursor;
    m_listSize +=1;
}
BufferList::~BufferList(){
    clear();
}

void BufferList::clear(){
    // *****************************
    // Implement clear() function
    // *****************************
    
    //check for nullptr
    if (m_cursor == nullptr){
        return;
    }

    //set both delete pointers to the beginning of the linked list
    Buffer* bufferDelete = m_cursor->m_next;
    Buffer* nodeDelete = m_cursor->m_next;
    
    //loop until bufferdelete is equal to the m_cursor
    //for each bufferdelete not equal to m_cursor, delete the Buffer
    //then update bufferdelete to be the next buffer and update nodedelete as well
    while (bufferDelete != m_cursor){
        bufferDelete->clear();
        bufferDelete = bufferDelete->m_next;
        delete nodeDelete;
        nodeDelete = bufferDelete;
    }

    //clear the cursor
    m_cursor->clear();
    delete m_cursor;
    m_cursor = nullptr;
    m_listSize = 0;

}

void BufferList::enqueue(const int& data) {
    // ****************************************************************************
    // Implement enqueue(...) function
    // we always insert at the back, that would be the cursor
    // if the current cursor is full we create a new node as cursor then we insert
    // ****************************************************************************

    //try to enqueue data
    try{
        this->m_cursor->enqueue(data);
    }

    //if caught then create new array
    catch (const std::overflow_error& e){
        int newSize = (m_cursor->capacity()) * INCREASE_FACTOR;
        //if new size is greater than the max factor, set to min buff capacity
        if (newSize > MAX_FACTOR * m_minBufCapacity){
            newSize = m_minBufCapacity;
        }

        //create a new buffer with newsize
        Buffer* newBuffer = new Buffer(newSize);

        //set new buffer's next to be this cursor's next
        newBuffer->m_next = m_cursor->m_next;

        //set up cursor to enqueue the data
        m_cursor->m_next = newBuffer;
        m_cursor = newBuffer;
        m_cursor->enqueue(data);

        //update list size
        m_listSize += 1;
    }
}

int BufferList::dequeue() {
    // *****************************
    // Implement dequeue() function
    // *****************************

    //if list size is 0 then there's nothing to dequeue
    if (m_listSize == 0){
        throw std::underflow_error("Nothing to dequeue!");
    }
    try{
        int data = this->m_cursor->m_next->dequeue();
        //try to dequeue the data
        if (m_cursor->m_next->empty() && m_listSize > 1) {
            //create a temporary pointer to the second buffer
            Buffer* temp = m_cursor->m_next->m_next;
            //delete first buffer and make sure cursors next is the second buffer
            delete m_cursor->m_next;
            m_cursor->m_next = temp;
            //update list size
            m_listSize -= 1;
        }    
        return data;
    }
    catch (const std::underflow_error& e){
        //if there is only one buffer in the list
        if (m_listSize == 1) {
            throw std::underflow_error("Nothing to dequeue! Only one buffer in the list.");
        }
        //create a temp pointer to second buffer
        Buffer* temp = m_cursor->m_next->m_next;
        //delete first buffer
        delete m_cursor->m_next;
        m_cursor->m_next = temp;
        //update list size
        m_listSize -= 1;
        int data = m_cursor->m_next->dequeue();
        return data;
    }

    
}

BufferList::BufferList(const BufferList & rhs){
    // *******************************
    // Implement the copy constructor
    // *******************************

    //if rhs cursor is nullptr
    if (rhs.m_cursor == nullptr){
        m_cursor = nullptr;
        return;
    }

    //update list size and minbuffcapacity
    m_listSize = rhs.m_listSize;
    m_minBufCapacity = rhs.m_minBufCapacity;

    //create pointers to update the new bufferlist
    Buffer* rcurrent = rhs.m_cursor->m_next;
    m_cursor = new Buffer(*rcurrent);
    Buffer* lstart = m_cursor;
    Buffer* lprev = m_cursor;

    //loop through rhs and copy over
    while(rcurrent != rhs.m_cursor) {
        rcurrent = rcurrent->m_next;
        m_cursor->m_next = new Buffer(*rcurrent);
        m_cursor = m_cursor->m_next;
        lprev->m_next = m_cursor;
        lprev = lprev->m_next;
    }

    m_cursor->m_next = lstart;
}

const BufferList & BufferList::operator=(const BufferList & rhs){
    // ******************************
    // Implement assignment operator
    // ******************************

    //self assignment check
    if (this == &rhs) {
        return *this;
    }

    this->clear();

    //check if rhs cursor is nullptr
    if (rhs.m_cursor == nullptr){
        this->m_cursor = nullptr;
        return *this;
    }
    
    //copy over list size and buffer capacity
    m_listSize = rhs.m_listSize;
    m_minBufCapacity = rhs.m_minBufCapacity;

   //create pointers to update the new bufferlist
    Buffer* rcurrent = rhs.m_cursor->m_next;
    m_cursor = new Buffer(*rcurrent);
    Buffer* lstart = m_cursor;
    Buffer* lprev = m_cursor;

    //loop through rhs and copy over
    while(rcurrent != rhs.m_cursor) {
        rcurrent = rcurrent->m_next;
        m_cursor->m_next = new Buffer(*rcurrent);
        m_cursor = m_cursor->m_next;
        lprev->m_next = m_cursor;
        lprev = lprev->m_next;
    }

    m_cursor->m_next = lstart;

    return *this;
}

void BufferList::dump(){
    Buffer* temp = m_cursor->m_next;
    for (int i = 0; i < m_listSize; i++){
        temp->dump();
        temp = temp->m_next;
    }
}