#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class Simulator {
   protected:
    int rainSteps;                                      // rain drop time steps
    float absorbRate;                                   // amount of rain absorbed into ground/timestep
    vector<vector<int>> landscape;                      // elevation of landscap
    vector<vector<vector<pair<int, int>>>> trickleDir;  // raindrop trickle directions on each coordinate
    vector<vector<float>> absorbed;                     // absorbed raindrops at each point
    double totalTime;                                   // simulation complete time
    int totalSteps;                                     // simulation complete time steps

    void initElevations(ifstream& elevationFile);
    void initTrickleDir();

   private:
    void simulate(vector<vector<float>>& status, vector<vector<float>>& trickled);
    bool updateStatus(int N, vector<vector<float>>& status, vector<vector<float>>& trickled);

   public:
    Simulator(const int timeSteps, const float absorbRate, const int N,
              ifstream& elevationFile)
        : rainSteps(timeSteps),
          absorbRate(absorbRate),
          landscape(N, vector<int>(N)),
          trickleDir(N, vector<vector<pair<int, int>>>(N)),
          absorbed(N, vector<float>(N, 0.0f)),
          totalTime(0.0),
          totalSteps(0) {
        initElevations(elevationFile);
        initTrickleDir();
    }
    virtual ~Simulator() {}
    virtual void simulate();
    void printResult();
};

#endif