#include "../../heads/common.h"
#include "../../heads/ec.h"
extern vector<int> ShuffArray(vector<int>);
extern int random_a_b(int, int);
extern void fitness(ec::Individual &ind);
// extern void sigma_fitness(ec::Individual &ind);
extern bool is_find_better(ec::Individual &ind, vector<double> &LocalBestFit);
extern string stage;
//
void get_mu_indexs(ec::Individual &ind, vector<int> &IndexList, vector<int> &ValueList, int &k, int &generation){
    vector<int> tmpBound; // 单个基因的取值集合
    int GeneIndex; // 变异基因的位置
    int MutationValueIndex; // 变异基因的取值
    // 得到k个基因的变异信息
    for (int i_k = 0; i_k < k; i_k++)
    {
        GeneIndex = random_a_b(0, ind.gene.size() - 1);
        // 不能有相同的变异基因
        while ((std::find(IndexList.begin(), IndexList.end(), GeneIndex) != IndexList.end()) && (ind.TabuTable[GeneIndex] < generation))
        {
            GeneIndex = random_a_b(0, ind.gene.size() - 1);
        }
        // 合法的 基因位置
        IndexList.push_back(GeneIndex);
        // tmpBound = ind.Bounds[GeneIndex];
        // MutationValueIndex = random_a_b(0, tmpBound.size() - 1);
        // 检查 变异值
        tmpBound = ind.Bounds[GeneIndex];
        MutationValueIndex = random_a_b(0, tmpBound.size() - 1);
        // 不能变异成一样的值
        while (tmpBound[MutationValueIndex] == ind.gene[GeneIndex])
        {
            MutationValueIndex = random_a_b(0, tmpBound.size() - 1);
        }
        ValueList.push_back(tmpBound[MutationValueIndex]);
    }
}
//
double mutation(ec::Individual &ind, vector<int> &IndexList, vector<int> &ValueList){
    // 执行变异操作
    // vector<int> oldInd = Ind;
    vector<int> oldGene; // 保存原本的取值
    int gIndex;
    for (int i = 0; i < IndexList.size(); i++)
    {
        gIndex = IndexList[i];
        oldGene.push_back(ind.gene[gIndex]);
        ind.gene[gIndex] = ValueList[i];
    }
    

    // 评价
    vector<double> old_items = ind.items;
    fitness(ind);
    if (ind.items[4] > old_items[4])
    {
        return ind.fit;
    }
    
    // 修复解
    for (int i = 0; i < IndexList.size(); i++)
    {
        gIndex = IndexList[i];
        ind.gene[gIndex] = oldGene[i];
    }    
    return ind.fit;
}
//

//************************************************************
void koptMultiMutationOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxMutationNum, int &k){
    int offsprintNum = random_a_b(5, maxMutationNum); // 要生成子代的个数
    vector<vector<int> > IndexLists; //
    vector<vector<int> > ValueLists; //
    vector<int> index_list; //
    vector<int> value_list; //
    vector<double> LocalFitness(offsprintNum, 0);
    double tmpFit;
    // // =========================
    
    int muCnt = 0; // 要变异的基因个数
    bool isUpdate = false;
    int GeneIndex; // 变异基因的位置
    int MutationValueIndex; // 变异基因的取值
    for (int i = 0; i < offsprintNum; i++)
    {
        muCnt = random_a_b(1, k);
        index_list.clear();
        value_list.clear();
        get_mu_indexs(ind, index_list, value_list, muCnt, generation);
        // 执行变异操作
        vector<int> oldGene; // 保存原本的取值
        int gIndex;
        for (int i = 0; i < index_list.size(); i++)
        {
            gIndex = index_list[i];
            oldGene.push_back(ind.gene[gIndex]);
            ind.gene[gIndex] = value_list[i];
        }
        
        // vector<double> items(4);

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
        int dimension = 1;//random_a_b(0, 2);
        if (is_find_better(ind, LocalBestFit))
        {
            continue;
        }
        
        // 修复解
        for (int i = 0; i < index_list.size(); i++)
        {
            gIndex = index_list[i];
            ind.gene[gIndex] = oldGene[i];
        }
        
    }
    
}
