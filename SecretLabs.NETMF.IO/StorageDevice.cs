/* Copyright (C) 2010 Secret Labs LLC
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace SecretLabs.NETMF.IO
{
    public static class StorageDevice
    {
        private static string SdCardPath = null;

        public static void MountSD(string path, SPI.SPI_module spi, Cpu.Pin chipSelect)
        {
            MountSD(path, spi, chipSelect, Cpu.Pin.GPIO_NONE);
        }

        public static void MountSD(string path, SPI.SPI_module spi, Cpu.Pin chipSelect, Cpu.Pin cardDetect)
        {
            // TEMPORARY LOGIC: if we already have an SD card mounted, don't do it again.
            if (SdCardPath != null)
                throw new NotSupportedException();

            // if paths begins with "\" then remove it
            if (path.Length > 1 && path.Substring(0, 1) == @"\")
                path = path.Substring(1);
            // if path ends with "\" then remove it
            if (path.Length > 1 && path.Substring(path.Length - 1, 1) == @"\")
                path = path.Substring(0, path.Length - 1);

            SDSPI_Mount(path, (UInt32)spi, (UInt32)chipSelect, (UInt32)cardDetect);

            SdCardPath = path;
        }

        public static void Unmount(string path)
        {
            // TEMPORARY LOGIC: making sure that we're unmounting the same card...
            if (SdCardPath != path)
                throw new ArgumentException();

            SDSPI_Unmount();

            SdCardPath = null;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void SDSPI_Mount(string path, UInt32 spi, UInt32 chipSelectPort, UInt32 cardDetectPin);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void SDSPI_Unmount();
    }
}
