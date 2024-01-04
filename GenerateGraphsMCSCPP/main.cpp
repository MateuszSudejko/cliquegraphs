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

using namespace std;

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

vector<vector<int>> modular_product(const vector<vector<int>>& graph1, const vector<vector<int>>& graph2) {
    int size = graph1.size();
    vector<vector<int>> result(size, vector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                // If there is an edge between i and k in graph1 and between j and k in graph2, add an edge between i and j in the result
                if (graph1[i][k] && graph2[j][k]) {
                    result[i][j] = 1;
                    break; // Break once an edge is found
                }
            }
        }
    }

    return result;
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

vector<vector<int>> MCS(int number_of_graphs, vector<vector<vector<int>>>& graphs) {
    vector<vector<int>> output;

    if (number_of_graphs == 0 || number_of_graphs == 1) {
        return output;
    }

    vector<vector<int>> graph1 = graphs[0];
    vector<vector<int>> graph2 = graphs[1];

    for (int k = 2; k < number_of_graphs; ++k) {
        auto modular = modular_product(graph1, graph2);
        auto all_cliques = bronKerboschDirected2(modular);

        int max_size = 0;
        int idx = 0;
        for (int i = 0; i < all_cliques.size(); ++i) {
            if (all_cliques[i].size() > max_size) {
                max_size = all_cliques[i].size();
                idx = i;
            }
        }

        auto max_clique = all_cliques[idx];
        vector<int> index_mapping_graph1;
        for (int i = 0; i < max_clique.size(); ++i) {
            index_mapping_graph1.push_back(modular[max_clique[i]][0]);
        }

        output = vector<vector<int>>(graph1.size(), vector<int>(graph1.size(), 0));
        for (int i = 0; i < graph1.size(); ++i) {
            for (int j = 0; j < graph1.size(); ++j) {
                if (find(index_mapping_graph1.begin(), index_mapping_graph1.end(), i) != index_mapping_graph1.end() &&
                    find(index_mapping_graph1.begin(), index_mapping_graph1.end(), j) != index_mapping_graph1.end()) {
                    output[i][j] = graph1[i][j];
                }
            }
        }

        vector<bool> isolatedVertices(graph1.size(), false);
        for (int i = 0; i < graph1.size(); ++i) {
            if (accumulate(output[i].begin(), output[i].end(), 0) == 0 &&
                accumulate(output.begin(), output.end(), 0, [i](int sum, const vector<int>& row) { return sum + row[i]; }) == 0) {
                isolatedVertices[i] = true;
            }
        }

        vector<vector<int>> temp;
        for (int i = 0; i < output.size(); ++i) {
            if (!isolatedVertices[i]) {
                vector<int> row;
                for (int j = 0; j < output.size(); ++j) {
                    if (!isolatedVertices[j]) {
                        row.push_back(output[i][j]);
                    }
                }
                temp.push_back(row);
            }
        }

        output = temp;
        graph1 = output;
        if (k + 1 < number_of_graphs) {
            graph2 = graphs[k + 1];
        }
    }

    return output;
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

    cout<<"How many graphs do you want to simulate?";
    cin>>N;
    cout<<"How many edges do you want to have in the graphs? (give number between 0-1)";
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
        vector<vector<int>> mcs = MCS(2, graphs);
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
