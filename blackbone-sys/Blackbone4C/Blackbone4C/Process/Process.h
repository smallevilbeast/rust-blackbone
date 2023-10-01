#pragma once
#ifndef BLACKBONE4C_PROCESS_H
#define BLACKBONE4C_PROCESS_H

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef PVOID ProcessPtr;
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


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !BLACKBONE4C_PROCESS_H
