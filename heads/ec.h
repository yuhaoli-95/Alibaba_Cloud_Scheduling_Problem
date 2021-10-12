#ifndef EC_H
    #define EC_H

    #include "common.h"

    namespace ec{
        struct Individual
        {
            vector<int> gene;
            double fit;
            double efficiency;
            double response;
            double sigma;
            vector<double> items; // 评价函数中涉及到的评价项
            vector<vector<int> > Bounds; // 个体的每个基因的取值范围
            task_list Tasks;
            staff_list Staffs;
            processTimeType processTime;
            vector<int> StaffIdList;
            vector<int> TabuTable; // 禁忌表
            void init(Individual &ind, vector<vector<int> > Bounds){
                ind.fit = DBL_MAX;
                ind.items = {0, 0, 0, 0, 0};
                // for (int i = 0; i < 4; i++)
                // {
                //     ind.items.push_back(DBL_MAX);
                // }
                // ind.items.push_back(0); // 
                ind.Bounds = Bounds;
                ind.TabuTable.clear();
                for (int i = 0; i < Bounds.size(); i++)
                {
                    ind.TabuTable.push_back(0);
                }
                
            }
        };
        struct Population
        {
            vector<Individual> ind;
            vector<double> fits;
            Individual bestInd;
            void set_best_ind(Population &pop){
                for (int i = 0; i < ind.size(); i++)
                {
                    if (fits.size() != pop.ind.size())
                    {
                        fits.push_back(ind[i].fit);
                    }
                    else
                    {
                        fits[i] = ind[i].fit;
                    }
                }
                int maxIndex = max_element(fits.begin(), fits.end()) - fits.begin();
                pop.bestInd = pop.ind[maxIndex];
            }
        };
        
    }


    // int ngene; // 基因个数
    // int popSize; // 种群大小
    // double pcro;
    // double pmut;
#endif