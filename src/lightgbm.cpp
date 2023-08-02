#include "cryptocontext.h"
#include "lwecore.h"
#include "math/backend.h"
#include "ringcore.h"
#include "hesea.h"

#include "Hom_Tree.h"

#include<iostream>
#include <fstream>
#include <sstream>



using namespace lbcrypto;


//定义一个结构体用于存储各个变量
struct Tree_data
{
	int tree_node;//记录树的编号
	std::vector<int> split_feature;
	std::vector<double> threshold;
	std::vector<int> left_child;
	std::vector<int> right_child;
	std::vector<double> leaf_value;
};

// 定义读取数据的函数
int read_file(std::vector<Tree_data*>* data_tree, std::string filename)
{
	std::ifstream file(filename); // 打开文件
	if (!file.is_open()) { // 检查文件是否成功打开
		std::cout << "无法打开文件" << std::endl;
		return 1;
	}
	std::string line; //用于暂时存储某一行数据
	while (std::getline(file, line))
	{
		std::istringstream iss(line);//流对象用于存储line中的数据
		std::string variableName;//存储变量名
		std::string data;//存储对应的数据

		if (std::getline(iss, variableName, '=')) {//提取变量名
			if (variableName == "Tree") {
				Tree_data* tree1 = new Tree_data();
				std::getline(iss, data);//存储当前数据
				std::istringstream isssData(data);//定义另一个流来分割数据
				isssData >> tree1->tree_node;
				data_tree->push_back(tree1);
			}
			else if (variableName == "split_feature")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				int element;
				while (issData >> element)
				{ // 提取每个元素
					data_tree->back()->split_feature.push_back(element); // 将元素添加到向量数据中
				}
			}
			else if (variableName == "threshold")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				std::istringstream issData2(data); // 字符串流对象用于将数据转换为向量元素
				double element;
				std::string s;
				while (issData2 >> s )
				{ // 提取每个元素
					issData >> element;
					if(s=="inf") data_tree->back()->threshold.push_back(1000000000); // 将元素添加到向量数据中
					else data_tree->back()->threshold.push_back(element); // 将元素添加到向量数据中
				}
			}
			else if (variableName == "left_child")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				int element;
				while (issData >> element)
				{ // 提取每个元素
					data_tree->back()->left_child.push_back(element); // 将元素添加到向量数据中
				}
			}
			else if (variableName == "right_child")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				int element;
				while (issData >> element)
				{ // 提取每个元素
					data_tree->back()->right_child.push_back(element); // 将元素添加到向量数据中
				}
			}
			else if (variableName == "right_child")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				int element;
				while (issData >> element)
				{ // 提取每个元素
					data_tree->back()->right_child.push_back(element); // 将元素添加到向量数据中
				}
			}
			else if (variableName == "leaf_value")
			{
				std::getline(iss, data); // 提取数据部分
				std::istringstream issData(data); // 字符串流对象用于将数据转换为向量元素
				double element;
				while (issData >> element)
				{ // 提取每个元素
					data_tree->back()->leaf_value.push_back(element); // 将元素添加到向量数据中
				}
			}
		}
	}
	file.close();
	return 0;
}


double calculateAccuracy(const std::vector<int>& predictions, const std::vector<int>& labels) 
{
	if (predictions.size() != labels.size()) {
		std::cout << "预测结果和标签数量不匹配" << std::endl;
		return 0.0;
	}

	int correctCount = 0;
	int totalCount = predictions.size();

	for (int i = 0; i < totalCount; ++i) {
		if (predictions[i] == labels[i]) {
			correctCount++;
		}
	}

	double accuracy = static_cast<double>(correctCount) / totalCount;
	return accuracy;
}

//! y>=?x
LWECiphertext comparexby2(LWECiphertext x, LWECiphertext y, CryptoContextImpl<DCRTPoly>& cc, int p){
    NativeInteger q = x->GetA().GetModulus();
    LWECiphertext temp = make_shared<LWECiphertextImpl>(*x);
    temp->SetA(y->GetA().ModMul(2).ModSub(x->GetA().ModMul(2)));
    temp->SetB(y->GetB().ModMul(2, q).ModSub(x->GetB().ModMul(2, q), q));

    // temp->SetA(y->GetA().ModSub(x->GetA()));
    // temp->SetB(y->GetB().ModSub(x->GetB(), q));

    auto ct_sign = cc.HESea_MyEvalSigndFunc(temp, p);

	auto enc_one = cc.HESea_TraivlEncrypt(1, p);
    ct_sign->SetA(ct_sign->GetA().ModAdd(enc_one->GetA()));
    ct_sign->SetB(ct_sign->GetB().ModAdd(enc_one->GetB(), q));

    return ct_sign;
}


//! y>=?x
LWECiphertext comparexby1(LWECiphertext x, LWECiphertext y, CryptoContextImpl<DCRTPoly>& cc, int p){
    NativeInteger q = x->GetA().GetModulus();
    LWECiphertext temp = make_shared<LWECiphertextImpl>(*x);
    temp->SetA(y->GetA().ModMul(2).ModSub(x->GetA().ModMul(2)));
    temp->SetB(y->GetB().ModMul(2, q).ModSub(x->GetB().ModMul(2, q), q));

    // temp->SetA(y->GetA().ModSub(x->GetA()));
    // temp->SetB(y->GetB().ModSub(x->GetB(), q));

    auto ct_sign = cc.HESea_MyEvalSigndFunc(temp, p);

    return ct_sign;
}

int main() 
{
	cout<<"aaaa"<<endl;
	ifstream file("../data/data_in_tree_label.csv"); // 打开文件

	if (!file.is_open()) { // 检查文件是否成功打开
		std::cout << "无法打开文件" << std::endl;
		return 1;
	}

	
	std::vector<std::vector<double>> all_numbers; //保存特征数据
	std::vector<int> lables;//保存标签数据
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) { // 逐行读取文件内容
		std::istringstream iss(line); // 字符串流对象用于将每行内容转换为 vector 元素
        std::vector<double> numbers;
		std::string element;
		while (std::getline(iss, element, ',')) { // 将每行内容按逗号分隔成元素
			int number = std::stoi(element); // 将元素转换为整数
			numbers.push_back(number); // 将元素添加到 vector 中
		}
		lables.push_back(numbers.back());
		numbers.pop_back();
		all_numbers.push_back(numbers);
	}

	file.close(); // 关闭文件

	//获取树
	std::vector<Tree_data*> data_tree;
	read_file(&data_tree, "../data/fold0.txt");
	//print_data(data_tree);

	
	int num_Trees = 0;
	//std::vector<Hom_Tree> Trees(100);
	//for (int i = 0; i < 100; i++)
	std::vector<Hom_Tree> Trees(data_tree.size());



	int scale_B = 200;
	int scale_E = 100;


	for (int i = 0; i < data_tree.size(); i++)
	{
		Trees[i] = Hom_Tree(data_tree[i]->split_feature, data_tree[i]->threshold,
			data_tree[i]->left_child, data_tree[i]->right_child, data_tree[i]->leaf_value, 6375, scale_B, scale_E);
		num_Trees++;

		//std::cout << i << std::endl;
		delete data_tree[i];//释放动态分配的内存
	}



    //! binfhecontext strat
    auto cc = CryptoContextImpl<DCRTPoly>();

    int p = 512;
    int N = 1024;
    int n = 128;
    int q_bits = 30;
    int qKS_bits = 30;
    int Q_bits = 55;
    int baseG_Q = 1<<15; 
    int baseKS = 1<<3; 
    int baseR = 2;
    NativeInteger Q = PreviousPrime<NativeInteger>(FirstPrime<NativeInteger>(Q_bits, 2*N), 2*N);
    cc.HESea_GenerateBinFHEContext(n, N, NativeInteger(1<<q_bits), Q, NativeInteger(1ull<<qKS_bits), 0, baseKS, baseG_Q, baseR, lbcrypto::GINX);
    int int_q = cc.HESea_GetParams()->GetLWEParams()->Getq().ConvertToInt();
	NativeInteger q = cc.HESea_GetParams()->GetLWEParams()->Getq();
    // Sample Program: Step 2: Key Generation
    // Generate the secret key
    auto sk = cc.HESea_KeyGen02();

    std::cout << "Generating the bootstrapping keys..." << std::endl;

    // Generate the bootstrapping keys (refresh and switching keys)
    cc.HESea_BTKeyGen(sk);

    std::cout << "Completed the key generation." << std::endl;








	//预测
	std::vector<int> mylables;

    for (int i = 0; i < all_numbers.size(); i++) 
    {
		//! Encrypt the image
		vector<LWECiphertext> enc_img;
		auto img = all_numbers[i];
		for(int k = 0; k < img.size(); k++)
		{
			int pixel = round(img[k]*scale_B); 
			enc_img.push_back( cc.HESea_TraivlEncrypt((pixel+p) % p, p));
		}

		//! Tree predict
		LWECiphertext enc_score = cc.HESea_TraivlEncrypt(0, p);

		cout<<"Tree.size()"<<Trees.size()<<endl;

		for(int j = 0; j < Trees.size(); j++)
		{
			//! constant 
			auto A = Trees[j].A_order;
			auto B = Trees[j].B_int;
			auto C = Trees[j].C;
			auto D = Trees[j].D;
			auto E = Trees[j].E_int;

			//! step1 select feathers
			vector<LWECiphertext> tempA;
			for(int k = 0; k<A.size(); k++)
			{
				tempA.push_back(enc_img[A[k]]);
			}

			//! compare with B
			vector<LWECiphertext> tempB;

			for (int k = 0; k < B.size(); k++)
			{
				int b = round(B[k]);
				auto sign_ct = cc.HESea_TraivlEncrypt(b, p);
				tempB.push_back( comparexby2(tempA[k], sign_ct, cc, p) );
				//！ return 2 or 0

			}
			// cout<<"B.size()"<<B.size()<<endl;
			//! multi with C
			vector<LWECiphertext> tempC;
			for (int k1 = 0; k1 < C[0].size(); k1++) 
			{
				LWECiphertext temp_sum = cc.HESea_TraivlEncrypt(0, p);
				for (int k = 0; k < C.size(); k++) 
				{
					NativeInteger temp =  NativeInteger(C[k][k1], int_q);
					const NativeVector temp_a = tempB[k]->GetA().ModMul(temp);
					auto temp_b = tempB[k]->GetB().ModMul(temp, q);
					temp_sum->SetA(temp_a.ModAdd(temp_sum->GetA()));
					temp_sum->SetB(temp_b.ModAdd(temp_sum->GetB(), q));

				}

				tempC.push_back(temp_sum);
			}			

			//! identity with D
			vector<LWECiphertext> tempD;

			for (int k = 0; k < D.size(); k++)
			{
				int d = round(D[k]);
				auto ct_d = cc.HESea_TraivlEncrypt(d, p);
				auto ct_x_d = comparexby1(tempC[k], ct_d, cc, p);
				auto ct_d_x = comparexby1(ct_d, tempC[k], cc, p);

				ct_x_d->SetA(ct_x_d->GetA().ModAdd(ct_d_x->GetA()));
				ct_x_d->SetB(ct_x_d->GetB().ModAdd(ct_d_x->GetB(), q));

				//! add two result! if x == y then return 2
				tempD.push_back( ct_x_d );
				//！ return 2 or 0

			}


			// cout<<E.size()<< "D.szie() " << tempD.size()<<endl;
			//! multi with E
			LWECiphertext tempE = cc.HESea_TraivlEncrypt(0,p);
			for (int k = 0; k < E.size(); k++) 
			{
				
				NativeInteger temp =  NativeInteger(E[k], int_q);
				
				const NativeVector temp_a = tempD[k]->GetA().ModMul(temp);
				
				auto temp_b = tempD[k]->GetB().ModMul(temp, q);
				
				tempE->SetA(temp_a.ModAdd(tempE->GetA()));
				// cout<<"BBBBBB"<<endl;
				tempE->SetB(temp_b.ModAdd(tempE->GetB(), q));
			}

			//! add to score!
			enc_score->SetA(enc_score->GetA().ModAdd(tempE->GetA()));
			enc_score->SetB(enc_score->GetB().ModAdd(tempE->GetB(), q));

			A.clear();
			B.clear();
			C.clear();
			D.clear();
			E.clear();
			tempA.clear();
			tempB.clear();
			tempC.clear();
			tempC.clear();
			tempD.clear();
		}

		//! decrypt the score!
		LWEPlaintext score_clear;
		cc.HESea_Decrypt(sk, enc_score, &score_clear, p);
		score_clear = (score_clear>p/2)? score_clear%p-p: score_clear%p;

		if(score_clear >= 0.5*2*scale_E)
		{
			mylables.push_back(1);
		}
		else
		{
			mylables.push_back(0);
		}

		cout<<"predict label is "<<mylables[i]<< "  true label is "<<lables[i]<<endl;

	}
	
	
	
	
	std::cout << "预测准确率为：" << calculateAccuracy(lables, mylables) << std::endl;
	std::cout <<std::endl;
	return 0;


}
