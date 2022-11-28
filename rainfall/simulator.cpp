#include "simulator.hpp"

#include <bits/stdc++.h>
#include <omp.h>

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

void Simulator::initElevations(ifstream& elevationFile) {
    string line;
    int row = 0;
    int col = 0;
    int elevation;
    while (getline(elevationFile, line)) {
        stringstream ss(line);
        col = 0;
        while (ss >> elevation) {
            this->landscape[row][col] = elevation;
            ++col;
        }
        ++row;
    }
    // for (auto& row : this->landscape) {
    //     for (auto& elevation : row) {
    //         cout << elevation << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "=====End of init Elevation=======" << endl;
}

void Simulator::initTrickleDir() {
    vector<vector<int>> directions{{-1, 0}, {1, 0}, {0, 1}, {0, -1}};  // north/south/east/west
    int N = this->landscape.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int lowest = INT_MAX;
            // find the lowerst neighbor value
            for (const auto& direction : directions) {
                int neighRow = i + direction[0], neighCol = j + direction[1];
                if (0 <= neighRow && neighRow < N && 0 <= neighCol && neighCol < N) {
                    lowest = min(lowest, this->landscape[neighRow][neighCol]);
                }
            }
            if (this->landscape[i][j] <= lowest) {
                continue;
            }
            for (const auto& direction : directions) {
                int neighRow = i + direction[0], neighCol = j + direction[1];
                if (0 <= neighRow && neighRow < N && 0 <= neighCol && neighCol < N) {
                    int neighVal = this->landscape[neighRow][neighCol];
                    if (neighVal == lowest) {
                        this->trickleDir[{i, j}].push_back({neighRow, neighCol});
                    }
                }
            }
        }
    }

    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         cout << "[" << i << ", " << j << "]'s neigh: ";
    //         for (auto& p : trickleDir[{i, j}]) {
    //             cout << "[" << p.first << ", " << p.second << "], ";
    //         }
    //         cout << endl;
    //     }
    // }
}

void Simulator::rain(vector<vector<float>>& status, vector<vector<float>>& trickled) {
    int N = this->landscape.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            // 1) Receive a new raindrop for each point while still raining
            if (this->totalSteps <= this->rainSteps) {
                status[i][j] += 1.0;
            }
            // 2) if points have raindrops, absorb
            if (status[i][j] > 0.0) {
                float toAbsorb = status[i][j] > this->absorbRate ? this->absorbRate : status[i][j];
                status[i][j] -= toAbsorb;
                this->absorbed[i][j] += toAbsorb;
            }
            // 3a) calculate # of raindrops will next trickle to the lowest neighbors
            if (status[i][j] > 0.0 && this->trickleDir[{i, j}].size() > 0) {
                vector<pair<int, int>> lowestNeighbors = this->trickleDir[{i, j}];
                float dropToTrickle = status[i][j] >= 1.0 ? 1.0 : status[i][j];
                status[i][j] -= dropToTrickle;
                float toNeigh = dropToTrickle / lowestNeighbors.size();
                for (const auto& point : lowestNeighbors) {
                    trickled[point.first][point.second] += toNeigh;
                }
            }
        }
    }
    // if (this->totalSteps < 10) {
    //     cout << this->totalSteps << ": absorbed" << endl;
    //     for (int i = 0; i < N; ++i) {
    //         for (int j = 0; j < N; ++j) {
    //             cout << this->absorbed[i][j] << " ";
    //         }
    //         cout << endl;
    //     }
    // }
}

bool Simulator::updateStatus(int N, vector<vector<float>>& status, vector<vector<float>>& trickled) {
    bool hasNextRound = false;
    // if (this->totalSteps < 10) cout << "======status: ======" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            status[i][j] += trickled[i][j];
            trickled[i][j] = 0.0;
            // if (this->totalSteps < 10) {
            //     cout << status[i][j] << " ";
            // }
            if (abs(status[i][j]) > FLT_EPSILON) {
                hasNextRound = true;
            }
        }
        // if (this->totalSteps < 10) {
        //     cout << endl;
        // }
    }
    return hasNextRound;
}

void Simulator::simulate() {
    // cout << "===========start simulation============" << endl;
    int N = this->landscape.size();
    vector<vector<float>> status(N, vector<float>(N, 0.0));                             // in-time raindrop status of landscape
    vector<vector<float>> trickled(N, vector<float>(N, 0.0));  // in-time tricked raindrops for each point
    const double begin = omp_get_wtime();
    bool isWet = true;
    while (isWet) {
        ++this->totalSteps;
        // 1st Traverse: rain, absorb, calculate trickled
        rain(status, trickled);
        // 3b) 2nd Traverse: update # of raindrops at each lowest neighbor & check wet
        isWet = updateStatus(N, status, trickled);
        // cout << "========== total steps: " << this->totalSteps << ", isWet: " << isWet << "==========" << endl;
    }
    // cout << "======End of simulation=======" << endl;
    const double end = omp_get_wtime();
    this->totalTime = end - begin;
}

void Simulator::printResult() {
    cout << "Rainfall simulation completed in " << this->totalSteps
         << " time steps" << endl;
    cout << "Runtime = " << this->totalTime << " seconds" << endl;
    cout << endl;
    cout << "The following grid shows the number of raindrops absorbed at each point:" << endl;
    for (const auto& row : this->absorbed) {
        for (const auto& res : row) {
            cout << setw(8) << setprecision(6) << res;
        }
        cout << endl;
    }
}
