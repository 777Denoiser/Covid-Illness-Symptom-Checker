#include <jni.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <limits>
#include <cmath> // Required for std::sqrt

#include "ACOAlgorithm.h"

// City structure to hold city data
struct City {
    int id;
    double x;
    double y;
};

class AntColonyOptimization {
public:
    AntColonyOptimization(int numAnts, double evaporationRate, double alpha, double beta)
        : numAnts(numAnts), evaporationRate(evaporationRate), alpha(alpha), beta(beta) {}

    std::string run(const std::string& filePath) {
        // Digital ghost in the machine... parse the city data file.
        std::vector<City> cities;
        if (!parseCityData(filePath, cities)) {
            return "ERROR: Data corruption detected in city file. Exiting.";
        }

        int numCities = cities.size();
        if (numCities < 2) {
            return "ERROR:  Insufficient city data (must have at least 2 cities).";
        }

        // Build the network of pheromone trails.
        pheromone = std::vector<std::vector<double>>(numCities, std::vector<double>(numCities, 1.0));

        // Time to unleash the digital swarm... find best Path Length
        std::vector<int> bestPath;
        double bestPathLength = std::numeric_limits<double>::max();

        for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
            std::vector<std::vector<int>> antPaths(numAnts);
            std::vector<double> pathLengths(numAnts, 0.0);

            // Each ant charts a course
            for (int ant = 0; ant < numAnts; ++ant) {
                antPaths[ant] = findPath(cities);
                pathLengths[ant] = calculatePathLength(antPaths[ant], cities);

                if (pathLengths[ant] < bestPathLength) {
                    bestPathLength = pathLengths[ant];
                    bestPath = antPaths[ant];
                }
            }
            updatePheromones(antPaths, cities, pathLengths);
            std::cout << "Iteration " << iteration << " Best Path Length: " << bestPathLength << std::endl;
        }
        return formatPathForOutput(bestPath); // Data stream complete. Return the optimal route
    }

private:
    // String format
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

    // Digtal ants find path through city
    std::vector<int> findPath(const std::vector<City>& cities) {
        int startCity = rand() % cities.size();
        std::vector<int> path;
        std::vector<bool> visited(cities.size(), false);

        path.push_back(startCity + 1);
        visited[startCity] = true;

        int currentCity = startCity;

        while (path.size() < cities.size()) {
            int nextCity = selectNextCity(currentCity, cities, visited);
            if (nextCity == -1) {
                std::cout << "Incomplete path due to no valid next city." << std::endl;
                break;
            }
            path.push_back(nextCity + 1);
            visited[nextCity] = true;
            currentCity = nextCity;
        }
        return path;
    }

    // Select next city based on probabilities
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
                probabilities.push_back(0.0); // Visited, probability to 0
            }
        }

        if (totalProbability > 0.0) {
            for (int i = 0; i < numCities; ++i) {
                probabilities[i] /= totalProbability;
            }
        } else {
            std::cout << "All paths exhausted from city " << currentCity + 1 << "." << std::endl;
            return -1;
        }

        // Roulette wheel random num
        double randomNumber = ((double)rand() / RAND_MAX);
        double cumulativeProbability = 0.0;

        for (int i = 0; i < numCities; ++i) {
            if (!visited[i]) {
                cumulativeProbability += probabilities[i];
                if (randomNumber <= cumulativeProbability) {
                    return i; // next city
                }
            }
        }
        std::cout << "Roulette selection failed from city " << currentCity + 1 << "." << std::endl;
        return -1;
    }

    // calculate path length
    double calculatePathLength(const std::vector<int>& path, const std::vector<City>& cities) {
        double length = 0.0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int cityIndex1 = path[i] - 1;
            int cityIndex2 = path[i + 1] - 1;
            length += calculateDistance(cities[cityIndex1], cities[cityIndex2]);
        }
        // Returns to start
        length += calculateDistance(cities[path.back() - 1], cities[path[0] - 1]);
        return length;
    }

    // dist between two points
    double calculateDistance(const City& city1, const City& city2) {
        double dx = city1.x - city2.x;
        double dy = city1.y - city2.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Inverse
    double calculateHeuristic(int city1Index, int city2Index, const std::vector<City>& cities) {
        return 1.0 / calculateDistance(cities[city1Index], cities[city2Index]);
    }

    // Update Phermones
    void updatePheromones(const std::vector<std::vector<int>>& antPaths, const std::vector<City>& cities, const std::vector<double>& pathLengths) {
        // phermones fade
        for (int i = 0; i < pheromone.size(); ++i) {
            for (int j = 0; j < pheromone[i].size(); ++j) {
                pheromone[i][j] *= (1.0 - evaporationRate);
                if (pheromone[i][j] < 0.001) pheromone[i][j] = 0.001;
            }
        }

        // Ants deposit
        for (int ant = 0; ant < numAnts; ++ant) {
            double pathLength = pathLengths[ant];
            for (size_t i = 0; i < antPaths[ant].size() - 1; ++i) {
                int cityIndex1 = antPaths[ant][i] - 1;
                int cityIndex2 = antPaths[ant][i + 1] - 1;
                pheromone[cityIndex1][cityIndex2] += (Q / pathLength);
                pheromone[cityIndex2][cityIndex1] = pheromone[cityIndex1][cityIndex2]; // Symmetric pheromone
            }
            // Start return deposit
            int startCityIndex = antPaths[ant][0] - 1;
            int endCityIndex = antPaths[ant].back() - 1;
            pheromone[endCityIndex][startCityIndex] += (Q / pathLength);
            pheromone[startCityIndex][endCityIndex] = pheromone[endCityIndex][startCityIndex];
        }
    }

    // Read file
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
    const int MAX_ITERATIONS = 100; //  Swarm's lifespan
    const double Q = 100.0; // constant.
};

JNIEXPORT jstring JNICALL Java_Main_runACOAlgorithm(JNIEnv* env, jobject, jstring inputData) {
    const char* input = env->GetStringUTFChars(inputData, nullptr);
    if (input == nullptr) {
        return env->NewStringUTF("ERROR: Input string is null."); // Handle potential null pointer
    }

    std::string filePath(input);
    env->ReleaseStringUTFChars(inputData, input);

    AntColonyOptimization aco(50, 0.5, 1.0, 2.0);
    std::string result = aco.run(filePath);

    return env->NewStringUTF(result.c_str());
}
