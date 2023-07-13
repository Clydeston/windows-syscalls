
; key things to note about syscalls is that the syscall number will change per os version 
; for instance win10 22h2 the no for NtCreateThreadEx is C2h or 194d
; this can and will be different for windows versions 
; we are getting the raw instructions of the native functions to later link this procedure
; to the function declaration via the linker

; https://github.com/jthuraisamy/SysWhispers

.CODE	
	NtCreateThreadEx proc 
		mov r10, rcx 
		mov eax, 0C2h
		syscall	
		ret
	NtCreateThreadEx endp	

	NtWriteVirtualMemory proc 
		mov r10, rcx 
		mov eax, 3Ah
		syscall	
		ret
	NtWriteVirtualMemory endp	
END	