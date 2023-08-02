#include "Hom_Tree.h"
#include <cmath>
using namespace std;
//计算ABCDE的函数
extern void computeABCDE(std::vector<int>& split_feature, std::vector<double>& threshold,
	std::vector<int>& left_child, std::vector<int>& right_child, std::vector<double>& leaf_value,
	int num_feature, std::vector<int>& A, std::vector<double>& B,
	std::vector<std::vector<int>>& C, std::vector<int>& D, std::vector<double>& E);
//向量与矩阵相乘
template<typename Type1, typename Type2>
std::vector<double> mutil(std::vector<Type1> m1, std::vector<std::vector<Type2>> m2) {

	int m = m1.size();
	//int n = m2.size();
	int p = m2[0].size();
	std::vector<double> array;



	for (int j = 0; j < p; j++) {
		double sum = 0;
		for (int k = 0; k < m; k++) {
			sum += m1[k] * m2[k][j];
		}
		array.push_back(sum);
	}

	return array;
}


Hom_Tree::Hom_Tree(std::vector<int> AA,std::vector<double> BB, 
	std::vector<std::vector<int>> CC, std::vector<int> DD, std::vector<double> EE,double ROU)
	:A_order(AA),B(BB),C(CC),D(DD), E(EE){
};
Hom_Tree::Hom_Tree(std::vector<int>& split_feature, std::vector<double>& threshold, std::vector<int>& left_child, 
	std::vector<int>& right_child, std::vector<double>& leaf_value, int num_feature, int scale_B, int scale_E) 
{
	computeABCDE(split_feature, threshold, left_child, right_child, leaf_value, num_feature,A_order, B, C, D, E);
    for(int i = 0; i<B.size() ; i++)
    {
        B_int.push_back (round(B[i] * scale_B));
    }

    for(int i = 0; i< D.size(); i++)
    {
        D[i] = 2* D[i];
    }

    for(int i = 0; i< E.size(); i++)
    {
        E_int.push_back(  round( E[i] * scale_E) );
    }

}


