using Acr.UserDialogs;
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
        /// <summary>
        /// How many times attempt to connect
        /// </summary>
        private const int ConnectionAttemptsCount = 5;

        private IBluetoothCommunicator _bluetoothCommunicator;
        private IUserNotifier _userNotifier;

        private OnFoxConnectorNewByteReadDelegate _onNewByteRead;
        private OnFoxConnectorConnectedDelegate _onConnected;
        private OnFoxConnectorDisconnectedDelegate _onDisconnected;
        private OnFoxConnectorFailedToConnectDelegate _onConnectionFailed;

        private bool _isDelegatesSetUp = false;

        public FoxConnector
        (
            IBluetoothCommunicator bluetoothCommunicator,
            IUserNotifier userNotifier
        )
        {
            _bluetoothCommunicator = bluetoothCommunicator;
            _userNotifier = userNotifier;

            _bluetoothCommunicator.SetupDelegates(OnConnect, OnDisconnect, OnNewByteRead);
        }

        public async Task ConnectAsync(PairedFoxDTO foxToConnect)
        {
            _ = foxToConnect ?? throw new ArgumentNullException(nameof(foxToConnect));

            if (!_isDelegatesSetUp)
            {
                throw new InvalidOperationException("Delegates aren't set up!");
            }

            var connectThread = new Thread(async () => await ConnectThreadRunAsync(foxToConnect));
            connectThread.Start();
        }

        public async Task DisconnectAsync()
        {
            var disconnectThread = new Thread(() => DisconnectThreadRun());
            disconnectThread.Start();
        }

        public void SetupDelegates
        (
            OnFoxConnectorNewByteReadDelegate onNewByteRead,
            OnFoxConnectorConnectedDelegate onConnected,
            OnFoxConnectorDisconnectedDelegate onDisconnected,
            OnFoxConnectorFailedToConnectDelegate onConnectionFailed
        )
        {
            _onNewByteRead = onNewByteRead ?? throw new ArgumentNullException(nameof(onNewByteRead));
            _onConnected = onConnected ?? throw new ArgumentNullException(nameof(onConnected));
            _onDisconnected = onDisconnected ?? throw new ArgumentNullException(nameof(onDisconnected));
            _onConnectionFailed = onConnectionFailed ?? throw new ArgumentNullException(nameof(onConnectionFailed));

            _isDelegatesSetUp = true;
        }

        /// <summary>
        /// Entry point of connection thread
        /// </summary>
        private async Task ConnectThreadRunAsync(PairedFoxDTO foxToConnect)
        {
            Exception lastException = null;

            using (IProgressDialog progress = UserDialogs.Instance.Progress("Connecting...", null, null, true, MaskType.Clear))
            {
                for (var attempt = 0; attempt < ConnectionAttemptsCount; attempt++)
                {
                    progress.PercentComplete = (int)Math.Round(100 * attempt / (double)ConnectionAttemptsCount);

                    try
                    {
                        await _bluetoothCommunicator.ConnectAsync(foxToConnect);

                        // We survived till here, so connection is estabilished
                        lastException = null;
                        break;
                    }
                    catch (Exception ex)
                    {
                        lastException = ex;
                    }
                }
            }

            if (lastException != null)
            {
                _onConnectionFailed(lastException);
                await _userNotifier.ShowErrorMessageAsync("Failed to connect!", $"Reason: { lastException.Message }");
            }
        }

        /// <summary>
        /// Entry point of disconnection thread
        /// </summary>
        private void DisconnectThreadRun()
        {
            _bluetoothCommunicator.Disconnect();
        }

        private void OnNewByteRead(byte data)
        {
            _onNewByteRead(data);
        }

        private void OnConnect(PairedFoxDTO connectedFox)
        {
            _onConnected(connectedFox);
        }

        private void OnDisconnect()
        {
            _onDisconnected();
        }
    }
}
