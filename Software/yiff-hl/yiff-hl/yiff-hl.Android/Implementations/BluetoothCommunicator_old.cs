using Android.Bluetooth;
using Java.IO;
using Java.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Droid.Implementations
{
    public class BluetoothCommunicator_old : IBluetoothCommunicator
    {
        /// <summary>
        /// Read no more then this amount
        /// </summary>
        private const int BufferSize = 1024;

        private string deviceName;

        private OnNewByteReadDelegate readDelegateInstance;

        private List<byte> messageToSend;

        private CancellationTokenSource cancellationToken;

        private Object locker = new Object();

        private bool isConnected = false;

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

            BluetoothSocket socket = null;

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

                // Endless loop start
                var reader = new InputStreamReader(socket.InputStream);
                var bufferedReader = new BufferedReader(reader);
                var readBuffer = new char[BufferSize];

                cancellationToken = new CancellationTokenSource();

                while (!cancellationToken.IsCancellationRequested)
                {
                    // Reading (non-blocking, because we need to send messages too)
                    if (bufferedReader.Ready())
                    {
                        lock (locker)
                        {
                            var readSize = bufferedReader.Read(readBuffer, 0, BufferSize);
                            for (var byteIndex = 0; byteIndex < readSize; byteIndex++)
                            {
                                readDelegateInstance((byte)readBuffer[byteIndex]);
                            }
                        }
                    }

                    // Sending
                    lock (locker)
                    {
                        if (messageToSend != null)
                        {
                            socket.OutputStream.Write(messageToSend.ToArray(), 0, messageToSend.Count);

                            messageToSend = null;
                        }
                    }
                }
            }
            finally
            {
                if (socket != null)
                {
                    socket.Close();
                }

                isConnected = false;
            }
        }

        public void Disconnect()
        {
            if (cancellationToken != null)
            {
                cancellationToken.Cancel();
            }
        }

        public bool IsConnected()
        {
            return isConnected;
        }

        public void SendMessage(IReadOnlyCollection<byte> message)
        {
            lock (locker)
            {
                messageToSend = message
                    .ToList();
            }
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
    }
}