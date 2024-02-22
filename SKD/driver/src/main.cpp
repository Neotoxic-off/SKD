#include <ntifs.h>

void Log(PCSTR data)
{
	#ifndef DEBUG
		UNREFERENCED_PARAMETER(data);
	#endif // DEBUG

	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, data));
}

NTSTATUS DriverEntry()
{
	return (STATUS_SUCCESS);
}