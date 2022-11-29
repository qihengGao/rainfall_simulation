#ifndef __BARRIER_HPP__
#define __BARRIER_HPP__

#include <condition_variable>
#include <mutex>

using namespace std;

class Barrier {
   private:
    mutex bMutex;
    condition_variable bCond;
    size_t bTarget;
    size_t threadCount;
    size_t bGeneration;

   public:
    explicit Barrier(size_t threadCount) : bTarget(threadCount),
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