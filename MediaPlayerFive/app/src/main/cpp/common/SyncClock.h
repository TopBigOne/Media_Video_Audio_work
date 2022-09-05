//
// Created by dev on 2022/9/4.
//

#ifndef MEDIAPLAYERFIVE_SYNCCLOCK_H
#define MEDIAPLAYERFIVE_SYNCCLOCK_H


#include <../util/LogUtil.h>

class SyncClock {
public:
    SyncClock(){
        curPts = 0;
        lastUpdate = 0;
        lastPts = 0;
        frameTimer = 0;
    }

    ~SyncClock() {
    }

    void SetClock(double pts, double time) {
        this->curPts = pts;
        this->lastUpdate = time;
    }

    double GetClock() {
        double time = GetSysCurrentTime();
        return curPts + time - lastUpdate;
    }

public:
    double lastPts;
    double frameTimer;
    double curPts;

private:
    double lastUpdate;
};


#endif //MEDIAPLAYERFIVE_SYNCCLOCK_H
