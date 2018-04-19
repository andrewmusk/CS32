//
//  main.cpp
//  Homework 3
//
//  Created by Andrew Musk on 2017/02/13.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here

//FILE
class File
{
public:
    File(string name){m_name = name;};
    virtual ~File(){};
    string name() const {return m_name;};
    virtual void open() const = 0;
    virtual void redisplay() const {cout << "refresh the screen";};
private:
    string m_name;
};

//VIDEO
class Video: public File
{
public:
    Video(string name, int length) : File(name)
    {
        m_length = length;
    }
    virtual~Video();
    virtual void open() const {cout << "play " << m_length << " second video";};
    virtual void redisplay() const {cout << "replay video";};
private:
    int m_length;
};

//TEXTMSG
class TextMsg: public File
{
public:
    TextMsg(string name) : File(name){};
    virtual ~TextMsg();
    virtual void open() const {cout << "open text message";};
};

//PICTURE
class Picture: public File
{
public:
    Picture(string name) : File(name){};
    virtual~Picture();
    virtual void open() const {cout << "show picture";};
};

//Destructors
TextMsg::~TextMsg(){
    cout << "Destroying " << name() << ", a text message" << endl;;
}

Video::~Video(){
    cout << "Destroying " << name() << ", a video" << endl;
}

Picture::~Picture(){
    cout << "Destroying the picture " << name() << endl;
}



void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}

int main()
{
    File* files[4];
    files[0] = new TextMsg("fromFred.txt");
    // Videos have a name and running time
    files[1] = new Video("goblin.mpg", 3780);
    files[2] = new Picture("kitten.jpg");
    files[3] = new Picture("baby.jpg");
    
    for (int k = 0; k < 4; k++)
        openAndRedisplay(files[k]);
    
    // Clean up the files before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete files[k];
}

