# New version of linux kernel with Sand-box
## in this project I implemented A sand-box in side the linux kernel code

---------------
## New added system calls

* long sbx421alt_block(pid_t proc, unsigned long nr): 
	
		Blocks the process identified by proc from accessing the system call identified by nr 
		using the extra credit block list stored in the task_struct for the specified process. 
		If proc is non-zero, this system call requires root privileges to run. If proc is zero 
		the current process id (of the process running the system call) is used instead and any 
		user may use the system call. That is to say that a process may always block itself 
		from making a particular system call, but may not 
		block any other process unless root is running it. 
		Returns 0 on success or an appropriate error code on failure.

* long sbx421alt_unblock(pid_t proc, unsigned long nr): 

		Unblocks the process identified by proc from accessing the system call identified by nr 
		using a block list stored in the task_struct for the specififed process. 
		This system call requires root privileges to run and the proc argument must be non-zero. 
		If the specified process is not blocking the system call given, 
		then an error should be returned. 
		Returns 0 on success or an appropriate error code on failure.


This project was part of the Operating system course from UMBC

[Full descreption to the project](https://bluegrit.cs.umbc.edu/~lsebald1/cmsc421-fa2019/project2.shtml)

## for how to run and compile

1. apt-get install build-essential qt5-default qtcreator valgrind nano patch diffutils curl fakeroot git pkg-config gedit libssl-dev libncurses5-dev libelf-dev bison flex bc
2. chmod 777 . // before cloning the repo
3. make mrproper
4. make xconfig
5. make localmodconfig
6. make bindeb-pkg 

		During the make xconfig step, you should go into the Library routines section of the configuration and 
		make the "CRC32c CRC algorithm" or "CRC32c (Castagnoli, et all) Cyclic Redundancy-Check" 
		option be built-in to the kernel instead of being built as a module (make it so it has a checkmark, 
			not a dot in the box next to it). Once you've done that, hit the floppy disk icon to save the 
			configuration. If the make localmodconfig step prompts you at any point, you can just hit 
			Enter to accept the defaults. If you have increased the number of cores given to your 
			VM earlier in the setup, you may wish to give the -j flag to the make bindeb-pkg 
			step to allow it to use multiple cores. If you've dedicated 2 cores to your VM, 
			you'd run that command as make -j2 bindeb-pkg instead.