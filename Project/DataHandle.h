#include<iostream>
#include<fstream>
using namespace std;

const double Angle_unit = 0.000277;

//从txt文本数据中读入angle数组
void ReadData(double* angle, ifstream &infile, int &Angle_length);
//角度差值变换成对应脉冲数算法,返回脉冲数
int Function_Convert(double Delta);
//将angle数组转换为pluse数组
void Angle_to_pulse(double* angle,const int angle_length, int* pluse,int &pluse_length);
//将pluse数组数据写入txt文本中
void WriteTxt(int* pluse, const int pluse_length, ofstream &outfile);