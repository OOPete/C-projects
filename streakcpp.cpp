// UMBC - CMSC 341 - Fall 2022 - Proj2
#include "streak.h"

//sets the root to nullptr at the start of the function
//copy constructor
Streak::Streak() {
    m_root = nullptr;
}
//deallocates the entire tree
Streak::~Streak() {
    clear();
    
}

//inserts a node into the BST
void Streak::insert(const Tiger& tiger) {
  
    if (!findTiger(tiger.getID()))
    {
        if ((tiger.getID() >= MINID) && (tiger.getID() <= MAXID))
        {
            m_root = insertRecur(m_root, tiger.getID(), tiger.getAge(), tiger.getState(), tiger.getGender());
            
            
        }
    }
   
    
}

//recursive function for the insert function
Tiger* Streak::insertRecur(Tiger* root, int id, AGE age, STATE state, GENDER gender)
{
    //creates a new Tiger object
    //base case
    if (root == nullptr)
    {
        //create a new node and make it root
         Tiger* newNode = new Tiger(id);
         newNode->setAge(age);
         newNode->setGender(gender);
         root = newNode;
         root->setState(state);
         updateHeight(root);

         
    }
    else {
        //traverses to the left side of the tree
        if (id < root->getID())
        {
            //updates m_left's nodes and the height
            root->m_left = insertRecur(root->m_left, id, age, state, gender);
            updateHeight(root->m_left);
            
            
        }
        //traverses to the right child of the tree
        else if ( id > root->getID())
        {
            
            root->m_right = insertRecur(root->m_right, id, age, state, gender); 
            updateHeight(root->m_right);
            
        }
        //updates the height of the node and rebalances the root
        
    }
    
    updateHeight(root);
    root = rebalance(root);
    return root;
}

//used to clear the entire tree
void Streak::clear() {
    m_root = clearRecur(m_root);
}
//recursive function for the clear function
Tiger* Streak::clearRecur(Tiger* root)
{
    if (root != nullptr)//base case, checks if the root is nullptr
    {
        //goes into the left subtree
        clearRecur(root->m_left);
        //goes into the right subtree
        clearRecur(root->m_right);
        //deletes the node and set's it to nullptr;
        delete root;
        root = nullptr;
    }
    return root;
}
//removes a node if found in the tree
void Streak::remove(int id) {
    if (findTiger(id)) {
        
        m_root = removeRecur(m_root, id);
    }
    else
    {
        
    }
    
}
//recursive function for the remove function
Tiger* Streak::removeRecur(Tiger* root, const int id)
{
    //returns the node if it's a nullptr
    if (root == nullptr)
    {
        return root;
    }
    //enters the left subtree if the id is less than that of the current root
    else if (id < root->m_id)
    {
        root->m_left = removeRecur(root->m_left, id);
        updateHeight(root->m_left);
    }
    //enters the right subtree
    else if (id > root->m_id)
    {
        root->m_right = removeRecur(root->m_right, id);
        updateHeight(root->m_right);
    }
    else if (id == root->m_id) {
        //if the node has two children
        if (root->m_left != nullptr && root->m_right != nullptr)
        {
            //pass in m_right to find the smallest node in the right side of the subtree
            Tiger* temp = findMin(root->m_right);
            //swap values with the smallest node in the right
            root->m_id = temp->m_id;
            root->m_age = temp->m_age;
            root->m_gender = temp->m_gender;
            root->m_state = temp->m_state;

            //looks for the node with the id in the right(sends it on an adventure)
            root->m_right = removeRecur(root->m_right, temp->m_id);
        }
        else
        {
            //case of 0 or 1 children
            Tiger* temp = root;
            
            //if the root only has a right node
            if (root->m_left == nullptr)
            {
                root = root->m_right;
            }
            //if it has a right node
            else
            {
                root = root->m_left;
            
            }
            //deletes the node
            delete temp;
        }
    }
    //updates the height of the node and rebalances the node if needed
    updateHeight(root);
    root = rebalance(root);
    return root;
}
//finds the smallest node on the right side
Tiger* Streak::findMin(Tiger* root)//we pass in the right child
{
    //will return nullptr if the root is nullptr or return the value of the root
    //if it's m_left is nullptr
    if (root == nullptr || root->m_left == nullptr)
    {
        return root;
    }
    //returns the next left object till it reaches nullptr or the smallest id
    else
     return findMin(root->m_left);
    
}

//updates the height of the node
void Streak::updateHeight(Tiger* aTiger) {

    if (aTiger != nullptr) {
        //updates the height if neither child is a nullptr
        if ((aTiger->m_left != nullptr) && (aTiger->m_right != nullptr)) {
            int leftHeight = aTiger->m_left->m_height;
            int rightHeight = aTiger->m_right->m_height;
            aTiger->m_height = 1 + maxHeight(leftHeight, rightHeight);
        }
        //if it only has a left node
        else if ((aTiger->m_left != nullptr) && (aTiger->m_right == nullptr))
        {
            aTiger->m_height = 1 + aTiger->m_left->m_height;
        }
        //if it only has a right node
        else if ((aTiger->m_left == nullptr) && (aTiger->m_right != nullptr))
        {
            aTiger->m_height = 1 + aTiger->m_right->m_height;
        }
        //if this node is a leaf set it's height to 0
        else if ((aTiger->m_right == nullptr) && (aTiger->m_left == nullptr))
        {
            aTiger->m_height = 0;
        }
    }
}

int Streak::checkImbalance(Tiger* aTiger) {
    //checks the differences between the heights of the left and right node
    //then returns it
    if (aTiger != nullptr) {
        //if neither nodes are nullptr
        if ((aTiger->m_left != nullptr) && (aTiger->m_right != nullptr)) {

            int checkBal = aTiger->m_left->m_height - aTiger->m_right->m_height;
            return checkBal;
        }
        //if the left right node is nullptr return left
        if (aTiger->m_left != nullptr && aTiger->m_right == nullptr)
        {

            return aTiger->m_left->m_height - (-1);
        }
        //if the left node is nullptr return right
        else if (aTiger->m_right != nullptr && aTiger->m_left == nullptr)
        {

            return ((aTiger->m_right->m_height) - (-1)) * -1;
        }
        //if both nodes are nullptr
        else if (aTiger->m_right == nullptr && aTiger->m_left == nullptr)
        {
            return (-1) - (-1);
        }
    }
    return 0;
}

//rebalances the node with a balance factor greater or less than 1 and -1
Tiger* Streak::rebalance(Tiger* aTiger) {
    //checks if the passed in node is a nullptr
    if (aTiger != nullptr) {
        
        //receives the balance factor of the tiger object
        int balanceFact = checkImbalance(aTiger);
        //checks if the left child is imbalanced
        
        if (balanceFact == 2)//2
        {
            
            if (checkImbalance(aTiger->m_left) > 0)//1
            {
                
                //rotates the subtree to the left
                aTiger = rotateRight(aTiger);
            }
            else if (checkImbalance(aTiger->m_left) == 0)
            {
                
                aTiger = rotateRight(aTiger);
            }
            //if the right side of the left child is longer rotate left right
            else if (checkImbalance(aTiger->m_left) < 0) { //-1
                
                aTiger = rotateLeftRight(aTiger);
            }

        }
        //checks if the right child is the longest
        else if (balanceFact == -2) //-2
        {
            
            if (checkImbalance(aTiger->m_right) > 0) //1
            {
                
                aTiger = rotateRightLeft(aTiger);
            }
            //checks if the right side has a balance factor of less than 0
            else if (checkImbalance(aTiger->m_right) < 0) //-1
            {
                
                aTiger = rotateLeft(aTiger);
            }
            else if (checkImbalance(aTiger->m_right) == 0)
            {
                
                aTiger = rotateLeft(aTiger);
            }
        }
        //here we will update the height of the balance function
        updateHeight(aTiger->m_left);
        updateHeight(aTiger->m_right);
        
    }

    updateHeight(aTiger);
  
     
    //returns the passed in object
    return aTiger;
    
}

void Streak::dumpTree() const { dump(m_root); }

void Streak::dump(Tiger* aTiger) const {
    if (aTiger != nullptr) {
        cout << "(";
        dump(aTiger->m_left);//first visit the left child
        cout << aTiger->m_id << ":" << aTiger->m_height;//second visit the node itself
        dump(aTiger->m_right);//third visit the right child
        cout << ")";
    }
}

void Streak::listTigers() const {
    inOrder(m_root);
}

void Streak::inOrder(Tiger* root) const
{
    if (root != nullptr)
    {
        //in order listing
        
        inOrder(root->m_left);
        cout << root->getID() << ":" << root->getAgeStr() << ":" << root->getGenderStr() << ":" << root->getStateStr() << endl;
        inOrder(root->m_right);
    }
}
//sets the state of the node
bool Streak::setState(int id, STATE state) {
    if (findTiger(id))
    {
        return setStateRecur(id, m_root, state);
    }
    return false;
}
//recursive function of setState
bool Streak::setStateRecur(int id, Tiger* root, STATE state)
{
    //if the tiger is not found, return false
    //else check the left and right subtree and return true when found
    
    if (root != nullptr)
    {
        //checks the left child
        if (id < root->getID())
        {
            setStateRecur(id, root->m_left, state);
        }
        //checks the right child
        if (id > root->getID())
        {
            setStateRecur(id, root->m_right, state);
        }
        //sets the state if found
        else
        {
            root->setState(state);
            return true;
        }
    }
    return false;
}

//removes all dead Tigers in the tree
void Streak::removeDead() {
    int id = findDead(m_root);
    while (id != -1)
    {
        remove(id);
        id = findDead(m_root);
    }
}
//used to find the dead tigers in the tree
int Streak::findDead(Tiger* root)
{
    if (root == nullptr)
    {
        return -1;
    }
    else
    {
        if (root->getState() == DEAD)
        {
            return root->m_id;
        }
        int left = findDead(root->m_left);
        int right = findDead(root->m_right);

        //if it hasn't find any dead nodes, it returns those values
        if (left != -1)
        {
            return left;
        }
        if (right != -1)
        {
            return right;
        }
       
    }
    return -1;
}

//recursive function for removeDead
void Streak::removeDeadRecur(Tiger* root)
{
    if (root != nullptr)
    {
        if (root->m_state == DEAD) {
            remove(root->m_id);
            removeDeadRecur(m_root);
        }
        
        removeDeadRecur(root->m_left);
        //traverses through the left and right subtrees
        //removes any node with a dead state
       removeDeadRecur(root->m_right); 
       
       
       
    } 
    
}

//returns true or false if a tiger is found or not
bool Streak::findTiger(int id) const {
    
    return isFound(id, m_root);
}

//counts the tiger cubs and returns a value
int Streak::countTigerCubs() const {
    int count = 0;
    countCubs(m_root, count);
    return count;
}
//recursive function of countTigerCubs
void Streak::countCubs(Tiger* root,int& count) const {
    
    if (root != nullptr)
    {
        //counter integer that stores the value of the current count
       
        countCubs(root->m_left, count);
        if (root->m_age == CUB)
        {
            
            //increments counter
            count++;
        }
        countCubs(root->m_right, count);
        //traverses the left and right side of the tree
        
        
      
    }
   
}
//returns the highest number
int Streak::maxHeight(int obj1, int obj2)
{
    int max = 0;
    //if object 1 is greater than object 2
    if (obj1 > obj2)
    {
        max = obj1;
    }
    //if object 2 is greater than object 1
    else if (obj2 > obj1)
    {
        max = obj2;
    }
    //if both are equal, return any of them
    else  if (obj1 == obj2){
        max = obj1;
    }
    return max;
}

//rotates the node to the left
Tiger* Streak::rotateLeft(Tiger* root)
{
   
    //create a Tiger pointer z to point to the passed in tiger pointer
    Tiger* z = root;
    //creates a Tiger pointer y that is the right child of z
    Tiger* y = z->m_right;
    //z's m_right will now point to y's m_left
    z->m_right = y->m_left;
    //y's new m_left will now point to z
    y->m_left = z;
    updateHeight(z);
    updateHeight(y);
    //we will now return y
    return y;
    
}
//similar to rotateLeft but to the right
Tiger* Streak::rotateRight(Tiger* root)
{
    //make z the root
    Tiger* z = root;
    //make y the left of z
    Tiger* y = z->m_left;
    //set the left of z to the right of right
    z->m_left = y->m_right;
    //set right of y to z
    y->m_right = z;
    updateHeight(z);
    updateHeight(y);
    
    return y;
}
//rotates the node and it's child 
Tiger* Streak::rotateLeftRight(Tiger* root)
{
    
    //create a tiger object and set it as the passed in object
    Tiger* z = root;
    //create a tiger object that points to the left side of the passed in object
    Tiger* y = z->m_left;
    Tiger* x = y->m_right;
    //rotate the left side of the passed in object
    y->m_right = x->m_left;
    x->m_left = y;
    z->m_left = x;
    z->m_left = x->m_right;
    x->m_right = z;
    updateHeight(z);
    updateHeight(y);
    updateHeight(x);
    
    return x;

}
//function used to check if an id already exists
bool Streak::isFound(int id, Tiger* root) const
{
    //if the root isn't nullptr
    if (root != nullptr)
    {
        //base case
        if (root->m_id == id)
        {
            return true;
        }
        //go through the left
        if (id < root->m_id) {


            return isFound(id, root->m_left);
        }
        
        else if (id > root->m_id) {
            //go through the right side
            return isFound(id, root->m_right);
        }

    }
    
    return false;
}
//rotates the subtree to the right
Tiger* Streak::rotateRightLeft(Tiger* root)
{
    //z is the passed in root
    Tiger* z = root;
    //y is the right child of z
    Tiger* y = z->m_right;
    //x is the left child of y
    Tiger* x = y->m_left;
    //set the left child of y to the right child of x
    y->m_left = x->m_right;
    //set y as the right child of x
    x->m_right = y;
    //set the right child of z as x
    z->m_right = x;
    //set the right child of z as the left child of x
    z->m_right = x->m_left;
    //set z as the left child of x
    x->m_left = z;
    //updates the heights afterwards
    updateHeight(z);
    updateHeight(y);
    updateHeight(x);
   
    return x;
}

//prints out the balance factor of each node
Tiger* Streak::printBalanceFactor(Tiger* root)
{
    if (root != nullptr)
    {
        printBalanceFactor(root->m_left);
        cout << root->getID() << " balance factor: " << checkImbalance(root) << endl;
        printBalanceFactor(root->m_right);
    }
    return root;
}
//counts the number of nodes inside of the tree and returns the value
int Streak::count(Tiger* root)
{
    int count = 0;
    countNodes(root, count);
    return count;
}
//recursive function for count
void Streak::countNodes(Tiger* root, int& counter)
{
    if (root != nullptr)
    {
        countNodes(root->m_left, counter);
        counter++;
        countNodes(root->m_right, counter);
        
    }
   
}
//helper function used to remove a certain amount of nodes if listed
void Streak::removeAmont(int amount)
{
    if (count(m_root) >= amount) {
        int counter = 0;
        do {
            m_root = removeAmountListed(counter,m_root);
        } while (counter < amount);
    }
}
//recursive function for removeAmount
Tiger* Streak::removeAmountListed(int& count, Tiger* root)
{
        if (root != nullptr)
        {
            removeAmountListed(count,root->m_left);
            remove(root->m_id);
            removeAmountListed(count, root->m_right);
            
            count++;
        }
    return root;
}

Tiger* Streak::rebalanceAll(Tiger* root)
{
    if (root != nullptr)
    {
        updateHeight(root);
        root = rebalance(root);
        rebalanceAll(root->m_left);
        rebalanceAll(root->m_right);
    }
    return root;
}