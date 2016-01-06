#include "stdafx.h"
#include "DataHandle.h"

void ReadData(double* angle, ifstream &infile, int &Angle_length){
	if (!infile){
		cout<<"No this file."<<endl;
		return;
	}    
	double data;
	int i=0;
	while (infile>>data)
	{
		angle[i]=data;
		i++;
	}
	Angle_length=i;
}

int Function_Convert(double Delta){
	int Count=0;
	if(Delta>=0){
		while ((Delta-Count*Angle_unit)>0)
		{
			Count++;
		}
		Count--;
		if ((Delta-Count*Angle_unit)>=0.5*Angle_unit)
		{
			return (Count+1);
		}
		else if ((Delta-Count*Angle_unit)<0.5*Angle_unit)
		{
			return Count;
		}
	}
	if(Delta<0){
		Delta=(-1)*Delta;  //È¡¾ø¶ÔÖµ
		while ((Delta-Count*Angle_unit)>0)
		{
			Count++;
		}
		Count--;
		if ((Delta-Count*Angle_unit)>=0.5*Angle_unit)
		{
			return (Count+1)*(-1);
		}
		else if ((Delta-Count*Angle_unit)<0.5*Angle_unit)
		{
			return Count*(-1);
		}
	}
}

void Angle_to_pulse(double* angle,const int angle_length, int* pluse,int &pluse_length){
	for (int i=0;i<(angle_length-1);i++)
	{
		double Delta= angle[i+1]-angle[i];
		pluse[i]=Function_Convert(Delta);
	}
	pluse_length=angle_length-1;
}

void WriteTxt(int* pluse, const int pluse_length, ofstream &outfile){
	for(int i=0;i<pluse_length;i++)
		outfile<<pluse[i]<<'\n';
}