#pragma once
// #include "DesionTree.h"
#include <vector>

using namespace std;

class Hom_Tree
{

public:

	std::vector<int> A_order;
	std::vector<double> B;
	vector<int> B_int;
	std::vector<std::vector<int>> C;
	std::vector<int> D;
	std::vector<double> E;
	std::vector<int> E_int;

public:
	//构造函数：初始化上述变量、分配空间
	Hom_Tree() {}
	Hom_Tree(std::vector<int> AA, std::vector<double> BB, std::vector<std::vector<int>> CC, std::vector<int> DD, std::vector<double> EE, double ROU=1);
	Hom_Tree(std::vector<int>& split_feature, std::vector<double>& threshold,
		std::vector<int>& left_child,std:: vector<int>& right_child, std::vector<double>& leaf_value, int num_feature, int scale_B, int scale_E);
};