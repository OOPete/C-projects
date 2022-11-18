//UMBC - CSEE - CMSC 341 - Fall 2022 - Proj1
#include "railroad.h"

Railroad::Railroad() {
    m_numStations = 0;
    m_head = nullptr;
    m_tail = nullptr;
    
    
}

//deallocates all memory and reinitialises the variables
Railroad::~Railroad() {
    
    clearNetwork();
}

void Railroad::clearNetwork() {
    
    //checks if the list is empty or not
       if (!isEmpty()) {
           clearAllRoutes();
           cout << "it is not empty" << endl;
           Station* temp = m_head;
           Station* next = m_head;

           while(temp != nullptr)
           {
               next = temp->m_next;
               delete temp;
               temp = next;
           }

       }
       else
       {
           cout << "it is empty" << endl;
       }
    m_head = nullptr;
    m_tail = nullptr;
}   
//extends the railroad at the head
bool Railroad::extendAtHead(int newCode, int passengers) {
    bool check = true;
    //if the object already exists return false
    if (isFound(newCode))
    {
        check = false;
    }
    else if (newCode < 1)
    {
        check = false;
    }
    else if (passengers < 0)
    {
        check = false;
    }

    else
    {
        Station* newObj = new Station(newCode, passengers);
        if (isEmpty()) {
            
            m_head = newObj;
            m_tail = newObj;
            newObj->m_north = nullptr;
            newObj->m_south = nullptr;
            newObj->m_east = nullptr;
            newObj->m_west = nullptr;
            
       }
        else
        {
            //sets the current m_head as the next node for the new object
            newObj->m_next = m_head;
            //sets the previous of the current m_head to the new node
            m_head->m_previous = newObj;
            //sets the m_head to newNode
            m_head = newObj;
            newObj->m_north = nullptr;
            newObj->m_south = nullptr;
            newObj->m_east = nullptr;
            newObj->m_west = nullptr;
        }
       
    }
    m_numStations++;
    return check;
}

//extends the railroad at the tail
bool Railroad::extendAtTail(int newCode, int passengers) {
    bool check = true;
    if (isFound(newCode))
    {
        check = false;
    }
    else if (newCode < 1)
    {
        check = false;
    }
    else if (passengers < 0)
    {
        check = false;
    }
    else
    {
        Station* newObj = new Station(newCode, passengers);
        if (isEmpty()) {

            m_head = newObj;
            m_tail = newObj;
            newObj->m_north = nullptr;
            newObj->m_south = nullptr;
            newObj->m_east = nullptr;
            newObj->m_west = nullptr;
        }
        else
        {
            //sets the current m_tail as the previous node for the new object
            newObj->m_previous = m_tail;
            //sets the current m_tail's m_next to the new node
            m_tail->m_next = newObj;
            //sets the m_tail to newNode
            m_tail = newObj;
            newObj->m_north = nullptr;
            newObj->m_south = nullptr;
            newObj->m_east = nullptr;
            newObj->m_west = nullptr;
        }
        
    }
    m_numStations++;
    
    return check;
}

//checks if a node already exists with the same information
bool Railroad::isFound(int newCode)
{
    bool check = false;
    //creates a dummy node to iterate through the linked list
    Station* temp = m_head;
    //checks whether the list is empty
    //iterates through the list as long as temp is not equal to m_tail
    if (isEmpty())
    {
        check = false;
    }
    else if (!isEmpty()) {
        if (m_head == m_tail && m_head->m_code != newCode)
        {
            check = false;
        }
        else if (m_head->m_code == newCode || m_tail->m_code == newCode)
        {
           
            check = true;
        }
        else {
            while (temp != nullptr)
            {
                
                //returns true if the m_code and number of passangers are equal
                if (temp->m_code == newCode)
                {
                    
                    check = true;
                }
                temp = temp->m_next;
            }
        }
    }
    return check;
}

//iterates through the linked list to find a node and return that node
Station* Railroad::find(int code)
{
    
    Station* temp = nullptr;
    if (isEmpty())
    {
        //does nothing
       
    }
    else if (!isFound(code))
    {
        //does nothing
        
    }
    else {
        Station* temp = m_head;
       
        
        //iterates through the list and returns temp if temp is found in the list
        while (temp != nullptr)
        {
            if (temp->m_code == code)
            {
                
                return temp;
            }
            temp = temp->m_next;
        }
       
    }
    return temp;
}

//if m_head's m_next is pointing to m_tail then it is empty
bool Railroad::isEmpty()
{
    return (m_head == nullptr);
}
//iterates through the linked list to find the correct node and returns it's position


void Railroad::dump() {
    if (m_head != nullptr) {
        int northCode, southCode, eastCode, westCode;
        Station* temp = m_head;
        while (temp != nullptr) {
            if (temp->m_north != nullptr) northCode = temp->m_north->m_code;
            else northCode = -1;
            if (temp->m_south != nullptr) southCode = temp->m_south->m_code;
            else southCode = -1;
            if (temp->m_east != nullptr) eastCode = temp->m_east->m_code;
            else eastCode = -1;
            if (temp->m_west != nullptr) westCode = temp->m_west->m_code;
            else westCode = -1;
            cout << temp->m_code <<
                " (" <<
                "P: " << temp->m_passengers <<
                ", N: " << northCode <<
                ", S: " << southCode <<
                ", E: " << eastCode <<
                ", W: " << westCode <<
                ")" << endl;
            temp = temp->m_next;
        }
    }
}

bool Railroad::makeRoute(list< pair<int, DIRECTION> > route) {
    //bool condition that passes or fails the function
    bool pass = true;
    list<pair<int, DIRECTION>>::iterator itr;
    
    //if the linked list is empty
    if (isEmpty())
    {
        
        pass = false;
    }
    
    //if the route is empty
    else if(route.empty())
    {
        
       
        
    }
    
    else
    {
        //checks if the first item in the list of pairs is inside the main linked list
        
        if (!isFound(route.front().first))
        {
            
            pass = false;
        }
        //if there are only one item in the list
        else if (route.size() == 1)
        {
            
            //does nothing
        }
        else
        {
            //iterates through the list of pairs
            for (itr = route.begin(); itr != route.end(); itr++)
            {
                
                
                //if a none route is called before the end of the list of pairs
                if (itr->second == NONE && itr != --route.end())
                {
                    
                    pass = false;
                }
                //prevents a route from being assigned to itself
                else if (itr->first == next(itr, 1)->first)
                {
                    
                    pass = false;
                }
                
                else
                {
                    //temp1 for the current item in the list
                    Station* temp1 = nullptr;
                    //temp 2 for the next item in the list
                    Station* temp2 = nullptr;
                    
                    if (itr != --route.end()) {
                        
                        if (extendAtTail(next(itr, 1)->first, DEFNUMPASS) || !extendAtTail(next(itr, 1)->first, DEFNUMPASS))
                        {

                            temp1 = find(itr->first);
                            

                            temp2 = find(next(itr, 1)->first);
                           


                            
                                switch (itr->second)
                                {

                                case NORTH:

                                    if (temp1->m_north == nullptr) {
                                        temp1->m_north = temp2;
                                        

                                    }
                                    else {
                                        pass = false;
                                    }
                                    break;

                                case SOUTH:

                                    if (temp1->m_south == nullptr) {

                                        temp1->m_south = temp2;
                                        


                                    }
                                    else {
                                        pass = false;
                                    }
                                    break;


                                case EAST:

                                    if (temp1->m_east == nullptr) {
                                        temp1->m_east = temp2;
                                        

                                    }
                                    else {
                                        pass = false;
                                    }
                                    break;

                                case WEST:

                                    if (temp1->m_west == nullptr) {
                                        temp1->m_west = temp2;
                                       

                                    }
                                    else {
                                        pass = false;
                                    }
                                    break;

                                case NONE:

                                    //does nothing
                                    break;
                                }
                                
                            
                        }

                    }
                }
            }
        }
    }
    
    return pass;
}

//adds up the number of passengers in the route
int Railroad::travel(list< pair<int, DIRECTION> > route) {
    //checks if the route is valid or not
    //checks if all routes are valid
    int count = 0;
    list<pair<int, DIRECTION>>::iterator itr;
    itr = route.begin();
    if (isEmpty())
    {
        return -1;
        
        //does nothing
    }
    //if the route only has one pair inside of it
    else if (route.size() == 1)
    {
        
        Station* temp = m_head;
        temp = find(route.front().first);
        count = temp->getNumPassegers();
    }
    //checks if the first pair in the passed in list exist in the main list
    else if (!isFound(itr->first))
    {
        
        count = -1;
    }
    else {
        //checks if every pair in the route is also in the main linked list
        for (itr = route.begin(); itr != route.end(); itr++)
        {
            //checks if the code is found int he main linked list
            if (!isFound(itr->first))
            {
                
                //I immediately return -1 as if any part isn't found, it is
                //automatically invalid
                return -1;

            }
            //checks if a pair with a direction of NONE is called before the end of the route
            else if (itr->second == NONE && itr != --route.end())
            {
                
                return -1;
            }

        }
        
        Station* temp = nullptr;
        Station* temp2 = nullptr;
        
        for (itr = route.begin(); itr != route.end(); itr++)
        {
            
            temp = find(itr->first);
            
            
                //creates a switch statement that checks the direction of the pair
                //then searches to see if the next node matches that pair
            
                switch (itr->second)
                {
                case NORTH:
                    if (itr != --route.end()) {
                        //adds the number of passengers to count
                        temp2 = find(next(itr, 1)->first);
                        if (temp->m_north == temp2)
                        {
                            count += temp->getNumPassegers();
                        }
                        else
                        {
                           
                            count = -1;
                        }
                    }
                    else {
                        count += temp->getNumPassegers();
                    }
                    break;
                case SOUTH:
                    if (itr != --route.end())
                    {
                        temp2 = find(next(itr, 1)->first);
                        if (temp->m_south == temp2)
                        {
                            count += temp->getNumPassegers();
                        }
                        else
                        {
                            
                            count = -1;
                        }
                    }
                    else
                    {
                        count += temp->getNumPassegers();
                    }
                    break;
                    
                case EAST:
                    if (itr != --route.end()) {
                        temp2 = find(next(itr, 1)->first);
                        if (temp->m_east == temp2)
                        {
                            count += temp->getNumPassegers();
                        }
                        else
                        {
                           
                            count = -1;
                        }
                    }
                    else {
                        count += temp->getNumPassegers();
                    }
                    break;

                case WEST:
                    if (itr != --route.end()) {
                        temp2 = find(next(itr, 1)->first);
                        if (temp->m_west == temp2)
                        {
                            count += temp->getNumPassegers();
                        }
                        else
                        {
                            
                            count = -1;
                        }
                    }
                    else
                    {
                        count += temp->getNumPassegers();
                    }
                    break;

                case NONE:
                    count += temp->getNumPassegers();
                    break;

                
            }
        }

    }
    
    return count;
}

bool Railroad::setNumPassengers(int code, int passengers) {

    //if the find function does not find the code, return false
    if (!isFound(code))
    {
        return false;
    }
    //is passengers is less than 0 return false
    else if (passengers < 0)
    {
        return false;
    }
    else
    {
        //create a temp node pointing to m_head
        Station* temp = m_head;
        //i starts at 0 while the find iteration will start at 1 due to m_head having no data
        while (temp != nullptr)
        {
            if (temp->m_code == code)
            {
                temp->m_passengers = passengers;
            }
            temp = temp->m_next;
      }
    }
    return true;
}
//accepts the code of the object in question from the parameter, and removes it accordingly
bool Railroad::removeStation(int aCode) {

    
    bool check = true;
    //returns false if the list is already empty
    
    if (isEmpty())
    {
       check = false;
    }
    
    //returns false if the code is not in the list
    
    else if (!isFound(aCode))
    {
        
        check = false;
    }
    
    else
    {
       
          //if both m_head and m_tail point to the same object
        if (m_head == m_tail)
        {
            delete m_head;
            delete m_tail;
            m_head = nullptr;
            m_tail = nullptr;
        }
        else
        {
           
            Station* temp = m_head;
            
            if (m_head->m_code == aCode)
            {
                
                m_head = m_head->m_next;
                
                delete temp;
                m_head->m_previous = nullptr;
                temp = nullptr;
                /*
                or
                delete m_head;
                m_head = temp->m_next;
                */
            }
            else if (m_tail->m_code == aCode)
            {
                
                Station* tempTail = m_tail;
                m_tail = m_tail->m_previous;
                
                delete tempTail;
                m_tail->m_next = nullptr;
                tempTail = nullptr;
                /*
                or
                delete m_tail;
                m_tail = tempTail->m_previous;
                */
            }
            else {
                while (temp != nullptr)
                {

                    if (temp->m_code == aCode)
                    {
                        Station* routes = m_head;
                        {
                            //checks if there are nodes with directions pointing to the station
                            //we want to remove
                            while (routes != nullptr)
                            {
                                if (routes->m_north == temp)
                                {
                                    routes->m_north = nullptr;
                                }
                                else if (routes->m_south == temp)
                                {
                                    routes->m_south = nullptr;
                                }
                                else if (routes->m_east == temp)
                                {
                                    routes->m_east = nullptr;
                                }
                                else if (routes->m_west == temp)
                                {
                                    routes->m_west = nullptr;
                                }
                                routes = routes->m_next;
                            }
                        }
                        //creates two nodes that signal the previous and next values of the station
                        //we want to remove
                        //make them point to one another and remove temp
                        Station* prev = temp->m_previous;

                        Station* next = temp->m_next;

                        prev->m_next = next;
                        next->m_previous = prev;
                        temp->m_north = nullptr;
                        temp->m_south = nullptr;
                        temp->m_east = nullptr;
                        temp->m_west = nullptr;
                        delete temp;
                        temp = nullptr;

                    }
                    else if (temp != nullptr) {
                        temp = temp->m_next;
                    }



                }
            }
        }
        m_numStations--;
    }
   
    return check;
}

//clears all directions in the station
void Railroad::clearAllRoutes() {
    Station* temp = m_head;
    if (!isEmpty()) {
        while (temp != nullptr) {

            temp->m_north = nullptr;
            temp->m_south = nullptr;
            temp->m_east = nullptr;
            temp->m_west = nullptr;
            temp = temp->m_next;
        }
    }
    else
    {
        //does nothing
    }
}

const Railroad& Railroad::operator=(const Railroad& rhs) {
    if (this != &rhs)
    {
        clearAllRoutes();
        clearNetwork();
        if (rhs.m_head != nullptr) {
            
           
            //Creates 2 temp nodes that will help us iterate through the rhs linked list
            
            Station* rhsTemp = rhs.m_head;
            //checks if there is only one item in the list
                while (rhsTemp != nullptr)
                {
                    //Sets the m_next of the current temp object to the m_next of the current rhs object
                    //Sets the m_previous of the m_next of temp to the current temp
                    //moves to the next node in the rhs linked list
                    extendAtTail(rhsTemp->m_code, rhsTemp->m_passengers);
                    rhsTemp = rhsTemp->m_next;
                   
                }
               
                
                Station* temp = m_head;
                rhsTemp = rhs.m_head;
                while (rhsTemp != nullptr)
                {
                    if (rhsTemp->m_north != nullptr)
                    {
                        temp->m_north = find(rhsTemp->m_north->m_code);
                    }
                    if (rhsTemp->m_south != nullptr)
                    {
                        temp->m_south = find(rhsTemp->m_south->m_code);
                    }
                    if (rhsTemp->m_east != nullptr)
                    {
                        temp->m_east = find(rhsTemp->m_east->m_code);
                    }
                    if (rhsTemp->m_west != nullptr)
                    {
                        temp->m_west = find(rhsTemp->m_west->m_code);
                    }
                    temp = temp->m_next;
                    rhsTemp = rhsTemp->m_next;
                }
                m_numStations = rhs.m_numStations;
        }
        else
        {
            //does nothing
        }
        
    }
    
    return *this;
}

//copy constructor for the Railroad class
Railroad::Railroad(const Railroad& rhs) {
    if (rhs.m_head != nullptr) {
        
        extendAtTail(rhs.m_head->m_code, rhs.m_head->m_passengers);

        //Creates 2 temp nodes that will help us iterate through the rhs linked list
        
        Station* rhsTemp = rhs.m_head;
        //checks if there is only one item in the list

        while (rhsTemp != nullptr)
        {
            //Sets the m_next of the current temp object to the m_next of the current rhs object
            //Sets the m_previous of the m_next of temp to the current temp
            //moves to the next node in the rhs linked list
            extendAtTail(rhsTemp->m_code, rhsTemp->m_passengers);
            rhsTemp = rhsTemp->m_next;
        }

        Station* temp = m_head;
        rhsTemp = rhs.m_head;
        while (rhsTemp != nullptr)
        {
            if (rhsTemp->m_north != nullptr)
            {
                temp->m_north = find(rhsTemp->m_north->m_code);
            }
            if (rhsTemp->m_south != nullptr)
            {
                temp->m_south = find(rhsTemp->m_south->m_code);
            }
            if (rhsTemp->m_east != nullptr)
            {
                temp->m_east = find(rhsTemp->m_east->m_code);
            }
            if (rhsTemp->m_west != nullptr)
            {
                temp->m_west = find(rhsTemp->m_west->m_code);
            }
            temp = temp->m_next;
            rhsTemp = rhsTemp->m_next;
        }
        m_numStations = rhs.m_numStations;
    }
    else
    {
        //does nothing
    }

}

