#include "../heads/common.h"
#include "../heads/ec.h"
//************************************************************
// convert time formate
string ConvertTime(int inputTime){
	inputTime = inputTime * 60;
	time_t t;
	struct tm *p;
	char c[80];
	string s;
	t = inputTime;
	p = gmtime(&t);
	strftime(c, 80, "%H:%M", p);
	s = string(c);
	return s;
}
//************************************************************
vector<int> ShuffArray(vector<int> oldArray) {
	vector<int> newArray;
	for (int i = oldArray.size(); i > 0; i--)
	{
		int index = rand() % i;
		
		newArray.push_back(oldArray[index]);
		
		oldArray.erase(oldArray.begin() + index);
	}
	return newArray;
}

//************************************************************
// get random int [a, b]
int random_a_b(int begin, int end){
	return rand() % (end - begin + 1) + begin;
}
//************************************************************
// get random int [a, b]
double random_r(){
	return rand() % (99999 + 1) / (float)(99999 + 1);
}
//************************************************************
// 保存个体信息
void save_ind_info(ec::Individual &ind, string filename){
	ofstream indInfo(filename);
    if (indInfo.is_open())
    {
		indInfo << ind.gene[0];
		for (int i = 1; i < ind.gene.size(); i++)
		{
			indInfo << endl << ind.gene[i];
		}
		indInfo.close();
    }
    else
    {
        throw new std::runtime_error("file not existes");
    }
	
}
//************************************************************
// 输出 明细 的 csv 文件
void write_csv_file(task_list &Tasks, processTimeType &processTime, string filename){
    ofstream datafile(filename);
	int cnt = 0;
    if (datafile.is_open())
    {
		vector<string> outputCsvFile;
		for (int i = 0; i < Tasks.size(); i++)
		{
			datafile << Tasks[i].to_csv(Tasks[i], processTime, cnt);
		}
		datafile.close();
    }
    else{
		cout << filename << "： 文件不存在！！" << endl;
        throw new std::runtime_error("");
    }
} 

/**
 * 求平均值
 */
double average(vector<double> &x)
{
    double sum = 0;
    for (int i = 0; i < x.size(); i++) // 求和
        sum += x[i];
    return sum/x.size(); // 得到平均值
}
/**
 * 求方差
 */
double variance(vector<double> &x)
{
    double ave = average(x);
	double sum = 0;
    for (int i = 0; i < x.size(); i++) // 求和
        sum += pow(x[i] - ave, 2);
    return sum/x.size(); // 得到平均值
}

//*********
void logInfo(string &info, int &g, int &ExecutionTime, double &fit, vector<double> &items, ofstream &logInfoFile){
    info = "g: " + to_string(g) + ", time: " + to_string(ExecutionTime) + ", " + to_string(fit) + ", sigma: " + to_string(items[0]) + ", M: " + to_string(items[1]) + ", e: " + to_string(-1 * items[2]) + ", delay: " + to_string(items[3]);
    cout << info << endl;
    logInfoFile << info << endl;
    ++g;
}

//****************
void clear(queue<int>& q) {
	queue<int> empty;
	swap(empty, q);
}

void getDirectory()
{	char *buffer;
	//也可以将buffer作为输出参数
	if((buffer = getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
	}
	else
	{
		cout << "当前目录： " << buffer << endl;
		free(buffer);
	}
}

