#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <chrono>
#include <random>
#include <tuple>
#include <string>
#include <utility>
#include <map>

using namespace std;

pair<vector<vector<int>>, vector<pair<int, int>>> modularProduct(vector<vector<int>> matrix1, vector<vector<int>> matrix2) {
    int nextVertex = 1;
    map<pair<int, int>, int> association;
    vector<pair<int, int>> edges;
    vector<pair<int, int>> indexMapping;

    for (int v1 = 0; v1 < matrix1.size(); v1++) {
        for (int v2 = 0; v2 < matrix2.size(); v2++) {
            for (int w1 = 0; w1 < matrix1.size(); w1++) {
                for (int w2 = 0; w2 < matrix2.size(); w2++) {
                    if (v1 != w1 && v2 != w2 && matrix1[v1][w1] == matrix2[v2][w2] && matrix1[w1][v1] == matrix2[w2][v2]) {
                        if (association.find({v1, v2}) == association.end()) {
                            association[{v1, v2}] = nextVertex;
                            indexMapping.push_back({v1, v2});
                            nextVertex++;
                        }
                        if (association.find({w1, w2}) == association.end()) {
                            association[{w1, w2}] = nextVertex;
                            indexMapping.push_back({w1, w2});
                            nextVertex++;
                        }
                        edges.push_back({association[{v1, v2}], association[{w1, w2}]});
                    }
                }
            }
        }
    }

    vector<vector<int>> adjacencyMatrix(nextVertex, vector<int>(nextVertex, 0));
    for (auto edge : edges) {
        adjacencyMatrix[edge.first][edge.second] = 1;
    }

    return {adjacencyMatrix, indexMapping};
}

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

vector<int> BronKerboschDirected(const vector<vector<int>>& graph) {
    vector<vector<int>> matrix = graph;
    vector<vector<int>> cliques = bronKerboschDirected2(matrix);
    vector<int> maxClique;

    size_t maxIndex = 0;
    size_t maxSize = 0;

    for (size_t i = 0; i < cliques.size(); ++i) {
        if (cliques[i].size() > maxSize) {
            maxIndex = i;
            maxSize = cliques[i].size();
        }
    }

    // Placeholder - pick the first found clique as the maximum clique
    if (!cliques.empty()) {
        maxClique = cliques[maxIndex];
    }

    return maxClique;
}

vector<vector<int>> submatrix(vector<vector<int>> matrix, vector<bool> keepRow, vector<bool> keepCol) {
    vector<vector<int>> output;
    for (int i = 0; i < matrix.size(); i++) {
        if (keepRow[i]) {
            vector<int> row;
            for (int j = 0; j < matrix[i].size(); j++) {
                if (keepCol[j]) {
                    row.push_back(matrix[i][j]);
                }
            }
            output.push_back(row);
        }
    }
    return output;
}

vector<vector<int>> maximumCommonSubgraph(vector<vector<int>> graph1, vector<vector<int>> graph2) {
    auto [modular, indexMapping] = modularProduct(graph1, graph2);
    auto maxClique = BronKerboschDirected(modular);

    vector<int> indexMappingGraph1;
    for (int i = 0; i < maxClique.size(); i++) {
        indexMappingGraph1.push_back(indexMapping[maxClique[i]].first);
    }

    vector<vector<int>> output(graph1.size(), vector<int>(graph1.size(), 0));
    for (int i = 0; i < graph1.size(); i++) {
        for (int j = 0; j < graph1.size(); j++) {
            if (find(indexMappingGraph1.begin(), indexMappingGraph1.end(), i) != indexMappingGraph1.end() &&
                find(indexMappingGraph1.begin(), indexMappingGraph1.end(), j) != indexMappingGraph1.end()) {
                output[i][j] = graph1[i][j];
            }
        }
    }

    vector<bool> isolatedVertices(graph1.size(), true);
    for (int i = 0; i < graph1.size(); i++) {
        for (int j = 0; j < graph1.size(); j++) {
            if (output[i][j] != 0 || output[j][i] != 0) {
                isolatedVertices[i] = false;
                break;
            }
        }
    }
    //output = submatrix(output, isolatedVertices, isolatedVertices);

    return output;
}

void displayGraph(const vector<vector<int>>& graph) {
    int numVertices = graph.size();

    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
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

    vector<vector<int>> mcs = maximumCommonSubgraph(graphs[0],graphs[1]);

    int s=2;

    while(s<graphs.size()){
        mcs = maximumCommonSubgraph(mcs,graphs[s]);
        s++;
    }

    cout<<"Maximum Common Subgraph adjacency matrix: "<<endl;

    displayGraph(mcs);

    return 0;
}
