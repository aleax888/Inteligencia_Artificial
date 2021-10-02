#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

struct checkerboard
{
	// 9 -> IA
	// 6 -> jugador
	// 0 -> espacio disponible
	// 1 -> espacio no disponible

	const int size_board = 8;
	int cb_matrix[8][8] =
	{
		{ 1, 9, 1, 9, 1, 9, 1, 9 },
		{ 9, 1, 9, 1, 9, 1, 9, 1 },
		{ 1, 9, 1, 9, 1, 9, 1, 9 },
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 6, 1, 6, 1, 6, 1, 6, 1 },
		{ 1, 6, 1, 6, 1, 6, 1, 6 },
		{ 6, 1, 6, 1, 6, 1, 6, 1 }
	};

	checkerboard() {}
	
	void print_checkerboard(bool player_turn_begin)
	{
		cout << "Tablero: " << endl;
		for (int i = 0; i < size_board; ++i)
		{
			cout << i << " | ";
			for (int j = 0; j < size_board; ++j)
			{
				if (cb_matrix[i][j] == 9 and player_turn_begin == 1)
					cout << 'N' << " ";
				else if (cb_matrix[i][j] == 6 and player_turn_begin == 1)
					cout << 'R' << " ";
				else if (cb_matrix[i][j] == 9 and player_turn_begin == 0)
					cout << 'R' << " ";
				else if (cb_matrix[i][j] == 6 and player_turn_begin == 0)
					cout << 'N' << " ";
				else if (cb_matrix[i][j] == 0)
					cout << '_' << " ";
				else if (cb_matrix[i][j] == 1)
					cout << ' ' << " ";
			}
			cout << endl;
		}
		cout << "_" << endl;
		cout << "  | " << "0 1 2 3 4 5 6 7" << endl << endl;
	}

	void copy(checkerboard cb)
	{
		for (int i = 0; i < size_board; ++i)
			for (int j = 0; j < size_board; ++j)
				this->cb_matrix[i][j] = cb.cb_matrix[i][j];
	}
};

struct node
{
	checkerboard cb;
	vector<node*> children;
	int value = 0;

	node(checkerboard cb_aux)
	{
		cb.copy(cb_aux);
	}
};

struct ktree
{
	node* head, **calculate;
	int deep;

	ktree() {}
	ktree(int d) 
	{
		deep = d;
		calculate = &head;
	}

	bool between(int index)
	{
		return index > -1 and index < 8;
	}

	void change_deep(int d)
	{
		calculate = &head;
		deep = d;
	}

	bool capture(checkerboard &cb, int token_x_position, int token_y_position, int move_x_position, int move_y_position, bool turn)
	{
		int enemy_x_position = (token_x_position + move_x_position) / 2;
		int enemy_y_position = (token_y_position + move_y_position) / 2;

		if (turn and cb.cb_matrix[enemy_y_position][enemy_x_position] == 9) //jugador
		{
			cb.cb_matrix[enemy_y_position][enemy_x_position] = 0;
			return true;
			//player_count_capture++;
		}
		else if (!turn and cb.cb_matrix[enemy_y_position][enemy_x_position] == 6) // IA
		{
			cb.cb_matrix[enemy_y_position][enemy_x_position] = 0;
			return true;
			//AI_count_capture++;
		}
		return false;
	}

	bool verify_move_destiny(checkerboard &cb, int token_x_position, int token_y_position, int move_x_position, int move_y_position, bool turn)
	{
		if (between(move_x_position) and between(move_y_position))
		{
			int x_aim = abs(token_x_position - move_x_position);
			int y_aim = token_y_position - move_y_position;

			if ((turn == true and y_aim < 0) or (turn == false and y_aim > 0))
				return false;
			y_aim = abs(y_aim);
			if (x_aim == 1 and y_aim == 1)
				return !cb.cb_matrix[move_y_position][move_x_position];
			
			else if (x_aim == 2 and y_aim == 2 and !cb.cb_matrix[move_y_position][move_x_position])
			{
				bool cap_aux = capture(cb, token_x_position, token_y_position, move_x_position, move_y_position, turn);
				if (cap_aux) return true;
				else return false;
			}
		}
		return false;
	}

	bool verify_token(checkerboard cb, int x_position, int y_position)
	{
		if (!between(x_position) or !between(y_position))
			return false;
		if (cb.cb_matrix[y_position][x_position] == 6)
			return true;	
		if (cb.cb_matrix[y_position][x_position] == 9)
			return true;
		return false;
	}

	vector<checkerboard> calculate_moves(checkerboard cb, int x_position, int y_position, bool turn)
	{
		if (!verify_token(cb, x_position, y_position)) return vector<checkerboard>();
		checkerboard cb_aux; 
		vector<checkerboard> vec_aux;
		int aux, tmp;
		if (turn)
			aux = -1;
		
		else
			aux = 1;
		
		cb_aux.copy(cb);
		
		if (verify_move_destiny(cb_aux, x_position, y_position, x_position - 1, y_position + aux, turn))
		{
			tmp = cb_aux.cb_matrix[y_position][x_position];
			cb_aux.cb_matrix[y_position][x_position] = 0;
			cb_aux.cb_matrix[y_position + aux][x_position - 1] = tmp;
			vec_aux.push_back(cb_aux);
		}
		cb_aux.copy(cb);
		if (verify_move_destiny(cb_aux, x_position, y_position, x_position + 1, y_position + aux, turn))
		{
			tmp = cb_aux.cb_matrix[y_position][x_position];
			cb_aux.cb_matrix[y_position][x_position] = 0;
			cb_aux.cb_matrix[y_position + aux][x_position + 1] = tmp;
			vec_aux.push_back(cb_aux);
		}
		
		cb_aux.copy(cb);
		if (verify_move_destiny(cb_aux, x_position, y_position, x_position - 2, y_position + aux * 2, turn))
		{
			tmp = cb_aux.cb_matrix[y_position][x_position];
			cb_aux.cb_matrix[y_position][x_position] = 0;
			cb_aux.cb_matrix[y_position + aux*2][x_position - 2] = tmp;
			vec_aux.push_back(cb_aux);
		}
		cb_aux.copy(cb);
		if (verify_move_destiny(cb_aux, x_position, y_position, x_position + 2, y_position + aux * 2, turn))
		{
			tmp = cb_aux.cb_matrix[y_position][x_position];
			cb_aux.cb_matrix[y_position][x_position] = 0;
			cb_aux.cb_matrix[y_position + aux * 2][x_position + 2] = tmp;
			vec_aux.push_back(cb_aux);
		}
		
		return vec_aux;
	}

	vector<checkerboard> possibilities(checkerboard cb, bool turn)
	{
		vector<checkerboard> vec;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{	
				if (turn and cb.cb_matrix[i][j] == 6)
				{
					vector<checkerboard> aux = calculate_moves(cb, j, i, turn);
					for (int i = 0, s = aux.size(); i < s; ++i)
						vec.push_back(aux[i]);
				}
				else if (!turn and cb.cb_matrix[i][j] == 9)
				{
					vector<checkerboard> aux = calculate_moves(cb, j, i, turn);
					for (int i = 0, s = aux.size(); i < s; ++i)
						vec.push_back(aux[i]);
				}
			}
		}
		return vec;
	}

	void make_possibilities(bool t)
	{
		vector<node*> vec;
		vector<node*> child;
		vec.push_back(*calculate);
		for (int i = 0; i < deep; ++i, t = !t)
		{
			while (!vec.empty())
			{
				vector<checkerboard> aux = possibilities(vec[0]->cb, t);
				for (int j = 0, s2 = aux.size(); j < s2; ++j)
				{
					vec[0]->children.push_back(new node(aux[j]));
				}
				child.push_back(vec[0]);
				vec.erase(vec.begin());
			}

			for (int j = 0, s = child.size(); j < s; ++j)
			{
				for (int e = 0, s1 = child[j]->children.size(); e < s1; ++e)
					vec.push_back(child[j]->children[e]);
			}
			child.clear();
		}
	}

	int calculate_value(node *n, bool mm)
	{
		/*int a = 0, b = 0;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (n->cb.cb_matrix[i][j] == 9)
					a++;
				else if (n->cb.cb_matrix[i][j] == 6)
					b++;
			}
		}
		return b - a;*/
		return possibilities(n->cb, mm).size();
	}

	node* _minmax(node *&n, bool mm)
	{
		if (!n->children.size())
		{
			n->value = calculate_value(n, mm);
			return n;
		}

		if (mm)
		{
			node *max = _minmax(n->children[0], !mm), *aux;
			int index = 0;
			for (int i = 1, s = n->children.size(); i < s; ++i)
			{
				aux = _minmax(n->children[i], !mm);
				if (max->value < aux->value)
				{
					index = i;
					max = aux;
				}
			}
			n->value = max->value;
			return n->children[index];
		}

		else
		{
			node* min = _minmax(n->children[0], !mm), * aux;
			int index = 0;
			for (int i = 1, s = n->children.size(); i < s; ++i)
			{
				aux = _minmax(n->children[i], !mm);
				if (min->value > aux->value)
				{
					index = i;
					min = aux;
				}
			}
			n->value = min->value;
			return n->children[index];
		}
	}

	checkerboard minmax(checkerboard cb_actual)
	{
		head = new node(cb_actual);
		
		make_possibilities(false);
		
		return _minmax(head, true)->cb;
	}

	// turn = 1 -> jugador
	// turn = 0 -> IA
};


struct checkers
{
	checkerboard cb;
	bool player_turn_begin;
	bool player_turn;
	int player_count_capture = 0;
	int AI_count_capture = 0;
	ktree IA;

	checkers(bool turn, int d)
	{
		player_turn_begin = turn;
		if (!turn)
		{
			cb.cb_matrix[2][1] = 0;
			cb.cb_matrix[3][2] = 9;
		}
		player_turn = true;
		IA.change_deep(d);
	}

	bool between(int index)
	{
		return index > -1 and index < cb.size_board;
	}

	void capture(int token_x_position, int token_y_position, int move_x_position, int move_y_position, bool turn)
	{
		int enemy_x_position = (token_x_position + move_x_position) / 2;
		int enemy_y_position = (token_y_position + move_y_position) / 2;

		if (turn and cb.cb_matrix[enemy_y_position][enemy_x_position] == 9) //jugador
		{
			cb.cb_matrix[enemy_y_position][enemy_x_position] = 0;
			player_count_capture++;

		}
		else if (cb.cb_matrix[enemy_y_position][enemy_x_position] == 6) // IA
		{
			cb.cb_matrix[enemy_y_position][enemy_x_position] = 0;
			AI_count_capture++;
		}
	}

	bool verify_move_destiny(int token_x_position, int token_y_position, int move_x_position, int move_y_position)
	{
		if (between(move_x_position) and between(move_y_position))
		{
			int x_aim = abs(token_x_position - move_x_position);
			int y_aim = token_y_position - move_y_position;

			if (player_turn == true and y_aim < 0 or player_turn == false and y_aim > 0)
				return false;

			if (x_aim == 1 and y_aim == 1)
				return !cb.cb_matrix[move_y_position][move_x_position];

			else if (x_aim == 2 and y_aim == 2 and !cb.cb_matrix[move_y_position][move_x_position])
			{
				capture(token_x_position, token_y_position, move_x_position, move_y_position, player_turn);
				return true;
			}
		}
		return false;
	}

	bool verify_token(int x_position, int y_position)
	{
		if (!between(x_position) or !between(y_position))
			return false;
		if (cb.cb_matrix[y_position][x_position] == 6)
			return true;
		if (cb.cb_matrix[y_position][x_position] == 9)
			return true;
		return false;
	}

	bool player_move(int token_x_position, int token_y_position, int move_x_position, int move_y_position)
	{
		if (verify_move_destiny(token_x_position, token_y_position, move_x_position, move_y_position)
			and verify_token(token_x_position, token_y_position))
		{
			int tmp = cb.cb_matrix[token_y_position][token_x_position];
			cb.cb_matrix[token_y_position][token_x_position] = 0;
			cb.cb_matrix[move_y_position][move_x_position] = tmp;
			return true;
		}
		return false;
	}

	void PC_move()
	{
		cb.copy(IA.minmax(cb));
	}
};

int main()
{
	int token_x_position, token_y_position, move_x_position, move_y_position, option;
	checkers game(0, 5);
	// primer parámetro
	// 0 -> inicia IA
	// 1 -> inicia Usuario
	// segundo parámetro
	// nivel de la profundidad
	game.player_turn = true;

	while (true)
	{
		game.cb.print_checkerboard(game.player_turn_begin);

		cout << "Opciones: " << endl;
		cout << "[1] mover ficha" << endl;
		cout << "[2] rendirse" << endl;
		cout << "ingrese el numero de la opcion: "; cin >> option;

		if (option == 1)
		{
			system("cls");
			game.cb.print_checkerboard(game.player_turn_begin);

			cout << "Ingrese la posicion de la ficha que quiere mover, \"x\" e \"y\" respectivamente" << endl;
			cin >> token_x_position; cin >> token_y_position; cout << endl << endl;
			cout << "Ingrese la posicion del destino, \"x\" e \"y\" respectivamente" << endl;
			cin >> move_x_position; cin >> move_y_position; cout << endl << endl;

			bool m = game.player_move(token_x_position, token_y_position, move_x_position, move_y_position);
			if (m) game.PC_move();
			system("cls");
		}
		else if (option == 2)
		{
			system("cls");
			break;
		}
		else
		{
			system("cls");
		}
	}
	cout << "GRACIAS POR JUGAR :D" << endl << endl;
	/*bool t = true;
	checkers game(t, 3);

	game.cb.cb_matrix[5][0] = 0;
	game.cb.cb_matrix[5][2] = 0;
	game.cb.cb_matrix[5][4] = 0;
	game.cb.cb_matrix[5][6] = 0;
	game.cb.cb_matrix[3][0] = 6;
	game.cb.cb_matrix[3][2] = 6;
	game.cb.cb_matrix[3][4] = 6;
	game.cb.cb_matrix[3][6] = 6;

	game.IA.head = new node(game.cb);

	game.IA.make_possibilities(!t);
	cout << game.IA.head->children.size();
	
	for (int i = 0, s = game.IA.head->children.size(); i < s; ++i)
	{
		game.IA.head->children[i]->cb.print_checkerboard(t);
	}*/

	//checkers game(0, 3);
	////vector<checkerboard> vec = game.IA.calculate_moves(game.cb, 0, 5, true);
	//vector<checkerboard> vec = game.IA.possibilities(game.cb, false);

	//game.cb.print_checkerboard(0);
	//for (int i = 0; i < vec.size(); ++i)
	//{
	//	cout << "posibilidad " << i << ": " << endl;
	//	vec[i].print_checkerboard(0);
	//}
		
}