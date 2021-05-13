using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
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

        private OnResponseDelegate onSetDateAndTimeResponse = null;

        private enum ReceiverState
        {
            WaitingForFirstByte,
            ReceivingNextBytes
        }

        private List<byte> receiverBuffer = new List<byte>();

        private ReceiverState receiverState = ReceiverState.WaitingForFirstByte;

        private int expectedPacketLength;

        private readonly IBluetoothCommunicator bluetoothCommunicator;

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

            var payloadLength = packetAsList[0] - AddToPacketLenght;

            var payload = packetAsList
                .GetRange(1, payloadLength)
                .AsReadOnly();

            OnPacketReceived(payload);
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

        public void OnPacketReceived(IReadOnlyCollection<byte> payload)
        {
            // First byte is packet type
            var payloadType = (PayloadType)payload.ElementAt(0);

            var payloadWithoutType = payload
                .ToList()
                .GetRange(1, payload.Count - 1)
                .AsReadOnly();

            switch(payloadType)
            {
                case PayloadType.CommandToFox:
                    return; // We can't receive command from fox

                case PayloadType.ResponseToCommand:
                    OnNewResponseToCommand(payloadWithoutType);
                    break;

                case PayloadType.EventFromFox:
                    OnNewEventFromFox(payloadWithoutType);
                    break;
            }
        }

        private void OnNewResponseToCommand(IReadOnlyCollection<byte> payload)
        {
            var responseTo = (CommandType)payload.ElementAt(0);

            var responsePayload = payload
                .ToList()
                .GetRange(1, payload.Count - 1)
                .AsReadOnly();

            switch(responseTo)
            {
                // Set date and time response
                case CommandType.SetDateAndTime:

                    if (onSetDateAndTimeResponse == null)
                    {
                        throw new InvalidOperationException("Call SetOnSetDateAndTimeResponse() first!");
                    }

                    onSetDateAndTimeResponse(responsePayload);
                    return;

                default:
                    return; // We get some junk
            }
        }

        private void OnNewEventFromFox(IReadOnlyCollection<byte> payload)
        {

        }

        public void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload)
        {
            var resultPayload = new List<byte>();
            resultPayload.Add((byte)PayloadType.CommandToFox);
            resultPayload.Add((byte)command);
            resultPayload.AddRange(commandPayload);

            SendPacket(resultPayload);
        }

        public void SetOnSetDateAndTimeResponse(OnResponseDelegate onSetDateAndTimeResponse)
        {
            this.onSetDateAndTimeResponse = onSetDateAndTimeResponse ?? throw new ArgumentNullException(nameof(onSetDateAndTimeResponse));
        }
    }
}
