#pragma once
#ifndef BLACKBONE4C_PROCESS_H
#define BLACKBONE4C_PROCESS_H

#include <Windows.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


struct Process_SectionInfo
{
	uint64_t size;
	uint32_t attrib;
};

struct Process_HandleInfo
{
	HANDLE handle;
	uint32_t access;
	uint32_t flags;
	uint64_t pObject;

	const wchar_t* typeName;
	const wchar_t* name;

	struct Process_SectionInfo* section;
};

struct Process_ThreadInfo
{
	uint32_t tid;
	uintptr_t startAddress;
	BOOL mainThread;
};

struct Process_ProcessInfo
{
	DWORD pid;
	const wchar_t* imageName;
	
	struct Process_ThreadInfo* threads;
	size_t threadCount;
};


typedef PVOID ProcessPtr;

// callbacks
typedef void (*PROCENUMBYNAMEPROC)(DWORD pid, LPARAM lParam);
typedef void (*PROCHANDLEENUMPROC)(struct Process_HandleInfo* handleInfo, LPARAM lParam);
typedef void (*PROCPROCESSENUMPROC)(struct Process_ProcessInfo* processInfo, LPARAM lParam);

ProcessPtr Process_Create();
void Process_Destroy(ProcessPtr process);
NTSTATUS Process_Attach(ProcessPtr process, DWORD pid, DWORD access);
NTSTATUS Process_AttachByName(ProcessPtr process, const wchar_t* name, DWORD access);
NTSTATUS Process_AttachByHandle(ProcessPtr process, HANDLE handle);
NTSTATUS Process_CreateAndAttach(
	ProcessPtr process,
	const wchar_t* path,
	BOOL suspended,
	BOOL forceInit,
	const wchar_t* cmdLine,
	const wchar_t* currentDir,
	STARTUPINFOW* pStartup
);
NTSTATUS Process_Detach(ProcessPtr process);
NTSTATUS Process_EnsureInit(ProcessPtr process);
NTSTATUS Process_Suspend(ProcessPtr process);
NTSTATUS Process_Resume(ProcessPtr process);
DWORD Process_pid(ProcessPtr process);
BOOL Process_valid(ProcessPtr process);
NTSTATUS Process_Terminate(ProcessPtr process, DWORD code);
void Process_EnumHandles(ProcessPtr process, PROCHANDLEENUMPROC lpEnumFunc, LPARAM lParam);
void Process_EnumByName(const wchar_t* name, PROCENUMBYNAMEPROC lpEnumFunc, LPARAM lParam);
void Process_EnumByNameOrPID(DWORD pid, const wchar_t* name, BOOL includeThreads, PROCPROCESSENUMPROC lpEnumFunc, LPARAM lParam);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !BLACKBONE4C_PROCESS_H
