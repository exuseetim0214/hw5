#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (Worker_T)-1;


// Add prototypes for any helper functions here
static bool shift_helper(
    const AvailabilityMatrix& avail, 
    size_t curDay,
    size_t curSlot,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftCount
);

// Add your implementation of schedule() and other helper functions here
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
){
    if(avail.size() == 0U){
        return false;
    }

    // Add your code below
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();
    
    if(dailyNeed > numWorkers){
      sched.clear();
      return false;
    }

    sched.assign(numDays, vector<Worker_T>(dailyNeed, INVALID_ID));
  
    vector<size_t> shiftCount(numWorkers, 0);

    bool success = shift_helper(avail,
                          0,
                          0,
                          dailyNeed,
                          maxShifts,
                          sched,
                          shiftCount);

    if(!success){
      sched.clear();
    }

    return success;
}


static bool shift_helper(
    const AvailabilityMatrix& avail, 
    size_t curDay,
    size_t curSlot,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftCount
){
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    if(curDay == numDays){
        return true;
    }

    if(curSlot == dailyNeed){
        return shift_helper(avail,
                            curDay +1,
                            0,
                            dailyNeed,
                            maxShifts,
                            sched,
                            shiftCount);
    }

    for(Worker_T w = 0;  w < numWorkers; w++){

        if(! avail[curDay][w]){
            continue;
        }
        if(shiftCount[w] >= maxShifts){
          continue;
        }

        if(find(sched[curDay].begin(), sched[curDay].begin() + curSlot, w) != sched[curDay].begin() + curSlot){
          continue;
        }

        sched[curDay][curSlot] = w;
        shiftCount[w]++;
        

        if(shift_helper(avail,
                        curDay,
                        curSlot + 1,
                        dailyNeed,
                        maxShifts,
                        sched,
                        shiftCount))
        {
          return true;
        }

        shiftCount[w]--;
        sched[curDay][curSlot] = INVALID_ID;
    }

    return false;
}
