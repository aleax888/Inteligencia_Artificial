#include <vector>
#include <utility>
#include <iostream>
using namespace std;
typedef vector<vector<char> > checkerboard;
class token
{
public:
    int x_position, y_position;
    char id; /// 0 rojo, 1 negro;
    bool dama = false;
    token(int x, int y, bool col)
    {
        this->x_position = x;
        this->y_position = y;
        this->id = col;
    }
    token(int x, int y, bool col, bool dama)
    {
        this->x_position = x;
        this->y_position = y;
        this->id = col;
        this->dama = dama;
    }
    ~token() {};
};
