


#include <iostream>
#include "video.h"

static int CPU_haveCPUID(void)
{
    int has_CPUID = 0;

/* *INDENT-OFF* */
#ifndef SDL_CPUINFO_DISABLED
#if defined(__GNUC__) && defined(i386)
    __asm__ (
"        pushfl                      # Get original EFLAGS             \n"
"        popl    %%eax                                                 \n"
"        movl    %%eax,%%ecx                                           \n"
"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
"        pushl   %%eax               # Save new EFLAGS value on stack  \n"
"        popfl                       # Replace current EFLAGS value    \n"
"        pushfl                      # Get new EFLAGS                  \n"
"        popl    %%eax               # Store new EFLAGS in EAX         \n"
"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
"        jz      1f                  # Processor=80486                 \n"
"        movl    $1,%0               # We have CPUID support           \n"
"1:                                                                    \n"
    : "=m" (has_CPUID)
    :
    : "%eax", "%ecx"
    );
#elif defined(__GNUC__) && defined(__x86_64__)
/* Technically, if this is being compiled under __x86_64__ then it has 
   CPUid by definition.  But it's nice to be able to prove it.  :)      */
    __asm__ (
"        pushfq                      # Get original EFLAGS             \n"
"        popq    %%rax                                                 \n"
"        movq    %%rax,%%rcx                                           \n"
"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
"        pushq   %%rax               # Save new EFLAGS value on stack  \n"
"        popfq                       # Replace current EFLAGS value    \n"
"        pushfq                      # Get new EFLAGS                  \n"
"        popq    %%rax               # Store new EFLAGS in EAX         \n"
"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
"        jz      1f                  # Processor=80486                 \n"
"        movl    $1,%0               # We have CPUID support           \n"
"1:                                                                    \n"
    : "=m" (has_CPUID)
    :
    : "%rax", "%rcx"
    );
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
    __asm {
        pushfd                      ; Get original EFLAGS
        pop     eax
        mov     ecx, eax
        xor     eax, 200000h        ; Flip ID bit in EFLAGS
        push    eax                 ; Save new EFLAGS value on stack
        popfd                       ; Replace current EFLAGS value
        pushfd                      ; Get new EFLAGS
        pop     eax                 ; Store new EFLAGS in EAX
        xor     eax, ecx            ; Can not toggle ID bit,
        jz      done                ; Processor=80486
        mov     has_CPUID,1         ; We have CPUID support
done:
    }
#elif defined(_MSC_VER) && defined(_M_X64)
    has_CPUID = 1;
#elif defined(__sun) && defined(__i386)
    __asm (
"       pushfl                 \n"
"       popl    %eax           \n"
"       movl    %eax,%ecx      \n"
"       xorl    $0x200000,%eax \n"
"       pushl   %eax           \n"
"       popfl                  \n"
"       pushfl                 \n"
"       popl    %eax           \n"
"       xorl    %ecx,%eax      \n"
"       jz      1f             \n"
"       movl    $1,-8(%ebp)    \n"
"1:                            \n"
    );
#elif defined(__sun) && defined(__amd64)
    __asm (
"       pushfq                 \n"
"       popq    %rax           \n"
"       movq    %rax,%rcx      \n"
"       xorl    $0x200000,%eax \n"
"       pushq   %rax           \n"
"       popfq                  \n"
"       pushfq                 \n"
"       popq    %rax           \n"
"       xorl    %ecx,%eax      \n"
"       jz      1f             \n"
"       movl    $1,-8(%rbp)    \n"
"1:                            \n"
    );
#endif
#endif
/* *INDENT-ON* */
    return has_CPUID;
}

int GetCPUCount(void)
{
	int SDL_CPUCount = 0;
	if (!SDL_CPUCount)
	{
#ifndef SDL_CPUINFO_DISABLED
#if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_ONLN)
		if (SDL_CPUCount <= 0) {
			SDL_CPUCount = (int)sysconf(_SC_NPROCESSORS_ONLN);
		}
#endif
#ifdef HAVE_SYSCTLBYNAME
		if (SDL_CPUCount <= 0) {
			size_t size = sizeof(SDL_CPUCount);
			sysctlbyname("hw.ncpu", &SDL_CPUCount, &size, NULL, 0);
		}
#endif
#ifdef __WIN32__
		if (SDL_CPUCount <= 0) {
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			SDL_CPUCount = info.dwNumberOfProcessors;
		}
#endif
#ifdef __OS2__
		if (SDL_CPUCount <= 0) {
			DosQuerySysInfo(QSV_NUMPROCESSORS, QSV_NUMPROCESSORS,
			                &SDL_CPUCount, sizeof(SDL_CPUCount) );
		}
#endif
#endif
		/* There has to be at least 1, right? :) */
		if (SDL_CPUCount <= 0) {
			SDL_CPUCount = 1;
		}
	}
	return SDL_CPUCount;
}

int main()
{

	std::cout << "Hello World" << std::endl;
	std::cout << "Have cpuinfo: " << std::to_string(CPU_haveCPUID()) << " : " << std::to_string(GetCPUCount()) << std::endl;

	WindowData tmp;

	
}