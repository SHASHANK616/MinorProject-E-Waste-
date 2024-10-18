//tsp with path
#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

const int INF = INT_MAX;
int n, vehicleCapacity;
vector<vector<int>> dist; // Adjacency matrix for city distances
vector<int> waste; // E-waste at each node

// TSP function to find the minimum path using Dynamic Programming and Bitmasking
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

// Function to find the path taken in the TSP solution
void findTSPPath(int mask, int pos, vector<vector<int>>& dp, vector<int>& path) {
    if (mask == (1 << n) - 1) {
        return;
    }

    for (int city = 0; city < n; city++) {
        if (!(mask & (1 << city))) {
            int currentCost = dist[pos][city] + tsp(mask | (1 << city), city, dp);
            if (currentCost == dp[mask][pos]) {
                path.push_back(city);
                findTSPPath(mask | (1 << city), city, dp, path);
                break;
            }
        }
    }
}

// Main function to track vehicles and waste collection based on the TSP path
void eWasteTracker() {
    vector<vector<int>> dp(1 << n, vector<int>(n, -1)); // DP array for TSP
    vector<int> path;

    // Solve TSP from node 0 (depot) and get the optimal path
    tsp(1, 0, dp);
    path.push_back(0); // Start from the depot
    findTSPPath(1, 0, dp, path);

    cout << "Optimal TSP Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << endl;

    int vehicles = 0; // Count of vehicles used
    int currentCapacity = 0; // Current waste collected by the vehicle
    vector<vector<int>> vehiclePaths; // Store paths taken by each vehicle
    vector<int> currentVehiclePath; // Track the current vehicle's path

    // Simulate vehicle trips based on the waste at each node
    for (int i = 0; i < path.size(); i++) {
        int node = path[i];
        while (waste[node] > 0) {
            currentVehiclePath.push_back(node); // Add the current node to the vehicle's path
            if (currentCapacity + waste[node] <= vehicleCapacity) {
                currentCapacity += waste[node];
                waste[node] = 0; // Collected all waste from this node
            } else {
                waste[node] -= (vehicleCapacity - currentCapacity); // Partial waste collected
                currentCapacity = vehicleCapacity; // Vehicle is full
            }

            // Once vehicle is full, it returns to depot
            if (currentCapacity == vehicleCapacity) {
                currentVehiclePath.push_back(0); // Vehicle returns to the depot
                vehiclePaths.push_back(currentVehiclePath); // Store the current vehicle's path
                currentVehiclePath.clear(); // Start a new vehicle
                currentCapacity = 0; // A new vehicle starts
                vehicles++;
            }
        }
    }

    // If the last vehicle still has capacity, it returns to the depot
    if (!currentVehiclePath.empty()) {
        currentVehiclePath.push_back(0); // Return to the depot
        vehiclePaths.push_back(currentVehiclePath); // Store the last vehicle's path
        vehicles++;
    }

    // Display the total number of vehicles used
    cout << "Total vehicles required: " << vehicles << endl;

    // Display the path taken by each vehicle
    for (int v = 0; v < vehiclePaths.size(); v++) {
        cout << "Vehicle " << v + 1 << " Path: ";
        for (int j = 0; j < vehiclePaths[v].size(); j++) {
            cout << vehiclePaths[v][j] << " ";
        }
        cout << endl;
    }
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
