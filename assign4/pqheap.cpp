#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * TODO: 初始化.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    if(_numAllocated == size()){
        enlarge();
    }
    int index = _numFilled;
    _elements[_numFilled++] = elem;
    int parent = getParentIndex(index);
    while (parent != NONE && _elements[parent].priority > _elements[index].priority) {
        swap(parent,index);
        index = parent;
        parent = getParentIndex(index);
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    if(!isEmpty()){
        _elements[0] = _elements[_numFilled];
        int index = 0;
        int leftChild;
        int rightChild;
        int smallChild;
        while(index != NONE){
            leftChild = getLeftChildIndex(index);
            rightChild = getRightChildIndex(index);
            if(leftChild != NONE && rightChild != NONE){
                if(_elements[rightChild].priority > _elements[leftChild].priority){
                    smallChild = leftChild;
                }else {
                    smallChild = rightChild;
                }
            }else if(leftChild != NONE){
                smallChild = leftChild;
            }else {
                smallChild = rightChild;
            }

            if(smallChild != NONE && _elements[index].priority > _elements[smallChild].priority){
                swap(index,smallChild);
            }

            index = smallChild;
        }
    }
    return front;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */

//果把不改变数据成员的函数都加上const关键字进行标识，显然，可提高程序的可读性。其实，它还能提高程序的可靠性，已定义成const的成员函数，一旦企图修改数据成员的值，则编译器按错误处理。
void PQHeap::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    //从头开始，依次比较parent和child的大小
    validateInternalStateRec(0);

}

void PQHeap::validateInternalStateRec(int index) const {
    //从头开始，依次比较parent和child的大小
    int leftChild = getLeftChildIndex(index);
    int rightChild = getRightChildIndex(index);

    if(leftChild != NONE){
        if(_elements[leftChild].priority < _elements[index].priority){
            printDebugInfo("validateInternalState");
            error("PQHeap has elements out of order at index " + integerToString(index));
        }
        validateInternalStateRec(leftChild);
    }

    if(rightChild != NONE){
        if(_elements[rightChild].priority < _elements[index].priority){
            printDebugInfo("validateInternalState");
            error("PQHeap has elements out of order at index " + integerToString(index));
        }
        validateInternalStateRec(rightChild);
    }

}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    /* TODO: Implement this function. */
    return double((child-1)/2) < 0? NONE : (child-1)/2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    /* TODO: Implement this function. */
    return (parent*2+1) > size()-1? NONE :parent*2+1;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    return (parent*2+2) > size()-1? NONE :parent*2+2;
}

void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}
/*
 *used for enlarging the array when the size of array is not enough
 */

void PQHeap::enlarge(){
     DataPoint* old_elements;
     old_elements = _elements;
     _numAllocated *= 2;
     _elements = new DataPoint[_numAllocated]();
     for(int i = 0;i < _numFilled;i++){
         _elements[i] = old_elements[i];
     }
     delete []old_elements;
}


/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("the enqueue function of PQHeap when input is smaller than the initial size") {
    PQHeap pq;
    for(int i = 0;i < 10;i++){
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

STUDENT_TEST("the enqueue function of PQHeap when input is larger than the initial size") {
    PQHeap pq;
    for(int i = 0;i < 100;i++){
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

STUDENT_TEST("test enque with validateInternalState") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
}
void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

//时间复杂度分析
STUDENT_TEST("PQHeap timing test, fillQueue ") {
    PQHeap pq;
    int n = 10000;
    //分析enqueue的复杂度为O(log(n))，则fillQueue的复杂度应该为O(nlogn)
    for(int i = 0;i < 5;i++){
        TIME_OPERATION(n*i, fillQueue(pq, n*i));
        TIME_OPERATION(n*i, emptyQueue(pq, n*i));
    }
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
}
