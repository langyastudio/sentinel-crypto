//如果编译有问题，添加#include <afxwin.h>

#if !defined(AFX_HDSERIAL_H__1F6D2512_EBA9_4443_9FEF_DE78D7A28AFB__INCLUDED_)
#define AFX_HDSERIAL_H__1F6D2512_EBA9_4443_9FEF_DE78D7A28AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <windows.h>

#define DFP_GET_VERSION			0x00074080 
#define DFP_SEND_DRIVE_COMMAND	0x0007c084 
#define DFP_RECEIVE_DRIVE_DATA	0x0007c088 

typedef struct _GETVERSIONOUTPARAMS 
{ 
	BYTE bVersion; // Binary driver version. 
	BYTE bRevision; // Binary driver revision. 
	BYTE bReserved; // Not used. 
	BYTE bIDEDeviceMap; // Bit map of IDE devices. 
	DWORD fCapabilities; // Bit mask of driver capabilities. 
	DWORD dwReserved[4]; // For future use. 
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS; 

typedef struct _IDEREGS 
{ 
	BYTE bFeaturesReg; // Used for specifying SMART "commands". 
	BYTE bSectorCountReg; // IDE sector count register 
	BYTE bSectorNumberReg; // IDE sector number register 
	BYTE bCylLowReg; // IDE low order cylinder value 
	BYTE bCylHighReg; // IDE high order cylinder value 
	BYTE bDriveHeadReg; // IDE drive/head register 
	BYTE bCommandReg; // Actual IDE command. 
	BYTE bReserved; // reserved for future use. Must be zero. 
} IDEREGS, *PIDEREGS, *LPIDEREGS; 

typedef struct _SENDCMDINPARAMS 
{ 
	DWORD cBufferSize; // Buffer size in bytes 
	IDEREGS irDriveRegs; // Structure with drive register values. 
	BYTE bDriveNumber; // Physical drive number to send command to (0,1,2,3). 
	BYTE bReserved[3]; // Reserved for future expansion. 
	DWORD dwReserved[4]; // For future use. 
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS; 

typedef struct _DRIVERSTATUS 
{ 
	BYTE bDriverError; // Error code from driver, 
	// or 0 if no error. 
	BYTE bIDEStatus; // Contents of IDE Error register. 
	// Only valid when bDriverError 
	// is SMART_IDE_ERROR. 
	BYTE bReserved[2]; // Reserved for future expansion. 
	DWORD dwReserved[2]; // Reserved for future expansion. 
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS; 

typedef struct _SENDCMDOUTPARAMS 
{ 
	DWORD cBufferSize; // Size of bBuffer in bytes 
	DRIVERSTATUS DriverStatus; // Driver status structure. 
	BYTE bBuffer[512]; // Buffer of arbitrary length 
	// in which to store the data read from the drive. 
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS; 

typedef struct _IDSECTOR 
{ 
	USHORT wGenConfig; 
	USHORT wNumCyls; 
	USHORT wReserved; 
	USHORT wNumHeads; 
	USHORT wBytesPerTrack; 
	USHORT wBytesPerSector; 
	USHORT wSectorsPerTrack; 
	USHORT wVendorUnique[3]; 
	CHAR sSerialNumber[20]; 
	USHORT wBufferType; 
	USHORT wBufferSize; 
	USHORT wECCSize; 
	CHAR sFirmwareRev[8]; 
	CHAR sModelNumber[40]; 
	USHORT wMoreVendorUnique; 
	USHORT wDoubleWordIO; 
	USHORT wCapabilities; 
	USHORT wReserved1; 
	USHORT wPIOTiming; 
	USHORT wDMATiming; 
	USHORT wBS; 
	USHORT wNumCurrentCyls; 
	USHORT wNumCurrentHeads; 
	USHORT wNumCurrentSectorsPerTrack; 
	ULONG ulCurrentSectorCapacity; 
	USHORT wMultSectorStuff; 
	ULONG ulTotalAddressableSectors; 
	USHORT wSingleWordDMA; 
	USHORT wMultiWordDMA; 
	BYTE bReserved[128]; 
} IDSECTOR, *PIDSECTOR; 


//--------------------------------------------------------------
//						硬盘序列号
//--------------------------------------------------------------
BOOL GetHDSerial(char *lpszHD, int len=128);


#endif // !defined(AFX_HDSERIAL_H__1F6D2512_EBA9_4443_9FEF_DE78D7A28AFB__INCLUDED_)
