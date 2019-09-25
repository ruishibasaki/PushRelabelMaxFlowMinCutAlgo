//
//  MaxFeasibleFlow.cpp
//  PushRelabelAlgo
//
//  Created by Rui Shibasaki on 29/05/16.
//  Copyright (c) 2016 Rui Shibasaki. All rights reserved.
//

#include "MaxFeasibleFlow.hpp"

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

void
MaxFlowPushRlAlgo::set_data(Data *d, int sr, int sk){
    data=d;
    nnodes = d->nnodes;
    narcs = d->narcs;
    source=sr;
    sink = sk;
    residual.resize(nnodes*nnodes);
    x.resize(nnodes*nnodes);
    label.resize(nnodes,0);
    balance.resize(nnodes,0);
    for(int a=0;a<narcs;++a)
        residual[(data->arcs[a].i-1)*nnodes+data->arcs[a].j-1]=data->arcs[a].capa;
    bkt_node.resize(nnodes);
    buckets.resize(nnodes+3);
}

//-------------------------------------------------

void
MaxFlowPushRlAlgo::initialize(){
    int index;
    for(int j=0;j<nnodes;++j) {
        index = data->grid[source*nnodes+j];
        if(index<0) continue;
        x[source*nnodes+j] = data->arcs[index].capa;
        balance[j] = x[source*nnodes+j];
        balance[source] = -x[source*nnodes+j];
        residual[source*nnodes+j] = 0;
        residual[j*nnodes+source] += x[source*nnodes+j];
        addToBucket(0,j);
    }
    label[source] = nnodes;
    max_nonempty_bkt=0;
    
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------


void
MaxFlowPushRlAlgo::addToBucket(int b, int node){
    if(buckets[b].empty){
        buckets[b].first_item = node;
        buckets[b].last_item = node;
        buckets[b].empty=false;
    }else{
        int n = buckets[b].last_item;
        bkt_node[n].next_item = node;
        buckets[b].last_item = node;
    }
}

//-------------------------------------------------


int
MaxFlowPushRlAlgo::getFromBucket(int b){
    if(buckets[b].empty){return -1;
    }else{
        int n = buckets[b].first_item;
        if(buckets[b].first_item == buckets[b].last_item){
            buckets[b].empty=true;
        }else{
            buckets[b].first_item = bkt_node[n].next_item;
        }
        return n;
    }
}

//-------------------------------------------------

int
MaxFlowPushRlAlgo::getHighestLabel(){
    if(buckets[max_nonempty_bkt].empty){
        for(int b=max_nonempty_bkt;b--;){
            if(!buckets[b].empty){
                max_nonempty_bkt=b;
                break;
            }
        }
    }
    return getFromBucket(max_nonempty_bkt);
}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------

bool
MaxFlowPushRlAlgo::push(int i){

    double epsilon, capaij, capaji;
    bool ret=false;
    for(int j=0;j<nnodes;++j) {
        if(residual[i*nnodes+j]>0 && label[i] == label[j]+1){
            //epsilon = min(fx(v), u~(ij))
            epsilon = balance[i];
            if(epsilon > residual[i*nnodes+j])
                epsilon = residual[i*nnodes+j];
            
            //push flow
            if(x[j*nnodes+i]>0){
                //std::cout<<"remove "<<epsilon<<" of flow, direction: "<<j<<std::endl;
                if(epsilon>x[j*nnodes+i]){
                    x[i*nnodes+j] = epsilon-x[j*nnodes+i];
                    x[j*nnodes+i] =0;
                    residual[j*nnodes+i] = x[i*nnodes+j];
                    residual[i*nnodes+j] -= epsilon;
                }else{
                    x[j*nnodes+i] -= epsilon;
                    residual[j*nnodes+i] += epsilon;
                    residual[i*nnodes+j] -= epsilon;
                }
            }else{
                //std::cout<<"send  "<<epsilon<<" of flow, direction: "<<j<<std::endl;
                x[i*nnodes+j] += epsilon;
                residual[i*nnodes+j] -= epsilon;
                residual[j*nnodes+i] += epsilon;
            }
            //update balances fx
            if(balance[j]==0) addToBucket(label[j],j);
            balance[i] -= epsilon;
            balance[j] += epsilon;
            if(balance[i]==0) return true;
            ret=true;
        }
    }
    /*if(ret){
        std::cout<<"FINISHED "<<std::endl;
        for (int a =0; a<narcs; ++a) {
            int i = data->arcs[a].i-1;
            int j = data->arcs[a].j-1;
            if(x[i*nnodes+j])
                std::cout<<i+1<<" to "<<j+1<<" : "<<x[i*nnodes+j]<<std::endl;
        }
        for (int n =0; n<nnodes; ++n) {
            if(balance[n])std::cout<<"balance "<<n+1<<": "<<balance[n]<<std::endl;
        }
    }*/
    
    return ret;
}


//-------------------------------------------------

bool
MaxFlowPushRlAlgo::relabel(int i){
    // Find the adjacent with minimum height
    int min=nnodes*100;
    bool ret=false;
    for (int j = 0; j < nnodes; j++){
        if(residual[i*nnodes+j]>0){
            // Update minimum height
            if (label[j] < min){
                min = label[j];
                ret=true;
            }
        }
    }
    //std::cout<<"relabel "<<i<<" to: "<<min+1<<std::endl;
    if(ret){
        label[i] = min+1;
        addToBucket(min+1, i);
        if(max_nonempty_bkt<min+1)max_nonempty_bkt=min+1;
    }return ret;
}


//-------------------------------------------------


bool
MaxFlowPushRlAlgo::solve(){
    
    int i = 0;
    int index;
    bool check, feasible = true;
    
    initialize();
    while(1){
        i = getHighestLabel();
        if(i<0 || i==sink) break;
        //std::cout<<"node: "<<i<<" label: "<<label[i]<<" b: "<<balance[i]<<std::endl;

        if(!push(i))
            relabel(i);
        else if(balance[i]>0)addToBucket(label[i], i);
        
        
    };

    if (balance[0] != balance[nnodes-1]) {
        feasible = false;
    }
    
    findMincut();
    return feasible;

}

//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------


void
MaxFlowPushRlAlgo::findMincut(){
    int index;
    std::vector<int> s(nnodes,0);
    for (int n =0; n<nnodes; ++n) {
        if(label[n]>=nnodes) s[n] = 1;
    }
    for (int i =0; i<nnodes; ++i) {
        if(s[i]==0) continue;
        for(int j=0;j<nnodes;++j) {
            if(s[j]==1) continue;
            index = data->grid[i*nnodes+j];
            if(index>=0) minCut.push_back(index);
        }
    }
}
