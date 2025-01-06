#include "multiframe.h"

namespace MFLib {

int Instr::exec(void* data) {
    return op(data); 
}

void Instr::setSharedMem(void* loc) {
    sharedMem = loc;
}

void Instr::setFunc(int (*f)(void*)) {
   op = f; 
}

Instr::Instr(int (*f)(void*), void* mem) {
    setFunc(f);
    setSharedMem(mem);
}

ForLoopInstr::ForLoopInstr(int beg, int end, int step, int (*func)(void*), void* mem) : Instr(func, mem) {
    i = beg;
    this->end = end;
    this->step = step;
    data.index = &i;
    data.sharedMem = mem;
    this->setSharedMem(&data);
}

int ForLoopInstr::exec(void* data) {
    ForLoopInstrData* d = (ForLoopInstrData*)data;
    d->invocationIndex += 1;
    int b = i;
    for (int j = b; (j < (d->subStep + b)) && (i < end) ; j+= step) {
        Instr::exec(data);
        i += step;
    }
    if (i >= end)
        return 1;
    return 0;
}

void ForLoopInstr::setSubstep(int s) {
    data.subStep = s;
}

void ForLoopInstr::setSubstepUniformDist(int num) {
    setSubstep(std::max((end - beg) / num, 1));
}

void Job::_step() {
    if (_currentInstr >= instrs.size())
        _state = JobState::ENDED;
    if (_state == JobState::ENDED)
        return;
    _state = JobState::WORKING;
    _currentInstr += instrs[_currentInstr]->exec(instrs[_currentInstr]->sharedMem);
    if (_currentInstr >= instrs.size())
        _state = JobState::ENDED;
}

void Job::step(uint32_t i) {
    for (uint32_t j = 0; j < i; ++j) {
        _step();
    }
}

void Job::addInstr(Instr* instr) {
   instrs.push_back(instr);
}

}
