#include "simulatorpro.hpp"

#include <omp.h>

#include <cfloat>
#include <mutex>
#include <thread>

using namespace std;

void SimulatorPro::simulate() {
    int N = this->landscape.size();
    vector<vector<float>> status(N, vector<float>(N, 0.0f));    // in-time raindrop status of landscape
    vector<vector<float>> trickled(N, vector<float>(N, 0.0f));  // in-time tricked raindrops for each point
    vector<thread> workers;
    // bool globalWet = true;
    const double beginTime = omp_get_wtime();
    for (int i = 0; i < this->threadNum; ++i) {
        workers.emplace_back(&SimulatorPro::process, this, i, N, ref(status), ref(trickled));
    }

    for (auto& t : workers) {
        t.join();
    }
    const double endTime = omp_get_wtime();
    this->totalTime = endTime - beginTime;
}

void SimulatorPro::process(const int id, const int N, vector<vector<float>>& status, vector<vector<float>>& trickled) {
    int rows = N / this->threadNum;
    int steps = 0;
    while (true) {
        ++steps;
        for (int i = id * rows; i < (id + 1) * rows; ++i) {
            for (int j = 0; j < N; ++j) {
                // 1) Receive a new raindrop for each point while still raining
                if (steps <= this->rainSteps) {
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
                    float dropToTrickle = min(status[i][j], 1.0f);
                    status[i][j] -= dropToTrickle;
                    float toNeigh = dropToTrickle / this->trickleDir[i][j].size();
                    for (const auto& point : this->trickleDir[i][j]) {
                        int lockID = point.first * N + point.second;
                        unique_lock<mutex> lk(this->mutexes[lockID]);
                        trickled[point.first][point.second] += toNeigh;
                    }
                }
            }
        }
        // add barrier, wait untill all current absorb and trickle matric are processed
        this->barrier.wait();
        bool localFinshed = true;
        for (int i = id * rows; i < (id + 1) * rows; ++i) {
            for (int j = 0; j < N; ++j) {
                status[i][j] += trickled[i][j];
                trickled[i][j] = 0.0;
                if (abs(status[i][j]) > FLT_EPSILON) {
                    localFinshed = false;
                }
            }
        }
        unique_lock<mutex> lk(this->globalStatusLock);
        this->globalFinished = this->globalFinished && localFinshed;
        lk.unlock();
        // add barrier, wait untill global status are passed to && with [locaFinished] of all threads
        this->barrier.wait();
        if (this->globalFinished) {
            this->totalSteps = steps;
            return;
        }
        // add barrier, wait untill all threads has passed the globalFinished determination, reset globalFinished
        this->barrier.wait();
        this->globalFinished = true;
    }
}