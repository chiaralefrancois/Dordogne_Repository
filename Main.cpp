#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include "Tower.h"
#include "csvCreator.h"
#include "Optimizer.h"

// Functions declaration
double TowerObjectiveFunction(std::vector<double> x);
void setOptimizationParameters(int caseNumber);

// Objects declaration
Tower testTower;
Optimizer optimizer(TowerObjectiveFunction);
CSVCreator writer("optimalTower.csv");
int numRadii;
double fixedVolume;



// Heights for 100 test ----------------
    //Creating the frustum heights array
//int N = 30; //the number of points
//double maxHeight = 50;
//double dy = maxHeight / (N - 1);
vector<double> frustumHeights;
vector<double> optimalRadii;
vector<double> Frustums;

int main() {
    int caseNumber;

    // Height for 100 test ------------
        //Initializing the frustum heights array
    //for (int i = 0; i < N; i++) {
    //    frustumHeights.push_back(i * dy);
    //}

    // Selecting case (from 1 to 8)
    std::cout << "Choose Optimization Case (1-8) : ";
    std::cin >> caseNumber;

    setOptimizationParameters(caseNumber);

    // Mesuring execution time
    auto start = std::chrono::high_resolution_clock::now();

    // Optimization with DFP

    // Optimizing radii (cases 1 to 7)
    //std::vector<double> optimalRadii = optimizer.dfp(numRadii, 1000, 1e-6, lowerBound, upperBound);
    //std::cout << "Nombre de rayons optimisés : " << optimalRadii.size() << "\n";

    // Optimizing heights (case 8)
    frustumHeights = optimizer.dfp(frustumHeights.size(), 1000, 1e-6, 0.0, 50.0);
    std::cout << "Nombre de hauteurs optimisés : " << frustumHeights.size() << "\n";



    // Adding R0 and Rm to results
    // optimalRadii.insert(optimalRadii.begin(), R0); // To comment for some cases
    // optimalRadii.push_back(Rm); // To comment for some cases

    // Getting frustums heights
    std::vector<double> Frustums = testTower.getFrustumHeights();

    std::cout << "Taille de optimalRadii : " << optimalRadii.size() << "\n"; // Doit être 101
    std::cout << "Taille de frustumHeights : " << frustumHeights.size() << "\n"; // Doit être 101

    // Saving results
   writer.generateCSV(optimalRadii, Frustums); 

    // End of chrono
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    // Updating tower
    //testTower.setSectionRadii(optimalRadii);
    //testTower.setFrustumHeigths(Frustums);

    // Tower case 8
    testTower.setFrustumHeigths(frustumHeights);  // On met à jour les hauteurs


    // Area and Volume
    double totalArea = testTower.calculateTotalArea();
    double totalVolume = testTower.calculateTotalVolume();

    //// Deviation
    //double sum = 0.0;
    //for (double r : optimalRadii) sum += r;
    //double mean = sum / optimalRadii.size();
    //sum = 0.0;
    //for (double r : optimalRadii) sum += pow(mean - r, 2);
    //double stdDeviation = sqrt(sum / (optimalRadii.size() - 1));

    //Calculating the standard deviation - Bodhi
    double sum = 0.0;
    for (int i = 0; i < optimalRadii.size(); i++) {
        sum += optimalRadii[i];
    }
    double mean = sum / optimalRadii.size();
    sum = 0.0;
    for (int i = 0; i < optimalRadii.size(); i++) {
        sum += pow(mean - optimalRadii[i], 2);
    }
    double stdDeviation = sqrt(sum / ((optimalRadii.size() - 1)));
    std::cout << "Standard deviation : " << stdDeviation << "\n";

    // Display of results
    std::cout << "\nOptimization completed for case " << caseNumber << " :\n";
    std::cout << "Optimized radii: ";
    for (double r : optimalRadii) std::cout << r << " ";
    std::cout << "\nFrustum heights: ";
    for (double h : Frustums) std::cout << h << " ";
    std::cout << "\nTotal surface area: " << totalArea << " m²\n";
    std::cout << "Total volume: " << totalVolume << " m³\n";
    std::cout << "Standard deviation of radii: " << stdDeviation << "\n";
    std::cout << "Execution time: " << elapsed << " seconds\n";
    std::cout << "Results saved in 'optimalTower.csv'.\n";

    return 0;
}

// Adjusting parameters depending on the chosen case
void setOptimizationParameters(int caseNumber) {
    switch (caseNumber) {
    case 1:
        // Global parameters (Need to be changed depending on the case)
        double R0 = 39.3;
        double Rm = 27.4;
        double fixedVolume = 70320;
        int numRadii = 11;  // Number of radii that need to be optimized
        double lowerBound = 20.0, upperBound = 40.0;  // Radii bounds
        std::cout << "frus" << frustumHeights.size() << ".\n";
        std::cout << "optimalRadii" << optimalRadii.size() << ".\n";
        break;
    case 2:
        // Global parameters
        double R0 = 39.3;
        double Rm = 27.4;
        double fixedVolume = 130000;
        int numRadii = 11;  // Number of radii that need to be optimized
        double lowerBound = 20.0, upperBound = 40.0;  // Radii bounds
        break;
    case 3:
        // Global parameters
// Need to be changed depending on the case
        double R0 = 39.3;
        double Rm = 27.4;
        double fixedVolume = 130000;
        double lowerBound = 20.0, upperBound = 40.0;  // Radii bounds
        numRadii = 1;
        break;
    case 4:
        // Global parameters
        // Need to be changed depending on the case
        double R0 = 39.3;
        double Rm = 27.4;
        double fixedVolume = 130000;
        int numRadii = 11;  // Number of radii that need to be optimized
        double lowerBound = 20.0, upperBound = 40.0;  // Radii bounds
        numRadii = 98; // We optimize 98 radii,  R0 and Rm are fixed

        // Heights are evenly spaced
        frustumHeights.clear();
        for (int i = 0; i < 100; i++) {
            frustumHeights.push_back(i * 0.5);
        }

        std::cout << "frus" << Frustums.size() <<".\n";
        std::cout << "optimalRadii" << optimalRadii.size() << ".\n";
        break;

    case 5:
        // Global parameters
        double R0 = 39.3;
        double Rm = 27.4;
        double fixedVolume = 200000;
        int numRadii = 11;  // Number of radii that need to be optimized
        double lowerBound = 20.0, upperBound = 40.0;  // Radii bounds

        // Heights evenly spaced
        frustumHeights.clear();
        for (int i = 0; i < 11; i++) {
            frustumHeights.push_back(i * 5);
        }
        break;
    case 6:
        numRadii = 10; // Rm is not fixed but R0 is
        fixedVolume = 200000;
        // Heights evenly spaced
        frustumHeights.clear();
        for (int i = 0; i < 11; i++) {
            frustumHeights.push_back(i * 5);
        }
        std::cout << "Optimisation avec R0 fixe et Rm variable.\n";
        std::cout << "frus" << Frustums.size() <<".\n";
        std::cout << "optimalRadii" << optimalRadii.size() << ".\n";
        break;
    case 7:
        // R0 = lowerBound;
        // Rm = upperBound;
        fixedVolume = 200000;

        // Heights evenly spaced
        frustumHeights.clear();
        for (int i = 0; i < 11; i++) {
            frustumHeights.push_back(i * 5);
        }
        numRadii = 11; // Both R0 and Rm are variable
        break;
    case 8:
        numRadii = 11;
        fixedVolume = 200000;
        frustumHeights.clear();

        for (int i = 0; i < numRadii; i++) {
            frustumHeights.push_back(5.0 * (i + 1));  // Initial values of heights
        }

        std::cout << "Number of optimized heights : " << frustumHeights.size() << ".\n";
        std::cout << "Size of optimalRadii : " << optimalRadii.size() << ".\n";
        std::cout << "Optimized radii : ";
        for (double r : optimalRadii) std::cout << r << " ";
        std::cout << "\nFrustums heights : ";
        for (double h : frustumHeights) std::cout << h << " ";


        break;
    default:
        std::cerr << "Invalid case. Using default parameters.\n";
    }
}

// Objective function for optimization
double TowerObjectiveFunction(std::vector<double> x) {
    if (x.empty()) {
        std::cerr << "Error : vector x is empty.\n";
        return 0;
    }

    // std::vector<double> frustumHeights = { 0.0, 3.6, 7.3, 10.9, 14.6, 18.2, 21.9, 25.5, 29.1, 32.8, 36.5 };
    // std::vector<double> frustumHeights = { 0.0, 3.6, 7.3, 10.9, 14.6, 18.2, 21.9, 25.5, 29.1, 32.8, 50 };
    // std::vector<double> frustumHeights = { 0.0, 25, 50 }; // Case 3

    // ---------------- Fixed radii, Variable heights
    std::vector<double> sectionRadii = { 39.3, 35, 30, 29, 28, 27, 24, 24, 23, 25, 27.4 };
    frustumHeights.insert(frustumHeights.begin(), 0.0); // First height set to 0
    for (size_t i = 0; i < sectionRadii.size() - 1; i++) {
        frustumHeights[i] = x[i];
    }

    std::cout << "frus" << Frustums.size() << ".\n";
    std::cout << "optimalRadii" << optimalRadii.size() << ".\n";

    // ---------------- Fixed R0 and Rm : 
    //if (x.size() == frustumHeights.size()) {
    //    frustumHeights = x;
    //    x = { R0, R0, R0, R0, R0, R0, R0, R0, R0, Rm };
    //}

    //std::vector<double> sectionRadii(frustumHeights.size());
    //sectionRadii[0] = R0;
    //for (size_t i = 1; i < sectionRadii.size() - 1; i++) {
    //    sectionRadii[i] = x[i - 1];
    //}
    //sectionRadii[sectionRadii.size() - 1] = Rm;

    // ---------------- Fixed R0 and Rm variable
    //if (x.size() == frustumHeights.size()) {
    //    frustumHeights = x;
    //    x = { R0, R0, R0, R0, R0, R0, R0, R0, R0, 10 };
    //}

    //std::vector<double> sectionRadii(frustumHeights.size());
    //sectionRadii[0] = R0;
    //for (size_t i = 1; i < sectionRadii.size() - 1; i++) {
    //    sectionRadii[i] = x[i - 1]; 
    //}

    // ---------------- R0 and Rm variables : 
    //if (x.size() == frustumHeights.size()) {
    //    frustumHeights = x;
    //    x = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    //}

    //std::vector<double> sectionRadii(frustumHeights.size());
    //for (size_t i = 0; i < sectionRadii.size(); i++) {
    //    sectionRadii[i] = x[i];
    //}

    double sum = 0.0;
    for (double r : sectionRadii) sum += r;
    double mean = sum / sectionRadii.size();
    sum = 0.0;
    for (double r : sectionRadii) sum += pow(mean - r, 2);
    double stdDeviation = sqrt(sum / (sectionRadii.size() - 1));

    testTower.setSectionRadii(sectionRadii);
    testTower.setFrustumHeigths(frustumHeights);

    double totalArea = testTower.calculateTotalArea();
    double totalVolume = testTower.calculateTotalVolume();

    double alpha = 1, beta = 10000, gamma = 5000;
    double objectiveStdDev = 2;

    double fitness = totalArea + alpha * pow(totalVolume - fixedVolume, 2)
        + beta * pow(stdDeviation - objectiveStdDev, 2);

    if (numRadii == 9) {
        for (size_t i = 1; i < sectionRadii.size() - 1; i++) {
            fitness += gamma * pow(sectionRadii[i] - sectionRadii[i - 1], 2);
        }
    }

    return fitness;
}
