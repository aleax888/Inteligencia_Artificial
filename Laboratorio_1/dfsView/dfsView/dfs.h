#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

struct point {
    float x, y;
    bool st; //state
    point(float a, float b) :x(a), y(b) {}
    point() {}
};

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(1, 625);//random end_point
int start = 0, finish = dis(gen), pt_del = 24;//path && points to delete
vector<int> ptsDel;
const int num_points = 25 * 25;
vector<point> points(num_points);
vector<int> graph[25 * 25]; ///matriz

bool DFS(vector<int>& prev, vector<int>& dist) {
    stack<int> s;
    vector<bool> visited(num_points, 0);
    dist = vector<int>(num_points, 10000); //inicializar con un max
    prev = vector<int>(num_points, -1); //inicializar con -1
    visited[start] = 1; //nodo inicial se marca como visitado
    dist[start] = 0; //distancia inicial es 0, porque va de inicio a inicio
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();
        for (int i = 0; i < graph[u].size(); i++) {
            if (!visited[graph[u][i]] && points[graph[u][i]].st) {
                visited[graph[u][i]] = 1;
                dist[graph[u][i]] = dist[u] + 1;
                prev[graph[u][i]] = u;
                s.push(graph[u][i]);
                if (graph[u][i] == finish)
                    return 1;
            }
        }
    }
    return false;
}
