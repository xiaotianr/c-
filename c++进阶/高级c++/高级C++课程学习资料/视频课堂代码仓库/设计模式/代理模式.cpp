#include "pch.h"
#include <iostream>
#include <memory>
using namespace std;

#if 0
/*
����Proxyģʽ : ͨ�������࣬������ʵ�ʶ���ķ���Ȩ��
�ͻ�     ����Proxy     �ϰ� ί����
*/
class VideoSite // #1 ������
{
public:
	virtual void freeMovie() = 0; // ��ѵ�Ӱ
	virtual void vipMovie() = 0; // vip��Ӱ
	virtual void ticketMovie() = 0; // ��ȯ�ۿ���Ӱ
};
// ί���� #2
class FixBugVideoSite : public VideoSite 
{
public:
	virtual void freeMovie() // ��ѵ�Ӱ
	{
		cout << "�ۿ���ѵ�Ӱ" << endl;
	}
	virtual void vipMovie() // vip��Ӱ
	{
		cout << "�ۿ�VIP��Ӱ" << endl;
	}
	virtual void ticketMovie() // ��ȯ�ۿ���Ӱ
	{
		cout << "��ȯ�ۿ���Ӱ" << endl;
	}
};

// ������ #3 ����FixBugVideoSite
class FreeVideoSiteProxy : public VideoSite
{
public:
	virtual void freeMovie() // ��ѵ�Ӱ
	{
		Video.freeMovie(); // ͨ����������freeMovie������������ί��������freeMovie����
	}
	virtual void vipMovie() // vip��Ӱ
	{
		cout << "��Ŀǰֻ����ͨ�οͣ���Ҫ������VIP�����ܹۿ�VIP��Ӱ" << endl;
	}
	virtual void ticketMovie() // ��ȯ�ۿ���Ӱ
	{
		cout << "��Ŀǰû��ȯ����Ҫ�����Ӱȯ�����ܹۿ���Ӱ" << endl;
	}
private:
	FixBugVideoSite Video; // # 4
};

// ������ ����FixBugVideoSite
class VipVideoSiteProxy : public VideoSite
{
public:
	VipVideoSiteProxy() { pVideo = new FixBugVideoSite(); }
	~VipVideoSiteProxy() { delete pVideo; }
	virtual void freeMovie() // ��ѵ�Ӱ
	{
		pVideo->freeMovie(); // ͨ����������freeMovie������������ί��������freeMovie����
	}
	virtual void vipMovie() // vip��Ӱ
	{
		pVideo->vipMovie();
	}
	virtual void ticketMovie() // ��ȯ�ۿ���Ӱ
	{
		cout << "��Ŀǰû��ȯ����Ҫ�����Ӱȯ�����ܹۿ���Ӱ" << endl;
	}
private:
	VideoSite *pVideo;
};
// ��Щ����ͨ�õ�API�ӿڣ�ʹ�õĶ��ǻ����ָ���������
void watchMovice(unique_ptr<VideoSite> &ptr)
{
	ptr->freeMovie();
	ptr->vipMovie();
	ptr->ticketMovie();
}
int main()
{
	unique_ptr<VideoSite> p1(new FreeVideoSiteProxy()); // #5 �ͻ�ֱ�ӷ��ʴ������
	unique_ptr<VideoSite> p2(new VipVideoSiteProxy());

	watchMovice(p1);
	watchMovice(p2);

	return 0;
}
#endif