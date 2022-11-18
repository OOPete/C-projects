// CMSC 341 - Fall 2022 - Project 3
#include "iqueue.h"
IQueue::IQueue(prifn_t priFn, HEAPTYPE heapType)
{
    //initializes the member variables
    //sets the type of heap whether minimum or maximum
    m_heapType = heapType;
    //sets the priority of the function
    m_priorFunc = priFn;
    m_heap = nullptr;
    m_size = 0;
}
//destructor for the IQueue class
IQueue::~IQueue()
{
    clear();
    m_size = 0;
}
//save this till after the insert is done
IQueue::IQueue(const IQueue& rhs)
{
    //recursive function that inserts the nodes from IQueue to the current heap
    m_heapType = rhs.m_heapType;
    m_priorFunc = rhs.m_priorFunc;
    m_heap = copyConstructRecur(rhs.m_heap);
    m_size = rhs.m_size;
    
}
//save till after the insert is done
IQueue& IQueue::operator=(const IQueue& rhs)
{
    if (this != &rhs)
    {
        clear();
        // recursive function that inserts the nodes from IQueue to the current heap
        m_heapType = rhs.m_heapType;
        m_priorFunc = rhs.m_priorFunc;
        m_heap = copyConstructRecur(rhs.m_heap);
        m_size = rhs.m_size;
    }
    return *this;
}

void IQueue::insertCrop(const Crop& crop) {
    Node* newNode = new Node(crop);
    //check if the queue is empty
    if (isEmpty())
    {  
        m_heap = newNode;
    }
    else
    {
        //creates an IQueue object and stores the new node inside of it
        //then merges the new IQueue with the current IQueue
        IQueue rhsObj(m_priorFunc, m_heapType);
        rhsObj.m_heap = newNode;
        mergeWithQueue(rhsObj);
    }
    m_size++;
}

//acts as the removal function
Crop IQueue::getNextCrop() {
    if (isEmpty())
    {
        throw out_of_range("Queue is empty");
    }
    //create a temp node to store the root of the current tree
    //create a second IQueue to store the right child of the current heap
    //make the left child of the current heap the new root
    //merge the right child with the new current heap
    //return the previous heap root
    Crop cropObj = m_heap->getCrop();

    Node* temp = m_heap->m_left;
    Node* temp2 = m_heap->m_right;
    delete m_heap;
    m_heap = temp;
    IQueue rhsObj(m_priorFunc, m_heapType);
    rhsObj.m_heap = temp2;
    mergeWithQueue(rhsObj);
    m_size--;
    return cropObj;
}

void IQueue::mergeWithQueue(IQueue& rhs) {
    //checks if both queues have the same priority
    if (m_priorFunc != rhs.m_priorFunc)
    {
        throw domain_error("Passed in queue has a different priority");
    }
    
    //if the heaps aren't dupes
    if (m_heap != rhs.m_heap)
    {
        //checks if its a min heap type
        if (m_heapType == MINHEAP)
        {
            m_heap = mergeQueueMin(m_heap, rhs.m_heap);
       }
        //checks if it's a max heap type
        else if (m_heapType == MAXHEAP)
        {
            m_heap = mergeQueueMax(m_heap, rhs.m_heap);
        }
        //empties the rhs heap
        rhs.m_heap = nullptr;
    }
    
}

//clears the heap
void IQueue::clear() {
    m_heap = clearRecur(m_heap);
    m_heap = nullptr;
}

int IQueue::numCrops() const
{
    return m_size;
}

void IQueue::printCropsQueue() const {
    printRecur(m_heap);
}

prifn_t IQueue::getPriorityFn() const {
    return m_priorFunc;
}

//changes the priority of the heap and rebuilds it
void IQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_heapType = heapType; //changes the heap type
    m_priorFunc = priFn; //changes the function priority
    //creating a new IQueue object to store the new priority
    IQueue newHeap(priFn, heapType);
    
    //changes the priority by readding the nodes from the current heap to the new heap, this
    //will add the nodes in the order of the new priority
    newHeap.m_heap = priorityChange(m_heap, newHeap);
    //clears the current heap
    clear();
    //sets the current heap to that of the new heap
    m_heap = newHeap.m_heap;
    //sets the new heap to nullptr
    newHeap.m_heap = nullptr;
}

void IQueue::dump() const
{
    if (m_size == 0) {
        cout << "Empty skew heap.\n";
    }
    else {
        dump(m_heap);
        cout << endl;
    }
}

void IQueue::dump(Node* pos) const {
    if (pos != nullptr) {
        cout << "(";
        dump(pos->m_left);
        cout << m_priorFunc(pos->m_crop) << ":" << pos->m_crop.getCropID();
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Crop& crop) {
    sout << "Crop ID: " << crop.getCropID()
        << ", current temperature: " << crop.getTemperature()
        << ", current soil moisture: " << crop.getMoisture() << "%"
        << ", current time: " << crop.getTimeString()
        << ", plant type: " << crop.getTypeString();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getCrop();
    return sout;
}

//RECURSIVE FUNCTIONS BELOW
// 
// 
//recursively clears the tree
Node* IQueue::clearRecur(Node* root)
{
    //deletes each node the binary tree from left to right and then root
    if (root != nullptr)
    {
        clearRecur(root->m_left);
        clearRecur(root->m_right);
        delete root;
        root = nullptr;
    }
    return root;
}

//using the min priority, it merges the nodes of the queues
Node* IQueue::mergeQueueMin(Node* heap1, Node* heap2)
{
    //check if the current heap is empty and adds heap2 into the current heap (heap1)
    if (heap1 == nullptr)
    {
        //makes m_heap the value of the rhs heap
        //empties the rhs heap
        return heap2;
    }
    //checks if the rhs heap is empty (heap2)
    else if (heap2 == nullptr)
    {
        return heap1;
    }
    else if (heap1 == nullptr && heap2 == nullptr)
    {
        return nullptr;
    }
    //checks if it is a min type heap
   
    //checks if the priority type of the current heap is less than or equal to
    //that of the rhs heap
    if (m_priorFunc(heap1->m_crop) <= m_priorFunc(heap2->m_crop))
     {
     //swap the left and right children of the current heap
        Node* temp = heap1->m_right;
        heap1->m_right = heap1->m_left;
        heap1->m_left = mergeQueueMin(heap2, temp);
        return heap1;
     }
     else
        return mergeQueueMin(heap2, heap1);

    return 0;
}
//using the max priority, it merges the nodes of the queues
Node* IQueue::mergeQueueMax(Node* heap1, Node* heap2)
{
    //check if the current heap is empty and adds heap2 into the current heap (heap1)
    if (heap1 == nullptr)
    {
        //makes m_heap the value of the rhs heap
        //empties the rhs heap
        return heap2;
    }
    //checks if the rhs heap is empty (heap2)
    else if (heap2 == nullptr)
    {
        return heap1;
    }
    else if (heap1 == nullptr && heap2 == nullptr)
    {
        return nullptr;
    }
   //if heap1 is greater than or equal to heap 2
    if (m_priorFunc(heap1->m_crop) >= m_priorFunc(heap2->m_crop))
    {
        
        //swap the left and right nodes of heap1
        Node* temp = heap1->m_right;
        heap1->m_right = heap1->m_left;
        heap1->m_left = mergeQueueMax(heap2, temp);
        return heap1;
    }
    else
        return mergeQueueMax(heap2, heap1);

    return 0;
}
//recursive function for the copy constructor and assignment operator
Node* IQueue::copyConstructRecur(Node* rhsHeap)
{
    if (rhsHeap != nullptr)
    {
        Node* newNode = new Node(rhsHeap->m_crop);
        newNode->m_left = copyConstructRecur(rhsHeap->m_left);
        newNode->m_right = copyConstructRecur(rhsHeap->m_right);
        return newNode;
         
    }
    return nullptr;
}
//recursively prints out the crops in the heap
void IQueue::printRecur(Node* root) const
{
    if (root != nullptr)
    {
        cout << root->getCrop() << endl;
        printRecur(root->m_left);
        printRecur(root->m_right);

    }
}


//recursively changes the priority of the heap
Node* IQueue::priorityChange(Node* currHeap,IQueue& heap1)
{
    if (currHeap != nullptr)
    {
        //inserts the crop from the current heap to the other heap  
        heap1.insertCrop(currHeap->getCrop());
         priorityChange(currHeap->m_left, heap1);
         priorityChange(currHeap->m_right, heap1);
       
        
    }
    return heap1.m_heap;
}
