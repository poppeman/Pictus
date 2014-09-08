#include "StdAfx.h"

#include "orz/orz_math.h"

#include "CacheUser.h"
#include "main.h"
#include "orz/fileops.h"

void CacheUser::ThreadMain() {
	Output(TX("Initializing CacheUser..."));

	Img::CodecFactoryStore cfs;
	cfs.AddBuiltinCodecs();

	m_cacher.Construct();
	m_cacher.MessageTarget(this);
	m_cacher.MemoryLimit(16 * 1024 * 1024);
	m_cacher.SetCodecFactoryStore(&cfs);

	SwitchFolder();

	Math::Randomizer random;
	
	Output(TX("Init complete, running ..."));

	while(IsTerminating() == false) {
		std::unique_lock<std::mutex> l(m_mutexNotifications);
		while (m_notifications.empty() == false) {
			CacheNotification currentNotification = m_notifications.front();
			m_notifications.pop_front();

			SignalError(TX("Cacher notification: ") + ToWString(currentNotification.message) + TX("\n") + currentNotification.desc + std::wstring(TX("\n")));
		}
		l.unlock();


		if (m_cacher.ImageCount() == 0) {
			Output(TX("Ran out of images, switching RAIT NAO!"));
			SwitchFolder();
		}

		if (m_cacher.ImageCount() == 0)
		{
			Output(TX("Still out of images, giving up!"));
			m_cacher.Stop();
			return;
		}

		Img::Image::Ptr currentImage = m_cacher.CurrentImage();

		int action = random.Random() % 13;
		int sleepChunks = random.Random() % 5;
		Sleep(30 * sleepChunks);

		if (action == 0)
			m_cacher.GotoImage(random.Random() % m_cacher.ImageCount());
		else if (action == 1)
			m_cacher.NextImage();
		else if (action == 2)
			m_cacher.PreviousImage();
/*		else if (action == 3 && currentImage->IsHeaderInformationValid())
			currentImage->FileSize();*/
		else if (action == 4 && currentImage->IsHeaderInformationValid())
			currentImage->Delay();
		else if (action == 5) {
			Img::Surface::Ptr surface(currentImage->CurrentSurface());
			if (surface)
				surface->IsDirty();
		}
		else if (action == 6)
			currentImage->IsFinished();
		else if (action == 7)
			currentImage->GetSize();
		else if (action == 8)
			Sleep(100);
		else if (action == 9)
			m_cacher.RemoveCurrentImage();
		else if (action == 10)
			SwitchFolder();
		/*else if (action == 11) {
			m_cacher.Pause();
			m_cacher.RemoveCurrentImage();
			m_cacher.Resume();
		}
		else if (action == 12) {
			m_cacher.Pause();
			Sleep(300);
			m_cacher.Resume();
		}*/

		//Output(ToWString(action));
	}
	m_cacher.Stop();
};

void CacheUser::SwitchFolder() {
	IO::Folder f;

	if (m_isOnFirst == false)
		f.Path(m_pathA.c_str());
	else
		f.Path(m_pathB.c_str());

	m_isOnFirst = !m_isOnFirst;

	Img::FillCacher(m_cacher, f);
}

void CacheUser::OnLoadMessage( Img::MessageReceiver::LoadMessage s, Img::Image*, const std::wstring& desc ) {
	if (s == Img::MessageReceiver::LoadErrorCritical) {
		std::lock_guard<std::mutex> l(m_mutexNotifications);
		CacheNotification newNotification = {s, desc};
		m_notifications.push_back(newNotification);
	}
}
