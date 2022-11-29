#ifndef __SIMULATORPRO_HPP__
#define __SIMULATORPRO_HPP__
#include <mutex>

// #include "barrier.hpp"
#include "simulator.hpp"

using namespace std;

class SimulatorPro : public Simulator {
   protected:
    int threadNum;
    vector<vector<mutex>> mutexes;
    mutex globalStatusLock;
    // Barrier barrier;

   public:
    SimulatorPro(const int threadNum, const int timeSteps, float absorbRate, int N,
                 ifstream& elevationFile) : Simulator(timeSteps, absorbRate, N, elevationFile),
                                            threadNum(threadNum),
                                            mutexes(N, vector<mutex>(N, mutex())),
                                            globalStatusLock() {}
                                            //  barrier(threadNum)
    virtual ~SimulatorPro() {}
    virtual void simulate() {}
    void process(const int id, const int N, vector<vector<float>>& status, vector<vector<float>>& trickled, bool& globalWet);
};

#endif