#ifndef __BARRIER_HPP__
#define __BARRIER_HPP__
// Reference: https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
#include <condition_variable>
#include <mutex>

using namespace std;

class Barrier {
   private:
    mutex bMutex;
    condition_variable bCond;
    int bTarget;
    int threadCount;
    int bGeneration;

   public:
    explicit Barrier(const int threadCount) : bTarget(threadCount),
                                              threadCount(threadCount),
                                              bGeneration(0) {
    }

    void wait() {
        unique_lock<mutex> lLock{bMutex};
        auto lGen = bGeneration;
        if (!--threadCount) {
            bGeneration++;
            threadCount = bTarget;
            bCond.notify_all();
        } else {
            bCond.wait(lLock, [this, lGen] { return lGen != bGeneration; });
        }
    }
};

#endif