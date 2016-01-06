#include "stdafx.h"
#include "func.h"
#include <math.h>

void initial_data(double angle[], int pluse[], int& angle_length, int& pluse_length,
	ifstream& infile, ofstream& outfile){
		//��txt�ı������ж���angle����
		ReadData(angle, infile, angle_length);
		//��angle����ת��Ϊpluse����
		Angle_to_pulse(angle, angle_length, pluse, pluse_length);
		//���������������������Ӧ�ļ���
		WriteTxt(pluse,pluse_length,outfile);
}

int Set_Device_Handle(HANDLE m_hDevice[]){
	for (int i=0;i<DeviceNum;i++)
		m_hDevice[i]=INVALID_HANDLE_VALUE;
	m_hDevice[0] =  USB1020_CreateDevice(0);	// �������
	int USB1020Count= USB1020_GetDeviceCount(m_hDevice[0]);
	if (USB1020Count != 0){
		for (int i=1;i<USB1020Count;i++)
			m_hDevice[i] = USB1020_CreateDevice(i);	// �������
	}
	return USB1020Count;
}

bool Work_State1_Set(HANDLE m_hDevice[], ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	const double x_angle, const double y_angle, const double z_angle,int& SpeedRatio){
		//��������Ĺ��ò���
		USB1020_PARA_DataList DL[3];	// ���ò���     
		USB1020_PARA_LCData LC[3];		// ֱ�ߺ�S���߲���
		USB1020_PARA_RR0 RR0[3];			//״̬�Ĵ���RR0
		LONG X_EP[DeviceNum],Y_EP[DeviceNum],Z_EP[DeviceNum] ;	//������ʵ��λ��
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //�����״̬�������洢��Ӧ״̬��Ϣ	
		double xangle[DeviceNum],yangle[DeviceNum],zangle[DeviceNum];
		for (int  i=0;i<DeviceNum;i++)
		{
			X_EP[i]=Y_EP[i]=Z_EP[i]=0;
			state_x[i]=state_y[i]=state_z[i]=0;
			xangle[i]=yangle[i]=zangle[i]=0;
		}
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// ���(USB1020_XAXIS:X��; USB1020_YAXIS:Y��;USB1020_ZAXIS:Z��; 
			LC[i].LV_DV= USB1020_DV;				// ������ʽ USB1020_DV:�������� USB1020_LV: ��������
			LC[i].PulseMode = USB1020_CPDIR;		// ģʽ0��CW/CCW��ʽ��1��CP/DIR��ʽ 
			LC[i].Line_Curve = USB1020_LINE;		// ֱ������(0:ֱ�߼�/����; 1:S���߼�/����)
			DL[i].Multiple=1;
			DL[i].Acceleration = 150;				// ���ٶ�(125~1000,000)(ֱ�߼Ӽ��������м��ٶ�һֱ���䣩
			DL[i].Deceleration = 150;				// ���ٶ�(125~1000000)	
		}
		DL[0].StartSpeed = 400 ;				// ��ʼ�ٶ�(1~8000)
		DL[1].StartSpeed = 400 ;
		DL[2].StartSpeed = 400 ;
		DL[0].DriveSpeed = int(30*SpeedRatio*abs(x_angle)) ;				// �����ٶ�	(1~8000)	
		DL[1].DriveSpeed = int(30*SpeedRatio*abs(y_angle)) ;				// �����ٶ�	(1~8000)	
		DL[2].DriveSpeed = int(30*SpeedRatio*abs(z_angle)) ;				// �����ٶ�	(1~8000)	
		if (x_angle>=0)          
			LC[0].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		if (x_angle<0)    
			LC[0].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		if (y_angle>=0)          
			LC[1].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		if (y_angle<0)    
			LC[1].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		if (z_angle>=0)          
			LC[2].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		if (z_angle<0)    
			LC[2].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
		LC[0].nPulseNum =  int(abs(x_angle)*3600) ;				//��Ӧx��ʼ�Ƕ� �������������(0~268435455)
		LC[1].nPulseNum =  int(abs(y_angle)*3600) ;				//��Ӧy��ʼ�Ƕ� �������������(0~268435455)
		LC[2].nPulseNum =  int(abs(z_angle)*3600) ;				//��Ӧz��ʼ�Ƕ� �������������(0~268435455)
		for (int k=0;k<DeviceNum;k++)
		{
			if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
			{
				for (int j=0;j<DeviceNum;j++)   //device k �� ��������
				{
					USB1020_InitLVDV(						//��ʼ���ữ����,������������
						m_hDevice[k],
						&DL[j], 
						&LC[j]);
					USB1020_SetEncoderSignalType(           //���������ã�0��ʾ2����������
						m_hDevice[k],
						LC[j].AxisNum,
						0,
						0);
					USB1020_PulseInputMode( m_hDevice[k],	   //��������ģʽΪA/B��ģʽ
						LC[j].AxisNum,	
						0);  	
				}
			}
		}
		for (int i=0;i<DeviceNum;i++)
		{
			if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
				USB1020_Start4D( m_hDevice[i]);				// 4��ͬʱ����
				USB1020_GetRR0Status(
					m_hDevice[i],			// �豸���
					&(RR0[i]));				// RR�Ĵ���״̬	

				state_x[i]=RR0[i].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
				state_y[i]=RR0[i].YDRV;
				state_z[i]=RR0[i].ZDRV;
			}
		}
		//�ȴ����н���
		while(state_x[0]==1||state_y[0]==1||state_z[0]==1||
			state_x[1]==1||state_y[1]==1||state_z[1]==1||
			state_x[2]==1||state_y[2]==1||state_z[2]==1){
				for (int i=0;i<DeviceNum;i++)
				{
					if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
						USB1020_GetRR0Status(
							m_hDevice[i],			// �豸���
							&(RR0[i]));				// RR�Ĵ���״̬	

						state_x[i]=RR0[i].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
						state_y[i]=RR0[i].YDRV;
						state_z[i]=RR0[i].ZDRV;
					}
				}
		} 
		for (int i=0;i<DeviceNum;i++)
		{
			if(INVALID_HANDLE_VALUE != m_hDevice[i]){
				X_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[0].AxisNum);	// ��x��ʵλ
				Y_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[1].AxisNum);	// ��Y��ʵλ
				Z_EP[i] = USB1020_ReadEP(m_hDevice[i], LC[2].AxisNum);	// ��z��ʵλ 
				xangle[i]=0.0001*X_EP[i]/0.36;
				yangle[i]=0.0001*Y_EP[i]/0.36;
				zangle[i]=0.0001*Z_EP[i]/0.36;
			}
		}
		x_realangle<<"�ؽ���1_X"<<"    "<<"�ؽ���2_X"<<"    "<<"�ؽ���3_X"<<endl;
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[0]<<"   ";
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[1]<<"   ";
		x_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<xangle[2]<<endl;
		y_realangle<<"�ؽ���1_Y"<<"    "<<"�ؽ���2_Y"<<"    "<<"�ؽ���3_Y"<<endl;
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[0]<<"   ";
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[1]<<"   ";
		y_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<yangle[2]<<endl;
		z_realangle<<"�ؽ���1_Z"<<"    "<<"�ؽ���2_Z"<<"    "<<"�ؽ���3_Z"<<endl;
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[0]<<"   ";
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[1]<<"   ";
		z_realangle<<setiosflags(ios::fixed)<<setprecision(4)<<zangle[2]<<endl;
		return true;
}

bool Work_State2_Set(HANDLE m_hDevice[],ofstream& x_realangle, ofstream& y_realangle, ofstream&z_realangle,
	int x_pluse[], int y_pluse[], int z_pluse[], int pluse_length,int& SpeedRatio, bool& cutflag){
		//��������Ĺ��ò���
		USB1020_PARA_DataList DL[3];	// ���ò���     
		USB1020_PARA_LCData LC[3];		// ֱ�ߺ�S���߲���
		USB1020_PARA_RR0 RR0[3];	 //״̬�Ĵ���RR0
		LONG X_EP[DeviceNum],Y_EP[DeviceNum],Z_EP[DeviceNum] ;	//������ʵ��λ��
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //�����״̬�������洢��Ӧ״̬��Ϣ	
		double xangle[DeviceNum],yangle[DeviceNum],zangle[DeviceNum];	
		for (int  i=0;i<DeviceNum;i++)
		{
			X_EP[i]=Y_EP[i]=Z_EP[i]=0;
			state_x[i]=state_y[i]=state_z[i]=0;
			xangle[i]=yangle[i]=zangle[i]=0;
		}
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// ���(USB1020_XAXIS:X��; USB1020_YAXIS:Y��;USB1020_ZAXIS:Z��; 
			LC[i].LV_DV= USB1020_DV;				// ������ʽ USB1020_DV:�������� USB1020_LV: ��������
			LC[i].PulseMode = USB1020_CPDIR;		// ģʽ0��CW/CCW��ʽ��1��CP/DIR��ʽ 
			LC[i].Line_Curve = USB1020_LINE;		// ֱ������(0:ֱ�߼�/����; 1:S���߼�/����)
			DL[i].Multiple=1;
			DL[i].Acceleration = 150;				// ���ٶ�(125~1000,000)(ֱ�߼Ӽ��������м��ٶ�һֱ���䣩
			DL[i].Deceleration = 150;				// ���ٶ�(125~1000000)	
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

			DL[0].StartSpeed = abs(x_pluse[i]*SpeedRatio) ;				// ��ʼ�ٶ�(1~8000)
			DL[0].DriveSpeed = abs(x_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[0].nPulseNum =  abs(x_pluse[i]) ;				// �������������(0~268435455)
			if (x_pluse[i]>=0)          
			{
				LC[0].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (x_pluse[i]<0)    //����0Ϊ��ʱ��
			{
				LC[0].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}

			DL[1].StartSpeed = abs(y_pluse[i]*SpeedRatio) ;			    // ��ʼ�ٶ�(1~8000)
			DL[1].DriveSpeed = abs(y_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[1].nPulseNum = abs(y_pluse[i]) ;				            // �������������(0~268435455)
			if (y_pluse[i]>=0)
			{
				LC[1].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (y_pluse[i]<0)
			{
				LC[1].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}

			DL[2].StartSpeed = abs(z_pluse[i]*SpeedRatio) ;					// ��ʼ�ٶ�(1~8000)
			DL[2].DriveSpeed = abs(z_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[2].nPulseNum = abs(z_pluse[i]) ;				     // �������������(0~268435455)
			if (z_pluse[i]>=0)
			{
				LC[2].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (z_pluse[i]<0)
			{
				LC[2].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}

			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<3;j++)
					{
						USB1020_InitLVDV(						//	��ʼ���ữ����,������������
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //���������ã�0��ʾ2����������
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //��������ģʽΪA/B��ģʽ
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					USB1020_Start4D( m_hDevice[k]);				// 4��ͬʱ����
					USB1020_GetRR0Status(
						m_hDevice[k],			// �豸���
						&(RR0[k]));				// RR�Ĵ���״̬	

					state_x[k]=RR0[k].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
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
							m_hDevice[k],			// �豸���
							&(RR0[k]));				// RR�Ĵ���״̬	
						state_x[k]=RR0[k].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
						state_y[k]=RR0[k].YDRV;
						state_z[k]=RR0[k].ZDRV;
					}
				}                               
			}
			for (int k=0;k<DeviceNum;k++)
			{
				if (INVALID_HANDLE_VALUE != m_hDevice[k])
				{
					X_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[0].AxisNum);	// ��x��ʵλ
					Y_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[1].AxisNum);	// ��Y��ʵλ
					Z_EP[k] = USB1020_ReadEP(m_hDevice[k], LC[2].AxisNum);	// ��z��ʵλ 
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
		//��������Ĺ��ò���
		USB1020_PARA_DataList DL[3];	// ���ò���     
		USB1020_PARA_LCData LC[3];		// ֱ�ߺ�S���߲���
		USB1020_PARA_RR0 RR0[3];	 //״̬�Ĵ���RR0
		int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //�����״̬�������洢��Ӧ״̬��Ϣ	
		for (int  i=0;i<DeviceNum;i++)
			state_x[i]=state_y[i]=state_z[i]=0;
		for (int i=0;i<DeviceNum;i++)
		{
			LC[i].AxisNum = i;						// ���(USB1020_XAXIS:X��; USB1020_YAXIS:Y��;USB1020_ZAXIS:Z��; 
			LC[i].LV_DV= USB1020_DV;				// ������ʽ USB1020_DV:�������� USB1020_LV: ��������
			LC[i].PulseMode = USB1020_CPDIR;		// ģʽ0��CW/CCW��ʽ��1��CP/DIR��ʽ 
			LC[i].Line_Curve = USB1020_LINE;		// ֱ������(0:ֱ�߼�/����; 1:S���߼�/����)
			DL[i].Multiple=1;
			DL[i].Acceleration = 600;				// ���ٶ�(125~1000,000)(ֱ�߼Ӽ��������м��ٶ�һֱ���䣩
			DL[i].Deceleration = 600;				// ���ٶ�(125~1000000)
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
			DL[0].StartSpeed = abs(x_pluse[i]*SpeedRatio) ;				// ��ʼ�ٶ�(1~8000)
			DL[0].DriveSpeed = abs(x_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[0].nPulseNum =  abs(x_pluse[i]) ;				// �������������(0~268435455)
			if (x_pluse[i]>=0)          
			{
				LC[0].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (x_pluse[i]<0)    //����0Ϊ��ʱ��
			{
				LC[0].Direction = USB1020_PDIRECTION;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}

			DL[1].StartSpeed = abs(y_pluse[i]*SpeedRatio) ;			    // ��ʼ�ٶ�(1~8000)
			DL[1].DriveSpeed = abs(y_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[1].nPulseNum = abs(y_pluse[i]) ;				            // �������������(0~268435455)

			if (y_pluse[i]>=0)
			{
				LC[1].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (y_pluse[i]<0)
			{
				LC[1].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			DL[2].StartSpeed = abs(z_pluse[i]*SpeedRatio) ;					// ��ʼ�ٶ�(1~8000)
			DL[2].DriveSpeed = abs(z_pluse[i]*SpeedRatio) ;				// �����ٶ�	(1~8000)	
			LC[2].nPulseNum = abs(z_pluse[i]) ;				// �������������(0~268435455)
			if (z_pluse[i]>=0)
			{
				LC[2].Direction = USB1020_MDIRECTION;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			if (z_pluse[i]<0)
			{
				LC[2].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k] != INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<3;j++)
					{
						USB1020_InitLVDV(						//	��ʼ���ữ����,������������
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //���������ã�0��ʾ2����������
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //��������ģʽΪA/B��ģʽ
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int k=0;k<3;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					USB1020_Start4D( m_hDevice[k]);				// 4��ͬʱ����
					USB1020_GetRR0Status(
						m_hDevice[k],			// �豸���
						&(RR0[k]));				// RR�Ĵ���״̬	

					state_x[k]=RR0[k].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
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
							m_hDevice[k],			// �豸���
							&(RR0[k]));				// RR�Ĵ���״̬	

						state_x[k]=RR0[k].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
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
			//��������Ĺ��ò���
			USB1020_PARA_DataList DL[3];	// ���ò���     
			USB1020_PARA_LCData LC[3];		// ֱ�ߺ�S���߲���
			USB1020_PARA_RR0 RR0[3];	 //״̬�Ĵ���RR0
			int state_x[DeviceNum],state_y[DeviceNum],state_z[DeviceNum];  //�����״̬�������洢��Ӧ״̬��Ϣ	
			for (int  i=0;i<DeviceNum;i++)
				state_x[i]=state_y[i]=state_z[i]=0;
			for (int i=0;i<DeviceNum;i++)
			{
				LC[i].AxisNum = i;						// ���(USB1020_XAXIS:X��; USB1020_YAXIS:Y��;USB1020_ZAXIS:Z��; 
				LC[i].LV_DV= USB1020_DV;				// ������ʽ USB1020_DV:�������� USB1020_LV: ��������
				LC[i].PulseMode = USB1020_CPDIR;		// ģʽ0��CW/CCW��ʽ��1��CP/DIR��ʽ 
				LC[i].Line_Curve = USB1020_LINE;		// ֱ������(0:ֱ�߼�/����; 1:S���߼�/����)
				DL[i].Multiple=1;
				DL[i].Acceleration = 150;				// ���ٶ�(125~1000,000)(ֱ�߼Ӽ��������м��ٶ�һֱ���䣩
				DL[i].Deceleration = 150;				// ���ٶ�(125~1000000)	
			}
			DL[0].StartSpeed = 400 ;				// ��ʼ�ٶ�(1~8000)
			DL[1].StartSpeed = 400 ;
			DL[2].StartSpeed = 400 ;
			DL[0].DriveSpeed = int(30*SpeedRatio*abs(x_angle)) ;				// �����ٶ�	(1~8000)	
			DL[1].DriveSpeed = int(30*SpeedRatio*abs(y_angle)) ;				// �����ٶ�	(1~8000)	
			DL[2].DriveSpeed = int(30*SpeedRatio*abs(z_angle)) ;				// �����ٶ�	(1~8000)	
			if (x_angle>=0)          
				LC[0].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			if (x_angle<0)    
				LC[0].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			if (y_angle>=0)          
				LC[1].Direction = USB1020_MDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			if (y_angle<0)    
				LC[1].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			if (z_angle>=0)          
				LC[2].Direction = USB1020_MDIRECTION;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	
			if (z_angle<0)    
				LC[2].Direction = USB1020_PDIRECTION ;	// ת������ USB1020_PDirection: ��ת  USB1020_MDirection:��ת	

			LC[0].nPulseNum =  int(abs(x_angle)*3600) ;				//��Ӧx��ʼ�Ƕ� �������������(0~268435455)
			LC[1].nPulseNum =  int(abs(y_angle)*3600) ;				//��Ӧy��ʼ�Ƕ� �������������(0~268435455)
			LC[2].nPulseNum =  int(abs(z_angle)*3600) ;				//��Ӧz��ʼ�Ƕ� �������������(0~268435455)
			for (int k=0;k<DeviceNum;k++)
			{
				if (m_hDevice[k]!=INVALID_HANDLE_VALUE)
				{
					for (int j=0;j<DeviceNum;j++)   //device k �� ��������
					{
						USB1020_InitLVDV(						//��ʼ���ữ����,������������
							m_hDevice[k],
							&DL[j], 
							&LC[j]);
						USB1020_SetEncoderSignalType(           //���������ã�0��ʾ2����������
							m_hDevice[k],
							LC[j].AxisNum,
							0,
							0);
						USB1020_PulseInputMode( m_hDevice[k],	   //��������ģʽΪA/B��ģʽ
							LC[j].AxisNum,	
							0);  	
					}
				}
			}
			for (int i=0;i<3;i++)
			{
				if ( m_hDevice[i]!=INVALID_HANDLE_VALUE){
					USB1020_Start4D( m_hDevice[i]);				// 4��ͬʱ����
					USB1020_GetRR0Status(
						m_hDevice[i],			// �豸���
						&(RR0[i]));				// RR�Ĵ���״̬	
					state_x[i]=RR0[i].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
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
								m_hDevice[i],			// �豸���
								&(RR0[i]));				// RR�Ĵ���״̬	

							state_x[i]=RR0[i].XDRV;       //��X,Y,Z���Ӧ��״̬�Ĵ���ֵ
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