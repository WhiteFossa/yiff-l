using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Models;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Delegate, called when new byte comes from fox
    /// </summary>
    public delegate void OnFoxConnectorNewByteReadDelegate(byte readByte);

    /// <summary>
    /// Delegate, called when connector successfully connects to fox
    /// </summary>
    public delegate void OnFoxConnectorConnectedDelegate(PairedFoxDTO connectedFox);

    /// <summary>
    /// Delegate, called when connection to fox is lost
    /// </summary>
    public delegate void OnFoxConnectorDisconnectedDelegate();

    /// <summary>
    /// Delegate, called connector uable to connect to fox
    /// </summary>
    public delegate void OnFoxConnectorFailedToConnectDelegate(Exception exception);

    /// <summary>
    /// Interface to connect to fox
    /// </summary>
    public interface IFoxConnector
    {
        void SetupDelegates
        (
            OnFoxConnectorNewByteReadDelegate onNewByteRead,
            OnFoxConnectorConnectedDelegate onConnected,
            OnFoxConnectorDisconnectedDelegate onDisconnected,
            OnFoxConnectorFailedToConnectDelegate onConnectionFailed
        );

        /// <summary>
        /// Connect to fox
        /// </summary>
        Task ConnectAsync(PairedFoxDTO foxToConnect);

        /// <summary>
        /// Disconnect from a fox
        /// </summary>
        Task DisconnectAsync();
    }
}
