using System;
using System.Collections.Generic;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Helpers;

namespace yiff_hl.Business.Implementations
{
    public class PacketsProcessor : IPacketsProcessor
    {
        /// <summary>
        /// Add this to payload to get full packet lenght. 1 - lenght byte + 4 - crc32 bytes
        /// </summary>
        private const int AddToPacketLenght = 5;

        private readonly IBluetoothCommunicator bluetoothCommunicator;

        public PacketsProcessor(IBluetoothCommunicator bluetoothCommunicator)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
        }

        public void SendPacket(IReadOnlyCollection<byte> payload)
        {
            if (!bluetoothCommunicator.IsConnected())
            {
                throw new InvalidOperationException("Fox is not connected");
            }

            var fullPacket = new List<byte>();

            // Length
            fullPacket.Add((byte)(payload.Count + AddToPacketLenght));

            // Payload
            fullPacket.AddRange(payload);

            // CRC32
            var crc = 0; // TODO: Implement me
            fullPacket.AddRange(BitsHelper.ConvertIntToBytes(crc));

            bluetoothCommunicator.SendMessage(fullPacket);
        }
    }
}
