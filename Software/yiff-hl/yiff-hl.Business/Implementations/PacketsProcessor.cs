using System;
using System.Collections.Generic;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Helpers;

namespace yiff_hl.Business.Implementations
{
    public class PacketsProcessor : IPacketsProcessor
    {
        /// <summary>
        /// Add this to payload to get full packet lenght. 1: lenght byte + 4: crc32 bytes
        /// </summary>
        private const int AddToPacketLenght = 5;

        private const int MinPayloadLength = 1;
        private const int MaxPayloadLength = 59;

        private enum ReceiverState
        {
            WaitingForFirstByte,
            ReceivingNextBytes
        }

        private List<byte> receiverBuffer = new List<byte>();

        private ReceiverState receiverState = ReceiverState.WaitingForFirstByte;

        private int expectedPacketLength;

        private readonly IBluetoothCommunicator bluetoothCommunicator;

        private OnNewPacketReceivedDelegate onNewPacketReceivedDelegate;

        public PacketsProcessor(IBluetoothCommunicator bluetoothCommunicator)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
        }

        public void NewByteReceived(byte data)
        {
            switch(receiverState)
            {
                case ReceiverState.WaitingForFirstByte:

                    // First byte is always lenght
                    expectedPacketLength = data;
                    var payloadLength = expectedPacketLength - AddToPacketLenght;

                    if (payloadLength < MinPayloadLength || payloadLength > MaxPayloadLength)
                    {
                        // Wrong packet size, discarding
                        return;
                    }

                    receiverBuffer.Clear();
                    receiverBuffer.Add(data);
                    receiverState = ReceiverState.ReceivingNextBytes;

                    break;

                case ReceiverState.ReceivingNextBytes:

                    receiverBuffer.Add(data);

                    if (receiverBuffer.Count == expectedPacketLength)
                    {
                        // Packet completed
                        receiverState = ReceiverState.WaitingForFirstByte;
                        NewRawPacketReceived(receiverBuffer.AsReadOnly());
                    }

                    break;

                default:
                    throw new InvalidOperationException("Invalid receiver state.");
            }
        }

        /// <summary>
        /// Called when new raw packet received
        /// </summary>
        public void NewRawPacketReceived(IReadOnlyCollection<byte> packet)
        {
            var packetAsList = new List<byte>(packet);

            // Checking CRC
            var crcBuffer = packetAsList
                .GetRange(packet.Count - 4, 4);

            var expectedCRC = BitsHelper.ConvertBytesToUint32(crcBuffer.ToArray());
            var calculatedCRC = CRCGenerator.CalculateSTMCRC32(packetAsList
                .GetRange(0, packetAsList.Count - 4));

            if (expectedCRC != calculatedCRC)
            {
                return;
            }

            if (onNewPacketReceivedDelegate == null)
            {
                throw new InvalidOperationException("Call SetNewPacketReceived() first.");
            }

            var payloadLength = packetAsList[0] - AddToPacketLenght;

            var payload = packetAsList
                .GetRange(1, payloadLength)
                .AsReadOnly();

            onNewPacketReceivedDelegate(payload);
        }

        public void SendPacket(IReadOnlyCollection<byte> payload)
        {
            if (!bluetoothCommunicator.IsConnected())
            {
                throw new InvalidOperationException("Fox is not connected");
            }

            if (payload.Count < MinPayloadLength || payload.Count > MaxPayloadLength)
            {
                throw new ArgumentException("Wrong payload size", nameof(payload));
            }

            var fullPacket = new List<byte>();

            // Length
            fullPacket.Add((byte)(payload.Count + AddToPacketLenght));

            // Payload
            fullPacket.AddRange(payload);

            // CRC32
            var crc = CRCGenerator.CalculateSTMCRC32(fullPacket);
            fullPacket.AddRange(BitsHelper.ConvertUInt32ToBytes(crc));

            bluetoothCommunicator.SendMessage(fullPacket);
        }

        public void SetNewPacketReceived(OnNewPacketReceivedDelegate onNewPacketReceivedDelegate)
        {
            _ = onNewPacketReceivedDelegate ?? throw new ArgumentNullException(nameof(onNewPacketReceivedDelegate));
            this.onNewPacketReceivedDelegate = onNewPacketReceivedDelegate;
        }
    }
}
