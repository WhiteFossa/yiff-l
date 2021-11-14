using Android.App;
using Android.Bluetooth;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Java.Util;
using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    internal class BluetoothCommunicator : IBluetoothCommunicator
    {
        /// <summary>
        /// Read no more then this amount at once
        /// </summary>
        private const int ReadBufferSize = 1024;

        private IPairedFoxesEnumerator _pairedFoxesEnumerator;

        private OnBTCommunicatorConnectDelegate _onBTCommunicatorConnect;
        private OnBtCommunicatorDisconnectDelegate _onBTCommunicatorDisconnect;
        private OnBTCommunicatorNewByteReadDelegate _onBTCommunicatorNewByteRead;

        private BluetoothSocket _socket;
        private Thread _readerThread;

        public BluetoothCommunicator(IPairedFoxesEnumerator pairedFoxesEnumerator)
        {
            _pairedFoxesEnumerator = pairedFoxesEnumerator;
        }

        public async Task ConnectAsync(PairedFoxDTO fox)
        {
            if (_socket != null && _socket.IsConnected)
            {
                throw new InvalidOperationException("Already connected to fox!");
            }

            if (_onBTCommunicatorConnect == null)
            {
                throw new InvalidOperationException("OnBTCommunicatorConnect delegate is null!");
            }

            if (_onBTCommunicatorDisconnect == null)
            {
                throw new InvalidOperationException("OnBTCommunicationDisconnect delegate is null!");
            }

            if (_onBTCommunicatorNewByteRead == null)
            {
                throw new InvalidOperationException("OnBTCommunicatorNewByteRead delegate is null!");
            }

            var foxes = await _pairedFoxesEnumerator.EnumerateAsync();

            if (!foxes.Any(f => f.MAC.Equals(fox.MAC)))
            {
                throw new InvalidOperationException($"Fox { fox.DisplayName } is not found!");
            }

            // We are sure that adapted exists and enabled because of checks in _pairedFoxesEnumerator.EnumerateAsync()
            // Also we are sure that fox with given MAC exists too
            var device = BluetoothAdapter
                .DefaultAdapter
                .BondedDevices
                .First(d => d.Address.Equals(fox.MAC));

            try
            {
                // What does this magic UUID mean?
                _socket = device.CreateInsecureRfcommSocketToServiceRecord(UUID.FromString("00001101-0000-1000-8000-00805f9b34fb"));
                if (_socket == null)
                {
                    throw new InvalidOperationException("Unable to get bluetooth socket!");
                }

                _socket.Connect();
                if (!_socket.IsConnected)
                {
                    throw new InvalidOperationException("Unable to connect to fox!");
                }

                // Starting reader thread
                _readerThread = new Thread(new ThreadStart(ReaderThreadRun));
                _readerThread.Start();

                _onBTCommunicatorConnect();
            }
            catch(Exception)
            {
                if (_socket != null && _socket.IsConnected)
                {
                    _socket.Close();
                }

                throw;
            }
        }

        public void Disconnect()
        {
            if (_socket != null && _socket.IsConnected)
            {
                _socket.Close();
            }

            _onBTCommunicatorDisconnect();
        }

        public void SendMessage(IReadOnlyCollection<byte> message)
        {
            throw new NotImplementedException();
        }

        public void SetupDelegates(OnBTCommunicatorConnectDelegate onConnect, OnBtCommunicatorDisconnectDelegate onDisconnect, OnBTCommunicatorNewByteReadDelegate onNewByteRead)
        {
            _onBTCommunicatorConnect = onConnect ?? throw new ArgumentNullException(nameof(onConnect));
            _onBTCommunicatorDisconnect = onDisconnect ?? throw new ArgumentNullException(nameof(onDisconnect));
            _onBTCommunicatorNewByteRead = onNewByteRead ?? throw new ArgumentNullException(nameof(onNewByteRead));
        }

        /// <summary>
        /// Entry point for reader thread
        /// </summary>
        private void ReaderThreadRun()
        {
            var buffer = new byte[ReadBufferSize];
            while (true)
            {
                try
                {
                    var readSize = _socket.InputStream.Read(buffer, 0, ReadBufferSize);

                    for (var i = 0; i < readSize; i++)
                    {
                        _onBTCommunicatorNewByteRead(buffer[i]);
                    }
                }
                catch (Exception)
                {
                    // Exceptions occurs when socket is getting closed on disconnect
                    Disconnect();
                    return;
                }
            }
        }
    }
}