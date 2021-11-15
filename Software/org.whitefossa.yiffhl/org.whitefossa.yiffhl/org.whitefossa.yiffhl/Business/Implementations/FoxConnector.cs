using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    /// <summary>
    /// Able to connect us to a fox
    /// </summary>
    public class FoxConnector : IFoxConnector
    {
        private IBluetoothCommunicator _bluetoothCommunicator;

        public FoxConnector
        (
            IBluetoothCommunicator bluetoothCommunicator
        )
        {
            _bluetoothCommunicator = bluetoothCommunicator;
        }

        public async Task ConnectAsync(PairedFoxDTO foxToConnect)
        {
            _ = foxToConnect ?? throw new ArgumentNullException(nameof(foxToConnect));

            var connectThread = new Thread(async () => await ConnectThreadRunAsync(foxToConnect));
            connectThread.Start();
        }

        public async Task DisconnectAsync()
        {
            var disconnectThread = new Thread(() => DisconnectThreadRun());
            disconnectThread.Start();
        }

        /// <summary>
        /// Entry point of connection thread
        /// </summary>
        private async Task ConnectThreadRunAsync(PairedFoxDTO foxToConnect)
        {
            await _bluetoothCommunicator.ConnectAsync(foxToConnect);
        }

        /// <summary>
        /// Entry point of disconnection thread
        /// </summary>
        private void DisconnectThreadRun()
        {
            _bluetoothCommunicator.Disconnect();
        }
    }
}
