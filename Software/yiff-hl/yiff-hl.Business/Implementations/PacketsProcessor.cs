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

        private OnResponseDelegate onSetDateAndTimeResponse;
        private OnResponseDelegate onSetFoxNameResponse;
        private OnResponseDelegate onGetFoxNameResponse;
        private OnResponseDelegate onGetProfilesCountResponse;
        private OnResponseDelegate onGetProfileNameResponse;
        private OnResponseDelegate onAddNewProfileResponse;
        private OnResponseDelegate onGetCurrentProfileIdResponse;
        private OnResponseDelegate onSwitchToProfileResponse;
        private OnResponseDelegate onSetProfileNameResponse;
        private OnResponseDelegate onGetFrequencyResponse;
        private OnResponseDelegate onSetFrequencyResponse;
        private OnResponseDelegate onGetCodeResponse;
        private OnResponseDelegate onSetCodeResponse;
        private OnResponseDelegate onGetSpeedResponse;
        private OnResponseDelegate onSetSpeedResponse;
        private OnResponseDelegate onGetCycleResponse;
        private OnResponseDelegate onSetCycleResponse;
        private OnResponseDelegate onGetEndingToneDurationResponse;
        private OnResponseDelegate onSetEndingToneDurationResponse;
        private OnResponseDelegate onGetBeginAndEndTimesResponse;
        private OnResponseDelegate onIsFoxArmedResponse;
        private OnResponseDelegate onArmFoxResponse;
        private OnResponseDelegate onDisarmFoxResponse;
        private OnResponseDelegate onSetBeginAndEndTimesResponse;
        private OnResponseDelegate onGetFoxPowerResponse;
        private OnResponseDelegate onSetFoxPowerResponse;
        private OnResponseDelegate onGetBatteryLevelResponse;
        private OnResponseDelegate onGetUAntADCResponse;

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
                    CheckOnResponseDelegate(onSetDateAndTimeResponse);

                    onSetDateAndTimeResponse(responsePayload);
                    return;

                // Set fox name response
                case CommandType.SetFoxName:
                    CheckOnResponseDelegate(onSetFoxNameResponse);

                    onSetFoxNameResponse(responsePayload);
                    return;

                // Get fox name response
                case CommandType.GetFoxName:
                    CheckOnResponseDelegate(onGetFoxNameResponse);

                    onGetFoxNameResponse(responsePayload);
                    return;

                // Get profiles count
                case CommandType.GetProfilesCount:
                    CheckOnResponseDelegate(onGetProfilesCountResponse);

                    onGetProfilesCountResponse(responsePayload);
                    return;

                // Get profile name
                case CommandType.GetProfileName:
                    CheckOnResponseDelegate(onGetProfileNameResponse);

                    onGetProfileNameResponse(responsePayload);
                    return;

                // Add new profile
                case CommandType.AddNewProfile:
                    CheckOnResponseDelegate(onAddNewProfileResponse);

                    onAddNewProfileResponse(responsePayload);
                    return;

                // Get current profile ID
                case CommandType.GetCurrentProfileId:
                    CheckOnResponseDelegate(onGetCurrentProfileIdResponse);

                    onGetCurrentProfileIdResponse(responsePayload);
                    return;

                // Switch to profile
                case CommandType.SwitchToProfile:
                    CheckOnResponseDelegate(onSwitchToProfileResponse);

                    onSwitchToProfileResponse(responsePayload);
                    return;

                // Set profile name
                case CommandType.SetProfileName:
                    CheckOnResponseDelegate(onSetProfileNameResponse);

                    onSetProfileNameResponse(responsePayload);
                    return;

                // Get frequency
                case CommandType.GetFrequency:
                    CheckOnResponseDelegate(onGetFrequencyResponse);

                    onGetFrequencyResponse(responsePayload);
                    return;

                // Set frequency
                case CommandType.SetFrequency:
                    CheckOnResponseDelegate(onSetFrequencyResponse);

                    onSetFrequencyResponse(responsePayload);
                    return;

                // Get code
                case CommandType.GetCode:
                    CheckOnResponseDelegate(onGetCodeResponse);

                    onGetCodeResponse(responsePayload);
                    break;

                // Set code
                case CommandType.SetCode:
                    CheckOnResponseDelegate(onSetCodeResponse);

                    onSetCodeResponse(responsePayload);
                    break;

                // Get speed
                case CommandType.GetSpeed:
                    CheckOnResponseDelegate(onGetSpeedResponse);

                    onGetSpeedResponse(responsePayload);
                    break;

                // Set speed
                case CommandType.SetSpeed:
                    CheckOnResponseDelegate(onSetSpeedResponse);

                    onSetSpeedResponse(responsePayload);
                    break;

                // Get cycle parameters
                case CommandType.GetCycle:
                    CheckOnResponseDelegate(onGetCycleResponse);

                    onGetCycleResponse(responsePayload);
                    break;

                // Set cycle parameters
                case CommandType.SetCycle:
                    CheckOnResponseDelegate(onSetCycleResponse);

                    onSetCycleResponse(responsePayload);
                    break;

                // Get ending tone duration
                case CommandType.GetEndingToneDuration:
                    CheckOnResponseDelegate(onGetEndingToneDurationResponse);

                    onGetEndingToneDurationResponse(responsePayload);
                    break;

                // Set ending tone duration
                case CommandType.SetEndingToneDuration:
                    CheckOnResponseDelegate(onSetEndingToneDurationResponse);

                    onSetEndingToneDurationResponse(responsePayload);
                    break;

                // Get begin and end times
                case CommandType.GetBeginAndEndTimes:
                    CheckOnResponseDelegate(onGetBeginAndEndTimesResponse);

                    onGetBeginAndEndTimesResponse(responsePayload);
                    break;

                // Is fox armed?
                case CommandType.IsFoxArmed:
                    CheckOnResponseDelegate(onIsFoxArmedResponse);

                    onIsFoxArmedResponse(responsePayload);
                    break;

                // Arm fox
                case CommandType.ArmFox:
                    CheckOnResponseDelegate(onArmFoxResponse);

                    onArmFoxResponse(responsePayload);
                    break;

                // Disarm fox
                case CommandType.DisarmFox:
                    CheckOnResponseDelegate(onDisarmFoxResponse);

                    onDisarmFoxResponse(responsePayload);
                    break;

                // Set begin and end times
                case CommandType.SetBeginAndEndTimes:
                    CheckOnResponseDelegate(onSetBeginAndEndTimesResponse);

                    onSetBeginAndEndTimesResponse(responsePayload);
                    break;

                // Get fox power
                case CommandType.GetFoxPower:
                    CheckOnResponseDelegate(onGetFoxPowerResponse);

                    onGetFoxPowerResponse(responsePayload);
                    break;

                // Set fox power
                case CommandType.SetFoxPower:
                    CheckOnResponseDelegate(onSetFoxPowerResponse);

                    onSetFoxPowerResponse(responsePayload);
                    break;

                // Get battery level
                case CommandType.GetBatteryLevel:
                    CheckOnResponseDelegate(onGetBatteryLevelResponse);

                    onGetBatteryLevelResponse(responsePayload);
                    break;

                // Get 3.5MHz antenna voltage (ADC)
                case CommandType.GetUAntADC:
                    CheckOnResponseDelegate(onGetUAntADCResponse);

                    onGetUAntADCResponse(responsePayload);
                    break;

                default:
                    return; // We've got some junk
            }
        }

        private void CheckOnResponseDelegate(OnResponseDelegate del)
        {
            if (del == null)
            {
                throw new InvalidOperationException();
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

        public void SetOnSetFoxNameResponse(OnResponseDelegate onSetFoxNameResponse)
        {
            this.onSetFoxNameResponse = onSetFoxNameResponse ?? throw new ArgumentNullException(nameof(onSetFoxNameResponse));
        }

        public void SetOnGetFoxNameResponse(OnResponseDelegate onGetFoxNameResponse)
        {
            this.onGetFoxNameResponse = onGetFoxNameResponse ?? throw new ArgumentNullException(nameof(onGetFoxNameResponse));
        }

        public void SetOnGetProfilesCountResponse(OnResponseDelegate onGetProfilesCountResponse)
        {
            this.onGetProfilesCountResponse = onGetProfilesCountResponse ?? throw new ArgumentNullException(nameof(onGetProfilesCountResponse));
        }

        public void SetOnGetProfileNameResponse(OnResponseDelegate onGetProfileNameResponse)
        {
            this.onGetProfileNameResponse = onGetProfileNameResponse ?? throw new ArgumentNullException(nameof(onGetProfileNameResponse));
        }

        public void SetOnAddNewProfileResponse(OnResponseDelegate onAddNewProfileResponse)
        {
            this.onAddNewProfileResponse = onAddNewProfileResponse ?? throw new ArgumentNullException(nameof(onAddNewProfileResponse));
        }

        public void SetOnGetCurrentProfileIdResponse(OnResponseDelegate onGetCurrentProfileIdResponse)
        {
            this.onGetCurrentProfileIdResponse = onGetCurrentProfileIdResponse ?? throw new ArgumentNullException(nameof(onGetCurrentProfileIdResponse));
        }

        public void SetOnSwitchToProfileResponse(OnResponseDelegate onSwitchToProfileResponse)
        {
            this.onSwitchToProfileResponse = onSwitchToProfileResponse ?? throw new ArgumentNullException(nameof(onSwitchToProfileResponse));
        }

        public void SetOnSetProfileNameResponse(OnResponseDelegate onSetProfileNameResponse)
        {
            this.onSetProfileNameResponse = onSetProfileNameResponse ?? throw new ArgumentNullException(nameof(onSetProfileNameResponse));
        }

        public void SetOnGetFrequencyResponse(OnResponseDelegate onGetFrequencyResponse)
        {
            this.onGetFrequencyResponse = onGetFrequencyResponse ?? throw new ArgumentNullException(nameof(onGetFrequencyResponse));
        }

        public void SetOnSetFrequencyResponse(OnResponseDelegate onSetFrequencyResponse)
        {
            this.onSetFrequencyResponse = onSetFrequencyResponse ?? throw new ArgumentNullException(nameof(onSetFrequencyResponse));
        }

        public void SetOnGetCodeResponse(OnResponseDelegate onGetCodeResponse)
        {
            this.onGetCodeResponse = onGetCodeResponse ?? throw new ArgumentNullException(nameof(onGetCodeResponse));
        }

        public void SetOnSetCodeResponse(OnResponseDelegate onSetCodeResponse)
        {
            this.onSetCodeResponse = onSetCodeResponse ?? throw new ArgumentNullException(nameof(onSetCodeResponse));
        }

        public void SetOnGetSpeedResponse(OnResponseDelegate onGetSpeedResponse)
        {
            this.onGetSpeedResponse = onGetSpeedResponse ?? throw new ArgumentNullException(nameof(onGetSpeedResponse));
        }

        public void SetOnSetSpeedResponse(OnResponseDelegate onSetSpeedResponse)
        {
            this.onSetSpeedResponse = onSetSpeedResponse ?? throw new ArgumentNullException(nameof(onSetSpeedResponse));
        }

        public void SetOnGetCycleResponse(OnResponseDelegate onGetCycleResponse)
        {
            this.onGetCycleResponse = onGetCycleResponse ?? throw new ArgumentNullException(nameof(onGetCycleResponse));
        }

        public void SetOnSetCycleResponse(OnResponseDelegate onSetCycleResponse)
        {
            this.onSetCycleResponse = onSetCycleResponse ?? throw new ArgumentNullException(nameof(onSetCycleResponse));
        }

        public void SetOnGetEndingToneDurationResponse(OnResponseDelegate onGetEndingToneDurationResponse)
        {
            this.onGetEndingToneDurationResponse = onGetEndingToneDurationResponse ?? throw new ArgumentNullException(nameof(onGetEndingToneDurationResponse));
        }

        public void SetOnSetEndingToneDurationResponse(OnResponseDelegate onSetEndingToneDurationResponse)
        {
            this.onSetEndingToneDurationResponse = onSetEndingToneDurationResponse ?? throw new ArgumentNullException(nameof(onSetEndingToneDurationResponse));
        }

        public void SetOnGetBeginAndEndTimesResponse(OnResponseDelegate onGetBeginAndEndTimesResponse)
        {
            this.onGetBeginAndEndTimesResponse = onGetBeginAndEndTimesResponse ?? throw new ArgumentNullException(nameof(onGetBeginAndEndTimesResponse));
        }

        public void SetOnIsFoxArmedResponse(OnResponseDelegate onIsFoxArmedResponse)
        {
            this.onIsFoxArmedResponse = onIsFoxArmedResponse ?? throw new ArgumentNullException(nameof(onIsFoxArmedResponse));
        }

        public void SetOnArmFoxResponse(OnResponseDelegate onArmFoxResponse)
        {
            this.onArmFoxResponse = onArmFoxResponse ?? throw new ArgumentNullException(nameof(onArmFoxResponse));
        }

        public void SetOnDisarmFoxResponse(OnResponseDelegate onDisarmFoxResponse)
        {
            this.onDisarmFoxResponse = onDisarmFoxResponse ?? throw new ArgumentNullException(nameof(onDisarmFoxResponse));
        }

        public void SetOnSetBeginAndEndTimesResponse(OnResponseDelegate onSetBeginAndEndTimesResponse)
        {
            this.onSetBeginAndEndTimesResponse = onSetBeginAndEndTimesResponse ?? throw new ArgumentNullException(nameof(onSetBeginAndEndTimesResponse));
        }

        public void SetOnGetFoxPowerResponse(OnResponseDelegate onGetFoxPowerResponse)
        {
            this.onGetFoxPowerResponse = onGetFoxPowerResponse ?? throw new ArgumentNullException(nameof(onGetFoxPowerResponse));
        }

        public void SetOnSetFoxPowerResponse(OnResponseDelegate onSetFoxPowerResponse)
        {
            this.onSetFoxPowerResponse = onSetFoxPowerResponse ?? throw new ArgumentNullException(nameof(onSetFoxPowerResponse));
        }

        public void SetOnGetBatteryLevelResponse(OnResponseDelegate onGetBatteryLevelResponse)
        {
            this.onGetBatteryLevelResponse = onGetBatteryLevelResponse ?? throw new ArgumentNullException(nameof(onGetBatteryLevelResponse));
        }

        public void SetOnGetUAntADCResponse(OnResponseDelegate onGetUAntADCResponse)
        {
            this.onGetUAntADCResponse = onGetUAntADCResponse ?? throw new ArgumentNullException(nameof(onGetUAntADCResponse));
        }
    }
}
