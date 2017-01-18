////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

/// <summary>
/// Demonstrates the use of the .NET Micro Framework's WeakDelegate class.  A 
/// WeakDelegate allows the target of an event to have its memory reclaimed if 
/// garbage collection occurs.
/// </summary>

namespace WeakDelegatesSample
{
    public static class MyWeakDelegates
    {
        // Stores a reference to an object to prevent the object from being 
        // garbage-collected.
        private static SomeType s_someType;

        /// <summary>
        /// Execution entry point.
        /// </summary>
        public static void Main()
        {
            // Create an instance of a type that offers an event.
            TypeWithWeakDelegateEvent twwde = new TypeWithWeakDelegateEvent();

            // Create an instance of a type that will register with the event.
            s_someType = new SomeType();

            // Register this object's SomeEventGotRaised method with our the other 
            // object's event.
            twwde.SomeEvent += s_someType.SomeEventGotRaised;

            // Raising the event calls through the delegate.
            twwde.RaiseSomeEvent("Before GC; this appears");

            // As long as a strong reference to the SomeType object exists, it will 
            // not be collected, and every call to RaiseSomeEvent will call the 
            // SomeType object's SomeEventGotRaised method.

            // However, if no strong reference to the SomeType object exists and 
            // garbage collection occurs, future calls to RaiseSomeEvent will NOT 
            // call the SomeType object's SomeEventGotRaised method, because the 
            // object no longer exists in memory.  No exception is raised, because 
            // that is not an error condition.

            // To demonstrate this, set s_someType to null and force garbage 
            // collection to occur.
            s_someType = null;
            Debug.GC(true);

            // Now the SomeType object no longer exists. Raising an event will not 
            // produce output.
            twwde.RaiseSomeEvent("After GC; this doesn't appear");

            Debug.Print("Program ending");
        }
    }

    /// <summary>
    /// Defines a type that has a weak delegate event.
    /// </summary>
    internal sealed class TypeWithWeakDelegateEvent
    {
        /// <summary>
        /// Prototype of SomeEvent's callback methods.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="s"></param>
        public delegate void SomeEventHandler(Object sender, String s);

        // Reference to the linked list of (weak) delegates.
        private SomeEventHandler m_someEvent;

        /// <summary>
        /// Raises an event.
        /// </summary>
        /// <param name="s"></param>
        public void RaiseSomeEvent(String s)
        {
            // Save the field in a temporary variable, to avoid a possible race 
            // condition.
            SomeEventHandler t = m_someEvent;

            // If at least one delegate is registered, invoke all the delegates.
            if (t != null) t(this, s);
        }

        /// <summary>
        /// This event implements an event using WeakDelegate.  The objects that 
        /// have registered to receive this event can go away at any time.
        /// </summary>
        public event SomeEventHandler SomeEvent
        {
            [MethodImpl(MethodImplOptions.Synchronized)]
            add
            {
                // Combine() turns the delegate referred to by value into a weak 
                // delegate.
                m_someEvent =
                    (SomeEventHandler)WeakDelegate.Combine(m_someEvent, value);
            }

            [MethodImpl(MethodImplOptions.Synchronized)]
            remove
            {
                // Delete the delegate referred to by value from the delegate chain.
                m_someEvent =
                    (SomeEventHandler)WeakDelegate.Remove(m_someEvent, value);
            }
        }
    }

    /// <summary>
    /// Shows that an event was raised.
    /// </summary>
    internal sealed class SomeType
    {
        /// <summary>
        /// Called when the SomeEvent event is raised, as long as the object still 
        /// exists.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="s"></param>
        public void SomeEventGotRaised(Object sender, String s)
        {
            Debug.Print("SomeEventGotRaised: " + s);
        }
    }
}
