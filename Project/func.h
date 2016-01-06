#include "DataHandle.h"
#include "Serial.h"
const int DeviceNum = 3;
const int Max_length  =1000;

//��ʼ����������������
void initial_data(double angle[], int pluse[], int& angle_length, int& pluse_length,
	ifstream& infile, ofstream& outfile);

//�����˶����ƿ���������سɹ����ӵ��˶����ƿ�����
int Set_Device_Handle(HANDLE m_hDevice[]);

//����ģʽ --  ��ˮƽ״̬�ߵ��и�ǰ�ĳ�ʼλ��
bool Work_State1_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	const double x_angle, const double y_angle, const double z_angle,int& SpeedRatio);

//����ģʽ --  �и��ʼλ�������и�
bool Work_State2_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length, int& SpeedRatio, bool& cutflag);

//��λģʽ --  ���и��ʼλ�ûص�ˮƽλ��
void Work_State1_Reset(HANDLE m_hDevice[],const double x_angle, const double y_angle, const double z_angle, int& SpeedRatio, bool& resetflag);

//��λģʽ --  ���и��յ��ߵ��и�ǰ�ĳ�ʼλ��
void Work_State2_Reset(HANDLE m_hDevice[],int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length, int& SpeedRatio, bool& resetflag);

//�ͷ��˶����ƿ����
void delete_Device_Handle(HANDLE m_hDevice[]);

//����ģ�鴮�����Ӽ��
bool Check_Cur_Com(char* com,CSerial& m_serial);

//�򿪵���ģ�鴮��
void Open_Cur_Com(char* com,CSerial& m_serial);
