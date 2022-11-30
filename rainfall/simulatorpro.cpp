#include "simulatorpro.hpp"

#include <omp.h>

#include <cfloat>
#include <mutex>
#include <thread>

using namespace std;
once_flag onceFlag;

void SimulatorPro::simulate() {
    int N = this->landscape.size();
    vector<vector<float>> status(N, vector<float>(N, 0.0f));    // in-time raindrop status of landscape
    vector<vector<float>> trickled(N, vector<float>(N, 0.0f));  // in-time tricked raindrops for each point
    vector<thread> workers;
    bool globalWet = true;
    const double beginTime = omp_get_wtime();
    for (int i = 0; i < this->threadNum; ++i) {
        workers.emplace_back(&SimulatorPro::process, this, i, N, ref(status), ref(trickled), ref(globalWet));
    }

    for (auto& t : workers) {
        t.join();
    }
    const double endTime = omp_get_wtime();
    this->totalTime = endTime - beginTime;
}

void SimulatorPro::process(const int id, const int N, vector<vector<float>>& status, vector<vector<float>>& trickled, bool& globalWet) {
    // cout << "thread: " << id << " processing" << endl;
    int rows = N / this->threadNum;
    int timeSteps = 0;
    while (globalWet) {
        ++timeSteps;
        this->barrier.wait();
        bool localWet = false;
        for (int i = id * rows; i < (id + 1) * rows; ++i) {
            for (int j = 0; j < N; ++j) {
                // 1) Receive a new raindrop for each point while still raining
                if (timeSteps <= this->rainSteps) {
                    status[i][j] += 1.0;
                }
                // 2) if points have raindrops, absorb
                if (status[i][j] > 0.0) {
                    float toAbsorb = min(status[i][j], this->absorbRate);
                    status[i][j] -= toAbsorb;
                    this->absorbed[i][j] += toAbsorb;
                }
                // 3a) calculate # of raindrops will next trickle to the lowest neighbors
                if (status[i][j] > 0.0 && this->trickleDir[i][j].size() > 0) {
                    // vector<pair<int, int>> lowestNeighbors = this->trickleDir[i][j];
                    float dropToTrickle = min(status[i][j], 1.0f);
                    status[i][j] -= dropToTrickle;
                    float toNeigh = dropToTrickle / this->trickleDir[i][j].size();
                    // may introduce race condition on increment, add lock
                    for (const auto& point : this->trickleDir[i][j]) {
                        int lockID = point.first * N + point.second;
                        // unique_lock<mutex> lk(this->mutexes[lockID]);
                        lock_guard<mutex> lk(this->mutexes[lockID]);
                        // pthread_mutex_trylock(&this->mutexes[i][j]);
                        trickled[point.first][point.second] += toNeigh;
                        // pthread_mutex_unlock(&this->mutexes[i][j]);
                        // lk.unlock();
                    }
                }
            }
        }

        // add barrier, wait untill all current absorb and trickle matric are processed
        this->barrier.wait();

        for (int i = id * rows; i < (id + 1) * rows; ++i) {
            for (int j = 0; j < N; ++j) {
                status[i][j] += trickled[i][j];
                trickled[i][j] = 0.0;
                if (abs(status[i][j]) > FLT_EPSILON) {
                    localWet = true;
                }
            }
        }
        // add barrier, wait untill all status and trickle matrix are processed
        this->barrier.wait();

        unique_lock<mutex> lk(this->globalStatusLock);
        globalWet = (false || localWet);
        lk.unlock();

        // add barrier, wait untill global status are determined by all threads
        this->barrier.wait();
    }
    cout << timeSteps << endl;
}