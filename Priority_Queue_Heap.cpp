/*
 * This is the class implementation for pqheap, where a heap is created
 * and member variable functions are defined.
 */

#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * This helper function expands the memory allocated to the array holding our heap
*/
void PQHeap::expandMemory() {
    DataPoint* newElements = new DataPoint[_numAllocated * 2];
    for (int i = 0; i < _numAllocated; i++) {
        newElements[i] = _elements[i];
    }
    delete [] _elements;
    _elements = newElements;
    _numAllocated *= 2;
}


/*
 * The 'constructor' initializes all of the member variables needed for an instance of the PQHeap class.
 * Numbers allocated for the array is set to 'INITIAL_CAPACITY', the numbers filled in the array is set to zero, and a
 * new array filled with DataPoints is created on the heap.
*/
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _numFilled = 0;
    _elements = new DataPoint[_numAllocated]();
}


/*
 * The 'destructor' deletes any memory used by this instance of the of the PQHeap class. The array memory
 * that was allocated for PQHeap is deleted here.
 */
PQHeap::~PQHeap() {
    delete [] _elements;
}


/*
 * 'bubbleUp' is a private member function that swaps DataPoints in the heap while the DataPoint is larger than either of its children nodes.
 * If the DataPoint has no children nodes or it is in the correct location, the function ends.
 */
void PQHeap::bubbleUp(DataPoint dp) {
    int i = size() - 1;
    while (getParentIndex(i) != -1 && dp.priority < _elements[getParentIndex(i)].priority) {
        swapElements(i, getParentIndex(i));
        i = getParentIndex(i);
        if (i == 0) {
            break;
        }
    }
}


/*
 * 'rightChildLess' will take a parent index in a min-heap and compare it's two children nodes (the function assumes that there
 * exists children node. The function returns a boolean value 'true' if the right children node has a smaller (or equal) priority to
 * the left children node.
 */
bool PQHeap::rightChildLess(int i) {
    return (_elements[getRightChildIndex(i)].priority <= _elements[getLeftChildIndex(i)].priority);
}


/*
 * 'bubbleDown' is a private member function that moves a DataPoint into its correct location while
 * the DataPoint is larger than either of its children nodes. If the DataPoint has no children nodes or it is in the correct location, the function
 * ends.
 */
void PQHeap::bubbleDown(DataPoint dp) {
    int i = 0;

    // checks if the parent node has children and is great than them
    while ((getLeftChildIndex(i) != NONE && dp.priority > _elements[getLeftChildIndex(i)].priority) ||
           (getRightChildIndex(i) != NONE && dp.priority > _elements[getRightChildIndex(i)].priority)) {

        // when a parent only has a left node, swap if the child is smaller
        if (getRightChildIndex(i) == NONE && getLeftChildIndex(i) != NONE) {
            if (dp.priority > _elements[getLeftChildIndex(i)].priority) {
                swapElements(i, getLeftChildIndex(i));
                i = getLeftChildIndex(i);
            }
        }

        // when a parent has both children nodes, swap with the smaller child
        if (getRightChildIndex(i) != NONE && getLeftChildIndex(i) != NONE) {
            if (rightChildLess(i)) {
                swapElements(i, getRightChildIndex(i));
                i = getRightChildIndex(i);
            } else {
                swapElements(i, getLeftChildIndex(i));
                i = getLeftChildIndex(i);
            }
        }
    }
}


/*
 * The 'enqueue' function will take a DataPoint and place it to the back of our heap. 'enqueue' then
 * calls a function, 'bubbleUp', to move the DataPoint into the correct location in the heap. The
 * heap uses a min property, where every parent node is less than its two child nodes.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (_numFilled == _numAllocated) {
        expandMemory();
    }
    else {
        _elements[_numFilled] = elem;
        _numFilled++;
        bubbleUp(elem);
    }
}


/*
 * The dequeue function will return the highest priority value from the min-heap, then
 * replace that value with the min-heap's lowest priority DataPoint. The function uses 'bubbleDown'
 * to move this DataPoint into its correct location.
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    _elements[0] = _elements[size()];
    DataPoint dp = _elements[0];
    bubbleDown(dp);
    return front;
}


/*
 * The heap is stored with the highest priority in the first node, which is stored
 * in the first index of the '_elements' array
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot access front element of empty pqueue!");
    }
    return _elements[0];
}


/*
 * Returns true if the queue contains no elements, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}


/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}


/*
 * Updates internal state to reflect that the queue is empty, i.e. count
 * of filled slots is reset to zero.
 */
void PQHeap::clear() {
    _numFilled = 0;
}


/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}


/*
 * Confirm the internal state of member variables appears valid.
 * In this case, check that the elements in the array are stored in
 * according to the min-heap properties
 */
void PQHeap::validateInternalState() const {
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");
    for (int i = 0; i < size() - 1; i++) {

        int elemPriority = _elements[i].priority;
        int leftChildPriority = _elements[getLeftChildIndex(i)].priority;
        int rightChildPriority =  _elements[getRightChildIndex(i)].priority;
        int leftChildIndex = getLeftChildIndex(i);
        int rightChildIndex = getRightChildIndex(i);

        if (leftChildIndex != NONE and elemPriority > leftChildPriority) {
            printDebugInfo("validateInternalState");
            error("PQHeap has elements that violate the heap property " + integerToString(i));
        }

        if (rightChildIndex != NONE  and elemPriority > rightChildPriority) {
            printDebugInfo("validateInternalState");
            error("PQHeap has elements that violate the heap property " + integerToString(i));
        }
    }
}


/*
 * The getParentIndex gets the index of the parent from a given child index, and will return
 * NONE if there is no parent for a child
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) {
        return NONE;
    }
    int parent = (child - 1) / 2;
    return parent;
}


/*
 * The validateIndex function will return an error if an index is not valid.
 */
void PQHeap::validateIndex(int index) {
    if (index < 0 || index >= _numFilled) {error("Invalid index " + integerToString(index));}
}


/*
 * The getLeftChildIndex will return the left child to a parent node, and NONE if there isn't one.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int leftChild = (parent * 2) + 1;
    if (leftChild >= size()) {
        return NONE;
    }
    return leftChild;
}


/*
 * The getRightChildIndex will return the right child to a parent node, and NONE if there isn't one.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int rightChild = (parent * 2) + 2;
    if (rightChild >= size()) {
        return NONE;
    }
    return rightChild;
}


/*
 * The 'swapElements' function will swap elements given both of its index locations.
 */
void PQHeap::swapElements(int indexA, int indexB) {
    validateIndex(indexA);
    validateIndex(indexB);
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Validate small, hand-made bad heap") {
    PQHeap pq;

    Vector<DataPoint> input = {
                               { "R", 4 }, { "A", 5 },  { "K", 7 }, {"", 1 }};

    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.printDebugInfo("Debug info after one step");
        pq.validateInternalState();
    }
}

STUDENT_TEST("Validate small, hand-made good heap") {
    PQHeap pq;

    pq.enqueue( { "Zoe", 3 } );
    pq.enqueue( { "Elmo", 2 } );
    pq.enqueue( { "Bert", 6 } );
    pq.validateInternalState();
}

STUDENT_TEST("Validate small, hand-made good heap") {
    PQHeap pq;

    pq.enqueue( { "Zoe", 3 } );
    pq.enqueue( { "Elmo", 3 } );
    pq.enqueue( { "Bert", 2 } );
    pq.validateInternalState();
}

STUDENT_TEST("Validate one-node good heap") {
    PQHeap pq;

    pq.enqueue( { "Zoe", 3 } );
    pq.validateInternalState();
}

STUDENT_TEST("Validate two-node bad heap") {
    PQHeap pq;

    pq.enqueue( { "Zoe", 3 } );
    pq.enqueue( {"Casey", 1 } );
    pq.validateInternalState();

    pq.dequeue();
    pq.validateInternalState();
    pq.dequeue();
    pq.validateInternalState();
}

STUDENT_TEST("Test peek on 2 element heap") {
    PQHeap pq;

    pq.enqueue( {"Casey", 1 } );
    pq.enqueue( { "Zoe", 3 } );

    int peek = pq.peek().priority;
    EXPECT_EQUAL(peek, 1);
}

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
                               { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 }};

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    pq.printDebugInfo("After enqueue");
}

STUDENT_TEST("Test peek on empty element heap") {
    PQHeap pq;
    int peek = pq.peek().priority;
}

PROVIDED_TEST("PQHeap example with repeated DataPoints, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
                               { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
                               { "V", 9 }, { "T", 1 }, { "O", 8 }, { "O", 8 }, { "O", 8 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    pq.printDebugInfo("After enqueue");
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
                               { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
                               { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
    pq.printDebugInfo("After enqueue");
}
