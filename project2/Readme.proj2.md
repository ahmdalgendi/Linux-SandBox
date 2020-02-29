# Sand box

## this project adds a new functionality to the linux kernel
	
		the new functionality is to include "Sand box"
		where you can run your suspicious executable
		you can do that by using the 3 newly added system calls

##	----------------------------------------------------------


## New system calls:
	
    long sbx421_block(pid_t proc, unsigned long nr): 
    Blocks the process identified by proc from accessing the system call identified by nr. If proc is non-zero, this system call requires root privileges to run. If proc is zero the current process id (of the process running the system call) is used instead and any user may use the system call. That is to say that a process may always block itself from making a particular system call, but may not block any other process unless root is running it. Returns 0 on success or an appropriate error code on failure.

    long sbx421_unblock(pid_t proc, unsigned long nr): 
    Unblocks the process identified by proc from accessing the system call identified by nr. This system call requires root privileges to run and the proc argument must be non-zero. If the specified process is not blocking the system call given, then an error should be returned. Returns 0 on success or an appropriate error code on failure.

    long sbx421_count(pid_t proc, unsigned long nr): 
    Returns the number of times the process identified by the PID proc attempted to run the blocked system call nr. This system call requires root privileges to run and the proc argument must be non-zero. If the system call specified is not blocked by the specified process, returns an appropriate error code, otherwise returns the 0 or positive access count.

## How to use the code?
	
		to use the code you need to go to project2/proj2/userland
		run:
				make
		4 files will be created
			1- sbx421_block
			2- sbx421_unblock
			3- sbx421_count
			4- sbx421_run
		the first 3 binaries refer to one of the system calls, to use any of them :
			Usage <Process ID> <system call number>
		the 4th one "sbx421_run" usage:
			Usage <username> <block syscall file> <what to run>

