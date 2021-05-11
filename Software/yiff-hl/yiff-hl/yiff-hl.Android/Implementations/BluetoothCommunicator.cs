using Android.Bluetooth;
using Java.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Droid.Implementations
{
    public class BluetoothCommunicator : IBluetoothCommunicator
    {
        /// <summary>
        /// Read no more then this amount
        /// </summary>
        private const int BufferSize = 1024;

        private string deviceName;

        private OnNewByteReadDelegate readDelegateInstance;

        private bool isConnected = false;

        private BluetoothSocket socket;

        private Thread readerThread;

        public void Connect()
        {
            if (deviceName == null)
            {
                throw new InvalidOperationException("Call SetDeviceName() first!");
            }

            if (readDelegateInstance == null)
            {
                throw new InvalidOperationException("Call SetReadDelegate() first!");
            }

            var adapter = BluetoothAdapter.DefaultAdapter;

            if (adapter == null)
            {
                throw new InvalidOperationException("No bluetooth adapters found.");
            }

            if (!adapter.IsEnabled)
            {
                throw new InvalidOperationException("Bluetooth adapter is disabled.");
            }

            var bluetoothDevice = adapter.BondedDevices
                .Where(d => d.Name.Equals(deviceName))
                .FirstOrDefault();

            if (bluetoothDevice == null)
            {
                throw new InvalidOperationException($"Device with name { deviceName } not found!");
            }

            try
            {
                socket = bluetoothDevice.CreateInsecureRfcommSocketToServiceRecord(UUID.FromString("00001101-0000-1000-8000-00805f9b34fb")); // What this magic UUID mean?

                if (socket == null)
                {
                    throw new InvalidOperationException("Can't get bluetooth socket!");
                }

                socket.Connect();

                if (!socket.IsConnected)
                {
                    throw new InvalidOperationException("Can't connect to remote device!");
                }

                isConnected = true;

                // Starting reader thread
                readerThread = new Thread(new ThreadStart(ReaderThreadRun));
                readerThread.Start();
            }
            catch (Exception)
            {
                if (socket != null)
                {
                    socket.Close();
                }

                throw;
            }
        }

        public void Disconnect()
        {
            if (socket != null)
            {
                socket.Close();
            }

            isConnected = false;
        }

        public bool IsConnected()
        {
            return isConnected;
        }

        public void SendMessage(IReadOnlyCollection<byte> message)
        {
            socket.OutputStream.Write(message.ToArray(), 0, message.Count);
        }

        public void SetDeviceName(string name)
        {
            _ = name ?? throw new ArgumentNullException(nameof(name));
            deviceName = name;
        }

        public void SetReadDelegate(OnNewByteReadDelegate readDelegate)
        {
            _ = readDelegate ?? throw new ArgumentNullException(nameof(readDelegate));
            readDelegateInstance = readDelegate;
        }

        /// <summary>
        /// Entry point for reader thread
        /// </summary>
        private void ReaderThreadRun()
        {
            var buffer = new byte[BufferSize];
            while(true)
            {
                try
                {
                    var readSize = socket.InputStream.Read(buffer, 0, BufferSize);

                    for (var i = 0; i < readSize; i++)
                    {
                        readDelegateInstance(buffer[i]);
                    }
                }
                catch (Exception)
                {
                    // Exceptions occurs when socket is getting closed on disconnect
                    return;
                }
            }
        }
    }
}