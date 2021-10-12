#include "../../heads/common.h"
extern double variance(vector<double> &x);
extern double average(vector<double> &x);

//************************************************************
// 计算得分
double calculate_score(task_list &Tasks, staff_list &Staffs, vector<int> &StaffIdList, vector<double> &items){
    int a = 99;
    int b = 99;
    int c = 1000;
    // 服务技术专家负荷的总体标准差
    double sigmaL; // 服务技术专家负荷的总体标准差
    double sum = 0;
    double tmpL; // 服务技术专家i负荷
    vector<double> L; // 服务技术专家负荷
    int sVId = 0;
    for (int i = 0; i < StaffIdList.size(); i++)
    {
        tmpL = 0;
        for (int j = 0; j < Staffs.size(); j++)
        {
            if (Staffs[j].StaffRealId == StaffIdList[i] - 1)
            {
                tmpL = tmpL + Staffs[j].WorkingTime;
            }
        }
        L.push_back(tmpL / (60 * 8 * 3));
    }
    sigmaL = std::sqrt(variance(L));
    items[0] = sigmaL;
    // 任务平均响应超时量
    double avg_timeout_response;
    double totalM;
    double tmpM; //单个任务超时
    vector<double> M; // 任务超时
    double fenzi, fenmu;
    for (int i = 0; i < Tasks.size(); i++)
    {
        fenzi = (double) (MAX(Tasks[i].processedBegin.back() - Tasks[i].GenerationTime - Tasks[i].ResponseTime, 0));
        fenmu = (double)Tasks[i].ResponseTime;
        tmpM = fenzi / fenmu;
        M.push_back(tmpM);
    }
    avg_timeout_response = average(M);
    items[1] = avg_timeout_response;
    // 任务平均处理效率
    double avg_efficiency; // 平均效率
    double tmpEfficiency; // 单个任务的效率
    // vector<double> efficiency; // 全部任务的效率
    double efficiency_fenzi = 0; // 全部任务的效率
    double efficiency_fenmu = 0;
    for (int i = 0; i < Tasks.size(); i++)
    {
        // fenzi = (double)(Tasks[i].EndTime - Tasks[i].BeginTime);
        efficiency_fenzi = efficiency_fenzi + (double)(Tasks[i].MinProcessTime);
        efficiency_fenmu = efficiency_fenmu + (double)(Tasks[i].EndTime - Tasks[i].GenerationTime);
        // fenzi = fenzi + (double)(Tasks[i].MinProcessTime);
        // fenmu = fenmu + (double)(Tasks[i].EndTime - Tasks[i].GenerationTime);
        // efficiency.push_back(tmpEfficiency);
    }
    // tmpEfficiency = fenzi / fenmu;
    // avg_efficiency = average(efficiency);
    avg_efficiency = efficiency_fenzi / efficiency_fenmu;
    items[2] = avg_efficiency;
    // 计算延误时长
    double totalDelay = 0;
    for (int i = 0; i < Tasks.size(); i++)
    {
        totalDelay = totalDelay + (Tasks[i].BeginTime - Tasks[i].GenerationTime);
    }
    items[3] = totalDelay / Tasks.size();
    // 计算得分
    // double score = (c * avg_efficiency) / (a * avg_timeout_response + b * sigmaL);
    double score = (c * avg_efficiency) - (a * avg_timeout_response) - (b * sigmaL);

    
    items[4] = score;
    return score;
}