// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <string>
#include <vector>
#include <ctime>

#define MEMSIZE_MB  1048576
#define MEMSIZE_4KB  4096
int function_counter = 0;

void mem_alloc_1(std::vector<LPVOID> p, DWORD alloc_type, DWORD page_type){
	int mem_megab = 0;
	int mem_kbyte = 0;
	bool flag = false;
	while (true) {
		p.push_back(VirtualAlloc(NULL, MEMSIZE_MB, alloc_type, page_type));
		if (GetLastError()) {
			while (true) {
				p.push_back(VirtualAlloc(NULL, MEMSIZE_4KB, alloc_type, page_type));
				if (GetLastError()) {
					std::cout << GetLastError() << "      " << mem_megab << " MB   " << mem_kbyte << " KB" << std::endl;
					flag = true;
					break;
				}
				mem_kbyte += 4;
			}
			if (flag)
				break;
		}
		mem_megab++;
	}
}

void mem_alloc_2(std::vector<LPVOID> p, DWORD alloc_type) {
	srand(time(0));
	int gb = rand() % 2;
	srand(time(0));
	int mb = rand() % 500;
	srand(time(0));
	int kb4 = rand() % 250;
	DWORD page_type;
	int num_mb = gb * 1024 + mb;
	int num_4kb = kb4;
	int num_page_guard = 0, num_page_rw = 0;
	srand(time(0));
	for (int i = 0; i < num_mb; i++) {
		int page_type_gen = rand() % 2;
		if (page_type_gen) {
			page_type = PAGE_READWRITE;
			num_page_rw++;
		}
		else {
			page_type = PAGE_GUARD;
			num_page_guard++;
		}

		SYSTEM_INFO lpSystemInfo;
		GetSystemInfo(&lpSystemInfo);
		int max_num = (char*)(lpSystemInfo.lpMaximumApplicationAddress) - (char*)(lpSystemInfo.lpMinimumApplicationAddress);
		int min_num = 1;
		int randNum = rand() % (max_num - min_num + 1) + min_num;
		LPVOID tp;
		tp = LPVOID((char*)lpSystemInfo.lpMaximumApplicationAddress - randNum);

		p.push_back(VirtualAlloc(tp, MEMSIZE_MB, alloc_type, page_type));
	}
	for (int i = 0; i < num_4kb; i++) {
		int page_type_gen = rand() % 2;
		if (page_type_gen) {
			page_type = PAGE_READWRITE;
			num_page_rw++;
		}
		else {
			page_type = PAGE_GUARD;
			num_page_guard++;
		}
		p.push_back(VirtualAlloc(NULL, MEMSIZE_4KB, alloc_type, page_type));
	}
	std::cout << num_mb << " MB   " << num_4kb * 4 << " KB" << std::endl << "PAGE_GUARD: " << num_page_guard
		<< "   PAGE_READWRITE: " << num_page_rw << std::endl;
}

void handle_3() {
	int cnt = 1;
	HANDLE event;
	event = CreateEvent(NULL, true, true, NULL);
	while (true) {
		HANDLE new_handle;
		if (DuplicateHandle(GetCurrentProcess(), event, GetCurrentProcess(), &new_handle,
			0, FALSE, DUPLICATE_SAME_ACCESS)) {
			cnt++;
		}
		else
			break;
	}
	std::cout << "Max number of handlers: " << cnt << std::endl;
}

void gdi_4() {
	int cnt = 0;

	while (true) {
		COLORREF color;
		color = RGB(100, 100, 10);
		if (CreatePen(PS_SOLID, 10, color) == NULL) {
			break;
		}
		cnt++;
	}
	std::cout << "Max number of pens: " << cnt << std::endl;
}

void empty_func_5(){
	function_counter++;
	empty_func_5();
}

void empty_func_6(int a) {
	function_counter++;
	empty_func_6(a);
}

void mem_free(std::vector<LPVOID> p) {
	for (int i = 0; i < p.size(); i++) {
		VirtualFree(p[i], 0, MEM_RELEASE);
	}
}

int main() {
	std::vector<LPVOID> p;
	//mem_alloc_1(p, MEM_RESERVE, PAGE_READWRITE);
	//mem_free(p);

	//mem_alloc_2(p, MEM_RESERVE);
	//system("pause");
	//mem_free(p);
	//mem_alloc_2(p, MEM_COMMIT);
	//system("pause");
	//mem_free(p);

	//handle_3();
	
	//gdi_4();

	//empty_func_5(); // result: 5120 KB and 4879 functions

	//int a = 0;
	//empty_func_6(a); // result: 

	return 0;
}

