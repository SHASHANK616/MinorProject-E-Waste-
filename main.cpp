// initials

#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

const int INF = INT_MAX;
int n, vehicleCapacity;
vector<vector<int>> dist; // Adjacency matrix for city distances
vector<int> waste; // E-waste at each node

// Function to find the minimum path using TSP (simplified for now)
int tsp(int mask, int pos, vector<vector<int>>& dp) {
    if (mask == (1 << n) - 1) {
        return dist[pos][0]; // Return to the depot
    }
    if (dp[mask][pos] != -1) return dp[mask][pos];

    int ans = INF;
    for (int city = 0; city < n; city++) {
        if (!(mask & (1 << city))) {
            int newAns = dist[pos][city] + tsp(mask | (1 << city), city, dp);
            ans = min(ans, newAns);
        }
    }

    return dp[mask][pos] = ans;
}

// Main function to track vehicles and waste collection
void eWasteTracker() {
    vector<vector<int>> dp(1 << n, vector<int>(n, -1)); // DP array for TSP
    int minPathDistance = tsp(1, 0, dp); // Start TSP from node 0 (depot)

    int vehicles = 0; // Count of vehicles used
    int currentCapacity = 0; // Current waste collected by the vehicle

    // Simulate vehicle trips based on the waste at each node
    for (int i = 0; i < n; i++) {
        while (waste[i] > 0) {
            if (currentCapacity + waste[i] <= vehicleCapacity) {
                currentCapacity += waste[i];
                waste[i] = 0; // Collected all waste from this node
            } else {
                waste[i] -= (vehicleCapacity - currentCapacity); // Partial waste collected
                currentCapacity = vehicleCapacity; // Vehicle is full
            }

            // Once vehicle is full, it returns to depot
            vehicles++;
            currentCapacity = 0; // A new vehicle starts
        }
    }

    cout << "Total vehicles required: " << vehicles << endl;
}

int main() {
    cout << "Enter the number of nodes (locations): ";
    cin >> n;

    dist = vector<vector<int>>(n, vector<int>(n)); // Distance matrix
    waste = vector<int>(n); // Waste at each node

    cout << "Enter the distance matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> dist[i][j];
        }
    }

    cout << "Enter the e-waste weight at each node:\n";
    for (int i = 0; i < n; i++) {
        cin >> waste[i];
    }

    cout << "Enter vehicle capacity: ";
    cin >> vehicleCapacity;

    eWasteTracker();

    return 0;
}
