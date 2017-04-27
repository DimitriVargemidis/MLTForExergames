
//code from https://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#include "Console.h"
 
#include "save.h"

DWORD g_BytesTransferred = 0; //global variable BAD PRACTISE!!

void save::writeToFile(LPCWSTR filename)
{
	HANDLE hFile;
	char DataBuffer[] = "ThisIsSomeTestDataToWriteToTheFile /n 123456789 /n 10 11 12 13 15 16 17 18 19 20 21 23 24 25 26 27 28 29 30";
	DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	printf("\n");
	

	hFile = CreateFile(filename,                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_NEW,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DisplayError(TEXT("CreateFile"));
		_tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), filename);
		return;
	}

	_tprintf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, filename);

	bErrorFlag = WriteFile(
		hFile,           // open file handle
		DataBuffer,      // start of data to write
		dwBytesToWrite,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		DisplayError(TEXT("WriteFile"));
		printf("Terminal failure: Unable to write to file.\n");
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten,filename);
		}
	}

	CloseHandle(hFile);
}


VOID CALLBACK save::FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	_tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
	_tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
	g_BytesTransferred = dwNumberOfBytesTransfered;
}

void save::readFromFile(LPCWSTR filename)
{
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[BUFFERSIZE] = { 0 };
	OVERLAPPED ol = { 0 };

	printf("\n");
	/*
	if (argc != 2)
	{
		printf("Usage Error: Incorrect number of arguments\n\n");
		_tprintf(TEXT("Usage:\n\t%s <text_file_name>\n"), argv[0]);
		return;
	}
	*/

	hFile = CreateFile(filename,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DisplayError(TEXT("CreateFile"));
		_tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), filename);
		return;
	}

	// Read one character less than the buffer size to save room for
	// the terminating NULL character. 

	ol.OffsetHigh = 1;

	if (FALSE == ReadFileEx(hFile, ReadBuffer, BUFFERSIZE - 1, &ol, FileIOCompletionRoutine))
	{
		DisplayError(TEXT("ReadFile"));
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
		CloseHandle(hFile);
		return;
	}
	SleepEx(5000, TRUE);
	dwBytesRead = g_BytesTransferred;
	// This is the section of code that assumes the file is ANSI text. 
	// Modify this block for other data types if needed.

	if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE - 1)
	{
		ReadBuffer[dwBytesRead] = '\0'; // NULL character

		_tprintf(TEXT("Data read from %s (%d bytes): \n"), filename, dwBytesRead);
		
		//Console::print(ReadBuffer);
		printf("%s\n", ReadBuffer);


	}
	else if (dwBytesRead == 0)
	{
		_tprintf(TEXT("No data read from file %s\n"), filename);
	}
	else
	{
		printf("\n ** Unexpected value for dwBytesRead ** \n");
	}

	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);
}

/*
void DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
			+ lstrlen((LPCTSTR)lpszFunction)
			+ 40) // account for format string
			* sizeof(TCHAR));

	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error code %d as follows:\n%s"),
		lpszFunction,
		dw,
		lpMsgBuf)))
	{
		printf("FATAL ERROR: Unable to output error code.\n");
	}

	_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
*/



void save::DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
			+ lstrlen((LPCTSTR)lpszFunction)
			+ 40) // account for format string
			* sizeof(TCHAR));

	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error code %d as follows:\n%s"),
		lpszFunction,
		dw,
		lpMsgBuf)))
	{
		printf("FATAL ERROR: Unable to output error code.\n");
	}

	_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

