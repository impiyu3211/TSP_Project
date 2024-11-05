#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric> // For std::iota

using namespace std;

struct City {
    double x, y;
};

double euclideanDistance(const City &a, const City &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calculateTourCost(const vector<int> &tour, const vector<City> &cities) {
    double cost = 0.0;
    for (size_t i = 0; i < tour.size(); i++) {
        cost += euclideanDistance(cities[tour[i]], cities[tour[(i + 1) % tour.size()]]);
    }
    return cost;
}

void swapCities(vector<int> &tour) {
    int i = rand() % tour.size();
    int j = rand() % tour.size();
    // Ensure different indices for swapping
    while (j == i) {
        j = rand() % tour.size();
    }
    swap(tour[i], tour[j]);
}

vector<int> simulatedAnnealing(const vector<City> &cities, int iterations, double startTemp, double alpha) {
    int n = cities.size();
    vector<int> currentTour(n);
    iota(currentTour.begin(), currentTour.end(), 0); // Initialize tour with 0, 1, ..., n-1
    
    double currentCost = calculateTourCost(currentTour, cities);
    vector<int> bestTour = currentTour;
    double bestCost = currentCost;

    double temperature = startTemp;

    for (int iter = 0; iter < iterations; iter++) {
        vector<int> newTour = currentTour;
        swapCities(newTour);
        double newCost = calculateTourCost(newTour, cities);

        if (newCost < currentCost || (static_cast<double>(rand()) / RAND_MAX) < exp((currentCost - newCost) / temperature)) {
            currentTour = newTour;
            currentCost = newCost;

            if (currentCost < bestCost) {
                bestTour = currentTour;
                bestCost = currentCost;
            }
        }

        temperature *= alpha; // Cooling schedule
        if (temperature < 1e-8) break; // Stop early if temperature is very low
    }

    return bestTour;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
    
    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<City> cities(numCities);
    
    // Input city coordinates
    for (int i = 0; i < numCities; ++i) {
        cout << "Enter coordinates for city " << i + 1 << " (x y): ";
        cin >> cities[i].x >> cities[i].y;
    }

    int iterations = 10000; // You can also take this as input if desired
    double startTemp = 10000.0; // Initial temperature
    double alpha = 0.995; // Cooling rate

    vector<int> optimalTour = simulatedAnnealing(cities, iterations, startTemp, alpha);
    
    cout << "Optimal Tour: ";
    for (int city : optimalTour) {
        cout << city + 1 << " "; // Adjusting index for user-friendly output
    }
    
    cout << "\nTotal Cost: " << calculateTourCost(optimalTour, cities) << endl;

    return 0;
}
