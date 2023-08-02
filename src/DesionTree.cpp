
#include<iostream>
#include<vector>
#include<map>
#include<set>
using namespace std;


//����һ����������A_order,B,C,E�ľ���
void traverse(vector<int>& split_feature, vector<double>& threshold, vector<int>& left_child, vector<int>& right_child, vector<double>& leaf_value, vector<int>& A_order, vector<double>& B, vector<vector<int>>& C_temper, vector<double>& E, vector<int> loc, vector<int>& node_order, int current_node)
{
    //���統ǰ�ڵ���Ϊ������ʾ��Ҷ�ӽڵ㣬��E����ֵ
    if (current_node < 0)
    {
        E.push_back(leaf_value[abs(current_node + 1)]);
        //�Ѹ��º��loc��ֵ��C_temper��
        for (int i = 0; i < C_temper.size(); i++)
        {
            //��push_back������v�ĵ�i��Ԫ�����ӵ�C�ĵ�i�е����
            C_temper[i].push_back(loc[i]);
        }
        current_node = node_order.back();
    }
    else //����ǰ�ڵ���Ϊ�Ǹ�����ʾ�Ǹ��ڵ㣬��A_order��B���и�ֵ
    {
        node_order.push_back(current_node);
        A_order.push_back(split_feature[current_node]);
        B.push_back(threshold[current_node]);
        for (int i = 0; i < 2; i++)
        {
      
            //����������֧�ڵ�
            if (i == 0)
            {
                
                loc[current_node] = 1;
                traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, left_child[current_node]);
            }
            //���������ҷ�֧�ڵ�
            else
            {
                loc[current_node] = -1;
                traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, right_child[current_node]);
            }
        }
    }
}


//����ABCDE�ĺ���
void computeABCDE(vector<int>& split_feature, vector<double>& threshold, vector<int>& left_child, vector<int>& right_child, vector<double>& leaf_value, int num_feature, vector<int>& A_order, vector<double>& B, vector<vector<int>>& C, vector<int>& D, vector<double>& E)
{
    vector<int> node_order; //�ڵ�ı��˳��
    int current_node = 0; //��¼��ǰ�ڵ�ı��
    vector<int> loc(threshold.size()); //����һ�����������߶�Ӧλ�ü�һ�������߶�Ӧλ�ü�һ��
    // vector<int> A_order;//��¼�����ڵ��˳���ţ�
    vector<vector<int>> C_temper(threshold.size());
    traverse(split_feature, threshold, left_child, right_child, leaf_value, A_order, B, C_temper, E, loc, node_order, current_node);
    // ���C����
    for (int i = 0; i < node_order.size(); i++)
    {
        C.push_back(C_temper[node_order[i]]);
    }

    //// ��ΪA��������Ӧ�ռ�
    ////��resize������A��������ΪA_order.size()
    //A.resize(num_feature);
    ////����A��ÿһ��
    //for (int i = 0; i < num_feature; i++) {
    //    //��resize������ÿһ�з���A_order.size()�пռ䣬����ʼ��Ϊ0
    //    A[i].resize(A_order.size(), 0);
    //    
    //}
    ////��þ���A
    //for (int j = 0; j < A_order.size(); j++)
    //{
    //    A[A_order[j]][j] = 1;//����ط�һ��Ҫע�⣬����˵�����±��Ǵ�1��ʼ����ô����ӦΪA[A_order[j]-1][j] = 1���������������±��Ǵ�0��ʼ�����Բ���Ҫ��һ
    //}
    ////��þ���D(�ȸ�D����ռ䣩
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
