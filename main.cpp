//
//  main.cpp
//  PushRelabelAlgo
//
//  Created by Rui Shibasaki on 29/05/16.
//  Copyright (c) 2016 Rui Shibasaki. All rights reserved.
//

#include <iostream>
#include "MaxFeasibleFlow.hpp"


int main(int argc, const char * argv[]) {
   
    
    Data d;
    d.nnodes = 6;
    d.narcs = 15;
    d.arcs.resize(15);
    
    d.arcs[0].i=1; d.arcs[0].j=2; d.arcs[0].capa=15;
    d.arcs[14].i=1; d.arcs[14].j=4; d.arcs[14].capa=10;
    d.arcs[13].i=2; d.arcs[13].j=1; d.arcs[13].capa=8;
    d.arcs[1].i=2; d.arcs[1].j=3; d.arcs[1].capa=8;
    d.arcs[2].i=2; d.arcs[2].j=5; d.arcs[2].capa=4;
    d.arcs[3].i=3; d.arcs[3].j=2; d.arcs[3].capa=4;
    d.arcs[4].i=3; d.arcs[4].j=4; d.arcs[4].capa=3;
    d.arcs[5].i=3; d.arcs[5].j=6; d.arcs[5].capa=3;
    d.arcs[6].i=4; d.arcs[6].j=1; d.arcs[6].capa=4;
    d.arcs[7].i=4; d.arcs[7].j=5; d.arcs[7].capa=6;
    d.arcs[8].i=5; d.arcs[8].j=2; d.arcs[8].capa=1;
    d.arcs[9].i=5; d.arcs[9].j=4; d.arcs[9].capa=4;
    d.arcs[10].i=5; d.arcs[10].j=6; d.arcs[10].capa=10;
    d.arcs[11].i=6; d.arcs[11].j=3; d.arcs[11].capa=4;
    d.arcs[12].i=6; d.arcs[12].j=5; d.arcs[12].capa=6;
    d.grid.resize(d.nnodes*d.nnodes,-1);
    for(int a=d.narcs;a--;)d.grid[(d.arcs[a].i-1)*d.nnodes+d.arcs[a].j-1]=a;
    
    MaxFlowPushRlAlgo pr;
    pr.set_data(&d,5,0);
    pr.solve();

    double mincut = 0;
    std::cout<<"FINISHED "<<std::endl;
    for (int a =0; a<d.narcs; ++a) {
        int i = d.arcs[a].i-1;
        int j = d.arcs[a].j-1;
        if(pr.x[i*d.nnodes+j])
            std::cout<<i+1<<" to "<<j+1<<" : "<<pr.x[i*d.nnodes+j]<<std::endl;
    }
   
    std::cout<<"LABEL "<<std::endl;
    for (int n =0; n<d.nnodes; ++n) {
        std::cout<<"label "<<n+1<<": "<<pr.label[n]<<std::endl;
    }
    
    std::cout<<"CUT "<<std::endl;
    for (int a=0; a<pr.minCut.size(); ++a) {
        mincut+=d.arcs[pr.minCut[a]].capa;
        std::cout<<d.arcs[pr.minCut[a]].i<<" "<<d.arcs[pr.minCut[a]].j<<std::endl;
    }
    std::cout<<"flow: "<<pr.balance[pr.sink]<<" mincut: "<<mincut<<std::endl;
    return 0;
}
