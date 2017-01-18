#include <tinyhal.h>
#include <mfupdate_decl.h>
#include <Drivers\MFUpdate\MicroBooterUpdate.h>


static const IUpdatePackage s_UpdatePackages[] =
{
    {
        "NetMF",
        &g_MicroBooterUpdateProvider,
        &g_X509UpdateValidationProvider,
        &g_FSUpdateStorageProvider,
        NULL,
    }, 
};

const IUpdatePackage* g_UpdatePackages     = s_UpdatePackages;
const INT32           g_UpdatePackageCount = ARRAYSIZE(s_UpdatePackages);


