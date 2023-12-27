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

int greedyRandom(vector<vector<int>>& matrix) {
    int max = 0;
    vector<int> R;
    vector<int> P(matrix.size());
    iota(P.begin(), P.end(), 0); // Fill P with values 0 to matrix.size()-1

    while (!P.empty()) {
        vector<int> N = P;
        while (!N.empty()) {
            int v_index = rand() % N.size(); // Randomly select an index from N
            int v = N[v_index]; // Retrieve the value at the selected index
            vector<int> neighbors;
            for (size_t i = 0; i < matrix[v].size(); ++i) {
                if (matrix[v][i] == 1 && matrix[i][v] == 1) {
                    neighbors.push_back(i);
                }
            }
            vector<int> intersectionN;
            set_intersection(N.begin(), N.end(), neighbors.begin(), neighbors.end(), back_inserter(intersectionN));
            N = intersectionN; // Intersect N with the neighborhood of v
            R.push_back(v); // Add v to R
        }

        sort(R.begin(), R.end());
        R.erase(unique(R.begin(), R.end()), R.end());

        if (R.size() > static_cast<size_t>(max)) {
            max = R.size();
        }

        // Remove elements in R from P
        vector<int> differenceP;
        set_difference(P.begin(), P.end(), R.begin(), R.end(), back_inserter(differenceP));
        P = differenceP;

        R.clear(); // Reset R to empty for the next iteration
    }

    return max;
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
    int reps = 20; // Number of repetitions for greedyRandom algorithm

    cout<<"How many graphs do you want to simulate?";
    cin>>N;
    cout<<"How many edges do you want to have in the graphs? (give number between 0-1)";
    cin>>probability;
    cout<<"How many vertices do you want to have at least in a graph?";
    cin>>lowerBoundVert;
    cout<<"How many vertices do you want to have at most in a graph?";
    cin>>higherBoundVert;
    cout<<"How many reps do you want to have for greedyRandom function?";
    cin>>reps;

    vector<double> T1(N);
    vector<double> T2(N);
    vector<double> err(N);

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

        // Plotting omitted for simplicity

        vector<vector<int>> graphMatrix(numVertices, vector<int>(numVertices, 0));
        for (const auto& edge : edges) {
            graphMatrix[edge[0]][edge[1]] = 1;
            graphMatrix[edge[1]][edge[0]] = 1;
        }

        auto start1 = chrono::steady_clock::now();
        vector<vector<int>> cliques = bronKerboschDirected2(graphMatrix);
        int maxSize = 0;
        for (const auto& clique : cliques) {
            int currentSize = clique.size();
            if (currentSize > maxSize) {
                maxSize = currentSize;
            }
        }
        auto end1 = chrono::steady_clock::now();
        T1[i] = chrono::duration<double>(end1 - start1).count();

        auto start2 = chrono::steady_clock::now();
        int max = 0;
        for (int j = 0; j < reps; ++j) {
            int newMax = greedyRandom(graphMatrix);
            if (newMax > max) {
                max = newMax;
            }
        }
        auto end2 = chrono::steady_clock::now();
        T2[i] = chrono::duration<double>(end2 - start2).count();

        err[i] = static_cast<double>(maxSize - max) / maxSize;
    }

    cout << "Average T1:" << calculateMean(T1, N) << endl;
    /*for (double time : T1) {
        cout << time << " ";
    }*/
    cout << endl;

    cout << "Average T2:" << calculateMean(T2, N) << endl;
    /*for (double time : T2) {
        cout << time << " ";
    }*/
    cout << endl;

    cout << "Error rate:" << calculateMean(err, N) << endl;
    /*for (double e : err) {
        cout << e << " ";
    }*/
    cout << endl;

    return 0;
}
