#include <QCoreApplication>

#include <iostream>
using namespace std;

class Sharp
{
public:
    Sharp(){
        cout<<"construction Sharp"<<endl;
    }
    virtual ~Sharp(){
        cout <<"destroy Sharp"<<endl;
    }

    double virtual calcArea(){
        cout <<"calcArea Sharp"<<endl;
        return 0;
    }
};

class Circle :public Sharp
{
public:
    Circle(double r){
        cout <<"construction Circle"<<endl;
        m_dR = r;
    }
    ~Circle(){
        cout <<"destroy Circle"<<endl;
    }

    double calcArea ()
    {
        cout <<"calcArea Circle"<<endl;
        return 3.14 * m_dR * m_dR;
    }

private:
    double m_dR;
};

class Rect :public Sharp
{
public:
    Rect(double width,double height){
        cout <<"construction Rect"<<endl;
        m_dWidth = width;
        m_dHeight = height;
    }
    ~Rect(){
        cout <<"destroy Rect"<<endl;
    }

    double calcArea ()
    {
       cout <<"calcArea Rect"<<endl;

        return m_dWidth * m_dHeight;
    }

private:
    double m_dWidth;
    double m_dHeight;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Sharp *circle = new Circle(4);
    //Sharp *rect= new Rect(3,4);

    circle->calcArea ();
    //rect->calcArea ();

    delete circle;
    circle = NULL;

    return a.exec();
}
