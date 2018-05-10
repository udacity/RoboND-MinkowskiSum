// Compile with: g++ minkowski-sum.cpp -o app -std=c++11
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Print 2D Vectors
void print2DVector(vector<vector<int> > vec)
{
    for (int i = 0; i < vec.size(); ++i) {
    cout << "(" ;
	for (int j = 0; j < vec[0].size(); ++j) {
            if (vec[i][j] >= 0)
            cout << vec[i][j] << "  ";
            else
            cout <<"\b"<< vec[i][j] << "  ";
            
        }
        cout << "\b\b)"<<endl;
    }
}

// Check for duplicates inside a 2D vector and delete them
vector<vector<int> > delete_duplicate(vector<vector<int> > C)
{
    // Sort the C vector
    sort(C.begin(), C.end());
    // Initialize a non duplicated vector
    vector<vector<int> > Cn;
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
vector<vector<int> > minkowski_sum(vector<vector<int> > A, vector<vector<int> > B)
{
    vector<vector<int> > C;
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B.size(); j++) {
            // Compute the current sum
            vector<int> Ci = { A[i][0] + B[j][0], A[i][1] + B[j][1] };
            // Push it to the C vector
            C.push_back(Ci);
        }
    }
    C = delete_duplicate(C);
    return C;
}

int main()
{
    // Define the edges of each triangle
    vector<vector<int> > A(3, vector<int>(2));
    A = {{ 1, 0 }, { 0, 1 }, { 0, -1 },};
    vector<vector<int> > B(3, vector<int>(2));
    B = {{ 0, 0 }, { 1, 1 }, { 1, -1 },};

    // Compute the minkowski sum of the two triangles
    vector<vector<int> > C;
    C = minkowski_sum(A, B);

    print2DVector(C);

    return 0;
}

