#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED
#include <vector>
#include <utility>
#include <iostream>
#include "token.h"
using namespace std;


bool turn = 0; /// 0 rojo, 1 negro;
int player1 = 0;
int player2 = 0;

vector<token*> tokens;
vector<checkerboard> move_position(checkerboard t, int x, int y, bool player) {
    vector<checkerboard> result;
    checkerboard current;
    if (player) {
        if (x - 1 >= 0 && y - 1 >= 0) {
            if (t[x - 1][y - 1] == '0') {
                current = t;
                current[x][y] = '0';
                current[x - 1][y - 1] = '2';
                result.push_back(current);
            }
        }
        if (x - 1 >= 0 && y + 1 <= 7) {
            if (t[x - 1][y + 1] == '0') {
                current = t;
                current[x][y] = '0';
                current[x - 1][y + 1] = '2';
                result.push_back(current);
            }
        }
    }
    else {
        if (x + 1 <= 7 && y - 1 >= 0) {
            if (t[x + 1][y - 1] == '0') {
                current = t;
                current[x][y] = '0';
                current[x + 1][y - 1] = '1';
                result.push_back(current);
            }
        }
        if (x + 1 <= 7 && y + 1 <= 7) {
            if (t[x + 1][y + 1] == '0') {
                current = t;
                current[x][y] = '0';
                current[x + 1][y + 1] = '1';
                result.push_back(current);
            }
        }
    }

    return result;
}

vector<checkerboard> move_positions_capture(checkerboard t, int x, int y, bool player) {
    vector<checkerboard> result;
    checkerboard current;
    if (player) {
        if (x - 2 >= 0 && y - 2 >= 0) {
            if (t[x - 1][y - 1] == '1' && t[x - 2][y - 2] == '0') {
                current = t;
                current[x][y] = '0';
                current[x - 1][y - 1] = '0';
                current[x - 2][y - 2] = '2';
                result.push_back(current);
            }
        }
        if (x - 2 >= 0 && y + 2 <= 7) {
            if (t[x - 1][y + 1] == '1' && t[x - 2][y + 2] == '0') {
                current = t;
                current[x][y] = '0';
                current[x - 1][y + 1] = '0';
                current[x - 2][y + 2] = '2';
                result.push_back(current);
            }
        }
    }
    else {
        if (x + 2 <= 7 && y - 2 >= 0) {
            if (t[x + 1][y - 1] == '2' && t[x + 2][y - 2] == '0') {
                current = t;
                current[x][y] = '0';
                current[x + 1][y - 1] = '0';
                current[x + 2][y - 2] = '1';
                result.push_back(current);
            }
        }
        if (x + 2 <= 7 && y + 2 <= 7) {
            if (t[x + 1][y + 1] == '2' && t[x + 2][y + 2] == '0') {
                current = t;
                current[x][y] = '0';
                current[x + 1][y + 1] = '0';
                current[x + 2][y + 2] = '1';
                result.push_back(current);
            }
        }
    }

    return result;
}


class Minimax
{
    int deep, max_deep, value;   
    bool max;
    checkerboard cb;
    vector<Minimax*> moves;
public:
    Minimax(checkerboard b, int d, int m_d, bool player) {
        this->cb = b;
        this->deep = d;
        this->max = player;
        this->max_deep = m_d;
        vector<checkerboard> posib1, posib2, posib;
        vector<checkerboard> current;
        if (deep < max_deep) {
            for (int i = 0; i < cb.size(); ++i) {
                for (int j = 0; j < cb[i].size(); ++j) {
                    if (max) {
                        if (cb[i][j] == '2') {
                            current = move_position(cb, i, j, max);
                            for (size_t i = 0; i < current.size(); ++i)
                                posib1.push_back(current[i]);
                            current = move_positions_capture(cb, i, j, max);
                            for (size_t i = 0; i < current.size(); ++i)
                                posib2.push_back(current[i]);
                        }
                    }
                    else {
                        if (cb[i][j] == '1') {
                            current = move_position(cb, i, j, max);
                            for (size_t i = 0; i < current.size(); ++i)
                                posib1.push_back(current[i]);
                            current = move_positions_capture(cb, i, j, max);
                            for (size_t i = 0; i < current.size(); ++i)
                                posib2.push_back(current[i]);
                        }
                    }
                }
            }
        }
        if (posib2.size() != 0)
            posib = posib2;
        else
            posib = posib1;
        for (size_t i = 0; i < posib.size(); ++i) {
            Minimax* m = new Minimax(posib[i], deep + 1, max_deep, !max);
            moves.push_back(m);
        }
    }
    void verify()
    {
        if (moves.size() == 0) {
            int result = 0;
            for (size_t i = 0; i < cb.size(); ++i) {
                for (size_t j = 0; j < cb[i].size(); ++j) {
                    if (cb[i][j] == '1')
                        --result;
                    if (cb[i][j] == '2')
                        ++result;
                }
            }
            value = result;
        }
        else {
            for (size_t i = 0; i < moves.size(); ++i)
                moves[i]->verify();
            if (max) {
                int m = -10000, current;
                for (size_t i = 0; i < moves.size(); ++i) {
                    current = moves[i]->value;
                    if (current > m)
                        m = current;
                }
                value = m;
            }
            else {
                int m = 100000, current;
                for (size_t i = 0; i < moves.size(); ++i) {
                    current = moves[i]->value;
                    if (current < m)
                        m = current;
                }
                value = m;
            }
        }
    }
    checkerboard play()
    {
        checkerboard result;
        int current = -10000;
        for (size_t i = 0; i < moves.size(); ++i) {
            if (moves[i]->value > current) {
                current = moves[i]->value;
                result = moves[i]->cb;
            }
        }
        return result;
    }
    virtual ~Minimax() {}
};

void game_over()
{
    if (player1 == 12)
    {
        cout << "FIN, GANO EL JUGADOR 1" << endl;
    }
    if (player2 == 12)
    {
        cout << "FIN, GANO EL JUGADOR 2" << endl;
    }
}
#endif