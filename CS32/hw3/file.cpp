#include <iostream>
#include <string>

using namespace std;

class File
{
public:
    File(string name)
    {
        m_name = name;
    }
    virtual ~File() {}
    string name() const
    {
        return m_name;
    }
    virtual void redisplay() const
    {
        cout << "refresh the screen";
    }
    virtual void open() const = 0;
private:
    string m_name;
};

class TextMsg : public File
{
public:
    TextMsg(string name) : File(name) {}
    ~TextMsg()
    {
        cout << "Destroying "<< this->name()<<", a text message" << endl;
    }
    void open() const
    {
        cout << "open text message";
    }
private:
};

class Video : public File
{
public:
    Video(string name, int len) : File(name)
    {
        m_len = len;
    }
    ~Video()
    {
        cout << "Destroying "<< this->name()<<", a video" << endl;
    }
    void open() const
    {
        cout << "play " << m_len << " second video";
    }
    virtual void redisplay() const
    {
        cout<< "replay video";
    }
private:
    int m_len;
};

class Picture : public File
{
public:
    Picture(string name) : File(name) {}
    ~Picture()
    {
        cout << "Destroying the picture "<< this->name()<< endl;
    }
    void open() const
    {
        cout << "show picture";
    }
private:
};

void openAndRedisplay(const File* f)
{
    cout << f->name() << ": ";
    f->open();
    cout << endl << "Redisplay: ";
    f->redisplay();
    cout << endl;
}



/*
DESIRED OUTPUT:

fromFred.txt: open text message
Redisplay: refresh the screen
goblin.mpg: play 3780 second video
Redisplay: replay video
kitten.jpg: show picture
Redisplay: refresh the screen
baby.jpg: show picture
Redisplay: refresh the screen
Cleaning up.
Destroying fromFred.txt, a text message
Destroying goblin.mpg, a video
Destroying the picture kitten.jpg
Destroying the picture baby.jpg

*/