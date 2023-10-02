#include "Process.h"
#include "Blackbone/Process/Process.h"

using namespace blackbone;

ProcessPtr Process_Create()
{
	return static_cast<void*>(new Process());
}

void Process_Destroy(ProcessPtr process)
{
	delete static_cast<Process*>(process);
}

NTSTATUS Process_Attach(ProcessPtr process, DWORD pid, DWORD access)
{
	return static_cast<Process*>(process)->Attach(pid, access);
}

NTSTATUS Process_AttachByName(ProcessPtr process, const wchar_t* name, DWORD access)
{
	return static_cast<Process*>(process)->Attach(name, access);
}

NTSTATUS Process_AttachByHandle(ProcessPtr process, HANDLE handle)
{
	return static_cast<Process*>(process)->Attach(handle);
}

NTSTATUS Process_CreateAndAttach(ProcessPtr process, const wchar_t* path, BOOL suspended, BOOL forceInit, const wchar_t* cmdLine, const wchar_t* currentDir, STARTUPINFOW* pStartup)
{
	std::wstring sCmdLine;
	if (cmdLine != nullptr) {
		sCmdLine = cmdLine;
	}
	return static_cast<Process*>(process)->CreateAndAttach(path, suspended, forceInit, sCmdLine, currentDir, pStartup);
}	

NTSTATUS Process_Detach(ProcessPtr process)
{
	return static_cast<Process*>(process)->Detach();
}

NTSTATUS Process_EnsureInit(ProcessPtr process)
{
	return static_cast<Process*>(process)->EnsureInit();
}

NTSTATUS Process_Suspend(ProcessPtr process)
{
	return static_cast<Process*>(process)->Suspend();
}

NTSTATUS Process_Resume(ProcessPtr process)
{
	return static_cast<Process*>(process)->Resume();
}

DWORD Process_pid(ProcessPtr process)
{
	return static_cast<Process*>(process)->pid();

}

BOOL Process_valid(ProcessPtr process)
{
	return static_cast<Process*>(process)->valid();

}

NTSTATUS Process_Terminate(ProcessPtr process, DWORD code)
{

	return static_cast<Process*>(process)->Terminate(code);
}

void Process_EnumByName(const wchar_t* name, PROCENUMBYNAMEPROC lpEnumFunc, LPARAM lParam)
{
	auto pidList = Process::EnumByName(name);
	for (auto pid : pidList) {
		if (lpEnumFunc != nullptr) {
			lpEnumFunc(pid, lParam);
		}
	}
}

void Process_EnumHandles(ProcessPtr process, PROCHANDLEENUMPROC lpEnumFunc, LPARAM lParam)
{
	auto result = static_cast<Process*>(process)->EnumHandles();
	
	for (auto& cppHandleInfo : *result) {
		if (lpEnumFunc != nullptr) {
			Process_SectionInfo sectionInfo = { 0 };
			if (cppHandleInfo.section.get() != nullptr) {
				sectionInfo.attrib = cppHandleInfo.section->attrib;
				sectionInfo.size = cppHandleInfo.section->size;
			}

			Process_HandleInfo handleInfo = { 0 };
			handleInfo.handle = cppHandleInfo.handle;
			handleInfo.access = cppHandleInfo.access;
			handleInfo.flags = cppHandleInfo.flags;
			handleInfo.pObject = cppHandleInfo.pObject;
			handleInfo.typeName = cppHandleInfo.typeName.c_str();
			handleInfo.name = cppHandleInfo.name.c_str();
			handleInfo.section = &sectionInfo;

			lpEnumFunc(&handleInfo, lParam);
		}
	}
}

void Process_EnumByNameOrPID(DWORD pid, const wchar_t* name, BOOL includeThreads, PROCPROCESSENUMPROC lpEnumFunc, LPARAM lParam)
{
	std::wstring sName;
	if (name != nullptr) {
		sName = name;
	}

	bool bIncludeThreads = includeThreads == TRUE;

	auto result = Process::EnumByNameOrPID(pid, sName, bIncludeThreads);
	for (auto& cppProcessInfo : *result) {
		if (lpEnumFunc != nullptr) {
			Process_ProcessInfo processInfo = { 0 };
			processInfo.pid = cppProcessInfo.pid;
			processInfo.imageName = cppProcessInfo.imageName.c_str();

			std::vector<Process_ThreadInfo> threadInfos;
			for (auto& cppThreadInfo : cppProcessInfo.threads) {
				Process_ThreadInfo threadInfo = { 0 };
				threadInfo.tid = cppThreadInfo.tid;
				threadInfo.startAddress = cppThreadInfo.startAddress;
				threadInfo.mainThread = cppThreadInfo.mainThread ? TRUE : FALSE;
				threadInfos.push_back(threadInfo);
			}
			processInfo.threads = threadInfos.data();
			processInfo.threadCount = threadInfos.size();

			lpEnumFunc(&processInfo, lParam);
		}
	}
}

