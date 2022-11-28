#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iostream>
namespace std {
template <typename X, typename Y>
struct hash<std::pair<X, Y>> {
    std::size_t operator()(const std::pair<X, Y>& pair) const {
        return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
    }
};
}  // namespace std

using namespace std;

class Simulator {
   protected:
    int rainSteps;                  // rain drop time steps
    float absorbRate;               // amount of rain absorbed into ground/timestep
    vector<vector<int>> landscape;  // elevation of landscap
    unordered_map<pair<int, int>, vector<pair<int, int>>>
        trickleDir;                  // raindrop trickle directions on each coordinate
    vector<vector<float>> absorbed;  // absorbed raindrops at each point
    double totalTime;                // simulation complete time
    int totalSteps;                  // simulation complete time steps

   public:
    Simulator(const int timeSteps, float absorbRate, int N,
              ifstream& elevationFile)
        : rainSteps(timeSteps),
          absorbRate(absorbRate),
          landscape(N, vector<int>(N)),
          trickleDir(),
          absorbed(N, vector<float>(N, 0.0f)),
          totalTime(0.0),
          totalSteps(0) {
        initElevations(elevationFile);
        initTrickleDir();
        cout << this->rainSteps << endl;
        // cout << "------Simulator created------" << endl;
    }
    virtual ~Simulator() {}
    void rain(vector<vector<float>>& status, vector<vector<float>>& trickled);
    bool updateStatus(int N, vector<vector<float>>& status, vector<vector<float>>& trickled);
    void simulate();
    void printResult();

   private:
    void initElevations(ifstream& elevationFile);
    void initTrickleDir();
};

#endif