using System;
using Microsoft.SPOT;

internal static class CRC8
{
    private static byte[] crcTab;

    public static byte Compute8(byte[] data, int index = 0, int count = -1)
    {
        if (data == null)
        {
            throw new ArgumentNullException();
        }
        if (count == -1)
        {
            count = data.Length - index;
        }
        if (count < 0)
        {
            throw new ArgumentOutOfRangeException();
        }
        if ((data.Length - index) < count)
        {
            throw new ArgumentException();
        }

        if (crcTab == null)
        {
            crcTab = GenerateTable(0x07); // CRC-8-CCITT polynomial
        }

        byte crc = 0;
        for (int endIndex = index + count; index < endIndex; index++)
        {
            crc = crcTab[crc ^ data[index]];
        }
        return crc;
    }

    public static byte[] GenerateTable(byte polynomial)
    {
        var tab = new byte[256];
        for (int i = 0; i < 256; i++)
        {
            int curr = i;
            for (int j = 0; j < 8; j++)
            {
                if ((curr & 0x80) != 0)
                {
                    curr = (curr << 1) ^ polynomial;
                }
                else
                {
                    curr <<= 1;
                }
            }
            tab[i] = (byte)curr;
        }
        return tab;
    }
}