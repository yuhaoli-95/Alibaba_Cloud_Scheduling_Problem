#include "../../heads/common.h"
#include "../../heads/ec.h"
extern vector<int> ShuffArray(vector<int>);
extern int random_a_b(int, int);
extern void fitness(ec::Individual &ind);
// extern void sigma_fitness(ec::Individual &ind);
extern bool is_find_better(ec::Individual &ind, vector<double> &LocalBestFit);
extern string stage;
//************************************************************
// 判断 第tmpGeneIndex位 上的基因是否可以执行 交换 操作
bool is_valid(vector<int> &tmpSwapGroup, int &tmpGeneIndex, vector<int> &Ind, vector<vector<int> > &Bounds){
    // 判断 i 是否已经在 组合内
    if (std::find(tmpSwapGroup.begin(), tmpSwapGroup.end(), tmpGeneIndex) != tmpSwapGroup.end())
    {
        return false;
    }
    // 判断 这个位置上的基因 是否可以和 前一个位置上的基因 交换
    // 判断 i 和 j 是否可以交换
    vector<int> tmpBound = Bounds[tmpSwapGroup.back()];
    if (std::find(tmpBound.begin(), tmpBound.end(), Ind[tmpGeneIndex]) == tmpBound.end())
    {
        return false;
    }

    return true;   
}
//************************************************************
vector<int> get_swap_index(ec::Individual &ind, int &k, int &generation){
    vector<int> SwapIndexs;
    int tmpGeneIndex;
    tmpGeneIndex = random_a_b(0, ind.gene.size() - 1);
    SwapIndexs.push_back(tmpGeneIndex);
    for (int i_k = 1; i_k < k; i_k++)
    {
        tmpGeneIndex = random_a_b(0, ind.gene.size() - 1);
        while ((!is_valid(SwapIndexs, tmpGeneIndex, ind.gene, ind.Bounds)) && (ind.TabuTable[tmpGeneIndex] < generation))
        {
            tmpGeneIndex = random_a_b(0, ind.gene.size() - 1);
        }
        SwapIndexs.push_back(tmpGeneIndex);
    }
    return SwapIndexs;
}

//
double swap_gene(ec::Individual &ind, vector <int> &SwapIndexs){
    
    // 交换操作
    vector<int> tmpInd = ind.gene;
    // 交换操作
    vector<int> oldSwap; // 保存原本的取值
    for (int i = 0; i < SwapIndexs.size(); i++)
    {
        oldSwap.push_back(ind.gene[SwapIndexs[i]]);
    }
    for (int i = 0; i < SwapIndexs.size() - 1; i++)
    {
        std::swap(ind.gene[SwapIndexs[i]], ind.gene[SwapIndexs[i + 1]]);
    }
    // 评价
    vector<double> old_items = ind.items;
    fitness(ind);
    if (ind.items[4] > old_items[4])
    {
        return ind.fit;
    }
    // 修复解
    for (int i = 0; i < SwapIndexs.size(); i++)
    {
        ind.gene[SwapIndexs[i]] = oldSwap[i];
    }
    return ind.fit;
}
//************************************************************
void koptSwapMultiOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxSwapNum, int &k){
    int offsprintNum = random_a_b(10, maxSwapNum); // 要生成子代的个数
    // vector<vector<int> > SwapGroups; // 一组交换的基因信息
    vector<int> SwapGroup;
    vector<double> LocalFitness(offsprintNum, 0);
    // // =========================
    
    int swapCnt = 0; // 要交换的基因个数
    int tmpGeneIndex;
    double tmpFit;
    for (int i = 0; i < offsprintNum; i++)
    {
        swapCnt = random_a_b(2, k);
        SwapGroup = get_swap_index(ind, swapCnt, generation);
        
        // 交换操作
        vector<int> tmpInd = ind.gene;
        // 交换操作
        vector<int> oldSwap; // 保存原本的取值
        for (int i = 0; i < SwapGroup.size(); i++)
        {
            oldSwap.push_back(ind.gene[SwapGroup[i]]);
        }
        for (int i = 0; i < SwapGroup.size() - 1; i++)
        {
            std::swap(ind.gene[SwapGroup[i]], ind.gene[SwapGroup[i + 1]]);
        }
        vector<double> items(5);
        // 评价
        if (stage == EFFICIENCY || stage == RESPONSE || stage == ALL)
        {
            fitness(ind);
        }
        else if (stage == SIGMA)
        {
            fitness(ind);
            // sigma_fitness(ind);
        }
        
        
        // 更新领域最优解
        // 随机挑选一个维度进行比较
        if (is_find_better(ind, LocalBestFit))
        {
            continue;
        }
        
        // 修复解
        for (int i = 0; i < SwapGroup.size(); i++)
        {
            ind.gene[SwapGroup[i]] = oldSwap[i];
        }
    }
}