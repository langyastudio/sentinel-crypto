#include "stdafx.h"
#include "Errorprinter.h"


CErrorPrinter::CErrorPrinter()
{
    m_errorMap.insert(StatusMap::value_type(HASP_STATUS_OK, "Operation completed successfully"));
    m_errorMap.insert(StatusMap::value_type(HASP_MEM_RANGE, "Invalid memory address"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_PROGNUM_OPT, "Unknown/invalid Feature ID option"));
    m_errorMap.insert(StatusMap::value_type(HASP_INSUF_MEM, "Memory allocation failed"));
    m_errorMap.insert(StatusMap::value_type(HASP_TMOF, "Too many open Features"));
    m_errorMap.insert(StatusMap::value_type(HASP_ACCESS_DENIED, "Feature access denied"));
    m_errorMap.insert(StatusMap::value_type(HASP_INCOMPAT_FEATURE, "Incompatible Feature"));
    m_errorMap.insert(StatusMap::value_type(HASP_HASP_NOT_FOUND, "HASP Key not found"));
    m_errorMap.insert(StatusMap::value_type(HASP_TOO_SHORT, "Encryption/decryption length too short"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_HND, "Invalid handle"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_FILEID, "Invalid file ID / memory descriptor"));
    m_errorMap.insert(StatusMap::value_type(HASP_OLD_DRIVER, "Driver or support daemon version too old"));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_TIME, "Real time support not available"));
    m_errorMap.insert(StatusMap::value_type(HASP_SYS_ERR, "Generic error from host system call"));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_DRIVER, "HASP driver not found"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_FORMAT, "Unrecognized info format"));
    m_errorMap.insert(StatusMap::value_type(HASP_REQ_NOT_SUPP, "Request not supported"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_OBJ, "Invalid update object"));
    m_errorMap.insert(StatusMap::value_type(HASP_KEYID_NOT_FOUND, "Key with specified ID was not found"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_DATA, "Update data consistency check failed"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_NOTSUPP, "Update not supported by this key"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_UPDATE_CNTR, "Update counter mismatch"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_VCODE, "Invalid Vendor Code"));
    m_errorMap.insert(StatusMap::value_type(HASP_ENC_NOT_SUPP, "Requested encryption algorithm not supported"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_TIME, "Invalid date/time"));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_BATTERY_POWER, "Clock has no power"));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_ACK_SPACE, "Update requested ack., but no area to return it"));
    m_errorMap.insert(StatusMap::value_type(HASP_TS_DETECTED, "Terminal services (remote terminal) detected"));
    m_errorMap.insert(StatusMap::value_type(HASP_FEATURE_TYPE_NOT_IMPL, "Feature type not implemented"));
    m_errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_ALG, "Unknown algorithm"));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_SIG, "Signature check failed"));
    m_errorMap.insert(StatusMap::value_type(HASP_FEATURE_NOT_FOUND, "Feature not found" ));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_LOG, "Trace log is not enabled" ));
    m_errorMap.insert(StatusMap::value_type(HASP_LOCAL_COMM_ERR, "Communication error between application and local LM" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_VCODE, "Vendor Code not recognized by API)" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_SPEC, "Invalid XML spec" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_SCOPE, "Invalid XML scope" ));
    m_errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_KEYS, "Too many keys connected" ));
    m_errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_USERS, "Too many users" ));
    m_errorMap.insert(StatusMap::value_type(HASP_BROKEN_SESSION, "Broken session" ));
    m_errorMap.insert(StatusMap::value_type(HASP_REMOTE_COMM_ERR, "Communication error between local and remote License Manager" ));
    m_errorMap.insert(StatusMap::value_type(HASP_FEATURE_EXPIRED, "The feature is expired" ));
    m_errorMap.insert(StatusMap::value_type(HASP_OLD_LM, "HASP LM version is too old" ));
    m_errorMap.insert(StatusMap::value_type(HASP_DEVICE_ERR, "HASP SL secure storage I/O error or USB request error" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UPDATE_BLOCKED, "Update installation not allowed" ));
    m_errorMap.insert(StatusMap::value_type(HASP_TIME_ERR, "System time has been tampered" ));
    m_errorMap.insert(StatusMap::value_type(HASP_SCHAN_ERR, "Secure channel communication error" ));
    m_errorMap.insert(StatusMap::value_type(HASP_STORAGE_CORRUPT, "Secure storage contains garbage" ));
    m_errorMap.insert(StatusMap::value_type(HASP_NO_VLIB, "Vendor lib cannot be found" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_VLIB, "Vendor lib cannot be loaded" ));
    m_errorMap.insert(StatusMap::value_type(HASP_SCOPE_RESULTS_EMPTY, "No feature matching scope found" ));
    m_errorMap.insert(StatusMap::value_type(HASP_VM_DETECTED, "Virtual machine detected" ));
    m_errorMap.insert(StatusMap::value_type(HASP_HARDWARE_MODIFIED, "HASP update incompatible with this hardware; HASP key is locked to other hardware" ));
    m_errorMap.insert(StatusMap::value_type(HASP_USER_DENIED, "Login denied because of user restrictions" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UPDATE_TOO_OLD, "Update was already installed" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UPDATE_TOO_NEW, "Another update must be installed first" ));
    m_errorMap.insert(StatusMap::value_type(HASP_OLD_VLIB, "Vendor lib is too old" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UPLOAD_ERROR, "Upload via ACC failed, e.g. because of illegal format" ));
    
    m_errorMap.insert(StatusMap::value_type(HASP_INV_RECIPIENT, "Invalid XML \"recipient\" parameter" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_DETACH_ACTION, "Invalid XML \"action\" parameter" ));
    m_errorMap.insert(StatusMap::value_type(HASP_TOO_MANY_PRODUCTS, "Scope does not specify a unique Product" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_PRODUCT, "Invalid Product information" ));
    m_errorMap.insert(StatusMap::value_type(HASP_UNKNOWN_RECIPIENT, "Unknown Recipient; update can only be applied to the Recipient specified in hasp_detach(), and not to this computer" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_DURATION, "Invalid duration specified" ));
    m_errorMap.insert(StatusMap::value_type(HASP_CLONE_DETECTED, "Cloned HASP SL secure storage detected" ));
	
    m_errorMap.insert(StatusMap::value_type(HASP_NO_API_DYLIB, "A required API dynamic library was not found" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INV_API_DYLIB, "The found and assigned API dynamic library could not verified" ));

    m_errorMap.insert(StatusMap::value_type(HASP_INVALID_OBJECT, "Object incorrectly initialized" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INVALID_PARAMETER, "Invalid function parameter" ));
    m_errorMap.insert(StatusMap::value_type(HASP_ALREADY_LOGGED_IN, "Logging in twice to the same object" ));
    m_errorMap.insert(StatusMap::value_type(HASP_ALREADY_LOGGED_OUT, "Logging out twice from the same object" ));

    m_errorMap.insert(StatusMap::value_type(HASP_NOT_IMPL, "Capability isn't available" ));
    m_errorMap.insert(StatusMap::value_type(HASP_INT_ERR, "Internal API error" ));

	// 自己新增的ErrorCode
	m_errorMap.insert(StatusMap::value_type(HASP_ERR_MACHINENUM, "Error Machine Num" ));
	m_errorMap.insert(StatusMap::value_type(HASP_MACHINENUM_OUT_OF_BOUNDS, "Machine Num More Than Five" ));
	m_errorMap.insert(StatusMap::value_type(HASP_INV_PRODUCT_INFO, "Error Product Info" ));
	m_errorMap.insert(StatusMap::value_type(HASP_INV_VERSION_INFO, "Error Version Info" ));
	m_errorMap.insert(StatusMap::value_type(HASP_INV_DEVELOP_INFO, "Error The Develop Of No" ));
	m_errorMap.insert(StatusMap::value_type(HASP_ERR_ENCRYPT_DATA_OF_ROM, "Error Encrypt Data Of ROM" ));
	m_errorMap.insert(StatusMap::value_type(HASP_INV_HLID, "Error HL ID" ));
}

void CErrorPrinter::PrintError(hasp_status_t status)
{
    StatusMap::const_iterator error = m_errorMap.find(status);
    if(error == m_errorMap.end())
        cout<<"unknown error: "<<status<<endl;
    else
        cout<<error->second<<endl;
}

const char* CErrorPrinter::GetError(hasp_status_t status)
{
    StatusMap::const_iterator error = m_errorMap.find(status);
    if(error == m_errorMap.end())
        return("Unknown Error");
    else
        return(error->second);
}
