#include "../../heads/common.h"
#include "../../heads/ec.h"
extern string stage;
//************************************************************
bool is_find_better(ec::Individual &ind, vector<double> &LocalBestFit){
    // 更新领域最优解
    // 随机挑选一个维度进行比较
    if (stage == EFFICIENCY)
    {
        // if (1000 * ind.items[2] - 99 * ind.items[0] > 1000 * LocalBestFit[2] - 99 * LocalBestFit[0]){
        //     LocalBestFit = ind.items;
        //     return true;
        // }
        // if (1000 * ind.items[2] - 99 * ind.items[0] > 1000 * LocalBestFit[2] - 99 * LocalBestFit[0]){
        if (ind.items[4] > LocalBestFit[4]){
            LocalBestFit = ind.items;
            return true;
        }
        return false;
    }
    else if (stage == RESPONSE)
    {
        if (ind.items[4] > LocalBestFit[4]){
            LocalBestFit = ind.items;
            return true;
        }
        return false;
    }
    else if (stage == SIGMA)
    {
        if (ind.items[4] > LocalBestFit[4]){
            LocalBestFit = ind.items;
            return true;
        }
        return false;
    }
    else if (stage == ALL)
    {
        if (ind.items[4] > LocalBestFit[4]){
            LocalBestFit = ind.items;
            return true;
        }
        return false;
    }
    
    else
    {
        cout << "没有这个优化项！！！！" << endl;
        throw new logic_error("");
    }
}