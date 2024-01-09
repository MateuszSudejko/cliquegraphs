#include <iostream>
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

    cout << "Adjacency Matrix:" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

double calculateMean(vector<double> arr, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum / size;
}

int main() {
    int N = 10; // Number of graphs to generate
    double probability = 0.5; // Probability of an edge between two vertices
    int lowerBoundVert = 40;
    int higherBoundVert = 50; // Boundaries for number of generated vertices in graphs

    cout<<"How many tests for pairs of graphs do you want to simulate?";
    cin>>N;
    cout<<"How much probability should there be for an edge to be between any two vertices? (give number between 0-1)";
    cin>>probability;
    cout<<"How many vertices do you want to have at least in a graph?";
    cin>>lowerBoundVert;
    cout<<"How many vertices do you want to have at most in a graph?";
    cin>>higherBoundVert;

    vector<double> T1(N);

    srand(static_cast<unsigned int>(time(NULL))); // Seed for random number generation

    for (int i = 0; i < N; ++i) {
        int numVertices = rand() % (higherBoundVert - lowerBoundVert + 1) + lowerBoundVert;
        vector<string> nodes(numVertices);
        for (int j = 0; j < numVertices; ++j) {
            nodes[j] = to_string(j + 1);
        }

        vector<vector<int>> edges;
        for (int j = 0; j < numVertices; ++j) {
            for (int k = 0; k < numVertices; ++k) {
                if (j != k && ((double)rand() / RAND_MAX) < probability) {
                    edges.push_back({j, k});
                }
            }
        }

        vector<vector<int>> graphMatrix(numVertices, vector<int>(numVertices, 0));
        for (const auto& edge : edges) {
            graphMatrix[edge[0]][edge[1]] = 1;
            graphMatrix[edge[1]][edge[0]] = 1;
        }

        int numVertices2 = rand() % (higherBoundVert - lowerBoundVert + 1) + lowerBoundVert;
        vector<string> nodes2(numVertices2);
        for (int j = 0; j < numVertices2; ++j) {
            nodes2[j] = to_string(j + 1);
        }

        vector<vector<int>> edges2;
        for (int j = 0; j < numVertices2; ++j) {
            for (int k = 0; k < numVertices2; ++k) {
                if (j != k && ((double)rand() / RAND_MAX) < probability) {
                    edges2.push_back({j, k});
                }
            }
        }

        vector<vector<int>> graphMatrix2(numVertices2, vector<int>(numVertices2, 0));
        for (const auto& edge : edges2) {
            graphMatrix2[edge[0]][edge[1]] = 1;
            graphMatrix2[edge[1]][edge[0]] = 1;
        }

        vector<vector<vector<int>>> graphs;
        graphs.push_back(graphMatrix);
        graphs.push_back(graphMatrix2);

        auto start1 = chrono::steady_clock::now();
        vector<vector<int>> mcs = maximumCommonSubgraph(graphs[0], graphs[1]);
        auto end1 = chrono::steady_clock::now();
        T1[i] = chrono::duration<double>(end1 - start1).count();

        cout<<"Test "<<i+1<<endl<<"Graph1 ";
        displayGraph(graphMatrix);
        cout<<"Graph2 ";
        displayGraph(graphMatrix2);
        cout<<"MCS ";
        displayGraph(mcs);
    }

    cout << "Average T1:" << calculateMean(T1, N) << endl;
    /*for (double time : T1) {
        cout << time << " ";
    }*/
    cout << endl;

    cout<<"Finish?";
    int n;
    cin>>n;

    return 0;
}
