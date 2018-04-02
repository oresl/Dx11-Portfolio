#pragma once
// h에 입력
#define Singleton_h(classname)\
private:\
	static classname* sInstance;\
public:\
	static classname& GetInstance();\
	static void Delete();

// cpp에 입력
#define Singleton_cpp(classname)\
classname* classname::sInstance = NULL;\
classname& classname::GetInstance()\
{\
	if (sInstance == NULL)\
		sInstance = new classname;\
	return *sInstance;\
}\
void classname::Delete()\
{\
	SafeDeletePtr(sInstance);\
}

#define GetInst(classname) classname::GetInstance()

#define D3D GetInst(Direct3D)
#define Input GetInst(DirectInput)
#define RenderMgr GetInst(RenderManager)