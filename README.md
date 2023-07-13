<h1>Malware Development Basics # 2: Syscalls</h1>

<p>Despite being titled basics, direct syscalls are an advanced topic but easy enough to understand. A system call is a part of any operating system 
allowing the usermode to carry out functions, sending queries / requests to the kernel to perform these requested functions. For example on Windows when 
you wish to interact with a file, the <b>ZwCreateFile</b></b> function will be called.  </p>

<P>This repository intends to teach you the very basics of windows architecure and how syscalls work, for proper implementation of syscalls these resources 
will be very useful.</P>
<ul>
  <li><a href="https://j00ru.vexillium.org/syscalls/nt/64/">Syscall codes</a></li>
   <li><a href="https://github.com/jthuraisamy/SysWhispers/tree/master">Syswhispers</a></li>  
</ul>

<h2>Windows Syscalls</h2>
<p>Windows contains over 2000 syscalls (a lot). These are stored in <b> Win32k.sys</b> for graphical related functions, and <b>ntdll.dll</b> (native library) for core operating 
system functionality, such as file creation.  The windows api uses system calls to perform any task.</p><p> The reason why you would want to make a direct system call is to avoid 
userland detection techniques, such as windows api hooking. If the windows api has been hooked, functions which you want to use can be blocked, and functions you don't 
want to be seen using can now be seen. A good example of this is opening restricted files, the windows api will block this request as the file is restricted via the call to NtCreateFile 
being blocked, instead we won't make this call, and directly invoke the syscall instead.</p>
<hr>
<p>By opening up ntdll.dll in IDA we can see the dissasembled native functions.  This is what is needed to make calls to them via the function declarations. This is different from 
using GetProcAddress to retrieve the virtual address of the function, as that is still using the api, instead we are skipping this and just doing what the api is doing under the hood,
and making the syscall directly, using the specified syscall number. All syscodes in windows use the same setup, the value in eax will be the desired syscall you want to make.</p>
<img src="https://github.com/Clydeston/windows-syscalls/blob/main/ntwritevirtualmem%20example.PNG"/>
