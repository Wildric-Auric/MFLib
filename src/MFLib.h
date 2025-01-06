#pragma once
#include <vector>

namespace MFLib {

enum class JobState {
    NOT_LAUNCHED,
    WORKING,
    ENDED
};

class Instr {
    public:
    void* sharedMem = nullptr; 
    int(*op)(void* data);
    virtual int exec(void* data = nullptr);

    Instr(int (*)(void*) = nullptr, void* mem = nullptr);
    void setFunc(int (*)(void*));
    void setSharedMem(void* loc);
};


struct ForLoopInstrData {
    int subStep = 1;
    int invocationIndex = 0;
    int*  index = 0;
    void* sharedMem = 0;
};

class ForLoopInstr : public Instr {
    public:  
    int i    = 0;
    int beg  = 0;
    int end  = 0;
    int step = 0;

    ForLoopInstrData data;

    ForLoopInstr(int beg, int end, int step = 1, int (*)(void*) = nullptr, void* mem = nullptr);
    int exec(void* data = nullptr) override;
    void setSubstep(int);
    void setSubstepUniformDist(int);
};

class Job {
    public:
    int _currentInstr = 0;
    JobState _state = JobState::NOT_LAUNCHED;
    std::vector<Instr*> instrs;

    void _step();
    void step(uint32_t i = 1);
    void addInstr(Instr* instr);
    inline bool hasFinsihed() { return _state == JobState::ENDED;}
    inline JobState getState() { return _state; }
};


}
