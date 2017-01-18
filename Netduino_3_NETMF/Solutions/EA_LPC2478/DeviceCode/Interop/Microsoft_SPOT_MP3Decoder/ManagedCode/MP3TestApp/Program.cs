using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using NXP.Interop;

namespace MP3TestApp
{
    public class Program
    {
        public static void Main()
        {
            byte[] inBuffer = Resources.GetBytes(Resources.BinaryResources.testfile);
            byte[] inDataPCM = new byte[1152 * 20];/*Resources.GetBytes(Resources.BinaryResources.test)*/ ;
            byte[][] inBufferPCM = new byte[inDataPCM.Length / 1152][];
            for (int i = 0; i < inBufferPCM.Length; i++)
            {
                inBufferPCM[i] = new byte[1152];
                Array.Copy(inDataPCM, i * 1152, inBufferPCM[i], 0, 1152);
            }


            MP3Decoder hMP3Decoder;
            DAC hDAC = new DAC();
            /*
            Debug.Print("PCM Output Test");
            hDAC.Initialize(8000);

            for (int i = 0; i < inBufferPCM.Length; i++)
            {
                try
                {
                    //Debug.Print("Frame "+i+":");
                    //for (int j = 0; j < 1152; j++)
                    //{
                    //    inBufferPCM[j] = inDataPCM[1152*i+j];
                    //}

                    hDAC.AddFrame(inBufferPCM[i], 576);
                    //Debug.Print("BUFlvl="+hDAC.GetBufferLevel());
                }
                catch (System.Exception)
                {
                    if (!hDAC.Enabled)
                    {
                        hDAC.On();
                    }
                    //i--;//don't skip this frame without adding it
                    //System.Threading.Thread.Sleep(72);//sleep one frame time (at 8kHz)
                }
            }

            System.DateTime outputBeginPCM = System.DateTime.Now;

            while (hDAC.GetBufferLevel() > 0)
            {
                System.Threading.Thread.Sleep(50);
                Debug.Print("Buffer fill level " + hDAC.GetBufferLevel());
            }
            System.TimeSpan outputRunningTimePCM = System.DateTime.Now.Subtract(outputBeginPCM);
            Debug.Print("PCM Output Over, took " + outputRunningTimePCM.Milliseconds + "ms");

            hDAC.Off();
            hDAC.Uninitialize();*/

            Debug.Print("MP3TestApp INIT...");

            /*GPIOButtonInputProvider inputProvider = new GPIOButtonInputProvider(null);

            Microsoft.SPOT.Presentation.Window a = new Window();
            a.AddHandler(Buttons.ButtonUpEvent, new ButtonEventHandler(OnButtonUp), false);
            */

            hMP3Decoder = new MP3Decoder(inBuffer);

            hDAC.Initialize(8000);


            Int16 maxFrames;

            hDAC.On();

            while (true)
            {
                //fill the buffer
                maxFrames = (short)hDAC.GetBufferFrameCapacity();

                try
                {
                    //Debug.Print("Decode & play");
                    hMP3Decoder.Decode(ref maxFrames, 5);

                    if (maxFrames > 0)  //no more frames to decode in inBuffer
                    {
                        Debug.Print("Decoding over");
                        break;
                    }
                                        
                    //Debug.Print("FRAMES LEFT: " + hDAC.GetFramesLeft());

                }
                catch (NXP.Interop.DecodingException)
                {
                    Debug.Print("Generic DecodingException, abort.");
                    break;
                }
                catch (System.Exception)
                {
                    Debug.Print("Generic exception, abort.");
                    break;
                }
            } //while

            Int16 sleeptime = 2 * 1000;
            Debug.Print("Wait: " + sleeptime);
            System.Threading.Thread.Sleep(sleeptime);
            hDAC.Uninitialize();

        }

        private static void OnButtonUp(object sender, ButtonEventArgs e)
        {
            // Print the button code to the Visual Studio output window.
            Debug.Print("Pressed Button " + e.Button.ToString());
            /*switch (e.Button)
            {
                case Microsoft.SPOT.Hardware.Button.VK_UP:
                    if (!hoggerThread.IsAlive)
                        hoggerThread.Start();
                    break;
                case Microsoft.SPOT.Hardware.Button.VK_SELECT:
                    MeasureDebugOutputTime();
                    break;
                case Microsoft.SPOT.Hardware.Button.VK_DOWN:
                    if (hoggerThread.IsAlive)
                        hoggerThread.Abort();
                    break;
            }*/
        }
           
    }

}

