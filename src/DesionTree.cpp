
#include<iostream>
#include<vector>
#include<map>
#include<set>
using namespace std;


//定义一个辅助计算A_order,B,C,E的矩阵
void traverse(vector<int>& split_feature, vector<double>& threshold, vector<int>& left_child, vector<int>& right_child, vector<double>& leaf_value, vector<int>& A_order, vector<double>& B, vector<vector<int>>& C_temper, vector<double>& E, vector<int> loc, vector<int>& node_order, int current_node)
{
    //假如当前节点编号为负，表示是叶子节点，给E赋初值
    if (current_node < 0)
    {
        E.push_back(leaf_value[abs(current_node + 1)]);
        //把更新后的loc赋值给C_temper；
        for (int i = 0; i < C_temper.size(); i++)
        {
            //用push_back函数将v的第i个元素增加到C的第i行的最后
            C_temper[i].push_back(loc[i]);
        }
        current_node = node_order.back();
    }
    else //否则当前节点编号为非负，表示是根节点，给A_order和B进行赋值
    {
        node_order.push_back(current_node);
        A_order.push_back(split_feature[current_node]);
        B.push_back(threshold[current_node]);
        for (int i = 0; i < 2; i++)
        {
      
            //如果是走左分支节点
            if (i == 0)
            {
                
                loc[current_node] = 1;
                traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, left_child[current_node]);
            }
            //否则是走右分支节点
            else
            {
                loc[current_node] = -1;
                traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, right_child[current_node]);
            }
        }
    }
}


//计算ABCDE的函数
void computeABCDE(vector<int>& split_feature, vector<double>& threshold, vector<int>& left_child, vector<int>& right_child, vector<double>& leaf_value, int num_feature, vector<int>& A_order, vector<double>& B, vector<vector<int>>& C, vector<int>& D, vector<double>& E)
{
    vector<int> node_order; //节点的编号顺序
    int current_node = 0; //记录当前节点的编号
    vector<int> loc(threshold.size()); //定义一个向量向左走对应位置加一，向右走对应位置减一；
    // vector<int> A_order;//记录遍历节点的顺序编号；
    vector<vector<int>> C_temper(threshold.size());
    traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, current_node);
    // 获得C矩阵
    for (int i = 0; i < node_order.size(); i++)
    {
        C.push_back(C_temper[node_order[i]]);
    }

    //// 先为A矩阵分配对应空间
    ////用resize函数把A的行数改为A_order.size()
    //A.resize(num_feature);
    ////遍历A的每一行
    //for (int i = 0; i < num_feature; i++) {
    //    //用resize函数给每一行分配A_order.size()列空间，并初始化为0
    //    A[i].resize(A_order.size(), 0);
    //    
    //}
    ////获得矩阵A
    //for (int j = 0; j < A_order.size(); j++)
    //{
    //    A[A_order[j]][j] = 1;//这个地方一定要注意，假如说特征下标是从1开始，那么这里应为A[A_order[j]-1][j] = 1，我们这里特征下标是从0开始，所以不需要减一
    //}
    ////获得矩阵D(先给D分配空间）
    D.resize(C[0].size());
    for (int j = 0; j < C[0].size(); j++)
    {
        for (int i = 0; i < C.size(); i++)
        {
            if (C[i][j] == 1)
            {
                D[j]++;
            }
        }
    }
}
