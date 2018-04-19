//
//  Sequence.cpp
//  Homework 2
//
//  Created by Andrew Musk on 2017/01/25.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include "Sequence.h"
#include <iostream>
using namespace std;

//Sequence constructor
Sequence::Sequence()
{
    m_head = nullptr;
    m_size = 0;
}

//Sequence Destructor
Sequence::~Sequence()
{
    Node* p = m_head;
    //run through each value in list and kill item
    while(p!=nullptr)
    {
        Node* killme = p;
        p=p->m_next;
        delete killme;
    }
}

//Sequence copy constructor
Sequence::Sequence(const Sequence& other)
{
    //initializing the size
    m_size = other.m_size;
    //using two temporary nodes
    if(other.m_head!=nullptr)
    {
        Node* o = other.m_head;
        Node* n = new Node();
        m_head = n;
        n->m_prev=nullptr;
        //while other list still has nodes
        while(o->m_next!=nullptr)
        {
            //setting the values equal
            n->m_value = o->m_value;
            //creating the next node
            Node* nn = new Node();
            //setting previous and next values
            n->m_next = nn;
            nn->m_prev = n;
            //moving the pointers forward
            n = n->m_next;
            o = o->m_next;
        }
        //setting values for last node
        n -> m_value = o->m_value;
        n -> m_next = nullptr;
    }
    else{
        m_head = other.m_head;
    }
}

//checking if the list is empty
bool Sequence::empty() const
{
    if(m_head==nullptr)
    {
        return true;
    }
    return false;
}

//returns size of the list
int Sequence::size() const
{
    return m_size;
}

//inserting values into a specific position
bool Sequence::insert(int pos, const ItemType &value)
{
    //if the pos is too small or big, return false
    if(pos<0||pos>m_size)
    {
        return false;
    }
    //if first node in list
    else if(pos==0)
    {
        //create new node
        Node* p = new Node();
        p->m_value=value;
        p->m_prev=nullptr;
        p->m_next = m_head;
        //sets p's value and pointers
        //if not first node set next node's preve to new
        if(m_head!=nullptr)
        {
            m_head->m_prev=p;
        }
        //change head, increment size and return
        m_head=p;
        m_size++;
        return true;
    }
    else if(pos==size())
    {
        //finding the last node
        Node* p = m_head;
        while (p->m_next!=nullptr)
        {
            p = p->m_next;
        }
        //create new node
        Node* latest = new Node();
        latest->m_prev = p;
        p->m_next=latest;
        latest->m_value= value;
        latest->m_next=nullptr;
        //increment and return
        m_size++;
        return true;
    }
    else
    {
        //create traversing node
        Node* p = m_head;
        int count =0;
        while (count != pos)
        {
            p= p->m_next;
            count ++;
        }
        //find node in desired pos
        Node* latest = new Node();
        //change each values
        latest->m_prev = p->m_prev;
        latest->m_next = p;
        latest->m_value = value;
        p->m_prev->m_next = latest;
        p->m_prev=latest;
        //increment size
        m_size++;
        return true;
    }
}

int Sequence::insert(const ItemType &value)
{
    //set count to count position
    int count=-1;
    //if creating first node
    if(m_head==nullptr)
    {
        insert(0, value);
        return 0;
    }
    //traverse through nodes
    Node* p = m_head;
    while (p!=nullptr)
    {
        count++;
        //check if value less then each value or if last node
        if(p->m_next == nullptr || value<=p->m_value)
        {
            break;
        }
        //move to next node
         p=p->m_next;
    }
    //if value was less than existing value
    if(value<=p->m_value)
    {
        insert(count,value);
        return count;
        
    }
    //insert into last position
    else
    {
        insert(size(),value);
        return size()-1;
    }
    
    
    //***** OLD CODE **********
    /*if(p->m_next==nullptr)
    {
        Node* latest = new Node();
        latest->m_prev=p;
        latest-> m_next=nullptr;
        latest->m_value=value;
        p->m_next=latest;
        m_size++;
        return count;
    }
    else
    {
        if(p->m_prev==nullptr)
        {
            insert(0, value);
            return count;
        }
        Node* latest = new Node();
        latest->m_next = p;
        latest->m_prev = p->m_prev;
        latest->m_value = value;
        (p->m_prev)->m_next = latest;
        p->m_prev = latest;
        m_size++;
        return count;
    }
     */
}



bool Sequence::erase(int pos)
{
    //if it is empty, return false
    if(m_head==nullptr)
    {
        return false;
    }
    //if you're erasing first value
    if(pos==0)
    {
        Node* killme = m_head;
        m_head = killme->m_next;
        //if there is another node
        if(m_head!=nullptr)
            //set next node prev to nullptr
            m_head->m_prev = nullptr;
        //delete current
        delete killme;
        m_size--;
        return true;
    }
    //if pos is invalid return false
    if(pos<0||pos>=size())
    {
        return false;
    }
    
    //traverse through list to find
    int count = 0;
    Node* p = m_head;
    while(count != pos)
    {
        p = p->m_next;
        count++;
    }
    //if not the last node
    if(p->m_next!=nullptr)
    {
        //link the prev and next nodes
        p->m_next->m_prev = p->m_prev;
        p->m_prev->m_next = p->m_next;
        //delete the node in position
        delete p;
        m_size--;
        return true;
    }
    else
    {
        //delete the last node
        p->m_prev->m_next=nullptr;
        delete p;
        m_size--;
        return true;
    }
}


int Sequence::remove(const ItemType& value)
{
    int count = 0;
    int count_pos=0;
    //creating traversing pointer
    Node* p = m_head;
    while(p!=nullptr)
    {
        //checking if value is value we're looking to delete
        if(p->m_value==value)
        {
            //move pointer onwards before erase
            p = p->m_next;
            //erase node in this pos
            erase(count_pos);
            //decrease pos because size-- too
            count_pos--;
            //increase total no. of deleted
            count++;
        }
        else
        {
            p=p->m_next;
        }
        //move onto next node
        count_pos++;
    }
    return count;
}
bool Sequence::get(int pos, ItemType& value) const
{
    //check if invalid pointer
    if(pos<0 || pos>=size())
        return false;
    Node* p = m_head;
    int count = 0;
    //loop till you get the variable
    while(pos!= count)
    {
        p=p->m_next;
        count++;
    }
    //set the value
    value = p->m_value;
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    //check if invalid pointer
    if(pos<0 || pos>=size())
        return false;
    Node* p = m_head;
    int count = 0;
    //loop through till you get the pos
    while(pos!= count)
    {
        p=p->m_next;
        count++;
    }
    //set the value of that position
    p->m_value=value;
    return true;
}

int Sequence::find(const ItemType& value) const
{
    Node* p = m_head;
    int count = 0;
    //loop through to find the value
    while(p!=nullptr && p->m_value != value)
    {
        p = p->m_next;
        count++;
    }
    //return the position or -1 if it wasn't found
    if(p==nullptr)
        return -1;
    else
        return count;
}

void Sequence::swap(Sequence& other)
{
    //set temp heads to use
    Node* temp = m_head;
    Node* temp_other = other.m_head;
    
    //swap the head pointers
    m_head= other.m_head;
    other.m_head = temp;
    
    //if the heads aren't null, setting the previous of respective 2nd nodes correctly
    if(temp!=nullptr && temp->m_next!=nullptr)
        temp->m_next->m_prev = temp_other;
    if(temp_other!=nullptr && temp_other->m_next!=nullptr)
        temp_other->m_next->m_prev=temp;
    
    //swapping the size of the lists
    int temp_size = m_size;
    m_size = other.m_size;
    other.m_size = temp_size;
}


Sequence& Sequence::operator=(const Sequence &other)
{
    //erase all the prev values that might exist
    bool flag = true;
    while(flag)
    {
        flag = erase(0);
    }
    //checking for aliasing
    if(this!=&other)
    {
        //same as copy constructor
        m_size = other.m_size;
        if(other.m_head!=nullptr)
        {
            Node* o = other.m_head;
            Node* n = new Node();
            m_head = n;
            while(o->m_next!=nullptr)
            {
                n->m_value = o->m_value;
                Node* nn = new Node();
                n->m_next = nn;
                nn->m_prev = n;
                n = n->m_next;
                o = o->m_next;
            }
            n -> m_value = o->m_value;
            n -> m_next = nullptr;
        }
        else{
            m_head = nullptr;
        }
        
    }
    //returning
    return (*this);
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    //if sizes are invalid return -1
    if(seq2.size() == 0 || seq1.size()<seq2.size())
    {
        return -1;
    }
    //set initial values
    int count = 0;
    ItemType seq1_val;
    ItemType seq2_val;
    //loop through sequence 1
    for(int i =0; i<seq1.size(); i ++)
    {
        //check if respcective j is equal
        seq1.get(i, seq1_val);
        seq2.get(0, seq2_val);
        if(seq1_val==seq2_val)
        {
            //add to count
            count++;
            //loop through next values of seq2 and compare to next values of seq1
            for(int j=1 ; j<seq2.size() ;j++)
            {
                seq1.get(i+j, seq1_val);
                seq2.get(j, seq2_val);
                //check if respective values are equal
                if(seq1_val==seq2_val)
                {
                    count++;
                }
            }
            //check if total vals equal is same as size(all equal)
            if(count==seq2.size())
            {
                return i;
            }
            //restart by setting count=0
            else
                count=0;
        }
    }
    //non found
    return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    Sequence tempResult(result);
    //erase all potential values in result
    bool flag = true;
    while(flag)
    {
        flag = tempResult.erase(0);
    }
    //checking for aliasing
        //if either empty just return the other
    if(seq2.empty())
    {
        tempResult = seq1;
        return;
    }
    else if(seq1.empty())
    {
        tempResult = seq2;
        return;
    }
    //find the bigger sequence
    int big =0;
    int small=0;
    Sequence big_seq;
    if(seq1.size()>=seq2.size())
    {
        //if seq1 bigger set respective variables
        big = seq1.size();
        small = seq2.size();
        big_seq = seq1;
    }
    else
    {
        //if seq1 bigger set respective variables
        big = seq2.size();
        small = seq1.size();
        big_seq = seq2;
    }
    //vals to store result return
    ItemType val1;
    ItemType val2;
    //run to the val of the bigger seq1
    for(int j=0; j < big; j++)
    {
        //if interleaving still happening(There is a value for seq1 and seq2)
        if(j<small)
        {
            seq1.get(j, val1);
            seq2.get(j, val2);
            tempResult.insert(tempResult.size(), val1 );
            tempResult.insert(tempResult.size(), val2 );
        }
        //else just enter in the next value for the bigger sequence
        else
        {
            big_seq.get(j, val1);
            tempResult.insert(tempResult.size(), val1 );
        }
    }
    result = tempResult;
}


