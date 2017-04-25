
#ifndef SAVE_H
#define SAVE_H

namespace save
{

	//#define BUFFERSIZE 5
	#define BUFFERSIZE 120
	//DWORD g_BytesTransferred = 0;

	void DisplayError(LPTSTR lpszFunction);

	VOID CALLBACK FileIOCompletionRoutine(
		__in  DWORD dwErrorCode,
		__in  DWORD dwNumberOfBytesTransfered,
		__in  LPOVERLAPPED lpOverlapped
	);

	


	void writeToFile(LPCWSTR filename);
	void readFromFile(LPCWSTR filename);
	void DisplayError(LPTSTR lpszFunction);

};

#endif