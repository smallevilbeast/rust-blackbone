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
