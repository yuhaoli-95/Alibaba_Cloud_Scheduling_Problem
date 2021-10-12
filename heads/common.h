#ifndef _COMMON_H

    #define _COMMON_H

    #define _CRT_SECURE_NO_DEPRECATE
    #include <stdlib.h>
    #include <unistd.h>
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    #include <fstream>
    #include <sstream>
    #include <cmath>
    #include <ctime>
    #include <vector>
    #include <queue>
    #include <list>
    #include <string.h>
    #include <ctime>
    #include <string>
    #include <time.h>
    #include <map>
    #include <malloc.h>
    #include <thread>
    #include <algorithm>
    #include <mutex>
    #include <exception>
    
    #include <locale>
    #include <codecvt>

    #include <sys/types.h>
    #include <sys/stat.h>
    // #include <unistd.h>
    #include <dirent.h>
    // #include <iostream>
    // #include <string.h>
    #include <stdio.h>
    #include <fcntl.h>
    // #include <stdlib.h>
    
    using namespace std;
    #define MAX_INT (((unsigned int)(-1))>>1) /* max value*/ 
    #define DBL_MAX 1.7976931348623158e+308 /* max value */
    #define Iter_Max 100000   //max iteration
    #define Tabu_tenure 20   // tabu setting
    #define MAX(a, b) a > b? a : b // get max between a and b
    #define MIN(a, b) a < b? a : b // get min between a and b
    #define SIGN(a, b) a > b? 1 : 0 // 判断a是否大于b
    #define STEPS 10
    
    #define EFFICIENCY "efficiency"
    #define RESPONSE "response"
    #define SIGMA "sigma"
    #define ALL "all"

    #define ENCODING_ASCII				"ASCII"
    #define ENCODING_UTF8       		"UTF8"
    #define ENCODING_UTF8_WITH_BOM		"UTF8_WITH_BOM"
    #define ENCODING_UTF16LE			"UTF16LE"
    #define ENCODING_UTF16BE			"UTF16BE"

    extern string stage;

    string ConvertTime(int inputTimeDouble);
    void clear(queue<int>& q);

    // [员工Id][任务类型Id]
    typedef map<int, map<int, int> > processTimeType; 

    // 任务结构体
    struct Task_Type{
        // 任务的固定属性
        int Id; // 任务唯一编号
        int TaskTypeId; // 任务类型编号
        int GenerationTime;   // 任务产生时间
        int ResponseTime; // 响应时长
        int MaxStaffCnt; // 处理任务的最大员工数
        int MinProcessTime; // 处理该任务的最小时间
        
        // 每次评价会改变的字段
        int StaffRealId; // 处理该任务的员工编号
        int StaffVId; // 处理该任务的虚拟员工编号
        double late; // 任务的延误时间
        vector<int> processedStaffId; // 处理这个任务的员工索引列表， 根据约束，最多只能5个人
        vector<int> processedStaffRealId; // 处理这个任务的员工索引列表， 根据约束，最多只能5个人
        vector<int> processedOffset; // 处理这个任务的员工的开始任务偏移时间
        
        int BeginTime;   // 开始处理时间
        int EndTime;   // 处理结束时间

        vector<int> processedBegin; // 任务的每次开始时间
        vector<int> processedEnd; // 任务的每次开始时间
        

        // 输出 csv 格式
        string to_csv(Task_Type &t, processTimeType &processTime, int &cnt){
            vector<string> taskInfo;
            string tmpTaskStr = "";
            int Id = t.Id;
            int StaffRealId;
            int BeginTime;
            if (t.processedBegin.size() > 2)
            {
                int a = 1;
            }
            
            while (t.processedBegin.size() > 0)
            {
                StaffRealId = t.processedStaffRealId.back();
                t.processedStaffRealId.pop_back();
                BeginTime = t.processedBegin.back();
                t.processedBegin.pop_back();

                taskInfo.clear();
                taskInfo.push_back(to_string(Id + 1));
                taskInfo.push_back(to_string(StaffRealId + 1));
                taskInfo.push_back(to_string(BeginTime));
                tmpTaskStr = tmpTaskStr + taskInfo[0];
                for (int i = 1; i < taskInfo.size(); i++){
                    tmpTaskStr += "," + taskInfo[i];
                }
                tmpTaskStr = tmpTaskStr + "\n";
            }
            return tmpTaskStr;
        }

        // 初始化任务
        void init(Task_Type &t, int &staffId, int &staffRealId){
            t.BeginTime = 0;
            t.EndTime = 0;
            t.late = 0;
            t.MaxStaffCnt = 5;
            // 清空
            if (stage == EFFICIENCY)
            {
                t.processedStaffId = {-1, -1, -1, -1};
                t.processedStaffRealId = {-1};
                t.processedBegin = {0};
                t.processedEnd = {0};
                t.StaffRealId = staffRealId;
                t.StaffVId = staffId;
            }
            else if (stage == RESPONSE)
            {
                t.processedStaffId = {-1, -1, -1, -1};
                t.processedStaffRealId = {-1};
                t.processedBegin = {0};
                t.processedEnd = {0};
                t.processedOffset = {0, 0, 0, 0, 0};
            }
            else if (stage == SIGMA)
            {
                t.processedBegin = {0};
                t.processedEnd = {0};
            }
            else if (stage == ALL)
            {
                t.processedStaffId = {-1, -1, -1, -1};
                t.processedStaffRealId = {-1};
                t.processedBegin = {0};
                t.processedEnd = {0};
                t.processedOffset = {0, 0, 0, 0, 0};
                
                t.StaffRealId = staffRealId;
                t.StaffVId = staffId;
            }
            
            else
            {
                cout << "task初始化中，碰到了没有预见的优化项！！！" << endl;
                throw new logic_error("");
            }
            
            
        }
        // 添加任务信息
        void process_info(Task_Type &t, int &BeginTime, int &EndTime, int &late, int &staffRealId, int &staffVId){
            t.late = late;
            t.StaffRealId = staffRealId;
            t.StaffVId = staffVId;
            t.BeginTime = BeginTime;
            t.EndTime = EndTime;            
            t.processedStaffRealId[0] = staffRealId;
            t.processedBegin[0] = BeginTime;
            t,processedEnd[0] = EndTime;
            
        }



        // 读取 csv 文件
        void from_csv(Task_Type &t, vector<int> &info){
            t.Id = info[0] - 1;
            t.GenerationTime = info[1];
            t.TaskTypeId = info[2];
            t.ResponseTime = info[3];
        }
    };

    
    /*
                    员工类型
    */
    // staff type
    struct Staff_Type{
        // 员工的固定属性
        int StaffRealId; // 员工编号
        int StaffVId; // 员工虚拟编号

        // 以下为在评价模型中会被更新的字段
        queue<int> TaskList; // 任务列表
        int FreeMoment; // 空闲时间
        int WorkingTime; // 员工的累计工作时长

        // 在第三阶段优化 工时均衡 时，需要固定员工的 预定任务的累计工作时长
        int prefexWorkingTime;

        // 在评价的时候将以下字段初始化
        void init(Staff_Type &s){
            s.FreeMoment = 0;
            s.WorkingTime = 0;
            clear(s.TaskList);
        }
        // 读取 csv 格式
        void from_csv(Staff_Type& s, int StaffRealId, int StaffVId)
        {
            s.FreeMoment = -1;
            s.StaffVId = StaffVId;
            s.StaffRealId = StaffRealId;
            s.WorkingTime = 0;
        }
    };


    typedef vector<Staff_Type> staff_list;
    typedef vector<Task_Type> task_list;
#endif