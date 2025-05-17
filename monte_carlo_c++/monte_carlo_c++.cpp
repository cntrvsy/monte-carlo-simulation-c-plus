// monte_carlo_c++.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>

int main() {
    // Simulation parameters
    const double annual_contribution = 10000.0;     // Victor contributes $10,000 per year
    const double mean_return = 0.08;                 // 8% mean annual return
    const double std_dev_return = 0.16;              // 16% standard deviation (typical for stocks)
    const int years = 30;                            // 30-year investment horizon
    const int num_simulations = 10000;               // Number of simulations to run
    const unsigned int seed = 12345;                // Fixed seed for reproducibility

    std::cout << "Running Monte Carlo simulation for Victor's portfolio\n";
    std::cout << "Parameters:\n";
    std::cout << "  Annual Contribution: $" << std::fixed << std::setprecision(2) << annual_contribution << "\n";
    std::cout << "  Mean Annual Return: " << mean_return * 100.0 << "%\n";
    std::cout << "  Standard Deviation: " << std_dev_return * 100.0 << "%\n";
    std::cout << "  Investment Horizon: " << years << " years\n";
    std::cout << "  Number of Simulations: " << num_simulations << "\n";
    std::cout << "  Random Seed: " << seed << "\n";

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Create seeded random number generator with normal distribution
    std::mt19937 gen(seed);
    std::normal_distribution<double> normal_dist(mean_return, std_dev_return);

    // Store final portfolio values from each simulation
    std::vector<double> final_values;
    final_values.reserve(num_simulations);

    // Run simulations
    for (int i = 0; i < num_simulations; ++i) {
        double portfolio_value = 0.0;

        for (int year = 0; year < years; ++year) {
            // Generate random annual return from normal distribution
            double annual_return = normal_dist(gen);

            // Apply annual return to existing portfolio
            portfolio_value = portfolio_value * (1.0 + annual_return);

            // Add annual contribution
            portfolio_value += annual_contribution;
        }

        final_values.push_back(portfolio_value);
    }

    // Sort for percentile calculations
    std::sort(final_values.begin(), final_values.end());

    // Calculate statistics
    double mean = std::accumulate(final_values.begin(), final_values.end(), 0.0) / final_values.size();

    // Median (50th percentile)
    double median;
    if (final_values.size() % 2 == 0) {
        median = (final_values[final_values.size() / 2 - 1] + final_values[final_values.size() / 2]) / 2.0;
    }
    else {
        median = final_values[final_values.size() / 2];
    }

    // Standard deviation
    double variance = 0.0;
    for (double value : final_values) {
        variance += std::pow(value - mean, 2);
    }
    variance /= final_values.size();
    double std_dev = std::sqrt(variance);

    // Percentiles
    double p05 = final_values[static_cast<size_t>(final_values.size() * 0.05)];
    double p25 = final_values[static_cast<size_t>(final_values.size() * 0.25)];
    double p75 = final_values[static_cast<size_t>(final_values.size() * 0.75)];
    double p95 = final_values[static_cast<size_t>(final_values.size() * 0.95)];

    // Calculate elapsed time
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print results
    std::cout << "\nResults after " << years << " years:\n";
    std::cout << "  Mean portfolio value: $" << std::fixed << std::setprecision(2) << mean << "\n";
    std::cout << "  Median portfolio value: $" << median << "\n";
    std::cout << "  Standard deviation: $" << std_dev << "\n";
    std::cout << "\nPercentiles:\n";
    std::cout << "  5th  percentile: $" << p05 << " (worst case scenario)\n";
    std::cout << "  25th percentile: $" << p25 << "\n";
    std::cout << "  75th percentile: $" << p75 << "\n";
    std::cout << "  95th percentile: $" << p95 << " (best case scenario)\n";
    std::cout << "\nSimulation completed in " << duration.count() << "ms\n";

    // Prompt the user to press any key before exiting
    std::cout << "\nPress any key to exit...\n";
    std::cin.get(); // Wait for a single character input (any key press)

    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
