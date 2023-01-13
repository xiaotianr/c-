#include "pch.h"
#include <iostream>
#include <string>
using namespace std;

#if 0
/*
������ģʽ���ò����ݵĽӿڿ�����һ����
����  =��   ͶӰ��    =��   ͶӰ����   VGA  HDMI  TypeC

VGA�ӿڵĵ��ԣ� (TV)ͶӰ��Ҳ��VGA�ӿ�
*/
class VGA // VGA�ӿ���
{
public:
	virtual void play() = 0;
};
// TV01��ʾ֧��VGA�ӿڵ�ͶӰ��
class TV01 : public VGA
{
public:
	void play()
	{
		cout << "ͨ��VGA�ӿ�����ͶӰ�ǣ�������Ƶ����" << endl;
	}
};

// ʵ��һ��������(ֻ֧��VGA�ӿ�)
class Computer
{
public:
	// ���ڵ���ֻ֧��VGA�ӿڣ����Ը÷����Ĳ���Ҳֻ��֧��VGA�ӿڵ�ָ��/����
	void playVideo(VGA *pVGA) 
	{
		pVGA->play();
	}
};
/*
����1����һ��֧��HDMI�ӿڵĵ��ԣ�����ͽд����ع�
����2����һ��ת��ͷ�������������ܹ���VGA�ź�ת��HDMI�źţ�����������������
*/

// ����һ���µ�ͶӰ�ǣ������µ�ͶӰ�Ƕ���ֻ֧��HDMI�ӿ�
class HDMI
{
public:
	virtual void play() = 0;
};
class TV02 : public HDMI
{
public:
	void play()
	{
		cout << "ͨ��HDMI�ӿ�����ͶӰ�ǣ�������Ƶ����" << endl;
	}
};


// ���ڵ��ԣ�VGA�ӿڣ���ͶӰ�ǣ�HDMI�ӿڣ��޷�ֱ��������������Ҫ�����������
class VGAToHDMIAdapter : public VGA
{
public:
	VGAToHDMIAdapter(HDMI *p) :pHdmi(p) {}
	void play() // �÷����൱�ھ���ת��ͷ������ͬ�ӿڵ��ź�ת����
	{
		pHdmi->play();
	}
private:
	HDMI *pHdmi;
};
int main()
{
	Computer computer;
	//computer.playVideo(new TV01());
	computer.playVideo(new VGAToHDMIAdapter(new TV02()));
	return 0;
}
#endif