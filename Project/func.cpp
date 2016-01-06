#include "stdafx.h"
#include "func.h"
#include <math.h>

void initial_data(double angle[], int pluse[], int& angle_length, int& pluse_length,
	ifstream& infile, ofstream& outfile){
		//从txt文本数据中读入angle数组
		ReadData(angle, infile, angle_length);
		//将angle数组转换为pluse数组
		Angle_to_pulse(angle, angle_length, pluse, pluse_length);
		//将脉冲数组数据输出到对应文件里
		WriteTxt(pluse,pluse_length,outfile);
}

int Set_Device_Handle(HANDLE m_hDevice[]){
	for (int i=0;i<DeviceNum;i++)
		m_hDevice[i]=INVALID_HANDLE_VALUE;
	m_hDevice[0] =  USB1020_CreateDevice(0);	// 创建句柄
	int USB1020Count= USB1020_GetDeviceCount(m_hDevice[0]);
	if (USB1020Count != 0){
		for (int i=1;i<USB1020Count;i++)
			m_hDevice[i] = USB1020_CreateDevice(i);	// 创建句柄
	}
	return USB1020Count;
}

bool Work_State1_Set(HANDLE m_hDevice[], ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	const double x_angle, const double y_angle, const double z_angle,int& SpeedRatio){
		//设置三轴的公用参数
		USB1020_PARA_DataList DL[3];	// 公用参数     
		USB1020_PARA_LCData LC[3];		// 直线和S曲线参数
		USB1020_PARA_RR0 RR0[3];			//状态寄存器RR0
		LONG X_EP[DeviceNum],Y_EP[DeviceNum],Z_EP[DeviceNum] ;	//读三轴实际位置
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //定义的状态变量，存储对应状态信息	
		double xangle[DeviceNum],yangle[DeviceNum],zangle[DeviceNum];
		for (int  i=0;i<DeviceNum;i++)
		{
			X_EP[i]=Y_EP[i]=Z_EP[i]=0;
			state_x[i]=state_y[i]=state_z[i]=0;
			xangle[i]=yangle[i]=zangle[i]=0;
		}
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
			LC[i].LV_DV= USB1020_DV;				// 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
			LC[i].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
			LC[i].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
			DL[i].Multiple=1;
			DL[i].Acceleration = 150;				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
			DL[i].Deceleration = 150;				// 减速度(125~1000000)	
		}
		DL[0].StartSpeed = 400 ;				// 初始速度(1~8000)
		DL[1].StartSpeed = 400 ;
		DL[2].StartSpeed = 400 ;
		DL[0].DriveSpeed = int(30*SpeedRatio*abs(x_angle)) ;				// 驱动速度	(1~8000)	
		DL[1].DriveSpeed = int(30*SpeedRatio*abs(y_angle)) ;				// 驱动速度	(1~8000)	
		DL[2].DriveSpeed = int(30*SpeedRatio*abs(z_angle)) ;				// 驱动速度	(1~8000)	
		if (x_angle>=0)          
			LC[0].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		if (x_angle<0)    
			LC[0].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		if (y_angle>=0)          
			LC[1].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		if (y_angle<0)    
			LC[1].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		if (z_angle>=0)          
			LC[2].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		if (z_angle<0)    
			LC[2].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
		LC[0].nPulseNum =  int(abs(x_angle)*3600) ;				//对应x初始角度 定量输出脉冲数(0~268435455)
		LC[1].nPulseNum =  int(abs(y_angle)*3600) ;				//对应y初始角度 定量输出脉冲数(0~268435455)
		LC[2].nPulseNum =  int(abs(z_angle)*3600) ;				//对应z初始角度 定量输出脉冲数(0~268435455)
		for (int k=0;k<DeviceNum;k++)
		{
			if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
			{
				for (int j=0;j<DeviceNum;j++)   //device k 的 三轴设置
				{
					USB1020_InitLVDV(						//初始单轴化连续,定长脉冲驱动
						m_hDevice[k],
						&DL[j], 
						&LC[j]);
					USB1020_SetEncoderSignalType(           //编码器设置，0表示2相脉冲输入
						m_hDevice[k],
						LC[j].AxisNum,
						0,
						0);
					USB1020_PulseInputMode( m_hDevice[k],	   //设置输入模式为A/B相模式
						LC[j].AxisNum,	
						0);  	
				}
			}
		}
		for (int i=0;i<DeviceNum;i++)
		{
			if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
				USB1020_Start4D( m_hDevice[i]);				// 4轴同时启动
				USB1020_GetRR0Status(
					m_hDevice[i],			// 设备句柄
					&(RR0[i]));				// RR寄存器状态	

				state_x[i]=RR0[i].XDRV;       //读X,Y,Z轴对应的状态寄存器值
				state_y[i]=RR0[i].YDRV;
				state_z[i]=RR0[i].ZDRV;
			}
		}
		//等待运行结束
		while(state_x[0]==1||state_y[0]==1||state_z[0]==1||
			state_x[1]==1||state_y[1]==1||state_z[1]==1||
			state_x[2]==1||state_y[2]==1||state_z[2]==1){
				for (int i=0;i<DeviceNum;i++)
				{
					if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
						USB1020_GetRR0Status(
							m_hDevice[i],			// 设备句柄
							&(RR0[i]));				// RR寄存器状态	

						state_x[i]=RR0[i].XDRV;       //读X,Y,Z轴对应的状态寄存器值
						state_y[i]=RR0[i].YDRV;
						state_z[i]=RR0[i].ZDRV;
					}
				}
		} 
		for (int i=0;i<DeviceNum;i++)
		{
			if(INVALID_HANDLE_VALUE != m_hDevice[i]){
				X_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[0].AxisNum);	// 读x轴实位
				Y_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[1].AxisNum);	// 读Y轴实位
				Z_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[2].AxisNum);	// 读z轴实位 
				xangle[i]=0.0001*X_EP[i]/0.36;
				yangle[i]=0.0001*Y_EP[i]/0.36;
				zangle[i]=0.0001*Z_EP[i]/0.36;
			}
		}
		x_realangle<<"关节腿1_X"<<"    "<<"关节腿2_X"<<"    "<<"关节腿3_X"<<endl;
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[0]<<"   ";
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[1]<<"   ";
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[2]<<endl;
		y_realangle<<"关节腿1_Y"<<"    "<<"关节腿2_Y"<<"    "<<"关节腿3_Y"<<endl;
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[0]<<"   ";
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[1]<<"   ";
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[2]<<endl;
		z_realangle<<"关节腿1_Z"<<"    "<<"关节腿2_Z"<<"    "<<"关节腿3_Z"<<endl;
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[0]<<"   ";
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[1]<<"   ";
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[2]<<endl;
		return true;
}

bool Work_State2_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length,int& SpeedRatio, bool& cutflag){
		//设置三轴的公用参数
		USB1020_PARA_DataList DL[3];	// 公用参数     
		USB1020_PARA_LCData LC[3];		// 直线和S曲线参数
		USB1020_PARA_RR0 RR0[3];	 //状态寄存器RR0
		LONG X_EP[DeviceNum],Y_EP[DeviceNum],Z_EP[DeviceNum] ;	//读三轴实际位置
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //定义的状态变量，存储对应状态信息	
		double xangle[DeviceNum],yangle[DeviceNum],zangle[DeviceNum];	
		for (int  i=0;i<DeviceNum;i++)
		{
			X_EP[i]=Y_EP[i]=Z_EP[i]=0;
			state_x[i]=state_y[i]=state_z[i]=0;
			xangle[i]=yangle[i]=zangle[i]=0;
		}
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
			LC[i].LV_DV= USB1020_DV;				// 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
			LC[i].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
			LC[i].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
			DL[i].Multiple=1;
			DL[i].Acceleration = 150;				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
			DL[i].Deceleration = 150;				// 减速度(125~1000000)	
		}
		for (int i=0;i<pluse_length;i++)         
		{
			if(!cutflag)
				break;
			if (!x_pluse[i])
			{
				x_pluse[i]=1;
			}
			if (!y_pluse[i])
			{
				y_pluse[i]=1;
			}
			if (!z_pluse[i])
			{
				z_pluse[i]=1;
			}

			DL[0].StartSpeed = abs(x_pluse[i]*SpeedRatio) ;				// 初始速度(1~8000)
			DL[0].DriveSpeed = abs(x_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[0].nPulseNum =  abs(x_pluse[i]) ;				// 定量输出脉冲数(0~268435455)
			if (x_pluse[i]>=0)          
			{
				LC[0].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (x_pluse[i]<0)    //大于0为逆时针
			{
				LC[0].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}

			DL[1].StartSpeed = abs(y_pluse[i]*SpeedRatio) ;			    // 初始速度(1~8000)
			DL[1].DriveSpeed = abs(y_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[1].nPulseNum = abs(y_pluse[i]) ;				            // 定量输出脉冲数(0~268435455)
			if (y_pluse[i]>=0)
			{
				LC[1].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (y_pluse[i]<0)
			{
				LC[1].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}

			DL[2].StartSpeed = abs(z_pluse[i]*SpeedRatio) ;					// 初始速度(1~8000)
			DL[2].DriveSpeed = abs(z_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[2].nPulseNum = abs(z_pluse[i]) ;				     // 定量输出脉冲数(0~268435455)
			if (z_pluse[i]>=0)
			{
				LC[2].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (z_pluse[i]<0)
			{
				LC[2].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}

			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<3;j++)
					{
						USB1020_InitLVDV(						//	初始单轴化连续,定长脉冲驱动
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //编码器设置，0表示2相脉冲输入
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //设置输入模式为A/B相模式
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					USB1020_Start4D( m_hDevice[k]);				// 4轴同时启动
					USB1020_GetRR0Status(
						m_hDevice[k],			// 设备句柄
						&(RR0[k]));				// RR寄存器状态	

					state_x[k]=RR0[k].XDRV;       //读X,Y,Z轴对应的状态寄存器值
					state_y[k]=RR0[k].YDRV;
					state_z[k]=RR0[k].ZDRV;
				}
			}
			while (state_x[0]==1||state_y[0]==1||state_z[0]==1||
				state_x[1]==1||state_y[1]==1||state_z[1]==1||
				state_x[2]==1||state_y[2]==1||state_z[2]==1)
			{
				for (int k=0;k<3;k++)
				{
					if ( m_hDevice[k]!=INVALID_HANDLE_VALUE){
						USB1020_GetRR0Status(
							m_hDevice[k],			// 设备句柄
							&(RR0[k]));				// RR寄存器状态	
						state_x[k]=RR0[k].XDRV;       //读X,Y,Z轴对应的状态寄存器值
						state_y[k]=RR0[k].YDRV;
						state_z[k]=RR0[k].ZDRV;
					}
				}                               
			}
			for (int k=0;k<DeviceNum;k++)
			{
				if (INVALID_HANDLE_VALUE != m_hDevice[k])
				{
					X_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[0].AxisNum);	// 读x轴实位
					Y_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[1].AxisNum);	// 读Y轴实位
					Z_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[2].AxisNum);	// 读z轴实位 
					xangle[k]=0.0001*X_EP[k]/0.36;
					yangle[k]=0.0001*Y_EP[k]/0.36;
					zangle[k]=0.0001*Z_EP[k]/0.36;
				}
			}
			x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[0]<<"   ";
			x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[1]<<"   ";
			x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[2]<<endl;

			y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[0]<<"   ";
			y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[1]<<"   ";
			y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[2]<<endl;

			z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[0]<<"   ";
			z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[1]<<"   ";
			z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[2]<<endl;
		}
		return true;
}

void delete_Device_Handle(HANDLE m_hDevice[]){
	for (int h=0;h<DeviceNum;h++)
		if (m_hDevice[h]!=INVALID_HANDLE_VALUE)
			USB1020_ReleaseDevice(m_hDevice[h]);
}

void Work_State2_Reset(HANDLE m_hDevice[],int x_pluse[], int y_pluse[], int z_pluse[],int pluse_length, int& SpeedRatio, bool& resetflag){
		//设置三轴的公用参数
		USB1020_PARA_DataList DL[3];	// 公用参数     
		USB1020_PARA_LCData LC[3];		// 直线和S曲线参数
		USB1020_PARA_RR0 RR0[3];	 //状态寄存器RR0
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //定义的状态变量，存储对应状态信息	
		for (int  i=0;i<DeviceNum;i++)
			state_x[i]=state_y[i]=state_z[i]=0;
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
			LC[i].LV_DV= USB1020_DV;				// 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
			LC[i].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
			LC[i].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
			DL[i].Multiple=1;
			DL[i].Acceleration = 600;				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
			DL[i].Deceleration = 600;				// 减速度(125~1000000)
		}
		for (int i=pluse_length-1; i>=0; i--)          
		{
			if(!resetflag)
				break;

			if (!x_pluse[i])
			{
				x_pluse[i]=1;
			}
			if (!y_pluse[i])
			{
				y_pluse[i]=1;
			}
			if (!z_pluse[i])
			{
				z_pluse[i]=1;
			}
			DL[0].StartSpeed = abs(x_pluse[i]*SpeedRatio) ;				// 初始速度(1~8000)
			DL[0].DriveSpeed = abs(x_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[0].nPulseNum =  abs(x_pluse[i]) ;				// 定量输出脉冲数(0~268435455)
			if (x_pluse[i]>=0)          
			{
				LC[0].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (x_pluse[i]<0)    //大于0为逆时针
			{
				LC[0].Direction = USB1020_PDIRECTION;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}

			DL[1].StartSpeed = abs(y_pluse[i]*SpeedRatio) ;			    // 初始速度(1~8000)
			DL[1].DriveSpeed = abs(y_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[1].nPulseNum = abs(y_pluse[i]) ;				            // 定量输出脉冲数(0~268435455)

			if (y_pluse[i]>=0)
			{
				LC[1].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (y_pluse[i]<0)
			{
				LC[1].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			DL[2].StartSpeed = abs(z_pluse[i]*SpeedRatio) ;					// 初始速度(1~8000)
			DL[2].DriveSpeed = abs(z_pluse[i]*SpeedRatio) ;				// 驱动速度	(1~8000)	
			LC[2].nPulseNum = abs(z_pluse[i]) ;				// 定量输出脉冲数(0~268435455)
			if (z_pluse[i]>=0)
			{
				LC[2].Direction = USB1020_MDIRECTION;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			if (z_pluse[i]<0)
			{
				LC[2].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k] != INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<3;j++)
					{
						USB1020_InitLVDV(						//	初始单轴化连续,定长脉冲驱动
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //编码器设置，0表示2相脉冲输入
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //设置输入模式为A/B相模式
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					USB1020_Start4D( m_hDevice[k]);				// 4轴同时启动
					USB1020_GetRR0Status(
						m_hDevice[k],			// 设备句柄
						&(RR0[k]));				// RR寄存器状态	

					state_x[k]=RR0[k].XDRV;       //读X,Y,Z轴对应的状态寄存器值
					state_y[k]=RR0[k].YDRV;
					state_z[k]=RR0[k].ZDRV;
				}
			}
			while (state_x[0]==1||state_y[0]==1||state_z[0]==1||
				state_x[1]==1||state_y[1]==1||state_z[1]==1||
				state_x[2]==1||state_y[2]==1||state_z[2]==1)
			{
				for (int k=0;k<3;k++)
				{
					if ( m_hDevice[k]!=INVALID_HANDLE_VALUE){
						USB1020_GetRR0Status(
							m_hDevice[k],			// 设备句柄
							&(RR0[k]));				// RR寄存器状态	

						state_x[k]=RR0[k].XDRV;       //读X,Y,Z轴对应的状态寄存器值
						state_y[k]=RR0[k].YDRV;
						state_z[k]=RR0[k].ZDRV;
					}
				}
			}
		}
}

void Work_State1_Reset(HANDLE m_hDevice[],const double x_angle, const double y_angle, 
	const double z_angle,int& SpeedRatio, bool& resetflag){
		if (resetflag)
		{
			//设置三轴的公用参数
			USB1020_PARA_DataList DL[3];	// 公用参数     
			USB1020_PARA_LCData LC[3];		// 直线和S曲线参数
			USB1020_PARA_RR0 RR0[3];	 //状态寄存器RR0
			int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //定义的状态变量，存储对应状态信息	
			for (int  i=0;i<DeviceNum;i++)
				state_x[i]=state_y[i]=state_z[i]=0;
			for (int i=0;i<DeviceNum;i++)
			{
				LC[i].AxisNum = i;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
				LC[i].LV_DV= USB1020_DV;				// 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
				LC[i].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
				LC[i].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
				DL[i].Multiple=1;
				DL[i].Acceleration = 150;				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
				DL[i].Deceleration = 150;				// 减速度(125~1000000)	
			}
			DL[0].StartSpeed = 400 ;				// 初始速度(1~8000)
			DL[1].StartSpeed = 400 ;
			DL[2].StartSpeed = 400 ;
			DL[0].DriveSpeed = int(30*SpeedRatio*abs(x_angle)) ;				// 驱动速度	(1~8000)	
			DL[1].DriveSpeed = int(30*SpeedRatio*abs(y_angle)) ;				// 驱动速度	(1~8000)	
			DL[2].DriveSpeed = int(30*SpeedRatio*abs(z_angle)) ;				// 驱动速度	(1~8000)	
			if (x_angle>=0)          
				LC[0].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			if (x_angle<0)    
				LC[0].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			if (y_angle>=0)          
				LC[1].Direction = USB1020_MDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			if (y_angle<0)    
				LC[1].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			if (z_angle>=0)          
				LC[2].Direction = USB1020_MDIRECTION;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	
			if (z_angle<0)    
				LC[2].Direction = USB1020_PDIRECTION ;	// 转动方向 USB1020_PDirection: 正转  USB1020_MDirection:反转	

			LC[0].nPulseNum =  int(abs(x_angle)*3600) ;				//对应x初始角度 定量输出脉冲数(0~268435455)
			LC[1].nPulseNum =  int(abs(y_angle)*3600) ;				//对应y初始角度 定量输出脉冲数(0~268435455)
			LC[2].nPulseNum =  int(abs(z_angle)*3600) ;				//对应z初始角度 定量输出脉冲数(0~268435455)
			for (int k=0;k<DeviceNum;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<DeviceNum;j++)   //device k 的 三轴设置
					{
						USB1020_InitLVDV(						//初始单轴化连续,定长脉冲驱动
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //编码器设置，0表示2相脉冲输入
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //设置输入模式为A/B相模式
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int i=0;i<3;i++)
			{
				if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
					USB1020_Start4D( m_hDevice[i]);				// 4轴同时启动
					USB1020_GetRR0Status(
						m_hDevice[i],			// 设备句柄
						&(RR0[i]));				// RR寄存器状态	
					state_x[i]=RR0[i].XDRV;       //读X,Y,Z轴对应的状态寄存器值
					state_y[i]=RR0[i].YDRV;
					state_z[i]=RR0[i].ZDRV;
				}
			}
			while(state_x[0]==1||state_y[0]==1||state_z[0]==1||
				state_x[1]==1||state_y[1]==1||state_z[1]==1||
				state_x[2]==1||state_y[2]==1||state_z[2]==1){
					for (int i=0;i<3;i++)
					{
						if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
							USB1020_GetRR0Status(
								m_hDevice[i],			// 设备句柄
								&(RR0[i]));				// RR寄存器状态	

							state_x[i]=RR0[i].XDRV;       //读X,Y,Z轴对应的状态寄存器值
							state_y[i]=RR0[i].YDRV;
							state_z[i]=RR0[i].ZDRV;
						}
					}
			} 
		}
}

bool Check_Cur_Com(char* com, CSerial& m_serial){
	if (m_serial.CheckPort(com)!=CSerial::EPortAvailable)
		return false;
	return true;
}

void Open_Cur_Com(char* com,CSerial& m_serial){
	m_serial.Open(com);
	m_serial.Setup(CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1);		
	m_serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
}