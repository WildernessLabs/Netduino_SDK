////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace ExtendedWeakReferencesSample
{
    /// <summary>
    /// This class demonstrates extended weak references, which are used to 
    /// store data in Flash RAM.
    /// </summary>
    public class MyExtendedWeakReferences
    {
        /// <summary>
        /// This static field refers to the extended weak reference object, in 
        // order to prevent it from being garbage-collected.
        /// </summary>
        private static ExtendedWeakReference s_numBootsExtendedWeakReference;

        /// <summary>
        /// This private class is used as a signature to uniquely identify data 
        /// we own.  It is not necessary to create a dedicated class for this 
        /// purpose.
        /// </summary>
        private static class TypeUniqueToOurApp { }

        /// <summary>
        /// This is the class that will be stored in Flash memory.  This class 
        /// must be serializable.
        /// </summary>
        [Serializable]
        private sealed class NumBoots
        {
            /// <summary>
            /// This method sets the number of boots.
            /// </summary>
            /// <param name="bootCount">The number of times the device has been 
            /// booted.</param>
            public NumBoots(Int32 bootCount)
            {
                BootCount = bootCount;
            }

            // Fields must be serializable.
            public Int32 BootCount;
        }

        /// <summary>
        /// The execution entry point.
        /// </summary>
        public static void Main()
        {
            // We are booting; try to retrieve the boot count.
            s_numBootsExtendedWeakReference =
            ExtendedWeakReference.RecoverOrCreate(
            typeof(TypeUniqueToOurApp),  // The unique type that identifies the data.
            0,                           // The ID of the specific data item.
            ExtendedWeakReference.c_SurvivePowerdown); // The CLR should try to persist data across a reboot.

            // Indicate how important this data is.  The CLR discards 
            // OkayToThrowAway items first, then NiceToHave items, then 
            // Important items, then Critical items, and finally System items.
            // In practice, System items are virtually never discarded.
            s_numBootsExtendedWeakReference.Priority =
                (Int32)ExtendedWeakReference.PriorityLevel.Important;

            // Try to get the persisted data, initializing it if it is not 
            // available.  The Target property of the extended weak reference 
            // must be cast to the actual type of the object.  The Target 
            // property is cast to prevent the object from being 
            // garbage-collected unexpectedly.
            NumBoots numBoots = (NumBoots)s_numBootsExtendedWeakReference.Target;

            if (numBoots == null)
            {
                // The object could not be found in flash memory, so create the 
                // object and initialize it.
                Debug.Print(
                    "The device was booted for the first time, or the boot counter was lost. Initializing the boot counter to 1.");
                numBoots = new NumBoots(1);
            }
            else
            {
                // The object was found in flash memory; increment the boot 
                // counter.
                numBoots = new NumBoots(numBoots.BootCount + 1);
                Debug.Print("Successfully read boot counter. This is boot #" +
                    numBoots.BootCount);
            }

            // Set the Target property of the extended weak reference to the 
            // boot count object, triggering persistence.
            s_numBootsExtendedWeakReference.Target = numBoots;

            // The CLR needs some time to asynchronously store the data in 
            // flash.  In this sample application, we use a two-second Sleep.  
            // A real application would simply continue without delay.  This app 
            // only needs to sleep because the application is about to end.

            // The system provides no guarantee about when the data will 
            // actually be stored in flash memory.  When the data is actually 
            // stored depends on the size of the object and the speed of the 
            // flash memory.

            System.Threading.Thread.Sleep(2000);
        }
    }
}
