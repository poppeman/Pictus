
#ifndef CACHEUSER_H
#define CACHEUSER_H

#include "imagecache.h"

#include "BasicThread.h"

class CacheUser
	:public BasicThread,
	 public Img::MessageReceiver

{
public:
	CacheUser(const std::wstring& folderA, const std::wstring& folderB)
		:m_pathA(folderA),
		 m_pathB(folderB),
		 m_isOnFirst(false)
	{}

private:
	struct CacheNotification
	{
		Img::MessageReceiver::LoadMessage message;
		std::wstring					desc;
	};

	typedef std::list<CacheNotification> NotificationList;
	NotificationList				m_notifications;
	std::mutex					m_mutexNotifications;

	void							ThreadMain();

	void							SwitchFolder();

	void							OnLoadMessage(Img::MessageReceiver::LoadMessage s, Img::Image* pImage, const std::wstring& desc);

	CacheUser(const CacheUser& b);

	std::wstring							m_pathA;
	std::wstring							m_pathB;

	bool							m_isOnFirst;
	Img::Cacher						m_cacher;
};

#endif