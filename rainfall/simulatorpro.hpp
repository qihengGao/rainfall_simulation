#ifndef __SIMULATORPRO_HPP__
#define __SIMULATORPRO_HPP__
#include <mutex>

// #include "barrier.hpp"
#include "simulator.hpp"

using namespace std;

class SimulatorPro : public Simulator {
   protected:
    int threadNum;
    // vector<vector<unique_ptr<mutex>>> mutexes;
    pthread_mutex_t** mutexes;
    mutex globalStatusLock;
    // Barrier barrier;
    void process(const int id, const int N, vector<vector<float>>& status, vector<vector<float>>& trickled, bool& globalWet);
   public:
    SimulatorPro(const int threadNum, const int timeSteps, const float absorbRate, const int N,
                 ifstream& elevationFile) : Simulator(timeSteps, absorbRate, N, elevationFile),
                                            threadNum(threadNum),
                                            mutexes(nullptr),
                                            globalStatusLock() {
        this->mutexes = new pthread_mutex_t*[N];
        for (int i = 0; i < N; ++i) {
            this->mutexes[i] = new pthread_mutex_t[N];
            for (int j = 0; j < N; ++j) {
                this->mutexes[i][j] = PTHREAD_MUTEX_INITIALIZER;
            }
        }
    }
    //  barrier(threadNum)
    virtual ~SimulatorPro() {}
    virtual void simulate();
    
};

#endif