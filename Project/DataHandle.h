#include<iostream>
#include<fstream>
using namespace std;

const double Angle_unit = 0.000277;

//��txt�ı������ж���angle����
void ReadData(double* angle, ifstream &infile, int &Angle_length);
//�ǶȲ�ֵ�任�ɶ�Ӧ�������㷨,����������
int Function_Convert(double Delta);
//��angle����ת��Ϊpluse����
void Angle_to_pulse(double* angle,const int angle_length, int* pluse,int &pluse_length);
//��pluse��������д��txt�ı���
void WriteTxt(int* pluse, const int pluse_length, ofstream &outfile);