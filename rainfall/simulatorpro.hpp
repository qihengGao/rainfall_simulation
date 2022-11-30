#ifndef __SIMULATORPRO_HPP__
#define __SIMULATORPRO_HPP__
#include <mutex>

#include "barrier.hpp"
#include "simulator.hpp"

using namespace std;

class SimulatorPro : public Simulator {
   protected:
    int threadNum;
    vector<mutex> mutexes;
    mutex globalStatusLock;
    Barrier barrier;
    bool globalFinished;
    void simulateTask(const int id, const int N, vector<vector<float>>& status, vector<vector<float>>& trickled);

   public:
    SimulatorPro(const int threadNum, const int rainSteps, const float absorbRate, const int N,
                 ifstream& elevationFile) : Simulator(rainSteps, absorbRate, N, elevationFile),
                                            threadNum(threadNum),
                                            mutexes(N * N),
                                            globalStatusLock(),
                                            barrier(threadNum),
                                            globalFinished(false) {}
    virtual ~SimulatorPro() {}
    virtual void simulate();
};

#endif