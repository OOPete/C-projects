// CMSC 341 - Fall 2022 - Project 3
#include "iqueue.h"
#include <random>
//Peter Obinomen
// cmsc 341
// Professor Donyaee
// We will be using a Priority Queue implemented in the form of a heap
// to retrieve and insert crop information
// the followings are sample priority functions to be used by IQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a crop.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Crop& crop);// works with a MAXHEAP
int priorityFn2(const Crop& crop);// works with a MINHEAP

class Tester{
    
public:
    //will test the heap property of the queue
    bool testHeapProperty(const IQueue& aHeap, Node* root);
    //testing the minheap/maxheap case for insert and delete
    bool testHeapTypeInsertion(IQueue& aHeap);
    bool testHeapTypeDeletion(IQueue& aHeap);
    //tests the merge function
    bool testMerge(IQueue& aHeap, IQueue& bHeap);
    //tests the copy constructor
    bool copyConstructorTest();
    //tests the assignment Operator
    bool assignmentOperatorTest();
    //checks if both heaps are equal or not
    bool checkIfEqual(Node* root1, Node* root2);
    //checks if the pointers of both heaps are the same(pointing to the same object)
    bool checkIfEqualPointers(Node* root1, Node* root2);
    //tests the setPriority function
    bool checkSetPriority();
    //tests the print and clear functions
    bool checkPrintAndClear();
    
};

bool Tester::testHeapProperty(const IQueue& aHeap, Node* root)
{
    //helps pass or fail the function
    bool pass = true;
    if (root != nullptr)
    {
        //checks the heap type
        if (aHeap.m_heapType == MINHEAP) {
            //check if both children exist
            if (root->m_left != nullptr && root->m_right != nullptr)
            {
                //checks that the parent root has a value less than that of its children
                pass = pass && (aHeap.m_priorFunc(root->m_crop) <= aHeap.m_priorFunc(root->m_left->m_crop))
                    && (aHeap.m_priorFunc(root->m_crop) <= aHeap.m_priorFunc(root->m_right->m_crop));
            }
            //check if only m_left exists
            else if (root->m_left != nullptr && root->m_right == nullptr)
            {
                pass = pass && (aHeap.m_priorFunc(root->m_crop) <= aHeap.m_priorFunc(root->m_left->m_crop));
            }
            //check if only m_right exists
            else if (root->m_right != nullptr && root->m_left == nullptr)
            {
                pass = pass && (aHeap.m_priorFunc(root->m_crop) <= aHeap.m_priorFunc(root->m_right->m_crop));
            }
            else {
                //do nothing as pass is already starting off as true
            }

        }
        //check if it is the max heap
        else if (aHeap.m_heapType == MAXHEAP) {
            //check if both children exist
            if (root->m_left != nullptr && root->m_right != nullptr)
            {
                //checks that the parent root has a value less than that of its children
                pass = pass && (aHeap.m_priorFunc(root->m_crop) >= aHeap.m_priorFunc(root->m_left->m_crop))
                    && (aHeap.m_priorFunc(root->m_crop) >= aHeap.m_priorFunc(root->m_right->m_crop));
            }
            //check if only m_left exists
            else if (root->m_left != nullptr && root->m_right == nullptr)
            {
                pass = pass && (aHeap.m_priorFunc(root->m_crop) >= aHeap.m_priorFunc(root->m_left->m_crop));
            }
            //check if only m_right exists
            else if (root->m_right != nullptr && root->m_left == nullptr)
            {
                pass = pass && (aHeap.m_priorFunc(root->m_crop) >= aHeap.m_priorFunc(root->m_right->m_crop));
            }
            else {
                //do nothing as pass is already starting off as true
            }

        }
        //checks if the children also follow the same heap property
        pass = pass && testHeapProperty(aHeap, root->m_left);
        pass = pass && testHeapProperty(aHeap, root->m_right);

        
        
    }
    return pass;
}



enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL };
class Random {
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL) {
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
    }
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};



int main() {
    
    Tester test;
    IQueue aHeap2(priorityFn2, MAXHEAP);
    IQueue aHeap(priorityFn1, MINHEAP);
    if (test.testHeapTypeInsertion(aHeap))
    {
        cout << "\nInsertion Testing has passedfor minHeap\n";
        
    }
    else
    {
        cout << "\nInsertion Testing has failed for minHeap\n";
    }

    if (test.testHeapTypeInsertion(aHeap2))
    {
        cout << "\nInsertion Testing has passed for maxHeap\n";

    }
    else
    {
        cout << "\nInsertion Testing has failed for maxHeap\n";
    }

    if (test.testHeapTypeDeletion(aHeap))
    {
        cout << "\nDeletion testing has passed for minHeap\n";
    }
    else
    {
        cout << "\nDeletion testing has failed for minHeap\n";
    }
    if (test.testHeapTypeDeletion(aHeap2))
    {
        cout << "\nDeletion testing has passed for maxHeap\n";
    }
    else
    {
        cout << "\nDeletion testing has failed for maxHeap\n";
    }

    if (test.testMerge(aHeap, aHeap2))
    {
        cout << "\nTest merge has passed\n";
    }
    else
    {
        cout << "\nTest merge has failed\n";
    }

    if (test.copyConstructorTest())
    {
        cout << "\ncopy constructor test has passed\n";
    }
    else {
        cout << "\ncopy constructor test has failed\n";
    }

    if (test.checkSetPriority())
    {
        cout << "\nset priority test has passed\n";
    }
    else {
        cout << "\nset priority test has failed\n";
    }
    if (test.assignmentOperatorTest())
    {
        cout << "\nAssignment operator has passed\n";
    }
    else {
        cout << "\nAssignment operator has failed\n";
    }

    if (test.checkPrintAndClear())
    {
        cout << "\nPrint and clear functions have passed\n";
    }
    else
    {
        cout << "\nprint and clear functions have failed\n";
    }
    /*queue1.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the queue with priorityFn1 (MAXHEAP):\n";
    queue1.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue1.printCropsQueue();

    cout << queue1.getNextCrop() << endl;
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue1.printCropsQueue();*/


    return 0;
}

int priorityFn1(const Crop& crop) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [30-116]
    //the highest priority would be 110+6 = 116
    //the lowest priority would be 30+0 = 30
    //the larger value means the higher priority
    int priority = crop.getTemperature() + crop.getType();
    return priority;
}

int priorityFn2(const Crop& crop) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-103]
    //the highest priority would be 0+0 = 0
    //the lowest priority would be 100+3 = 103
    //the smaller value means the higher priority
    int priority = crop.getMoisture() + crop.getTime();
    return priority;
}

bool Tester::testHeapTypeInsertion(IQueue& aHeap)
{
    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);
    bool pass = true;
    
    //IQueue queue1(priorityFn2, MINHEAP);

    cout << "\nclearing the heap\n" << endl;
    aHeap.clear();

    cout << "\nchecking if heap is currently empty\n";
    if (aHeap.isEmpty())
    {
        cout << "\nThe heap is currently empty\n";
    }
    else
    {
        cout << "\nThe heap is not empty\n";
        aHeap.printCropsQueue();
        pass = pass && false;
    }

    //creating a crop to add into the heap
    Crop aCrop(idGen.getRandNum(), temperature, moistureGen.getRandNum()
        , time, typeGen.getRandNum());
    aHeap.insertCrop(aCrop);

    if (aHeap.m_size != 1)
    {
        pass = pass && false;
    }

    cout << "\ndumping queue with only one crop\n";
    aHeap.dump();

    aHeap.clear();

    cout << "\nclearing heap\n";
    if (aHeap.isEmpty())
    {
        cout << "\nThe heap is currently empty\n";
    }
    else
    {
        cout << "\nThe heap is not empty\n";
        aHeap.printCropsQueue();
        pass = pass && false;
    }

    cout << "\ninserting 300 nodes into the heap\n";

    for (int i = 0; i < 300; i++) {
        Crop bCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        aHeap.insertCrop(bCrop);
    }
    cout << "\nDump of the queue with priorityFn2 (MAXHEAP):\n";
    aHeap.dump();

    cout << "\nPrinting out Heap\n";

    aHeap.printCropsQueue();

    if (testHeapProperty(aHeap, aHeap.m_heap)) {
        cout << "\nTest property has been passed\n";
    }
    else
    {
        cout << "\nTest property has failed\n";
        pass = pass && false;
    }
    return pass;
}


bool Tester::testHeapTypeDeletion(IQueue& aHeap)
{
    bool pass = true;
    cout << "\nTesting the remove function\n";

    int heapSize = aHeap.m_size;

    cout << "\nThere are " << heapSize << " crops in the heap\n";

    aHeap.getNextCrop();

    int heapSize2 = aHeap.m_size;
    

    cout << "\nThere are " << heapSize2 << " crops in the heap\n";

    cout << "\nDump of heap\n";

    aHeap.dump();

    //checks if the heapsize has decreased since the removal of the top node
    if (heapSize2 >= heapSize)
    {
        pass = pass && false;
    }
    
    if (testHeapProperty(aHeap, aHeap.m_heap)) {
        cout << "\nTest property has been passed\n";
    }
    else
    {
        cout << "\nTest property has failed\n";
        pass = pass && false;
    }

    cout << "\nclearing the heap\n";

    aHeap.clear();
    if (aHeap.isEmpty())
    {
        cout << "\nThe heap is currently empty\n";
    }
    else
    {
        cout << "\nThe heap is not empty\n";
        aHeap.printCropsQueue();
        pass = pass && false;
    }

    try
    {
        cout << "\nattempting to get crop from an empty heap\n";
        aHeap.getNextCrop();
    }
    catch (out_of_range& e)
    {
        cerr << e.what() << endl;
    }

    return pass;
}

bool Tester::testMerge(IQueue& aHeap, IQueue& bHeap)
{
    bool pass = true;

    cout << "\ntesting merge heap functionality\n";
    aHeap.clear();
    bHeap.clear();

    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);

    IQueue cHeap(aHeap.m_priorFunc, aHeap.m_heapType);

    cout << "\nInserting 10 nodes into two heaps\n";
    for (int i = 0; i < 10; i++) {
        Crop aCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        aHeap.insertCrop(aCrop);
    }

    for (int i = 0; i < 10; i++) {
        Crop bCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        bHeap.insertCrop(bCrop);
        cHeap.insertCrop(bCrop);
    }




    cout << "\nPrinting of heap1's nodes\n";
    aHeap.printCropsQueue();

    cout << "\nPrinting of heap2's nodes\n";
    bHeap.printCropsQueue();

    try {
       
        cout << "\nclearing one heap\n";
        aHeap.clear();
        if (aHeap.isEmpty())
        {
            cout << "\nThe first heap is currently empty\n";
        }
        else
        {
            cout << "\nThe first heap is not empty\n";
            aHeap.printCropsQueue();
            pass = pass && false;
        }
        //merging a non empty queue to an empty queue
        aHeap.mergeWithQueue(cHeap);

        if (!aHeap.isEmpty() && cHeap.isEmpty())
        {
            cout << "\nMerging an unempty queue to an empty queue has passed\n";
        }
        else
        {
            pass = pass && false;
        }

        cout << "\nPrinting of heap1's nodes\n";
        aHeap.printCropsQueue();

        cout << "\nPrinting of heap3's nodes (heap3 has same priority as heap1)\n";
        aHeap.printCropsQueue();

        //merging an empty queue to a non empty queue
        aHeap.mergeWithQueue(cHeap);

        if (!aHeap.isEmpty() && cHeap.isEmpty())
        {
            cout << "\nMerging an empty queue to an unempty queue has passed\n";
        }
        else
        {
            pass = pass && false;
        }

        cout << "\nPrinting of heap1's nodes\n";
        aHeap.printCropsQueue();

        cout << "\nPrinting of heap3's nodes\n";
        cHeap.printCropsQueue();


        aHeap.clear();

        //merging an empty queue to another empty queue
        aHeap.mergeWithQueue(cHeap);

       
        if (aHeap.isEmpty() && cHeap.isEmpty())
        {
            cout << "\nMerging two empty queues has passed\n";
        }
        else
        {
            pass = pass && false;
        }
        
        Crop cropA(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());

        Crop cropB(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());

        aHeap.insertCrop(cropA);
        cHeap.insertCrop(cropB);

        cout << "\nPrinting of heap1's nodes\n";
        aHeap.printCropsQueue();

        cout << "\nPrinting of heap3's nodes\n";
        cHeap.printCropsQueue();

        aHeap.mergeWithQueue(cHeap);
        if (!aHeap.isEmpty() && cHeap.isEmpty())
        {
            cout << "\nMerging two non empty queues has passed\n";
        }
        else
        {
            pass = pass && false;
        }

        cout << "\nPrinting of heap1's nodes\n";
        aHeap.printCropsQueue();
        int sizeHold = aHeap.m_size;
        aHeap.mergeWithQueue(aHeap);
        int sizeHold2 = aHeap.m_size;

        if (sizeHold == sizeHold2) {
            cout << "\nattemping to self merge test has passed (did not self merge)\n";
        }
        else {
            pass = pass && false;
        }

        cout << "\nPrinting of heap1's nodes\n";
        aHeap.printCropsQueue();

        cout << "\nattempting to merge nodes with different priorities (heap1 and heap2)\n";
        aHeap.mergeWithQueue(bHeap);
    }

    catch (domain_error& e)
    {
        cerr << "\n" << e.what() << endl;
    }
    return pass;
}
    //this function will check if both nodes share the same data (not pointers)
bool Tester::checkIfEqual(Node* root1, Node* root2) {
    bool pass = false;
    //Check if both nodes are nullptr
    if (root1 == nullptr && root2 == nullptr)
    {
        pass = true;
    }
    //checks if all crop data are equal
    if (root1 != nullptr && root2 != nullptr) {
        if ((root1->m_crop.getTime() == root2->m_crop.getTime()) && (root1->m_crop.getCropID()
            == root2->m_crop.getCropID()) && (root1->m_crop.getMoisture() == root2->m_crop.getMoisture())
            && (root1->m_crop.getTemperature() == root2->m_crop.getTemperature()) &&
            (root1->m_crop.getType() == root2->m_crop.getType()))
        {
            pass = true;
        }
        //checks the children of both nodes and returns their truth value
        return (checkIfEqual(root1->m_left, root2->m_left));
        return (checkIfEqual(root1->m_right, root2->m_right));
    }
    return pass;
}


bool Tester::checkIfEqualPointers(Node* root1, Node* root2)
{
    bool pass = false;
    //checks if both nodes are nullptr
    if (root1 == nullptr && root2 == nullptr)
    {
        //does nothing as while both would be true, it would fail my other test
        //of not having the pointers be equal
    }
    if (root1 != nullptr && root2 != nullptr)
    {
        if (root1 == root2)
        {
            pass = true;
        }
        return checkIfEqualPointers(root1->m_left, root2->m_left);
        return checkIfEqualPointers(root1->m_right, root2->m_right);
    }
    return pass;
}

//tests the copy constructor
bool Tester::copyConstructorTest() {
    cout << "\nTesting the copy constructor\n";
    bool pass = true;
    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);

    //creates two IQueue objects
    IQueue a(priorityFn1, MAXHEAP);

    //inserts 10 nodes into both heaps
    for (int i = 0; i < 10; i++) {
        Crop aCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        a.insertCrop(aCrop);
    }

        //creating a new IQueue object (c) and assigning a to it
        cout << "\nassigning a heap to another heap\n";
        IQueue c(a);

        cout << "\nchecking properties of the new heap\n";

        if (testHeapProperty(a, a.m_heap))
        {
            cout << "\nFirst heap passes the heap property\n";
        }
        else
        {
            cout << "\nFirst heap does not pass the heap property\n";
            pass = pass && false;
        }

        if (testHeapProperty(c, c.m_heap))
        {
            cout << "\nsecond heap passes the heap property\n";
        }
        else
        {
            cout << "\nsecond heap does not pass the heap property\n";
            pass = pass && false;
        }

        if (c.m_heapType != a.m_heapType)
        {
            cout << "\nheap types are different\n";
            pass = pass && false;
        }

        if (c.m_priorFunc != a.m_priorFunc)
        {
            cout << "\npriorities are different\n";
            pass = pass && false;
        }
        if (c.m_size != a.m_size)
        {
            cout << "\nsizes are different\n";
            cout << "\nfirst heap's size :" << a.m_size;
            cout << "\nsecond heaps size: " << c.m_size;
            pass = pass && false;
        }
        //updates the pass truth value
        if (!checkIfEqual(a.m_heap, c.m_heap))
        {
            cout << "\nThe heaps are not equal\n";
            pass = pass && false;
        }
       

        //checks if the pointers are equal (they should not be equal)
        if (checkIfEqualPointers(a.m_heap, c.m_heap))
        {
            cout << "\nThe pointers are the same\n";
            pass = pass && false;
        }

        cout << "\nprint out of heap a\n";
        a.printCropsQueue();
        a.dump();

        cout << "\nprint out of heap c\n";
        c.printCropsQueue();
        c.dump();

       
    return pass;
}

bool Tester::checkSetPriority()
{
    bool pass = true;
    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);

    //creates two IQueue objects
    IQueue a(priorityFn1, MAXHEAP);

    //inserts 10 nodes into both heaps
    for (int i = 0; i < 300; i++) {
        Crop aCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        a.insertCrop(aCrop);
    }

    
    if (testHeapProperty(a, a.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }
    a.dump();

    //changing the priority of the heap

    a.setPriorityFn(priorityFn1, MINHEAP);
    if (testHeapProperty(a, a.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }
    a.dump();

    a.setPriorityFn(priorityFn2, MINHEAP);
    if (testHeapProperty(a, a.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }
    a.dump();

    return pass;
}

bool Tester::assignmentOperatorTest()
{
    cout << "\nTesting the assignment operator\n";
    bool pass = true;
    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);

    //creates two IQueue objects
    IQueue a(priorityFn1, MAXHEAP);
    IQueue b(priorityFn2, MINHEAP);

    //inserts 10 nodes into both heaps
    for (int i = 0; i < 10; i++) {
        Crop aCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        a.insertCrop(aCrop);
    }
    for (int i = 0; i < 10; i++) {
        Crop bCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        b.insertCrop(bCrop);
    }

    
    
    cout << "\nprint out of heap a\n";
    a.printCropsQueue();
    a.dump();

    cout << "\nprint out of heap b\n";
    b.printCropsQueue();
    b.dump();

    //creating a new IQueue object (c) and assigning a to it
    cout << "\nassigning a heap to another heap\n";
    b = a;
    cout << "\nchecking properties of the new heap\n";

    if (testHeapProperty(a, a.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (testHeapProperty(b, b.m_heap))
    {
        cout << "\nsecond heap passes the heap property\n";
    }
    else
    {
        cout << "\nsecond heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (b.m_heapType != a.m_heapType)
    {
        cout << "\nheap types are different\n";
        pass = pass && false;
    }

    if (b.m_priorFunc != a.m_priorFunc)
    {
        cout << "\npriorities are different\n";
        pass = pass && false;
    }
    if (b.m_size != a.m_size)
    {
        cout << "\nsizes are different\n";
        cout << "\nfirst heap's size :" << a.m_size;
        cout << "\nsecond heaps size: " << b.m_size;
        pass = pass && false;
    }
    //updates the pass truth value
    if (!checkIfEqual(a.m_heap, b.m_heap))
    {
        cout << "\nThe heaps are not equal\n";
        pass = pass && false;
    }


    //checks if the pointers are equal (they should not be equal)
    if (checkIfEqualPointers(a.m_heap, b.m_heap))
    {
        cout << "\nThe pointers are the same\n";
        pass = pass && false;
    }

    cout << "\nprint out of heap a\n";
    a.printCropsQueue();
    a.dump();

    cout << "\nprint out of heap b\n";
    b.printCropsQueue();
    b.dump();

    //assigning an empty heap to a non empty heap

    IQueue c(priorityFn1, MINHEAP);

    b = c;

    if (testHeapProperty(c, c.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (testHeapProperty(b, b.m_heap))
    {
        cout << "\nsecond heap passes the heap property\n";
    }
    else
    {
        cout << "\nsecond heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (b.m_heapType != c.m_heapType)
    {
        cout << "\nheap types are different\n";
        pass = pass && false;
    }

    if (b.m_priorFunc != c.m_priorFunc)
    {
        cout << "\npriorities are different\n";
        pass = pass && false;
    }
    if (b.m_size != c.m_size)
    {
        cout << "\nsizes are different\n";
        cout << "\nfirst heap's size :" << c.m_size;
        cout << "\nsecond heaps size: " << b.m_size;
        pass = pass && false;
    }
    //updates the pass truth value
    if (!checkIfEqual(c.m_heap, b.m_heap))
    {
        cout << "\nThe heaps are not equal\n";
        pass = pass && false;
    }


    //checks if the pointers are equal (they should not be equal)
    if (checkIfEqualPointers(c.m_heap, b.m_heap))
    {
        cout << "\nThe pointers are the same\n";
        pass = pass && false;
    }

    cout << "\nprint out of heap c\n";
    c.printCropsQueue();
    c.dump();

    cout << "\nprint out of heap b\n";
    b.printCropsQueue();
    b.dump();

    //assigning an IQueue with only one node to an empty IQueue

    IQueue d(priorityFn1, MAXHEAP);
    Crop dCrop(idGen.getRandNum(),
        temperature,// it is the same in the region
        moistureGen.getRandNum(),
        time,       // it is the same in the region
        typeGen.getRandNum());
    d.insertCrop(dCrop);

    c = d;

    if (testHeapProperty(d, d.m_heap))
    {
        cout << "\nFirst heap passes the heap property\n";
    }
    else
    {
        cout << "\nFirst heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (testHeapProperty(c, c.m_heap))
    {
        cout << "\nsecond heap passes the heap property\n";
    }
    else
    {
        cout << "\nsecond heap does not pass the heap property\n";
        pass = pass && false;
    }

    if (c.m_heapType != d.m_heapType)
    {
        cout << "\nheap types are different\n";
        pass = pass && false;
    }

    if (c.m_priorFunc != d.m_priorFunc)
    {
        cout << "\npriorities are different\n";
        pass = pass && false;
    }
    if (c.m_size != d.m_size)
    {
        cout << "\nsizes are different\n";
        cout << "\nfirst heap's size :" << d.m_size;
        cout << "\nsecond heaps size: " << c.m_size;
        pass = pass && false;
    }
    //updates the pass truth value
    if (!checkIfEqual(d.m_heap, c.m_heap))
    {
        cout << "\nThe heaps are not equal\n";
        pass = pass && false;
    }


    //checks if the pointers are equal (they should not be equal)
    if (checkIfEqualPointers(d.m_heap, c.m_heap))
    {
        cout << "\nThe pointers are the same\n";
        pass = pass && false;
    }

    cout << "\nprint out of heap d\n";
    d.printCropsQueue();
    d.dump();

    cout << "\nprint out of heap c\n";
    c.printCropsQueue();
    c.dump();


    return pass;

}
//we will be testing print and clear in this function
bool Tester::checkPrintAndClear()
{
    bool pass = true;
    Random idGen(MINCROPID, MAXCROPID);
    Random temperatureGen(MINTEMP, MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE, MAXMOISTURE);
    Random timeGen(MINTIME, MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE, MAXTYPE);
    cout << "\nwe will be testing print and clear\n";

    IQueue d(priorityFn1, MAXHEAP);
    Crop dCrop(idGen.getRandNum(),
        temperature,// it is the same in the region
        moistureGen.getRandNum(),
        time,       // it is the same in the region
        typeGen.getRandNum());
    d.insertCrop(dCrop);

    cout << "\nprint of heap d\n";
    d.printCropsQueue();

    int sizeStorage = d.m_size;
    if (!d.isEmpty() && sizeStorage != 0)
    {
        cout << "\nheap is currently not empty\n";
    }
    else
    {
        pass = pass && false;
    }

    d.clear();

    if (d.isEmpty())
    {
        cout << "\nheap is now empty\n";
    }
    else
    {
        pass = pass && false;
    }
    d.printCropsQueue();

    for (int i = 0; i < 10; i++)
    {
        Crop dCrop(idGen.getRandNum(),
            temperature,// it is the same in the region
            moistureGen.getRandNum(),
            time,       // it is the same in the region
            typeGen.getRandNum());
        d.insertCrop(dCrop);
    }

    sizeStorage = d.m_size;

    cout << "\n heap after entering 10 nodes\n";
    d.printCropsQueue();

    d.clear();

    if (d.isEmpty())
    {
        cout << "\nheap is now empty\n";
    }
    else
    {
        pass = pass && false;
    }
    d.printCropsQueue();

    d.clear();
    if (d.isEmpty())
    {
        cout << "\nheap is still empty\n";
    }
    else
    {
        pass = pass && false;
    }
    d.printCropsQueue();

    return pass;
}
