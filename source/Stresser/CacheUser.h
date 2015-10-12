
#ifndef CACHEUSER_H
#define CACHEUSER_H

#include "imagecache.h"

#include "BasicThread.h"

class CacheUser:public BasicThread,	 public Img::MessageReceiver
{
public:
	CacheUser(const std::string& folderA, const std::string& folderB);

private:
	struct CacheNotification
	{
		Img::MessageReceiver::LoadMessage message;
		std::string desc;
	};

	typedef std::list<CacheNotification> NotificationList;
	NotificationList m_notifications;
	std::mutex m_mutexNotifications;

	void ThreadMain();

	void SwitchFolder();

	void OnLoadMessage(Img::MessageReceiver::LoadMessage s, Img::Image* pImage, const std::string& desc);

	CacheUser(const CacheUser& b);

	std::string m_pathA;
	std::string m_pathB;

	bool m_isOnFirst;
	Img::Cacher m_cacher;
};

#endif