

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/resource.h>

void printMemoryInfo(void);
void printSystemMemoryInfo(void);

int main() {
	printSystemMemoryInfo();
	printMemoryInfo();
	return 0;
}


void printSystemMemoryInfo(void) {
	std::ifstream meminfo("/proc/meminfo");
	std::string line;

	std::cout << "=== /proc/meminfo (System Memory) ===" << std::endl;
	while (std::getline(meminfo, line)) {
		if (line.rfind("MemTotal:", 0) == 0 ||
		    line.rfind("MemFree:", 0) == 0 ||
		    line.rfind("MemAvailable:", 0) == 0 ||
		    line.rfind("Buffers:", 0) == 0 ||
		    line.rfind("Cached:", 0) == 0)
		{
			std::cout << line << std::endl;
		}
	}
}

void printMemoryInfo(void) {
	long pageSizeKB = sysconf(_SC_PAGE_SIZE) / 1024; // in KB

	// Read from /proc/self/status
	std::ifstream statusFile("/proc/self/status");
	std::string line;
	std::cout << "=== /proc/self/status ===" << std::endl;
	while (std::getline(statusFile, line)) {
		if (line.rfind("VmSize:", 0) == 0 ||  // total virtual memory
		    line.rfind("VmRSS:", 0) == 0 ||   // resident set size
		    line.rfind("VmData:", 0) == 0 ||  // data segment size (heap)
		    line.rfind("VmStk:", 0) == 0 ||   // stack size
		    line.rfind("VmExe:", 0) == 0)     // text segment size
		{
			std::cout << line << std::endl;
		}
	}
	statusFile.close();

	// Read from /proc/self/statm
	std::ifstream statmFile("/proc/self/statm");
	unsigned long size, resident, shared, text, lib, data, dt;
	statmFile >> size >> resident >> shared >> text >> lib >> data >> dt;
	statmFile.close();

	std::cout << "\n=== /proc/self/statm ===" << std::endl;
	std::cout << "Size: "     << size     * pageSizeKB << " KB" << std::endl;
	std::cout << "Resident: " << resident * pageSizeKB << " KB" << std::endl;
	std::cout << "Shared: "   << shared   * pageSizeKB << " KB" << std::endl;
	std::cout << "Text: "     << text     * pageSizeKB << " KB" << std::endl;
	std::cout << "Data: "     << data     * pageSizeKB << " KB" << std::endl;

	// Stack size limit
	struct rlimit rl;
	if (getrlimit(RLIMIT_STACK, &rl) == 0) {
		std::cout << "\n=== Stack Limit ===" << std::endl;
		if (rl.rlim_cur == RLIM_INFINITY)
			std::cout << "Stack size: unlimited" << std::endl;
		else
			std::cout << "Stack size: " << (rl.rlim_cur / 1024) << " KB" << std::endl;
	}
}
