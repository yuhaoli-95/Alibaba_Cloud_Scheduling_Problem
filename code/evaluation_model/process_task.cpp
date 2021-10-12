#include "../../heads/common.h"

//************************************************************
// 单个虚拟员工 处理 单个任务的 时间节点
void process_task(Task_Type &task, Staff_Type &staff, processTimeType &ProcessTime, int &tmpBeginTime, int &tmpEndTime, int &tmpLateTime, int &tmpProcessTime){
    int tmpFreeMoment; // 员工的最早空闲时间
    bool isLate; // 员工是否会迟到
    tmpFreeMoment = staff.FreeMoment;
    // 计算迟到时间
    tmpLateTime = MAX(0, tmpFreeMoment - task.GenerationTime);
    // 计算由员工i执行该任务的实际开始时间
    tmpBeginTime = MAX(tmpFreeMoment, task.GenerationTime);
    // 计算是否会迟到
    isLate = SIGN(tmpBeginTime, task.GenerationTime);
    // 执行任务时间
    tmpProcessTime = ProcessTime[staff.StaffRealId][task.TaskTypeId];
    // 计算由员工i执行该任务的实际结束时间
    tmpEndTime = tmpBeginTime + tmpProcessTime;
}
