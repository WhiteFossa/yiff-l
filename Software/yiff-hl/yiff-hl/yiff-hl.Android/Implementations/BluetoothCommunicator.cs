using Android.App;
using Android.Bluetooth;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Java.IO;
using Java.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Droid.Implementations
{
    public class BluetoothCommunicator : IBluetoothCommunicator
    {
        private string deviceName;

        private OnNewByteReadDelegate readDelegateInstance;

        private List<byte> messageToSend;

        private CancellationTokenSource cancellationToken { get; set; }

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

                // Endless loop start
                var reader = new InputStreamReader(socket.InputStream);
                var bufferedReader = new BufferedReader(reader);
                var readBuffer = new char[1024]; // TODO: Set me to correct size

                while (!cancellationToken.IsCancellationRequested)
                {
                    // Reading (non-blocking, because we need to send messages too)
                    if (bufferedReader.Ready())
                    {
                        var readSize = bufferedReader.Read(readBuffer);
                        for (var byteIndex = 0; byteIndex < readSize; byteIndex++)
                        {
                            readDelegateInstance((byte)readBuffer[byteIndex]);
                        }
                    }

                    // Sending
                    if (messageToSend != null)
                    {
                        socket.OutputStream.Write(messageToSend.ToArray(), 0, messageToSend.Count);

                        messageToSend = null;
                    }
                }
            }
            finally
            {
                if (socket != null)
                {
                    socket.Close();
                }
            }
        }

        public void Disconnect()
        {
            if (cancellationToken != null)
            {
                cancellationToken.Cancel();
            }
        }

        public void SendMessage(IReadOnlyCollection<byte> message)
        {
            messageToSend = message
                .ToList();
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