#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

int main(int argc, char **argv)
{

	//returns a Handle containing information about running processes
	HANDLE curSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//if handle retrieval fails, close handle and exit program
	if (curSnap == INVALID_HANDLE_VALUE) {
		CloseHandle(curSnap);
		return 0;
	}
	//stores the first process into the curProc handle. If fails, free handle and exit program
	PROCESSENTRY32 curProc = { sizeof(PROCESSENTRY32) };
	while (strcmp(curProc.szExeFile, "notepad.exe")) {
		Process32Next(curSnap, &curProc);
	}
	cout << curProc.szExeFile << endl; 
	CloseHandle(curSnap);
	
	HANDLE myProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, curProc.th32ProcessID);
	FARPROC LoadLibAdd = GetProcAddress(LoadLibrary("kernel32.dll"), "LoadLibraryA");

	//path of dll to inject
	char dllpath[MAX_PATH] = "C:\\Users\\Brian\\source\\repos\\Sample DLL for Injecting\\Project1\\x64\\Debug\\Sample DLL.dll";

	LPVOID dllpathMem = (LPVOID)VirtualAllocEx(myProcess, NULL, strlen(dllpath)+1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//used to store the path of the dll in the memory created by virtualallocex. 
	WriteProcessMemory(myProcess, dllpathMem, dllpath, strlen(dllpath) + 1, NULL);
	//injects DLL by calling the LoadLibrary function from kernel32.dll. the path of the DLL is passed in as an argument
	CreateRemoteThread(myProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAdd, (LPVOID)dllpathMem, 0, NULL); 
	

	return 0;
}