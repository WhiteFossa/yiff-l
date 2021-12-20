using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Helpers;
using org.whitefossa.yiffhl.Business.Implementations.Events;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class PacketsProcessor : IPacketsProcessor
    {
        /// <summary>
        /// Add this to payload to get full packet lenght. 1: lenght byte + 4: crc32 bytes
        /// </summary>
        private const int AddToPacketLenght = 5;

        private const int MinPayloadLength = 1;
        private const int MaxPayloadLength = 59;

        /// <summary>
        /// How long one iteration of waiting for response is
        /// </summary>
        private const int WaitForResponseIterationSleepLength = 1;

        /// <summary>
        /// If we have >= than this iterations during waiting for response, we will throw an exception.
        /// </summary>
        private const int WaitForResponseIterationsTimeout = int.MaxValue; // TODO: Return to sane value

        #region Command responses

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
        private OnResponseDelegate onGetUBattADCResponse;
        private OnResponseDelegate onGetU80mADCResponse;
        private OnResponseDelegate onGetUBattVoltsResponse;
        private OnResponseDelegate onGetU80mVoltsResponse;
        private OnResponseDelegate onGetLastFailureCodeResponse;
        private OnResponseDelegate onNoOperationResponse;
        private OnResponseDelegate onGetIdentificationDataResponse;

        #endregion

        #region Events

        private OnFoxArmedEventDelegate _onFoxArmedEvent;
        private OnAntennaMatchingMeasurementEventDelegate _onAntennaMatchingMeasurementEvent;
        private OnEnteringSleepmodeEventDelegate _onEnteringSleepmodeEvent;
        private OnFoxArmingInitiatedEventDelegate _onFoxArmingInitiatedEvent;
        private OnFoxDisarmedEventDelegate _onFoxDisarmedEvent;

        #endregion


        private enum ReceiverState
        {
            WaitingForFirstByte,
            ReceivingNextBytes
        }

        private List<byte> receiverBuffer = new List<byte>();

        private ReceiverState receiverState = ReceiverState.WaitingForFirstByte;

        private int expectedPacketLength;

        private readonly IBluetoothCommunicator bluetoothCommunicator;

        private bool isWaitingForResponse;

        public PacketsProcessor(IBluetoothCommunicator bluetoothCommunicator)
        {
            this.bluetoothCommunicator = bluetoothCommunicator;
        }

        public void NewByteReceived(byte data)
        {
            switch (receiverState)
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

            switch (payloadType)
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
            StopWaitingForResponse();

            var responseTo = (CommandType)payload.ElementAt(0);

            Debug.WriteLine($"Response to command: {responseTo}");

            var responsePayload = payload
                .ToList()
                .GetRange(1, payload.Count - 1)
                .AsReadOnly();

            switch (responseTo)
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

                // Get battery voltage (ADC)
                case CommandType.GetUBattADC:
                    CheckOnResponseDelegate(onGetUBattADCResponse);

                    onGetUBattADCResponse(responsePayload);
                    break;

                // Get 3.5MHz output stage power source voltage (ADC)
                case CommandType.GetU80mADC:
                    CheckOnResponseDelegate(onGetU80mADCResponse);

                    onGetU80mADCResponse(responsePayload);
                    break;

                // Get battery voltage (volts)
                case CommandType.GetUBattVolts:
                    CheckOnResponseDelegate(onGetUBattVoltsResponse);

                    onGetUBattVoltsResponse(responsePayload);
                    break;

                // Get 3.5MHz output stage power source voltage (volts)
                case CommandType.GetU80mVolts:
                    CheckOnResponseDelegate(onGetU80mVoltsResponse);

                    onGetU80mVoltsResponse(responsePayload);
                    break;

                // Get last failure code
                case CommandType.GetLastFailureCode:
                    CheckOnResponseDelegate(onGetLastFailureCodeResponse);

                    onGetLastFailureCodeResponse(responsePayload);
                    break;

                // No operation
                case CommandType.NoOperation:
                    CheckOnResponseDelegate(onNoOperationResponse);

                    onNoOperationResponse(responsePayload);
                    break;

                // Get identification data
                case CommandType.GetIdentificationData:
                    CheckOnResponseDelegate(onGetIdentificationDataResponse);

                    onGetIdentificationDataResponse(responsePayload);
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
            var eventType = (EventType)payload.ElementAt(0);

            var eventPayload = payload
                .ToList()
                .GetRange(1, payload.Count - 1)
                .AsReadOnly();

            switch (eventType)
            {
                // Fox is armed
                case EventType.Armed:
                    OnFoxIsArmedEvent(eventPayload);
                    break;

                // New antenna matching measurement arrived
                case EventType.AntennaMatchingMeasurement:
                    OnAntennaMatchingMeasurementEvent(eventPayload);
                    break;

                // Entering sleepmode
                case EventType.EnteringSleepmode:
                    OnEnteringSleepmodeEvent(eventPayload);
                    break;

                // Arming initiated
                case EventType.ArmingInitiatedEvent:
                    OnArmingInitiatedEvent(eventPayload);
                    break;

                // Disarmed
                case EventType.Disarmed:
                    OnDisarmedEvent(eventPayload);
                    break;

                // We've got some junk
                default:
                    return;
            }
        }

        private void OnFoxIsArmedEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onFoxArmedEvent ?? throw new InvalidOperationException("Handler for Fox Is Armed event isn't registered");

            _onFoxArmedEvent(new FoxArmedEvent());
        }

        private void OnAntennaMatchingMeasurementEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onAntennaMatchingMeasurementEvent ?? throw new InvalidOperationException("Handler for Antenna Matching Measurement event isn't registered");

            if (payload.Count != 5)
            {
                return;
            }

            var matchingPosition = (int)payload.ElementAt(0);
            var antennaVoltage = BitConverter.ToSingle(payload.ToArray(), 1);

            _onAntennaMatchingMeasurementEvent(new AntennaMatchingMeasurementEvent(matchingPosition, antennaVoltage));
        }

        private void OnEnteringSleepmodeEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onEnteringSleepmodeEvent ?? throw new InvalidOperationException("Handler for Entring Sleepmode event isn't registered");

            _onEnteringSleepmodeEvent(new EnteringSleepmodeEvent());
        }

        private void OnArmingInitiatedEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onFoxArmingInitiatedEvent ?? throw new InvalidOperationException("Handler for Fox Arming Initiated event isn't registered");

            _onFoxArmingInitiatedEvent(new FoxArmingInitiatedEvent());
        }

        private void OnDisarmedEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onFoxDisarmedEvent ?? throw new InvalidOperationException("Handler for Fox Disarmed event isn't registered");

            _onFoxDisarmedEvent(new FoxDisarmedEvent());
        }

        public void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload)
        {
            var thread = new Thread(() => SendCommandInner(command, commandPayload));
            thread.Start();
        }

        private void SendCommandInner(CommandType command, IReadOnlyCollection<byte> commandPayload)
        {
            Debug.WriteLine($"Command requested: {command}");

            var waitTimer = 0;
            while (isWaitingForResponse)
            {
                Thread.Sleep(WaitForResponseIterationSleepLength);
                waitTimer++;

                if (waitTimer >= WaitForResponseIterationsTimeout)
                {
                    throw new TimeoutException("Timeout during waiting for command response");
                }
            }

            StartWaitingForResponse();

            Debug.WriteLine($"Command send: {command}");

            var resultPayload = new List<byte>();
            resultPayload.Add((byte)PayloadType.CommandToFox);
            resultPayload.Add((byte)command);
            resultPayload.AddRange(commandPayload);

            SendPacket(resultPayload);
        }

        private void StartWaitingForResponse()
        {
            isWaitingForResponse = true;
        }

        private void StopWaitingForResponse()
        {
            isWaitingForResponse = false;
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

        public void SetOnGetUBattADCResponse(OnResponseDelegate onGetUBattADCResponse)
        {
            this.onGetUBattADCResponse = onGetUBattADCResponse ?? throw new ArgumentNullException(nameof(onGetUBattADCResponse));
        }

        public void SetOnGetU80mADCResponse(OnResponseDelegate onGetU80mADCResponse)
        {
            this.onGetU80mADCResponse = onGetU80mADCResponse ?? throw new ArgumentNullException(nameof(onGetU80mADCResponse));
        }

        public void SetOnGetUBattVoltsResponse(OnResponseDelegate onGetUBattVoltsResponse)
        {
            this.onGetUBattVoltsResponse = onGetUBattVoltsResponse ?? throw new ArgumentNullException(nameof(onGetUBattVoltsResponse));
        }

        public void SetOnGetU80mVoltsResponse(OnResponseDelegate onGetU80mVoltsResponse)
        {
            this.onGetU80mVoltsResponse = onGetU80mVoltsResponse ?? throw new ArgumentNullException(nameof(onGetU80mVoltsResponse));
        }

        public void SetOnGetLastFailureCodeResponse(OnResponseDelegate onGetLastFailureCodeResponse)
        {
            this.onGetLastFailureCodeResponse = onGetLastFailureCodeResponse ?? throw new ArgumentNullException(nameof(onGetLastFailureCodeResponse));
        }

        public void RegisterOnFoxArmedEventHandler(OnFoxArmedEventDelegate onFoxArmedEvent)
        {
            _onFoxArmedEvent = onFoxArmedEvent ?? throw new ArgumentNullException(nameof(onFoxArmedEvent));
        }

        public void RegisterOnAntennaMatchingMeasurementEventHandler(OnAntennaMatchingMeasurementEventDelegate onAntennaMatchingMeasurementEvent)
        {
            _onAntennaMatchingMeasurementEvent = onAntennaMatchingMeasurementEvent ?? throw new ArgumentNullException(nameof(onAntennaMatchingMeasurementEvent));
        }

        public void RegisterOnEnteringSleepmodeEventHandler(OnEnteringSleepmodeEventDelegate onEnteringSleepmodeEvent)
        {
            _onEnteringSleepmodeEvent = onEnteringSleepmodeEvent ?? throw new ArgumentNullException(nameof(onEnteringSleepmodeEvent));
        }

        public void SetOnNoOperationResponse(OnResponseDelegate onNoOperationResponse)
        {
            this.onNoOperationResponse = onNoOperationResponse ?? throw new ArgumentNullException(nameof(onNoOperationResponse));
        }

        public void SetOnGetIdentificationDataResponse(OnResponseDelegate onGetIdentificationDataResponse)
        {
            this.onGetIdentificationDataResponse = onGetIdentificationDataResponse ?? throw new ArgumentNullException(nameof(onGetIdentificationDataResponse));
        }

        public void RegisterOnFoxArmingInitiatedEventHandler(OnFoxArmingInitiatedEventDelegate onFoxArmingInitiatedEvent)
        {
            _onFoxArmingInitiatedEvent = onFoxArmingInitiatedEvent ?? throw new ArgumentNullException(nameof(onFoxArmingInitiatedEvent));
        }

        public void RegisterOnFoxDisarmedEventsHandler(OnFoxDisarmedEventDelegate onFoxDisarmedEvent)
        {
            _onFoxDisarmedEvent = onFoxDisarmedEvent ?? throw new ArgumentNullException(nameof(onFoxDisarmedEvent));
        }
    }
}
