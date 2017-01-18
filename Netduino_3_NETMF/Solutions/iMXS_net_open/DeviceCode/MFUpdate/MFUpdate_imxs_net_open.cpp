#include <tinyhal.h>
#include <mfupdate_decl.h>
#include <Drivers\MFUpdate\MicroBooterUpdate.h>


static const IUpdatePackage s_UpdatePackages[] =
{
    {
        "NetMF",
        &g_MicroBooterUpdateProvider,
        &g_SslUpdateValidationProvider,
        &g_BlockStorageUpdateProvider,
        NULL,
    }, 
    {
        "HTTPSUpdate",
        &g_MicroBooterUpdateProvider,
        &g_CrcUpdateValidationProvider,
        &g_BlockStorageUpdateProvider,
        NULL,
    }
};

const IUpdatePackage* g_UpdatePackages     = s_UpdatePackages;
const INT32           g_UpdatePackageCount = ARRAYSIZE(s_UpdatePackages);


