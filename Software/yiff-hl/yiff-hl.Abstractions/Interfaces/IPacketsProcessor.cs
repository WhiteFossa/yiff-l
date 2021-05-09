using System.Collections.Generic;

namespace yiff_hl.Abstractions.Interfaces
{
    /// <summary>
    /// Delegate, which called when new correct packet received
    /// </summary>
    public delegate void OnNewPacketReceivedDelegate(IReadOnlyCollection<byte> payload);

    /// <summary>
    /// Interface to work with packets
    /// </summary>
    public interface IPacketsProcessor
    {
        /// <summary>
        /// Send packet with given payload to fox
        /// </summary>
        void SendPacket(IReadOnlyCollection<byte> payload);

        /// <summary>
        /// Call this when new byte arrive
        /// </summary>
        /// <param name="data"></param>
        void NewByteReceived(byte data);

        /// <summary>
        /// Call this to set new packet processing method.
        /// </summary>
        void SetNewPacketReceived(OnNewPacketReceivedDelegate onNewPacketReceivedDelegate);
    }
}
