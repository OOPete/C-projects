switch (itr->second)
{

case NORTH:
    cout << "In north" << endl;
    if (temp1->m_north == nullptr) {
        temp1->m_north = temp2;

    }
    else {
        pass = false;
    }
    break;

case SOUTH:
    cout << "In south" << endl;
    if (temp1->m_south == nullptr) {
        cout << "checking the south" << endl;
        temp1->m_south = temp2;
        cout << "Out of the south" << endl;

    }
    else {
        pass = false;
    }
    break;


case EAST:
    cout << "In east" << endl;
    if (temp1->m_east == nullptr) {
        temp1->m_east = temp2;

    }
    else {
        pass = false;
    }
    break;

case WEST:
    cout << "In west" << endl;
    if (temp1->m_west == nullptr) {
        temp1->m_west = temp2;

    }
    else {
        pass = false;
    }
    break;

case NONE:
    cout << "Nothing" << endl;
    //does nothing
    break;
}