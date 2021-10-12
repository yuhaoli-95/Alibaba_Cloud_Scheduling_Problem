#include "../../heads/common.h"
#include "../../heads/ec.h"
extern string stage;
extern void process_task(Task_Type &task, Staff_Type &staff, processTimeType &ProcessTime, int &tmpBeginTime, int &tmpEndTime, int &LateTime, int &processTime);
extern double calculate_score(task_list &Tasks, staff_list &Staffs, vector<int> &StaffIdList, vector<double> &items);
//************************************************************
void transfer_task(task_list &Tasks, staff_list &Staffs, int &taskIndex, int &scheduleStaffVID, processTimeType &ProcessTime){
    // 如果员工可以及时响应该任务
    if (Staffs[scheduleStaffVID].FreeMoment <= Tasks[taskIndex].GenerationTime)
    {
        return;
    }
    int Begin;
    int End;
    int pTime; // 员工执行任务时长
    int processedStaffId; // 中间处理人员编号
    int processedRealStaffId; // 中间处理人员的真实编号
    int nextTaskIndex;
    // Task_Type tmpTask = Tasks[taskIndex];
    Tasks[taskIndex].processedBegin.clear();
    Tasks[taskIndex].processedEnd.clear();
    Tasks[taskIndex].processedStaffRealId.clear();
    Tasks[taskIndex].processedBegin.push_back(MAX(Tasks[taskIndex].GenerationTime, Staffs[scheduleStaffVID].FreeMoment));
    Tasks[taskIndex].processedEnd.push_back(Staffs[scheduleStaffVID].FreeMoment);
    Tasks[taskIndex].processedStaffRealId.push_back(Staffs[scheduleStaffVID].StaffRealId);
    for (int i = 0; i < 4; i++)
    {
        if (Tasks[taskIndex].processedStaffId[i] == -1)
        {
            continue;
        }
        
        processedStaffId = Tasks[taskIndex].processedStaffId[i];
        processedRealStaffId = Staffs[processedStaffId].StaffRealId;
        nextTaskIndex = Staffs[processedStaffId].TaskList.front();
        // if (Tasks[taskIndex].Id + 1 == 798 && Tasks[taskIndex].StaffRealId == 114)
        // {
        //     int a = 1;
        // }
        // 判断备胎员工是否与预定员工一致
        // vector<int> tmpList = Tasks[taskIndex].processedStaffRealId;
        if (std::find(Tasks[taskIndex].processedStaffRealId.begin(), Tasks[taskIndex].processedStaffRealId.end(), processedRealStaffId) 
                                                    != 
                                    Tasks[taskIndex].processedStaffRealId.end()
            )
        {
            continue;
        }
        // 判断备胎员工能否暂时hold住过渡时间
        // 1. 备胎员工早于预定员工空闲
        // 2.  晚于 塔下一个
        
        // 如果 备胎员工 比 预定员工还要晚 空下来， 则没必要让备胎员工过度
        if (Staffs[processedStaffId].FreeMoment >= Tasks[taskIndex].processedBegin.back())
        {
            continue;
        }
        // 如果备胎员工有下一个任务做， 考虑能否让这个员工过渡
        if (Staffs[processedStaffId].TaskList.size() != 0)
        {
            // 如果 备胎员工的下一个任务 早于 预定员工的空闲时间， 则不能立马将该任务转给 预定员工
            if (Tasks[nextTaskIndex].GenerationTime < Staffs[scheduleStaffVID].FreeMoment)
            {
                continue;
            }
        }
        
        // 临时任务的开始时间应该为 MAX(任务的产生时间, 员工自身的空闲时间) 
        Begin = MAX(Tasks[taskIndex].GenerationTime, Staffs[processedStaffId].FreeMoment);
        Begin = Begin + Tasks[taskIndex].processedOffset[i];
        // 临时任务的结束时间应该为 后面员工的开始处理时间
        End = Tasks[taskIndex].processedBegin.back();
        pTime = ProcessTime[processedRealStaffId][Tasks[taskIndex].TaskTypeId];
        // 如果 不能执行 1分钟以上
        if (End < Begin + 1)
        {
            continue;
        }
        // 备胎员工 不能执行完 任务
        if (End - Begin >= pTime)
        {
            continue;
        }
        

        Tasks[taskIndex].processedBegin.push_back(Begin);
        Tasks[taskIndex].processedEnd.push_back(End);
        Tasks[taskIndex].processedStaffRealId.push_back(processedRealStaffId);
        Staffs[processedStaffId].FreeMoment = End;
        Staffs[processedStaffId].WorkingTime = Staffs[processedStaffId].WorkingTime + End - Begin;
    }
    
}
//************************************************************
void fitness(ec::Individual &ind){
    // 初始化任务和人员
    // task_list &Tasks, staff_list &Staffs, processTimeType &ProcessTime, vector<int> &StaffIdList, 
    // 初始化任务和人员
    for (int i = 0; i < ind.Staffs.size(); i++)
    {
        ind.Staffs[i].init(ind.Staffs[i]);
    }
    int staffVId;
    for (int i = 0; i < ind.Tasks.size(); i++)
    {
        // 初始化 完成任务的员工 的员工编号
        if (stage == EFFICIENCY || stage == ALL)
        {
            staffVId = ind.gene[i];
        }
        else if (stage == RESPONSE || stage == SIGMA)
        {
            staffVId = ind.Tasks[i].StaffVId;
        }
        
        ind.Tasks[i].init(ind.Tasks[i], staffVId, ind.Staffs[staffVId].StaffRealId);
        ind.Staffs[staffVId].TaskList.push(i);
    }
    
    // int tmpStaffIndex;
    // for (int i = 0; i < ind.Tasks.size(); i++)
    // {
    //     // tmpStaffIndex = ind.Tasks[i].processedStaffId[4];
    //     ind.Tasks[i].StaffVId = tmpStaffIndex;
    //     ind.Tasks[i].StaffRealId = ind.Staffs[tmpStaffIndex].StaffRealId;
    //     ind.Staffs[tmpStaffIndex].TaskList.push(i);
    // }
    if (stage == RESPONSE)
    {    
        for (int s_i = 0; s_i < 4; s_i++)
        {
            for (int i = 0; i < ind.Tasks.size(); i++){
                ind.Tasks[i].processedStaffId[s_i] = ind.gene[s_i * ind.Tasks.size() + i];
            }
        }
    }
    else if (stage == SIGMA)
    {
        for (int t_i = 0; t_i < ind.Tasks.size(); t_i++)
        {
            for (int off_i = 0; off_i < 4; off_i++)
            {
                ind.Tasks[t_i].processedOffset[off_i] = ind.gene[t_i + off_i];
            }
            
        }
        
    }
    else if (stage == ALL)
    {
        for (int s_i = 1; s_i < 5; s_i++)
        {
            for (int i = 0; i < ind.Tasks.size(); i++){
                ind.Tasks[i].processedStaffId[s_i - 1] = ind.gene[(s_i - 1) * ind.Tasks.size() + i];
            }
        }
    }
    
    
    /*
                                    开始计算
    */
    Task_Type tmpTask; //遍历任务
    int sch_staffId; // 预定员工ID
    // int staffId; // 执行任务的实际员工ID
    int tmpBeginTime;
    int tmpEndTime;
    int tmpLateTime;
    int tmpProcessTime;
    int totalDelay; // 累计延误
    int fenzi;
    for (int i = 0; i < ind.Tasks.size(); i++)
    {
        sch_staffId = ind.Tasks[i].StaffVId;
        if (stage != EFFICIENCY)
        {
            transfer_task(ind.Tasks, ind.Staffs, i, sch_staffId, ind.processTime);
        }
        
        // 删除执行过的任务
        int deleteTask;
        deleteTask = ind.Staffs[sch_staffId].TaskList.front();
        ind.Staffs[sch_staffId].TaskList.pop();
        
        
        // 由staffId员工开始处理问题
        tmpBeginTime = -1;
        tmpEndTime = -1;
        tmpLateTime = -1;
        tmpProcessTime = -1;
        process_task(ind.Tasks[i], ind.Staffs[sch_staffId], ind.processTime, \
                        tmpBeginTime, tmpEndTime, tmpLateTime, tmpProcessTime);
        ind.Staffs[sch_staffId].FreeMoment = tmpEndTime;
        ind.Staffs[sch_staffId].WorkingTime = ind.Staffs[sch_staffId].WorkingTime + tmpProcessTime;
        
        ind.Tasks[i].process_info(ind.Tasks[i], tmpBeginTime, tmpEndTime, tmpLateTime, ind.Staffs[sch_staffId].StaffRealId, ind.Staffs[sch_staffId].StaffVId);
        
    }
    ind.fit = calculate_score(ind.Tasks, ind.Staffs, ind.StaffIdList, ind.items);
    
}