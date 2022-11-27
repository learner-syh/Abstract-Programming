#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    /* TODO: Implement this function. */
}

void partition(ListNode*& front,ListNode*& small,ListNode*& equal,ListNode*& large){
    int reference = front->data;
    for(ListNode *cur = front; cur != nullptr; cur = cur->next){
        if(cur->data < reference){
            if(small == nullptr){
                small = cur;
            }else{
                ListNode *copy = small;
                while(copy != nullptr){
                    copy = copy->next;
                }
                copy = cur;
            }
            small->next == nullptr;
        }else if(cur->data == reference){
            if(equal == nullptr){
                equal = cur;
            }else{
                ListNode *copy = equal;
                while(copy != nullptr){
                    copy = copy->next;
                }
                copy = cur;
            }
            equal->next = nullptr;
        }else{
            if(large == nullptr){
                large = cur;
            }else{
                ListNode *copy = large;
                while(copy != nullptr){
                    copy = copy->next;
                }
                copy = cur;
            }
            large->next = nullptr;
        }
    }
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    /* TODO: Implement this function. */
    ListNode *nowNode = front;
    while(nowNode != nullptr){
        front = front->next;
        delete  nowNode;
        nowNode = front;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    /* TODO: Implement this function. */
    ListNode *front  = nullptr;
    for(int elem : values){
        if(front == nullptr){
            front = new ListNode{elem,nullptr};
        }else{
                ListNode *copy  = front;
                while(copy->next != nullptr){
                    copy = copy->next;
                }
                copy->next = new  ListNode{elem,nullptr};
        }
    }
    return  front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    int i =0;
    for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
        if(i < v.size()){
            if (cur->data != v[i]){
                return false;//元素不同
            }
        }else{
            return false;//链表长度大于向量长度
        }
        i++;
    }
    if(i != v.size()){
        return false;//向量长度大于链表
    }
    return true;
}

STUDENT_TEST("Test partition with normal case"){
    /* Initialize vector to sequence of values */
    Vector<int> values = {2, 1, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);
    ListNode* small = nullptr;
    ListNode* equal = nullptr;
    ListNode* large = nullptr;

    partition(list,small,equal,large);
    EXPECT(areEquivalent(small, {1}));
    EXPECT(areEquivalent(equal, {2}));
    EXPECT(areEquivalent(large, {3,4}));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}



STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

