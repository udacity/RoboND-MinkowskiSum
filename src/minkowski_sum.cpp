#include <iostream>
#include <vector>
#include <algorithm>
#include "../lib/matplotlibcpp.h"
#include <math.h>

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

// Check for duplicate coordinates inside a 2D vector and delete them
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

// Compute the centroid of a polygon
vector<double> compute_centroid(vector<vector<double> > vec)
{
    vector<double> centroid(2);
    double centroid_x=0, centroid_y=0;
    for (int i = 0; i < vec.size(); i++) {
        centroid_x += vec[i][0];
        centroid_y += vec[i][1];
    }
    centroid[0] = centroid_x / vec.size();
    centroid[1] = centroid_y / vec.size();
    return centroid;
}

// Compute the angle of each point with respect to the centroid and append in a new column
// Resulting vector[xi,yi,anglei]
vector<vector<double> > compute_angle(vector<vector<double> > vec)
{
    vector<double> centroid = compute_centroid(vec);
    double prec = 0.0001;
    for (int i = 0; i < vec.size(); i++) {
        double dy = vec[i][1] - centroid[1];
        double dx = vec[i][0] - centroid[0];
        // If the point is the centroid then delete it from the vector
        if (abs(dx) < prec && abs(dy) < prec) {
            vec.erase(vec.begin() + i);
        }
        else {
            // compute the centroid-point angle
            double theta = (atan2(dy, dx) * 180) / M_PI;
            // append it to the vector in a 3rd column
            vec[i].push_back(theta);
        }
    }
    return vec;
}

// Sort the vector in increasing angle (clockwise) for plotting
vector<vector<double> > sort_vector(vector<vector<double> > vec)
{
    vector<vector<double> > sorted_vec = compute_angle(vec);
    // Change the 0 angle to 90 degrees
    for (int i = 0; i < sorted_vec.size(); i++) {
        if (sorted_vec[i][2] == 0)
            sorted_vec[i][2] = 90.0;
    }
    // Sort with respect to the 3rd column(angles)
    sort(sorted_vec.begin(),
        sorted_vec.end(),
        [](const vector<double>& a, const vector<double>& b) {
            return a[2] < b[2];
        });
    return sorted_vec;
}

// Process the shapes and plot them
void plot(vector<vector<double> > vec, string color)
{
    // Sort the vector coordinates in clockwise
    vector<vector<double> > sorted_vec;
    sorted_vec = sort_vector(vec);
    // Add the first element to the end of the vector
    sorted_vec.push_back(sorted_vec[0]);
    // Loop through vector original size
    for (int i = 0; i < sorted_vec.size() - 1; i++) {
        // Connect coordinate point and plot the lines (x1,x2)(y1,y2)
        plt::plot({ sorted_vec[i][0], sorted_vec[i + 1][0] }, { sorted_vec[i][1], sorted_vec[i + 1][1] }, color);
    }
}

// Translate the configuration space toward the obstacle
vector<vector<double> > shift_space(vector<vector<double> > B, vector<vector<double> > C)
{
    // Compute the obstacle and space centroids
    vector<double> centroid_obstacle = compute_centroid(B);
    vector<double> centroid_space = compute_centroid(C);
    // Compute the translations deltas
    double dx = centroid_space[0] - centroid_obstacle[0];
    double dy = centroid_space[1] - centroid_obstacle[1];
    // Translate the space
    for (int i = 0; i < C.size(); i++) {
        C[i][0] = C[i][0] - dx;
        C[i][1] = C[i][1] - dy;
    }
    return C;
}

// Draw A, B and C shapes
void draw_shapes(vector<vector<double> > A, vector<vector<double> > B, vector<vector<double> > C)
{
    //Graph Format
    plt::title("Minkowski Sum");
    plt::xlim(-5, 5);
    plt::ylim(-5, 5);
    plt::grid(true);

    // Draw triangle A
    plot(A, "b-");

    // Draw triangle B
    plot(B, "r-");

    // Draw configuration space C
    // Trasnlate the C space
    vector<vector<double> > shifted_C = shift_space(B, C);
    plot(shifted_C, "y-");
    // Plot the original C shape
    plot(C, "g-");
    
    //Save the image and close the plot
    plt::save("../images/Minkowski_Sum.png");
    plt::clf();
}

int main()
{
    // Define the coordinates of triangle A and B in 2D vectors
    vector<vector<double> > A(3, vector<double>(2));
    // Robot A
    A = {
        { 0, -1 }, { 0, 1 }, { 1, 0 },
    };
    vector<vector<double> > B(3, vector<double>(2));
    // Obstacle B
    B = {
        { 0, 0 }, { 1, 1 }, { 1, -1 },
    };
    
    // Translating Robot toward the obstacle
    A=shift_space(B,A);
    
    // Compute the Minkowski Sum of triangle A and B
    vector<vector<double> > C;
    C = minkowski_sum(A, B);

    // Print the resulting vector
    print2DVector(C);

    // Draw all the shapes
    draw_shapes(A, B, C);
    
    return 0;
}
