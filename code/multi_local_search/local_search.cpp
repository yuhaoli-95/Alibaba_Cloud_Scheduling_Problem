#include "../../heads/common.h"
#include "../../heads/ec.h"
extern clock_t Start, End;
extern double initFit;
extern int MaxRunningTime;
extern ec::Individual BestInd;
//*********
extern int random_a_b(int, int);
extern void write_csv_file(task_list &Tasks, processTimeType &processTime, string filename);
// 计算个体的适应度值
extern void fitness(ec::Individual &ind);
// extern void sigma_fitness(ec::Individual &ind);
// 交换算子
extern void koptSwapMultiOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxMutationNum, int &k);
// 变异算子
extern void koptMultiMutationOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxMutationNum, int &k);
// 混合算子
// extern void koptMultiMixedOperator(int &generation, ec::Individual &ind, vector<double> &LocalBestFit, int &maxMutationNum, int &k);
//*********
extern void logInfo(string &info, int &g, int &ExecutionTime, double &fit, vector<double> &items, ofstream &logInfoFile);
extern void save_ind_info(ec::Individual &ind, string filename);
extern void recursion_scan_dir_file(char *dir, int depth);
//*********
void multiOBJ_local_search(ec::Individual &ind, string &stage){
    // ofstream logInfoFile("../prediction_result/log_Info.csv");
    string info;
    bool isUpdate = false;
    int ExecutionTime = 0;// 程序运行时长， 单位： 秒

    vector<double> LocalBestFit(5);
    LocalBestFit = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 0};
    vector<double> OldLocalBestFit(5);
    OldLocalBestFit = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 0};
    fitness(ind);
    LocalBestFit = ind.items;
    
    int maxMutationOffspringNum = 1000; // 每次迭代中，最大子代的个数
    int maxSwapOffspringNum = 1000; // 每次迭代中，最大交换次数
    
    int kSwap = 2; // 交换基因的个数
    int kMutation = 2; // 变异基因的个数
    // if (stage == EFFICIENCY)
    // {
    //     kSwap = 2; // 交换基因的个数
    //     kMutation = 2; // 变异基因的个数
    // }
    // else if (stage == ALL)
    // {
    //     kSwap = 100; // 交换基因的个数
    //     kMutation = 100; // 变异基因的个数
    // }
    // if (kSwap < 0 || kMutation < 0)
    // {
    //     cout << "kSwap 或者 kMutation 的值小于0！！" << endl;
    //     throw new logic_error("");
    // }
    
    

    int g = 1; // 迭代次数
    ExecutionTime = (int)((clock() - Start) / CLOCKS_PER_SEC);

    int operatorIndex; // 选择操作子
    int tmpK;
    int a = 1, b = 3;
    bool isupdate = true;
    vector<int> TabuIndex;
    while (ExecutionTime < MaxRunningTime - 10) //ExecutionTime < MaxRunningTime - 10
    {
        OldLocalBestFit = LocalBestFit;
        operatorIndex = random_a_b(a, b);
        if (operatorIndex == 1)
        {
            // 进行变异算子计算
            tmpK = random_a_b(2, kMutation);
            
            koptMultiMutationOperator(g, ind, LocalBestFit, maxMutationOffspringNum, tmpK);
            
            
        }
        else if (operatorIndex == 2){
            // 进行k交换算子计算
            tmpK = random_a_b(2, kSwap);
            koptSwapMultiOperator(g, ind, LocalBestFit, maxMutationOffspringNum, tmpK);
            
            
        }
        else if (operatorIndex == 3)
        {
            // 进行单个基因的变异算子计算
            tmpK = 1;
            koptMultiMutationOperator(g, ind, LocalBestFit, maxMutationOffspringNum, tmpK);
            
        }
        fitness(ind);
        
        ExecutionTime = (int)((clock() - Start) / CLOCKS_PER_SEC);

        if (ind.items[4] > OldLocalBestFit[4])
        {
            info = "g: " + to_string(g) + ", time: " + to_string(ExecutionTime) + ", " + to_string(ind.fit) + ", sigma: " + to_string(ind.items[0]) + ", M: " + to_string(ind.items[1]) + ", e: " + to_string(ind.items[2]) + ", delay: " + to_string(ind.items[3]);
            cout << info << endl;
            // write_csv_file(ind.Tasks, ind.processTime, "./result.csv");
        }
        
        if (ind.fit > BestInd.fit)
        {
            BestInd = ind;
            fitness(BestInd);
            write_csv_file(BestInd.Tasks, BestInd.processTime, "./result.csv");
        }
        
        
        if (stage == EFFICIENCY && ExecutionTime > 0.4 * MaxRunningTime)
        {
            break;   
        }
        // else if (stage == RESPONSE)
        // {
        //     if (ind.fit > 325)
        //     {
        //         break;
        //     }
        // }
        else if (stage == ALL && ExecutionTime > 0.8 * MaxRunningTime)
        {
            break;
        }
        
        // else if (stage == SIGMA)
        // {
        //     if (ind.items[0] > 0.36)
        //     {
        //         break;
        //     }
        // }
        
        
        g++;
    }
    // // 如果实时更新的话不要添加这一行，不然会 重置 result.csv
    // fitness(ind);
    // write_csv_file(ind.Tasks, ind.processTime, "./result.csv");
    
    // save_ind_info(ind, "./Ind.csv");
}