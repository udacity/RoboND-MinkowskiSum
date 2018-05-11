#include <iostream>
#include <vector>
#include <algorithm>
#include "src/matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

// Print 2D vectors coordinate values
void print2DVector(vector<vector<double> > vec)
{
    for (int i = 0; i < vec.size(); ++i) {
        cout << "(";
        for (int j = 0; j < vec[0].size(); ++j) {
            if (vec[i][j] >= 0)
                cout << vec[i][j] << "  ";
            else
                cout << "\b" << vec[i][j] << "  ";
        }
        cout << "\b\b)" << endl;
    }
}

// Check for duplicate inside a 2D vector and delete them
vector<vector<double> > delete_duplicate(vector<vector<double> > C)
{
    // Sort the C vector
    sort(C.begin(), C.end());
    // Initialize a non duplicated vector
    vector<vector<double> > Cn;
    for (int i = 0; i < C.size() - 1; i++) {
        // Check if it's a duplicate coordinate
        if (C[i] != C[i + 1]) {
            Cn.push_back(C[i]);
        }
    }
    Cn.push_back(C[C.size() - 1]);
    return Cn;
}

// Compute the minkowski sum of two vectors
vector<vector<double> > minkowski_sum(vector<vector<double> > A, vector<vector<double> > B)
{
    vector<vector<double> > C;
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B.size(); j++) {
            // Compute the current sum
            vector<double> Ci = { A[i][0] + B[j][0], A[i][1] + B[j][1] };
            // Push it to the C vector
            C.push_back(Ci);
        }
    }
    C = delete_duplicate(C);
    return C;
}

// Workout the combination: http://rosettacode.org/wiki/Combinations#C.2B.2B
vector<vector<double> > generate_combinations(int N)
{
    int K = 2;
    vector<vector<double> > comb;
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    // print integers and permute bitmask
    do {
        vector<double> combi = { 0, 0 };
        int j = 0;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) {
                combi[j] = i;
                j++;
            }
        }
        comb.push_back(combi);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return comb;
}

// Process the combinations of a shape and plot its coordinates
void plot(vector<vector<double> > vec, string color)
{

    vector<vector<double> > comb;
    comb = generate_combinations(vec.size());
    for (int i = 0; i < comb.size(); i++) {
        if (abs(vec[comb[i][0]][1] - vec[comb[i][1]][1]) <= 2 && abs(vec[comb[i][0]][0] - vec[comb[i][1]][0]) <= 1)
            plt::plot({ vec[comb[i][0]][0], vec[comb[i][1]][0] }, { vec[comb[i][0]][1], vec[comb[i][1]][1] }, color);
    }
}

// Draw A, B and C shapes
void draw_shapes(vector<vector<double> > A, vector<vector<double> > B, vector<vector<double> > C)
{
    //Graph Format
    plt::title("Minkowski Sum");
    plt::xlim(-5, 5);
    plt::ylim(-5, 5);
    plt::grid(true);

    // Draw configuration space C
    plot(C, "g-");

    // Draw triangle A
    plot(A, "b-");

    // Draw triangle B
    plot(B, "r-");

    //Save the image and close the plot
    plt::save("./Images/Minkowski_Sum.png");
    plt::clf();
}

int main()
{
    // Define the coordinates of triangle A and B in 2D vectors
    vector<vector<double> > A(3, vector<double>(2));
    A = {
        { 1, 0 }, { 0, 1 }, { 0, -1 },
    };
    vector<vector<double> > B(3, vector<double>(2));
    B = {
        { 0, 0 }, { 1, 1 }, { 1, -1 },
    };

    // Compute the Minkowski Sum of triangle A and B
    vector<vector<double> > C;
    C = minkowski_sum(A, B);

    // Print the resulting vector
    print2DVector(C);

    // Draw all the shapes
    draw_shapes(A, B, C);

    return 0;
}

