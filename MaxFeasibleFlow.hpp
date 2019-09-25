//
//  MaxFeasibleFlow.h
//  PushRelabelAlgo
//
//  Created by Rui Shibasaki on 29/05/16.
//  Copyright (c) 2016 Rui Shibasaki. All rights reserved.
//

#ifndef __PushRelabelAlgo__MaxFeasibleFlow__
#define __PushRelabelAlgo__MaxFeasibleFlow__

#include <vector>
#include <stdio.h>
#include <algorithm>
#include <climits>
#include <iostream>
#include "structures.hpp"


struct bucket{
    int first_item;
    int last_item;
    bool empty;
    bucket():empty(true){}
};

struct bucketItems{
    int next_item;
};


class MaxFlowPushRlAlgo{
public:
    
    Data *data;
    std::vector<int> residual;
    std::vector<int> label;
    std::vector<double> balance; //flow balance
    std::vector<double> x;    //flows
    std::vector<int> minCut;
    std::vector<bucket> buckets;//n+2
    std::vector<bucketItems> bkt_node;
    int nnodes, narcs;
    int source, sink;
    int max_nonempty_bkt;
    
    void set_data(Data *d, int sr, int sk);
    void initialize();
    
    bool push(int i);
    bool relabel(int i);
    bool solve();
    
    int getHighestLabel();
    void addToBucket(int b, int node);
    int getFromBucket(int b);
    
    void findMincut();
};

#endif /* defined(__PushRelabelAlgo__MaxFeasibleFlow__) */
