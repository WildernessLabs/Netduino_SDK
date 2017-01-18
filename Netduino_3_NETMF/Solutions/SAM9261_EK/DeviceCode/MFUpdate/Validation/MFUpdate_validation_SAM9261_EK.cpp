#include <tinyhal.h>
#include <com_decl.h>

static LPCSTR s_SelfSignedCert = 
"Bag Attributes\n\
    localKeyID: 01 00 00 00 \n\
subject=/CN=ZACHL-SBA1.redmond.corp.microsoft.com\n\
issuer=/CN=ZACHL-SBA1.redmond.corp.microsoft.com\n\
-----BEGIN CERTIFICATE-----\n\
MIIDRzCCAjOgAwIBAgIQSWR87GBg4IZDycI5/XzhGzAJBgUrDgMCHQUAMDAxLjAs\n\
BgNVBAMTJVpBQ0hMLVNCQTEucmVkbW9uZC5jb3JwLm1pY3Jvc29mdC5jb20wHhcN\n\
MTIwNzI0MjM0ODU0WhcNMzkxMjMxMjM1OTU5WjAwMS4wLAYDVQQDEyVaQUNITC1T\n\
QkExLnJlZG1vbmQuY29ycC5taWNyb3NvZnQuY29tMIIBIjANBgkqhkiG9w0BAQEF\n\
AAOCAQ8AMIIBCgKCAQEAnwsjI63dIkE0Pr4kZjeBRIEm0i5a+jnMQYm0lh1PW2c6\n\
N0DP/6a7hYBuGd1TLfa0H+edcuDw6jrV9teF5AsUAZm7S0g3/Y2bBYZaWDO7YShf\n\
UbAVhMfwHXRu+8hCLM6BJEYTManSuBWy57umd01p7D4DJ2HcfZOKLKb6yehEx0Ar\n\
WZzzBMMyg+gnhBDvk094ftETHf9KNwXy30kY0uhXvz3JFqzkIVDs4NitRZnl+ZA3\n\
GlhBkD2AgEm1/OuuS+1DvI7SYz3IKCYld5UJ9dGwinMWVs/zVi0jcmVqG6ZiFz7W\n\
RNlLLPBhCzYDtLde413PSqh5YSWFk59bnUIJ3g5NCQIDAQABo2UwYzBhBgNVHQEE\n\
WjBYgBCWZh5kttkW+biGytMvzQKWoTIwMDEuMCwGA1UEAxMlWkFDSEwtU0JBMS5y\n\
ZWRtb25kLmNvcnAubWljcm9zb2Z0LmNvbYIQSWR87GBg4IZDycI5/XzhGzAJBgUr\n\
DgMCHQUAA4IBAQBjInvF3F/79WJrf+hDd/oSSZT94X+mXgrnMYqYePULLnHt9lrb\n\
Zs5lM4yMreC/1cohUHB0VCw3zAKvKr7IMubW7mKuiRcg+UgLJVtXE/1tznkVuqvz\n\
ONSLF2RxqMxdLNgQ6OFrdXaCu43CK+fQoKVM/amZMye3kenVPN6pDfXznHW6Un1w\n\
tUuDIR4wDaYmAwyiPoAn+1D1g3RrBFLqc4scdltwfCf2FR31ze44sDsMEj8DKJE5\n\
YFA8UhWZiEZBXpIkq531UlXLeb69Pi7nUw5wfbVzzWOUzobhx9t55eWAmWnL3IuR\n\
RnQUqxNLNVZH9o+6m4PyRyWxD48J3o29ZvjP\n\
-----END CERTIFICATE-----\n\
Bag Attributes\n\
    localKeyID: 01 00 00 00 \n\
    friendlyName: 354955e4-03ee-4d94-bb10-bb4613b823ba\n\
    Microsoft CSP Name: Microsoft Strong Cryptographic Provider\n\
Key Attributes\n\
    X509v3 Key Usage: 10 \n\
-----BEGIN RSA PRIVATE KEY-----\n\
Proc-Type: 4,ENCRYPTED\n\
DEK-Info: DES-EDE3-CBC,1F57F64B73C799E2\n\
\n\
ezuXF8GJcI/Mr/RgEVzMgzn45xOdLb7qM2fPmfvtAJfREZqUKKT0NhE1Pt+vVpUn\n\
qCPGNzNsw4saM1Ti2uRe1z9mStfavGMUV0BEnJRs7sMj/DHVvgFp6qwJCHQO8zUA\n\
B9BDFXT3a2ggoCsmp/o0G6qXXZrzVtmg8u5S/RgG28Mm61Uq3OACdXsX0jI5Av+4\n\
EzvyZ3Ple/mto9HY1kyHhMUQDHO+5+UdzxWpBr6sz5p5Wdi3TaEHaYkF9qeG21j8\n\
8zT3EYFfVPBsRpXnnhzsElyXtrStuotfWG7UCiJenrjCqDmhghkXWeeAC0vWQ66r\n\
9CiA5fcmQJMAfGBoDpxh8HNf2JYIuQb0/6uLF6yrsI2jC/Cx77BD94Jkp1j9uz2A\n\
zBTeGiwth5X6dlzEDUrTxsrF63sYX64GAWMZxxXoaOKnlqUU50HaMaxLDwIMGvnw\n\
qP3Dy9rU990t5wCVtHHAHQogCUqbxffACyQFyF8nbmakiiY8aFbE+XHbNP7NHuPG\n\
PYET1ZnHWBvazz98dKR4PIoW5Bc4fCIecYNv8qyv8PlfwZcT5rbbf4NTDzbNmUyI\n\
qEyePJ3LauXP8nwAh4EUQY4GIK+r45UJvBBSQLVK60jHO2WELOicvBvj6fTuW2LO\n\
NqVoM95yXseDNvcTiKcIv29pQOVTRjZhMqoqUkeGBhBgP9FPjXkgEBSUHLSDRGUw\n\
xphuaoYMpR3ELbJIT7Y2F4CM56sr97Q4DByPNZ2GlXfNHubT3sQku3ghxuio0nFj\n\
GcXTkjiWuIowB7ZCT1dVXj8AbI+oBH/jAb6W3C/QoN7szNGJXFkWtjvUB8HjC8tg\n\
rAQOGPwPumGfiShZ3CtDXgAEOSNsZKFyJF8Bzx/NkRR+vuFnUEt7h6sYaWpHfbzD\n\
NP6L17mnqettt7qQO2IbJZI0ukHhEIWIodHXkjmmxBJKKbI5sXub8nZmgubPQZTl\n\
0fK0All5WyTXjyK4wYtBAAc8RsHTVOPrDJCB8VbTSIkYVFPXB+UkF7Eeyky/mSia\n\
6h91ILx610cO7yD1jQlRarLTmYz8KlzwjDUs3CdiSNCBkRNUNGmK71lbrQJySeym\n\
TWwctrVujGFr4AIaWfh7YREQcPl8YYP1/Npg/algo6stSpxKCi7ycSAaID2kcn5y\n\
4vIe/+ZhyMA87Gpd7vt2rpmsloYNnFQGg6uYr2yu1/2+3ieA3K9Tcrtshc2zidZt\n\
wD3qeT3BAbkUCUrEpOgW/YMCSLo1ecfCUMXwSOMqmlAMyXtePj6kO3rQ2XMaaf5b\n\
3IYcZ0Gbm0n8E8JH1M2o975wRiwGIzF2qjHjOwGdT3Cx5PuTA2IK29uxMypIN7UB\n\
y8E9nadevLny9J/EqcNNabEzqtdQ+vXcR5NEK8HXxbARkpp+Tv8O/+gjbf8+ABnh\n\
XJC12njfusz7kFzNZzxpA8oNfZE66PqJPnwfDEJnwY7ZZEmAcBl56cFMrwmJCAuY\n\
QqZV+D/fPLpp1zIpzf1dgzTpQHFvdPPFbmfo0mdSl8RZbE7VU7/M9jHd/OstsZqw\n\
VLm9Ai8ulAjAXHK/npR5KzK0KUAzaxdkYGc23Yif4W7xc5SzheyxAJBqerxZNnNW\n\
-----END RSA PRIVATE KEY-----\n\0";


static LPCSTR s_TargetHost = "ZACHL-SBA1.redmond.corp.microsoft.com";

static BOOL MFUpdate_SSL_GetCACert( UINT8** caCert, UINT32* certLen )
{
    *caCert   = (UINT8*)s_SelfSignedCert;
    *certLen = hal_strlen_s(s_SelfSignedCert);

    return TRUE;
}

static BOOL MFUpdate_SSL_GetTargetHost( LPCSTR* strTargetHost )
{
    *strTargetHost = s_TargetHost;
    return TRUE;
}

static BOOL MFUpdate_SSL_GetDeviceCert( UINT8** caCert, UINT32* certLen )
{
    *caCert   = (UINT8*)s_SelfSignedCert;
    *certLen = hal_strlen_s(s_SelfSignedCert);

    return TRUE;
}

IDebuggerPortSslConfig g_DebuggerPortSslConfig =
{
    MFUpdate_SSL_GetCACert,
    MFUpdate_SSL_GetTargetHost,
    MFUpdate_SSL_GetDeviceCert,
};

