#include "../heads/common.h"
//
extern std::string check_file_encoding(std::string path);
//************************************************************
ifstream open_csv_file(string filename){
    ifstream datafile(filename);
    if (datafile.is_open())
    {
        return datafile;
    }
    else
    {
        cout << filename << "： 文件不存在！！！" << endl;
        throw new std::runtime_error("file not existes");
    }
}
//************************************************************
ifstream pro_treatment(std::string &encoding, string filename){
    encoding =  check_file_encoding(filename);
    cout << "文件： " << filename << " 编码方式为： " << encoding << endl;
    ifstream filedata = open_csv_file(filename);
    cout << "能成功打开 文件： " << filename << "  ！！" << endl;    
    if (encoding == ENCODING_UTF8_WITH_BOM)
    {
        int ch1 = filedata.get();
        int ch2 = filedata.get();
        int ch3 = filedata.get();
        cout << "文件是utf-8 with bom格式，已跳过开头3字符。" << endl;
    }
    return filedata;
}
//************************************************************
// read running time matrix
processTimeType read_process_time(string filename, vector<int> &StaffIdList)
{
    // 打开文件
    string encoding;
    ifstream processTimeData = pro_treatment(encoding, filename);
    // 读取数据
    processTimeType ProTimeMatrix; // RunTime between two point
    // ifstream runtimefile;                           // file input operation
    vector<int> TaskTypeList;                         // name of all positions
    string s, Oneline, StaffId;
    int processTime;
    // get position name
    getline(processTimeData, Oneline);
    istringstream Readstr(Oneline);
    getline(Readstr, s, ','); // flit the first blank " "
    while (getline(Readstr, s, ','))
    {
        TaskTypeList.push_back(atoi(s.c_str()));
    }
    // get runtime matrix
    for (int i = 0; i < StaffIdList.size(); i++)
    {
        getline(processTimeData, Oneline); // get one line
        istringstream Readstr(Oneline);
        getline(Readstr, StaffId, ','); // get the start position
        for (int j = 0; j < TaskTypeList.size(); j++)
        {
            getline(Readstr, s, ','); // get the distance between pos[i] and pos[j]
            ProTimeMatrix[atoi(StaffId.c_str()) - 1][TaskTypeList[j]] = atof(s.c_str());
        }
    }
    processTimeData.close();
    // for (int i = 0; i < StaffIdList.size(); i++)
    // {
    // 	for (int j = 0; j < TaskTypeList.size(); j++)
    // 	{
    // 		printf("%d ", ProTimeMatrix[StaffIdList[i]][TaskTypeList[j]]);
    // 	}
    // 	printf("\n");
    // }
    return ProTimeMatrix;
}
//************************************************************
// open json
task_list read_task_file(string filename)
{
    // 打开文件
    string encoding;
    ifstream taskData = pro_treatment(encoding, filename);
    // 读取数据
    task_list Tasks;    
    string lineStr;
    int cnt = 1;
    int maxCnt = 300;
	while(getline(taskData, lineStr)) //getline来自sstream
	{        
        Task_Type tmpTask;
		//存成二维表结构
		stringstream ss(lineStr);//来自sstream
		string str;
		vector<int> lineArray;
		//按照逗号分隔
		while(getline(ss, str, ',')){
			lineArray.push_back(atoi(str.c_str()));//一行数据以vector保存
        }
        if (cnt == 1 && lineArray.front() != 1)
        {
            cout << "读入的第一行的编号不为1(id = " << lineArray.front() << ")，现在手动改变第一个任务的id" << endl;
            lineArray.front() = 1;
        }
        tmpTask.from_csv(tmpTask, lineArray);
        tmpTask.MaxStaffCnt = 5;
        Tasks.push_back(tmpTask);
        ++cnt;
	}
    taskData.close();
    return Tasks;

}
//************************************************************
// 创建员工
staff_list read_staff(string filename, vector<int> &StaffIdList)
{
    // 打开文件
    string encoding;
    ifstream processTimeData = pro_treatment(encoding, filename);
    // 读取数据
    staff_list Staffs;
    int maxTaskCnt = 3;
    string s, Oneline, StaffId;
    int processTime;
    int staffRealId;
    // get position name
    getline(processTimeData, Oneline);
    istringstream Readstr(Oneline);
    // 获取员工ID
    while (!processTimeData.eof())
    {
        getline(processTimeData, Oneline);
        istringstream Readstr(Oneline);
        getline(Readstr, s, ','); // 得到员工Id
        staffRealId = atoi(s.c_str());
        StaffIdList.push_back(staffRealId);
        staffRealId = staffRealId - 1;
        for (int i = 0; i < maxTaskCnt; i++)
        {
            Staff_Type tmpStaff;
            tmpStaff.from_csv(tmpStaff, staffRealId, staffRealId * maxTaskCnt + i);
            tmpStaff.init(tmpStaff);
            Staffs.push_back(tmpStaff);
        }
    }
    
    processTimeData.close();
    // for (int i = 0; i < PosName.size(); i++)
    // {
    // 	for (int j = 0; j < PosName.size(); j++)
    // 	{
    // 		printf("%f ", RunTimeMatrix[PosName[i]][PosName[j]]);
    // 	}
    // 	printf("\n");
    // }
    return Staffs;
}