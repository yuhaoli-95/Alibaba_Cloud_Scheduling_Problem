#include "../heads/common.h"

extern string stage;
//************************************************************
// read flight data
vector<vector<int> > GetBounds(task_list &Tasks, staff_list &Staffs, processTimeType &processTime) {
    vector<int> tmpBound; // 一个变量的取值范围
    vector<vector<int> > Bounds; // 所有变量的取值范围
    vector<int> processtimeList;
    int minProcessTimeIndex; //  处理任务的最小时间
    int minProcessTime; //  处理任务的最小时间
    int tmpMaxProcessTime;
    // 遍历每一个任务，得到这个任务的员工可能取值
    for (size_t i_t = 0; i_t < Tasks.size(); i_t++)
    {
        // 得到每类任务的最大处理时间
        tmpMaxProcessTime = -1;
        for (int i_s = 0; i_s < Staffs.size(); i_s++)
        {
            // 根据人员技能与任务类型约束，筛出一部分员工
            int staffRealId = i_s / 3;
            if (processTime[staffRealId][Tasks[i_t].TaskTypeId] > tmpMaxProcessTime){
                tmpMaxProcessTime = processTime[staffRealId][Tasks[i_t].TaskTypeId];
            }
        }
        tmpBound.clear();
        processtimeList.clear();
        for (int i_s = 0; i_s < Staffs.size(); i_s++)
        {
            // 根据人员技能与任务类型约束，筛出一部分员工
            int staffRealId = i_s / 3;
            processtimeList.push_back(processTime[staffRealId][Tasks[i_t].TaskTypeId]);
            // tmpMaxProcessTime = MIN(tmpMaxProcessTime - 1, 1000);
            if (processTime[staffRealId][Tasks[i_t].TaskTypeId] >= tmpMaxProcessTime -1){
                int a = 1;
                continue;
            }
            tmpBound.push_back(i_s);
        }
        Bounds.push_back(tmpBound);        
        minProcessTimeIndex = min_element(processtimeList.begin(), processtimeList.end()) - processtimeList.begin();
        minProcessTime = processtimeList[minProcessTimeIndex];
        Tasks[i_t].MinProcessTime = minProcessTime;
    }
    if (stage == EFFICIENCY)
    {
        return Bounds;
    }
    else if (stage == RESPONSE)
    {
        vector<vector<int> > NewBounds; // 所有变量的取值范围
        // // 如果筛选员工
        // for (int i = 0; i < 4; i++)
        // {
        //         for (size_t i_t = 0; i_t < Tasks.size(); i_t++)
        //     {
        //         NewBounds.push_back(Bounds[i_t]);        
        //     }
        // }
        // 如果不筛选员工
        tmpBound.clear();
        NewBounds.clear();
        for (int i_s = 0; i_s < Staffs.size(); i_s++)
        {
            // 根据人员技能与任务类型约束
            // 不筛员工
            tmpBound.push_back(i_s);
        }
        
        for (int i = 0; i < 4; i++)
        {
            for (size_t i_t = 0; i_t < Tasks.size(); i_t++)
            {
                NewBounds.push_back(tmpBound);        
            }
        }
        return NewBounds;
    }
    else if (stage == SIGMA)
    {
        vector<vector<int> > NewBounds; // 所有变量的取值范围
        tmpBound.clear();
        NewBounds.clear();
        for (int i_s = 0; i_s < 50; i_s++)
        {
            // 根据人员技能与任务类型约束
            // 不筛员工
            tmpBound.push_back(i_s);
        }
        
        for (int i = 0; i < 4; i++)
        {
            for (size_t i_t = 0; i_t < Tasks.size(); i_t++)
            {
                NewBounds.push_back(tmpBound);        
            }
        }
        return NewBounds;
    }
    else if (stage == ALL)
    {
        vector<vector<int> > NewBounds = Bounds; // 所有变量的取值范围
        tmpBound.clear();
        for (int i_s = 0; i_s < Staffs.size(); i_s++)
        {
            // 根据人员技能与任务类型约束
            // 不筛员工
            tmpBound.push_back(i_s);
        }
        
        for (int i = 0; i < 4; i++)
        {
            for (size_t i_t = 0; i_t < Tasks.size(); i_t++)
            {
                NewBounds.push_back(tmpBound);        
            }
        }
        cout << NewBounds.size() << endl;
        return NewBounds;
    }
    
    
}