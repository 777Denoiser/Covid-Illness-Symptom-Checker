#include <jni.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream> // for stringstream
#include <fstream> // for file reading
#include <iostream> // for debugging
#include <limits> // for numeric limits

#include "ACOAlgorithm.h"

// Structures to hold city (node) data
struct City {
    int id;
    double x;
    double y;
};

// Structure to hold Edge Data
struct Edge {
    int from;
    int to;
    double weight;

    Edge(int from, int to, double weight) : from(from), to(to), weight(weight) {}
};

class AntColonyOptimization {
public:
    AntColonyOptimization(int numAnts, double evaporationRate, double alpha, double beta)
        : numAnts(numAnts), evaporationRate(evaporationRate), alpha(alpha), beta(beta) {}

    std::string run(const std::string& filePath) {
        // Digital ghost in the machine... let's parse the city data file.
        std::vector<City> cities;
        if (!parseCityData(filePath, cities)) {
            return "ERROR: Data corruption detected in city file.  Exiting.";
        }

        int numCities = cities.size();

        // Build the network of pheromone trails... every connection is a possibility.
        pheromone = std::vector<std::vector<double>>(numCities, std::vector<double>(numCities, 1.0));

        // Time to unleash the digital swarm...
        std::vector<int> bestPath;
        double bestPathLength = std::numeric_limits<double>::max(); // initialize best path length with max double value

        for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
            std::vector<std::vector<int>> antPaths(numAnts);
            std::vector<double> pathLengths(numAnts, 0.0); // Stores path length for each ant

            // Each digital ant charts a course...
            for (int ant = 0; ant < numAnts; ++ant) {
                antPaths[ant] = findPath(cities);
                pathLengths[ant] = calculatePathLength(antPaths[ant], cities);

                // Debugging: Print ant's path and length
                std::cout << "Ant " << ant << " Path: ";
                for (int cityId : antPaths[ant]) {
                    std::cout << cityId << " ";
                }
                std::cout << "Length: " << pathLengths[ant] << std::endl;

                if (pathLengths[ant] < bestPathLength) {
                    bestPathLength = pathLengths[ant];
                    bestPath = antPaths[ant];
                }
            }
            // Evaporate pheromones, then reinforce the best routes... survival of the fittest code.
            updatePheromones(antPaths, cities, pathLengths);

            std::cout << "Iteration " << iteration << " Best Path Length: " << bestPathLength << std::endl;
        }

        // Data stream complete.  Return the optimal route...
        return formatPathForOutput(bestPath);
    }

private:

    // Method to format the best path as a string
    std::string formatPathForOutput(const std::vector<int>& path) {
        std::stringstream ss;
        for (size_t i = 0; i < path.size(); ++i) {
            ss << path[i];
            if (i < path.size() - 1) {
                ss << ",";
            }
        }
        return ss.str();
    }

    // The digital ants sniff out a path through the city network...
    std::vector<int> findPath(const std::vector<City>& cities) {
        int startCity = rand() % cities.size();  //Ant starts at a random city
        std::vector<int> path;
        std::vector<bool> visited(cities.size(), false); // Keep track of visited cities

        path.push_back(startCity + 1);  // Add starting city to the path
        visited[startCity] = true; // Mark as visited

        int currentCity = startCity;

        while (path.size() < cities.size()) {
            int nextCity = selectNextCity(currentCity, cities, visited);
            if (nextCity == -1) {
                // No valid next city found, return incomplete path. Happens when a city has no unvisited neighbors.
                std::cout << "Incomplete path due to no valid next city." << std::endl;
                break;
            }
            path.push_back(nextCity + 1);  // City IDs are 1-based
            visited[nextCity] = true;
            currentCity = nextCity;
        }

        return path;
    }

    // Roulette wheel selection
    int selectNextCity(int currentCity, const std::vector<City>& cities, const std::vector<bool>& visited) {
        double totalProbability = 0.0;
        std::vector<double> probabilities;
        int numCities = cities.size();

        for (int i = 0; i < numCities; ++i) {
            if (!visited[i]) {
                double pheromoneLevel = pheromone[currentCity][i];
                double heuristicValue = calculateHeuristic(currentCity, i, cities);
                double probability = pow(pheromoneLevel, alpha) * pow(heuristicValue, beta);
                probabilities.push_back(probability);
                totalProbability += probability;
            } else {
                probabilities.push_back(0.0); // If already visited probability of 0
            }
        }

        // Normalizing probabilities
        if (totalProbability > 0.0) {
            for (int i = 0; i < numCities; ++i) {
                probabilities[i] /= totalProbability;
            }
        } else {
            //If no path is available, return -1
            std::cout << "All paths exhausted from city " << currentCity + 1 << "." << std::endl;
            return -1;
        }

        // Select next city based on probabilities
        double randomNumber = ((double)rand() / RAND_MAX);
        double cumulativeProbability = 0.0;

        for (int i = 0; i < numCities; ++i) {
            if (!visited[i]) {
                cumulativeProbability += probabilities[i];
                if (randomNumber <= cumulativeProbability) {
                    return i;  // Return index of the next city
                }
            }
        }

        // In case of any rounding errors return -1
        std::cout << "Roulette selection failed from city " << currentCity + 1 << "." << std::endl;
        return -1;
    }

    // Calculate path length
    double calculatePathLength(const std::vector<int>& path, const std::vector<City>& cities) {
        double length = 0.0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int cityIndex1 = path[i] - 1;
            int cityIndex2 = path[i + 1] - 1;
            length += calculateDistance(cities[cityIndex1], cities[cityIndex2]);
        }
        // Return to starting city
        length += calculateDistance(cities[path.back() - 1], cities[path[0] - 1]);
        return length;
    }

    // Distance between two cities
    double calculateDistance(const City& city1, const City& city2) {
        double dx = city1.x - city2.x;
        double dy = city1.y - city2.y;
        return sqrt(dx * dx + dy * dy);
    }

    // Heuristic value: inverse of distance.
    double calculateHeuristic(int city1Index, int city2Index, const std::vector<City>& cities) {
        return 1.0 / calculateDistance(cities[city1Index], cities[city2Index]);
    }

    // Update pheromones
    void updatePheromones(const std::vector<std::vector<int>>& antPaths, const std::vector<City>& cities, const std::vector<double>& pathLengths) {
        // Evaporate existing pheromones... memories fade in the digital world.
        for (int i = 0; i < pheromone.size(); ++i) {
            for (int j = 0; j < pheromone[i].size(); ++j) {
                pheromone[i][j] *= (1.0 - evaporationRate);
                if (pheromone[i][j] < 0.001) pheromone[i][j] = 0.001;
            }
        }

        // Each ant deposits pheromones on its path... reinforcing successful routes.
        for (int ant = 0; ant < numAnts; ++ant) {
            double pathLength = pathLengths[ant];
            for (size_t i = 0; i < antPaths[ant].size() - 1; ++i) {
                int cityIndex1 = antPaths[ant][i] - 1;
                int cityIndex2 = antPaths[ant][i + 1] - 1;
                pheromone[cityIndex1][cityIndex2] += (Q / pathLength);
                pheromone[cityIndex2][cityIndex1] = pheromone[cityIndex1][cityIndex2]; // Symmetric pheromone
            }
            // Pheromone deposit for returning to the starting city
            int startCityIndex = antPaths[ant][0] - 1;
            int endCityIndex = antPaths[ant].back() - 1;
            pheromone[endCityIndex][startCityIndex] += (Q / pathLength);
            pheromone[startCityIndex][endCityIndex] = pheromone[endCityIndex][startCityIndex];
        }
    }

    // Read city data from a file.  Format is "id x y" on each line.
    bool parseCityData(const std::string& filePath, std::vector<City>& cities) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "ERROR: Unable to access city data file." << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            City city;
            if (ss >> city.id >> city.x >> city.y) {
                cities.push_back(city);
            } else {
                std::cerr << "WARNING: Corrupted entry in city data file. Skipping line." << std::endl;
            }
        }

        file.close();
        if (cities.empty()) {
            std::cerr << "ERROR: No city data loaded. File may be empty or corrupted." << std::endl;
            return false;
        }
        return true;
    }

private:
    int numAnts;
    double evaporationRate;
    double alpha;
    double beta;

    std::vector<std::vector<double>> pheromone; // Pheromone matrix
    const int MAX_ITERATIONS = 100; // Limits the swarm's lifespan.
    const double Q = 100.0; // Pheromone deposit constant.
};

JNIEXPORT jstring JNICALL Java_Main_runACOAlgorithm(JNIEnv* env, jobject, jstring inputData) {
    const char* input = env->GetStringUTFChars(inputData, nullptr);
    std::string filePath(input);
    env->ReleaseStringUTFChars(inputData, input);

    AntColonyOptimization aco(50, 0.5, 1.0, 2.0); // Unleash 50 digital ants
    std::string result = aco.run(filePath);

    return env->NewStringUTF(result.c_str());
}
