using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Delegate, called when new byte comes from fox
    /// </summary>
    public delegate void OnBTCommunicatorNewByteReadDelegate(byte readByte);

    /// <summary>
    /// Delegate, called when communicator successfully connects to fox
    /// </summary>
    public delegate void OnBTCommunicatorConnectDelegate();

    /// <summary>
    /// Delegate, called when connection to fox is lost
    /// </summary>
    public delegate void OnBtCommunicatorDisconnectDelegate();

    /// <summary>
    /// Low-level bluetooth communicator
    /// </summary>
    public interface IBluetoothCommunicator
    {
        /// <summary>
        /// Setups delegates. Call this before attempt to connect
        /// </summary>
        void SetupDelegates
        (
            OnBTCommunicatorConnectDelegate onConnect,
            OnBtCommunicatorDisconnectDelegate onDisconnect,
            OnBTCommunicatorNewByteReadDelegate onNewByteRead
        );

        /// <summary>
        /// Connect to a paired fox
        /// </summary>
        Task ConnectAsync(PairedFoxDTO fox);

        /// <summary>
        /// Disconnect from fox
        /// </summary>
        void Disconnect();

        /// <summary>
        /// Send a message (raw) to a fox
        /// </summary>
        void SendMessage(IReadOnlyCollection<byte> message);
    }
}
