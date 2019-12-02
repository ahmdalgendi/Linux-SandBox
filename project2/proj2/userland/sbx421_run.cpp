#include<sys/wait.h>
#include <bits/stdc++.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include<iostream>
#include <iostream>
#include <sys/types.h>
#include <pwd.h>

/*
    utility that blocks a given pid with a set of syscalls numbers given from a file
    the code exits with 0 on success and with 1 on failure
*/
using namespace std;

#define SYSCALL_NM 434

long do_syscall(pid_t proc, unsigned long nr) {
	return syscall(SYSCALL_NM, proc , nr);
}


int is_digit(char c)
{
	return c >= '0' && c <= '9';
}

int  check_line(string line) {

	for (int i = 0; i < line.size(); ++i)
	{
		if (is_digit(line[i]) == 0 )
			return 0;
	}
	return 1;
}


vector<int> get_syscalls_from_file(string filename)
{
	fstream in;
	in.open(filename.c_str());
	string line;
	std::vector<int> ret;

	if (in.good() == 0)
	{
		puts("syscall file not found");
		exit(1);
	}
	while (getline(in , line))
	{
		if (check_line(line) == 0)
			exit(1); // exit failure

		stringstream z(line);
		int tmp;
		z << tmp;
		ret.push_back(tmp);
	}
	return ret;
}


void block_from_file(pid_t proc, char * filename)
{
	string filen(filename);

	auto syscalls = get_syscalls_from_file(filen);

	for (auto i : syscalls) {
		long rv = do_syscall(proc, i);

		if (rv >= 0) {
			printf("Process Blocked successfully");
		}
		else {
			perror("err:");
		}
	}

}

set<string> get_users_list()
{
	set<string> ret;
	while (true) {
		errno = 0; // so we can distinguish errors from no more entries
		passwd* entry = getpwent();
		if (!entry) {
			if (errno) {
				std::cerr << "Error reading password database\n";
				exit(1);
				return set<string>();
			}
			break;
		}
		// std::cout << entry->pw_name << '\n';
		ret.insert(entry->pw_name);
	}
	endpwent();
	return ret;
}
int main(int argc  , char *argv[]) {
	auto users_list = get_users_list();
	if (argc < 4)
	{
		printf("Usage <username> <block syscall file> <what to run>\n\t\tEXITING\n");
		exit(1);

	}
	if (users_list.find(argv[1]) == users_list.end())
	{
		puts("Given user is not a valid user\nExiting.....");
		exit(1);
	}


	block_from_file(getpid(), argv[2]);

	auto ret= execvp(argv[3], argv +3);
	if(ret < 0 )
	{
		perror("err:");
	}
	return  0;
}