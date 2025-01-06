#include "MFLib.h"
#include <iostream>

int main() {
    int data = 0;
    int iter = 0;
    auto func = [](void* t) -> int { 
        int* d = (int*)t;
        *d += 1;
        std::cout << (*d) << std::endl;
        return 1;
    };
    
    auto func2 = [](void* t) -> int { 
        MFLib::ForLoopInstrData d = *(MFLib::ForLoopInstrData*)t;
        int* it = (int*)d.sharedMem;
        std::cout << "Iteration: " << d.invocationIndex << " "<<"Index: " << *d.index << std::endl;
        return 1;
    };

    MFLib::Job j;
    MFLib::Instr ins = MFLib::Instr(func, &data);
    j.addInstr(&ins);
    j.addInstr(&ins);
    j.step();
    j.step();
    j.step();
    j.step();
    MFLib::Job jl;

    MFLib::ForLoopInstr lInstr = MFLib::ForLoopInstr(0, 6, 1, func2, &iter);
    lInstr.setSubstepUniformDist(2); 
    jl.addInstr(&lInstr);
    int i = 0;
    while (!jl.hasFinsihed()) {
        jl.step();
        i++;
    }
    std::cout << i << std::endl;
    return 0;
}
