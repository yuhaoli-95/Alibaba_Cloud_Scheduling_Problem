// #include "../../heads/common.h"
// #include "../../heads/ec.h"
// extern vector<int> ShuffArray(vector<int>);
// extern int random_a_b(int, int);
// extern void fitness(ec::Individual &ind);
// extern bool is_find_better(ec::Individual &ind, vector<double> &LocalBestFit);
// extern string stage;
// //
// extern void get_mu_indexs(ec::Individual &ind, vector<int> &IndexList, vector<int> &ValueList, int &k, int &generation);
// extern vector<int> get_swap_index(ec::Individual &ind, int &k, int &generation);
// //************************************************************
// void koptMultiMixedOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxMixedNum, int &k){
//     int offsprintNum = random_a_b(5, maxMixedNum); // 要生成子代的个数
//     vector<vector<int> > IndexLists; //
//     vector<vector<int> > ValueLists; //
//     vector<int> index_list; //
//     vector<int> value_list; //
//     vector<double> LocalFitness(offsprintNum, 0);
//     double tmpFit;
//     // // =========================
    
//     int swapCnt = 0;
//     bool isUpdate = false;
//     int GeneIndex; // 变异基因的位置
//     int MutationValueIndex = 0; // 变异基因的取值
//     vector<int> SwapGroup; // 
//     int muNum = 0;
//     int swapNum = 0;
//     int gIndex; // 要操作的基因的位置
//     for (int i = 0; i < offsprintNum; i++)
//     {
//         vector<int> oldInd = ind.gene;
//         // 保存原本的取值
//         vector<int> oldGeneValue; // 要操作的基因的取值列表
//         vector<int> gIndex_list; // 要操作的基因的位置列表
        
//         // 执行变异操作
//         muNum = random_a_b(1, k-2);
//         if (muNum == k - 2)
//         {
//             int a = 1;
//         }
        
//         index_list.clear();
//         value_list.clear();
//         get_mu_indexs(ind, index_list, value_list, muNum, generation);
//         for (int i = 0; i < muNum; i++)
//         {
//             gIndex = index_list[i];
//             gIndex_list.push_back(gIndex);
//             oldGeneValue.push_back(ind.gene[gIndex]);
//         }
        
//         // 交换操作
//         swapNum = random_a_b(2, k - muNum);
//         SwapGroup.clear();
//         SwapGroup = get_swap_index(ind, swapNum, generation);

//         for (int i = 0; i < swapNum; i++)
//         {
//             gIndex = SwapGroup[i];
//             gIndex_list.push_back(gIndex);
//             oldGeneValue.push_back(ind.gene[gIndex]);
//         }


//         // 执行变异操作
        


//         // 评价
//         fitness(ind);

//         // 更新领域最优解
//         if (is_find_better(ind, LocalBestFit))
//         {
//             continue;
//         }
        
//         // 修复解
//         for (int i = 0; i < muNum + swapNum; i++)
//         {
//             // gIndex = index_list[i]; 
//             // ind.gene[gIndex] = oldGene[i]; 
//             gIndex = gIndex_list[i]; 
//             ind.gene[gIndex] = oldGeneValue[i];
//         }
//         if (oldInd != ind.gene)
//         {
//             cout << "mixed算子的修复解步骤有问题" << endl;
//             throw new logic_error("");
//         }
        
//     }
// }
