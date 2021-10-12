#include "../heads/common.h"
#include "../heads/ec.h"
extern string stage;

extern int random_a_b(int, int);
extern ifstream open_csv_file(string filename);
//************************************************************
vector<int> Iniltialize(task_list &Tasks, staff_list &Staffs, processTimeType &ProcessTime, vector<vector<int> > &Bounds, ec::Individual &ind)
{
    vector<int> tmpBound;
    vector<int> Ind;
    int randomIndex;
    if (stage == EFFICIENCY)
    {
        for (int i = 0; i < Tasks.size(); i++)
        {
            tmpBound = Bounds[i];
            randomIndex = random_a_b(0, tmpBound.size() - 1);
            Ind.push_back(tmpBound[randomIndex]);
        }
    }
    else if (stage == RESPONSE)
    {
        for (int s = 0; s < 4; s++)
        {
            for (int i = 0; i < Tasks.size(); i++)
            {
                tmpBound = Bounds[i];
                randomIndex = random_a_b(0, tmpBound.size() - 1);
                Ind.push_back(tmpBound[randomIndex]);
            }
        }
    }   
    else if (stage == SIGMA)
    {
        for (int s = 0; s < 4; s++)
        {
            for (int i = 0; i < Tasks.size(); i++)
            {
                // tmpBound = Bounds[i];
                // randomIndex = 0;
                Ind.push_back(0);
            }
        }
    }
    else if (stage == ALL)
    {
        // // 如果第一阶段优化5j个变量
        // for (int i = 0; i < Tasks.size(); i++)
        // {
        //     tmpBound = Bounds[i];
        //     randomIndex = random_a_b(0, tmpBound.size() - 1);
        //     Ind.push_back(tmpBound[randomIndex]);
        // }
        // 如果第一阶段为优化效率，则初始解为第一阶段得到的最优解
        Ind.clear();
        for (int i = 0; i < ind.Tasks.size(); i++)
        {
            Ind.push_back(ind.Tasks[i].StaffVId);
        }
        
        for (int s = 0; s < 4; s++)
        {
            for (int i = 0; i < Tasks.size(); i++)
            {
                tmpBound = Bounds[ind.Tasks.size() * (s + 1) + i];
                randomIndex = random_a_b(0, tmpBound.size() - 1);
                Ind.push_back(tmpBound[randomIndex]);
            }
        }
    }
    
    

    
    if (Ind.size() != Bounds.size())
    {
        cout << "染色体长度: " << Ind.size() << " 不等于 Bounds长度: " << Bounds.size() << endl;
        throw new logic_error("");
    }
    
    
    return Ind;
}


//************************************************************
vector<int> Iniltialize_from_csv(string filename)
{
    string ss, str;
    ifstream indData = open_csv_file(filename);
    vector<int> Ind;
    //按照逗号分隔
    while (getline(indData, str, '\n')){
        Ind.push_back(atoi(str.c_str()));//一行数据以vector保存
    }
    return Ind;
}