#ifndef PICTTHUMBS_CLASSFACTORY_H
#define PICTTHUMBS_CLASSFACTORY_H

typedef HRESULT(*PFNCREATEINSTANCE)(_In_ REFIID riid, _COM_Outptr_ void **ppvObject);

struct CLASS_OBJECT_INIT {
	const CLSID *pClsid;
	PFNCREATEINSTANCE pfnCreate;
};

class CClassFactory : public IClassFactory {
public:
	static HRESULT CreateInstance(REFCLSID clsid, const CLASS_OBJECT_INIT *pClassObjectInits, size_t cClassObjectInits, REFIID riid, void **ppv);

	CClassFactory(PFNCREATEINSTANCE pfnCreate);

	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv);

	IFACEMETHODIMP_(ULONG) AddRef();

	IFACEMETHODIMP_(ULONG) Release();

	// IClassFactory
	IFACEMETHODIMP CreateInstance(_In_opt_ IUnknown *punkOuter, _In_ REFIID riid, _COM_Outptr_ void **ppv);

	IFACEMETHODIMP LockServer(BOOL fLock);

private:
	~CClassFactory();

	long _cRef;
	PFNCREATEINSTANCE _pfnCreate;
};

#endif
