using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GoBus
{
    static class GoBusManager
    {
        static private object _shieldBase = null;

        static bool RegisterShieldBase(object shieldBase)
        {
            if (_shieldBase == null)
            {
                _shieldBase = shieldBase;
                return true;
            }
            else
            {
                return false;
            }
        }

        static void DeregisterShieldBase()
        {
            _shieldBase = null;
        }

        static object GetShieldBase()
        {
            return _shieldBase;
        }
    }
}
