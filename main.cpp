#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


using namespace std;

// Function to calculate the number of vehicles required
int estimateVehicleRequirement(const vector<int>& weights, int capacity) {
    int total_weight = 0;
    for (int weight : weights) {
        total_weight += weight;
    }
    return (total_weight + capacity - 1) / capacity; 
}

// Function to calculate the distance for a given route in TSP
int calculateRouteDistance(const vector<vector<int>>& adj_matrix, const vector<int>& route) {
    int total_distance = 0;
    total_distance += adj_matrix[0][route[0]]; 
    for (size_t i = 0; i < route.size() - 1; ++i) {
        total_distance += adj_matrix[route[i]][route[i + 1]];
    }
    total_distance += adj_matrix[route.back()][0]; 
    return total_distance;
}

// Function to solve TSP (brute-force)
pair<vector<int>, int> tspRoute(const vector<vector<int>>& adj_matrix, const vector<int>& nodes) {
    vector<int> best_route;
    int min_distance = INT_MAX;

    vector<int> current_route = nodes;
    do {
        int current_distance = calculateRouteDistance(adj_matrix, current_route);
        if (current_distance < min_distance) {
            min_distance = current_distance;
            best_route = current_route;
        }
    } while (next_permutation(current_route.begin(), current_route.end()));

    return {best_route, min_distance};
}

// Function to schedule collection points (greedy)
vector<vector<int>> scheduleCollection(const vector<int>& weights, int capacity) {
    priority_queue<pair<int, int>> pq; 
    for (size_t i = 1; i < weights.size(); ++i) { 
        pq.push({weights[i], i}); 
    }

    vector<vector<int>> vehicles;
    vector<int> current_vehicle;
    int current_capacity = 0;

    while (!pq.empty()) {
        int weight = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (current_capacity + weight <= capacity) {
            current_vehicle.push_back(node);
            current_capacity += weight;
        } else {
            vehicles.push_back(current_vehicle);
            current_vehicle = {node};
            current_capacity = weight;
        }
    }

    if (!current_vehicle.empty()) {
        vehicles.push_back(current_vehicle);
    }

    return vehicles;
}

pair<int, vector<vector<int>>> eWasteCollection(int n, const vector<vector<int>>& adj_matrix, const vector<int>& weights, int capacity) {
    int num_vehicles = estimateVehicleRequirement(weights, capacity);

    vector<vector<int>> vehicle_schedules = scheduleCollection(weights, capacity);

    vector<vector<int>> routes;
    for (const auto& vehicle_nodes : vehicle_schedules) {
        auto [route, distance] = tspRoute(adj_matrix, vehicle_nodes);
        routes.push_back(route);
    }

    return {num_vehicles, routes};
}

int main() {
    int n = 11; // (locations + 1 depot)
    
    vector<vector<int>> adj_matrix = {
        {  0,   3,   5,   7,   9,   6,   8,  10,  12,  11,  13}, // Depot to others
        {  3,   0,   4,   2,   5,   9,   7,   8,  10,  12,   6},
        {  5,   4,   0,   1,   6,   8,  10,  12,  14,  16,  10}, 
        {  7,   2,   1,   0,   3,  10,   7,   5,  11,  13,  15}, 
        {  9,   5,   6,   3,   0,   2,   4,   6,   8,   9,  11},
        {  6,   9,   8,  10,   2,   0,   3,   1,   5,   7,   9},
        {  8,   7,  10,   7,   3,   0,   2,   6,   8,   8,   4}, 
        { 10,   8,  12,   5,   1,   2,   0,   3,   4,   4,   2}, 
        { 12,  10,  14,  11,   5,   6,   3,   0,   2,   4,   5}, 
        { 11,  12,  16,  13,   7,   8,   4,   2,   0,   4,   8}, 
        { 13,   6,  10,  15,   9,   8,   4,   4,   4,   0,   9}  
    };

    // Waste volume at each node (with 0 for the depot at node 0)
    vector<int> weights = {0, 20,13,21,26,15,23,25,1,7,17}; 

    int capacity = 50; 

    
    auto [num_vehicles, routes] = eWasteCollection(n, adj_matrix, weights, capacity);

    std::cout << "Minimum Number of vehicles required: " << num_vehicles << endl;
    std::cout << "Routes for each vehicle (starting and ending at depot):" << endl;
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "Vehicle " << i + 1 << ": Depot -> ";
        for (int node : routes[i]) {
            std::cout << node << " -> ";
        }
        std::cout << "Depot" << endl;
    }

    return 0;
}
