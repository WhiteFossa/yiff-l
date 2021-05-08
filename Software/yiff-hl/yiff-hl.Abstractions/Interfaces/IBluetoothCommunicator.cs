using System.Collections.Generic;

namespace yiff_hl.Abstractions.Interfaces
{
    public delegate void OnNewByteReadDelegate(byte readByte);

    /// <summary>
    /// Communicator with bluetooth device
    /// </summary>
    public interface IBluetoothCommunicator
    {
        /// <summary>
        /// Provide a name of device to what we will connect
        /// </summary>
        void SetDeviceName(string name);

        /// <summary>
        /// Set delegate, which will be called on each byte read
        /// </summary>
        void SetReadDelegate(OnNewByteReadDelegate readDelegate);

        /// <summary>
        /// Create connection (call it in separate thread)
        /// </summary>
        void Connect();

        void Disconnect();

        /// <summary>
        /// Send this bytes to device
        /// </summary>
        void SendMessage(IReadOnlyCollection<byte> message);

        /// <summary>
        /// Returns true if connected to fox
        /// </summary>
        bool IsConnected();
    }
}
