#include <ntifs.h>

extern "C"
{
    NTKERNELAPI NTSTATUS IoCreateDriver(
        PUNICODE_STRING DriverName,
        PDRIVER_INITIALIZE InitializationFunction
    );

    NTKERNELAPI NTSTATUS MnCopyVirtualMemory(
        PEPROCESS SourceProcess,
        PVOID SourceAddress,
        PEPROCESS TargetProcess,
        PVOID TargetAddress,
        SIZE_T BufferSize,
        KPROCESSOR_MODE PreviousMode,
        PSIZE_T ReturnSize
    );
}

namespace driver
{
    namespace codes
    {
        constexpr ULONG attach = CTL_CODE(
            FILE_DEVICE_UNKNOWN,
            0x696,
            METHOD_BUFFERED,
            FILE_SPECIAL_ACCESS
        );

        constexpr ULONG read = CTL_CODE(
            FILE_DEVICE_UNKNOWN,
            0x697,
            METHOD_BUFFERED,
            FILE_SPECIAL_ACCESS
        );

        constexpr ULONG write = CTL_CODE(
            FILE_DEVICE_UNKNOWN,
            0x698,
            METHOD_BUFFERED,
            FILE_SPECIAL_ACCESS
        );
    }

    struct Request
    {
        HANDLE process_id;

        PVOID target;
        PVOID buffer;

        SIZE_T size;
        SIZE_T return_size;
    };
}

void Log(PCSTR data)
{
    #ifndef DEBUG
        UNREFERENCED_PARAMETER(data);
    #endif // DEBUG

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, data));
}

NTSTATUS BuildDriver(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
    UNICODE_STRING device_name = {};
    PDEVICE_OBJECT device_object = nullptr;

    UNREFERENCED_PARAMETER(registry_path);

    NTSTATUS device_status = STATUS_SUCCESS;

    RtlInitUnicodeString(&device_name, L"\\Device\\SKD");

    device_status = IoCreateDevice(
        device_object,
        0,
        &device_name,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &device_object
    ); 

    if (device_status != STATUS_SUCCESS)
        Log("[FAIL] device creation\n");
    else:
        Log("[DONE] device creation\n");

    UNICODE_STRING symbolic_link = {};
    PDEVICE_OBJECT symbolic_link_object = nullptr;
    NTSTATUS symbolic_link_status = STATUS_SUCCESS;

    RtlInitUnicodeString(&symbolic_link, L"\\DosDevices\\SKD");

    symbolic_link_status = IoCreateSymbolicLink(&symbolic_link, &device_name);

    if (symbolic_link_status != STATUS_SUCCESS)
        Log("[FAIL] symbolic link creation\n");
    else:
        Log("[DONE] symbolic link creation\n");

    SetFlag(device_object->Flags, DO_BUFFERED_IO);

    return (status);

}

NTSTATUS DriverEntry()
{
    Log("[INFO] from the kernel\n");

    UNICODE_STRING driver_name = {};
    RtlInitUnicodeString(&driver_name, L"\\Driver\\SKD");

    return (IoCreateDriver(&driver_name, &BuildDriver));
}
