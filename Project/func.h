#include "DataHandle.h"
#include "Serial.h"
const int DeviceNum = 3;
const int Max_length  =1000;

//初始化并计算脉冲数组
void initial_data(double angle[], int pluse[], int& angle_length, int& pluse_length,
	ifstream& infile, ofstream& outfile);

//设置运动控制卡句柄，返回成功连接的运动控制卡数量
int Set_Device_Handle(HANDLE m_hDevice[]);

//工作模式 --  从水平状态走到切割前的初始位置
bool Work_State1_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	const double x_angle, const double y_angle, const double z_angle,int& SpeedRatio);

//工作模式 --  切割初始位置向下切割
bool Work_State2_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length, int& SpeedRatio, bool& cutflag);

//复位模式 --  从切割初始位置回到水平位置
void Work_State1_Reset(HANDLE m_hDevice[],const double x_angle, const double y_angle, const double z_angle, int& SpeedRatio, bool& resetflag);

//复位模式 --  从切割终点走到切割前的初始位置
void Work_State2_Reset(HANDLE m_hDevice[],int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length, int& SpeedRatio, bool& resetflag);

//释放运动控制卡句柄
void delete_Device_Handle(HANDLE m_hDevice[]);

//电流模块串口连接检测
bool Check_Cur_Com(char* com,CSerial& m_serial);

//打开电流模块串口
void Open_Cur_Com(char* com,CSerial& m_serial);
