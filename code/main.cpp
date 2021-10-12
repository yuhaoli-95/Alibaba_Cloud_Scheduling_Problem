#include "../heads/common.h"
#include "../heads/ec.h"
//************************************************************
clock_t Start, End;
int MaxRunningTime = 20 * 60; // 程序运行时长，单位：秒
double initFit;
ec::Individual BestInd;
string stage;
//*********
extern void logInfo(string &info, int &g, int &ExecutionTime, double &fit, vector<double> &items, ofstream &logInfoFile);
extern void getDirectory();
// 读取文件
extern task_list read_task_info_str(string taskInfo);
extern void GetFileNames(string path,vector<string>& filenames);
// extern ifstream open_csv_file(string filename);
extern task_list read_task_file(string filename);
extern staff_list read_staff(string filename, vector<int> &StaffIdList);
extern processTimeType read_process_time(string filename, vector<int> &StaffIdList);
extern void write_csv_file(task_list Tasks, string filename);
// 基于贪心算法的初始化
extern vector<int> Iniltialize(task_list &Tasks, staff_list &Staffs, processTimeType &ProcessTime, vector<vector<int> > &Bounds, ec::Individual &ind);
extern vector<int> Iniltialize_from_csv(string filename);
// 得到各个变量的取值范围
extern vector<vector<int> > GetBounds(task_list &Tasks, staff_list &Staffs, processTimeType &processTime);
// 计算个体的适应度值
extern void fitness(ec::Individual &ind);
//************************************************************
// // 局部搜索
extern void multiOBJ_local_search(ec::Individual &ind, string &stage);
//************************************************************
int main(int a, char** args) {
    cout << "------ OPTIMIZATION START------" << std::endl;
    getDirectory();
    Start = clock();
    // 读入基础数据
    vector<string> file_name;
    
    
    string filePath = "./tcdata/";
    string filename;
    ifstream dataFile;
    string encoding;

    srand(1);
    // 读取任务信息
    filename = filePath + "sic_semi_work_order.csv";
    task_list Tasks = read_task_file(filename);

    cout << "               读取 workload 文件完成" << endl;
    // 读取员工信息
    filename = filePath + "sic_semi_process_time_matrix.csv";
    vector<int> StaffIdList;
    staff_list Staffs = read_staff(filename, StaffIdList);

    cout << "               创建 staff 变量完成" << endl;
    // 读取处理时长信息
    filename = filePath + "sic_semi_process_time_matrix.csv";
    processTimeType ProcessTime = read_process_time(filename, StaffIdList);

    cout << "               读取 sic_semi_process_time_matrix 文件完成" << endl;
    /*               测试docker                                            */ 
    vector<vector<int> > Bounds;
    ec::Individual ind;
    /*               优化 EFFICIENCY                                            */ 
    cout << "============================== 优化效率 ==============================" << endl;
    cout << "最大运行时间: " << MaxRunningTime << "s" << endl;
    stage = EFFICIENCY;
    // 得到每个变量的取值范围
    Bounds = GetBounds(Tasks, Staffs, ProcessTime);
    cout << "得到 " << Bounds.size() << " 个变量取值范围完成" << endl;
    // 初始化
    ind.gene = Iniltialize(Tasks, Staffs, ProcessTime, Bounds, ind);
    cout << "初始化 " << ind.gene.size() <<" 个基因完成" << endl;
    ind.init(ind, Bounds);
    ind.Tasks = Tasks;
    ind.Staffs = Staffs;
    ind.processTime = ProcessTime;
    ind.StaffIdList = StaffIdList;
    fitness(ind);
    initFit = ind.fit;
    cout << "计算初始解fitness：" << ind.fit << endl;
    
    BestInd = ind;
    cout << "进入效率优化模块" << endl;
    multiOBJ_local_search(ind, stage);

    /*               优化M                                            
                    在这个阶段里，每个task的staffVid都被固定住了
                        
    */ 
    // cout << "============================== 优化响应 ==============================" << endl;
    // // 初始化 计时变量start
    // Start = clock();
    // stage = RESPONSE;

    // Bounds = GetBounds(Tasks, Staffs, ProcessTime);
    // cout << "得到 " << Bounds.size() << " 个变量取值范围完成" << endl;
    // ind.gene = Iniltialize(Tasks, Staffs, ProcessTime, Bounds, ind);
    // cout << "初始化 " << ind.gene.size() <<" 个基因完成" << endl;

    // ind.init(ind, Bounds);
    // fitness(ind);
    // cout << "计算初始解fitness：" << ind.fit << endl;
    
    // BestInd = ind;
    // cout << "进入响应优化模块" << endl;
    // multiOBJ_local_search(ind, stage);
    
    
    /*               优化 response + sigma                                            */ 
    cout << "============================== 优化 ALL ==============================" << endl;
    cout << "最大运行时间: " << MaxRunningTime << "s" << endl;
    // Start = clock();
    stage = ALL;

    Bounds = GetBounds(Tasks, Staffs, ProcessTime);
    // ==========================================================
    // // 如果有第一阶段的优化，则将以下代码注释掉
    // ind.Tasks = Tasks;
    // ind.Staffs = Staffs;
    // ind.processTime = ProcessTime;
    // ind.StaffIdList = StaffIdList;
    // ==========================================================
    cout << "得到 " << Bounds.size() << " 个变量取值范围完成" << endl;
    ind.gene = Iniltialize(Tasks, Staffs, ProcessTime, Bounds, ind);
    cout << "初始化 " << ind.gene.size() <<" 个基因完成" << endl;

    ind.init(ind, Bounds);
    fitness(ind);
    cout << "计算初始解fitness：" << ind.fit << endl;
    
    BestInd = ind;
    cout << "进入ALL优化模块" << endl;
    multiOBJ_local_search(ind, stage);
    // /*               优化sigma                                            */ 
    cout << "============================== 优化工时均衡 ==============================" << endl;
    // 初始化 计时变量start
    // Start = clock();
    stage = SIGMA;

    Bounds = GetBounds(Tasks, Staffs, ProcessTime);
    cout << "得到变量取值范围完成" << endl;
    ind.gene = Iniltialize(Tasks, Staffs, ProcessTime, Bounds, ind);
    cout << "初始化完成" << endl;

    ind.init(ind, Bounds);
    fitness(ind);
    cout << "计算初始解fitness" << endl;
    
    BestInd = ind;
    cout << "进入优化模块" << endl;
    multiOBJ_local_search(ind, stage);
    return 0;
}