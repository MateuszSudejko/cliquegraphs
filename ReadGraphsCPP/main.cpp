#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <chrono>
#include <random>
#include <tuple>
#include <string>
#include <utility>

using namespace std;

void findCliques(vector<vector<int>>& matrix, vector<int>& R, vector<int>& P, vector<int>& X, vector<vector<int>>& cliques) {
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
    } else {
        for (size_t node : P) {
            vector<int> neighbors;
            for (size_t i = 0; i < matrix[node].size(); ++i) {
                if (matrix[node][i] == 1) {
                    neighbors.push_back(i);
                }
            }
            vector<int> intersectionP;
            vector<int> intersectionX;
            set_intersection(P.begin(), P.end(), neighbors.begin(), neighbors.end(), back_inserter(intersectionP));
            set_intersection(X.begin(), X.end(), neighbors.begin(), neighbors.end(), back_inserter(intersectionX));

            R.push_back(node);
            findCliques(matrix, R, intersectionP, intersectionX, cliques);
            R.pop_back();

            auto it = find(P.begin(), P.end(), node);
            if (it != P.end()) {
                P.erase(it);
            }
            X.push_back(node);
        }
    }
}

vector<vector<int>> bronKerboschDirected2(vector<vector<int>>& matrix) {
    vector<vector<int>> cliques;
    vector<vector<int>> tmp = matrix;
    matrix = vector<vector<int>>(matrix.size(), vector<int>(matrix.size(), 0));

    for (size_t i = 0; i < tmp.size(); ++i) {
        for (size_t j = 0; j < tmp[i].size(); ++j) {
            if (tmp[i][j] == 1 && tmp[j][i] == 1) {
                matrix[i][j] = 1;
            }
        }
    }

    vector<int> R;
    vector<int> P(matrix.size());
    iota(P.begin(), P.end(), 0); // Fill P with values 0 to matrix.size()-1
    vector<int> X;

    findCliques(matrix, R, P, X, cliques);

    return cliques;
}

vector<vector<vector<int>>> readGraphFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "File not found or unable to open the file." << endl;
        exit(1);
    }

    int numGraphs;
    file >> numGraphs; // Read the number of graphs

    vector<vector<vector<int>>> graphs;
    for (int graphCounter = 0; graphCounter < numGraphs; ++graphCounter) {
        int numVertices;
        file >> numVertices; // Read the number of vertices for the current graph

        vector<vector<int>> graphMatrix(numVertices, vector<int>(numVertices, 0));
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                file >> graphMatrix[i][j];
            }
        }

        graphs.push_back(move(graphMatrix)); // Move the graphMatrix into graphs vector
    }

    file.close();
    return graphs;
}

int main() {
    cout<<"Provide the full path to your file with graphs";
    string filename; // Replace with your file name
    cin>>filename;
    vector<vector<vector<int>>> graphs = readGraphFile(filename);

    for (size_t j = 0; j < graphs.size(); ++j) {
        vector<vector<int>>& graphMatrix = graphs[j];
        if (!graphMatrix.empty()) {
            vector<vector<int>> cliques = bronKerboschDirected2(graphMatrix);

            int maxSize = 0;
            for (size_t k = 0; k < cliques.size(); ++k) {
                int currentSize = cliques[k].size();
                if (currentSize > maxSize) {
                    maxSize = currentSize;
                }
            }

            cout << "Max clique size for graph " << j + 1 << ": " << maxSize << endl;
        }
    }

    return 0;
}
