﻿using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Helpers;
using org.whitefossa.yiffhl.Business.Implementations.Events;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;

namespace org.whitefossa.yiffhl.Business.Implementations.PacketsProcessor
{
    public class PacketsProcessor : IPacketsProcessor
    {
        /// <summary>
        /// Add this to payload to get full packet lenght. 1: lenght byte + 4: crc32 bytes
        /// </summary>
        private const int AddToPacketLenght = 5;

        private const int MinPayloadLength = 1;
        private const int MaxPayloadLength = 59;

        private const int WaitForNewCommandsSleepDuration = 1;

        private const int WaitForResponseSleepDuration = 1;

        /// <summary>
        /// If we have >= than this iterations during waiting for response, we will throw an exception.
        /// </summary>
        private const int WaitForResponseIterationsTimeout = 10000;

        private readonly IBluetoothCommunicator _bluetoothCommunicator;

        #region Command responses

        private OnResponseDelegate _onSetDateAndTimeResponse;
        private OnResponseDelegate _onSetFoxNameResponse;
        private OnResponseDelegate _onGetFoxNameResponse;
        private OnResponseDelegate _onGetProfilesCountResponse;
        private OnResponseDelegate _onGetProfileNameResponse;
        private OnResponseDelegate _onAddNewProfileResponse;
        private OnResponseDelegate _onGetCurrentProfileIdResponse;
        private OnResponseDelegate _onSwitchToProfileResponse;
        private OnResponseDelegate _onSetProfileNameResponse;
        private OnResponseDelegate _onGetFrequencyResponse;
        private OnResponseDelegate _onSetFrequencyResponse;
        private OnResponseDelegate _onGetCodeResponse;
        private OnResponseDelegate _onSetCodeResponse;
        private OnResponseDelegate _onGetSpeedResponse;
        private OnResponseDelegate _onSetSpeedResponse;
        private OnResponseDelegate _onGetCycleResponse;
        private OnResponseDelegate _onSetCycleResponse;
        private OnResponseDelegate _onGetEndingToneDurationResponse;
        private OnResponseDelegate _onSetEndingToneDurationResponse;
        private OnResponseDelegate _onGetBeginAndEndTimesResponse;
        private OnResponseDelegate _onIsFoxArmedResponse;
        private OnResponseDelegate _onArmFoxResponse;
        private OnResponseDelegate _onDisarmFoxResponse;
        private OnResponseDelegate _onSetBeginAndEndTimesResponse;
        private OnResponseDelegate _onGetFoxPowerResponse;
        private OnResponseDelegate _onSetFoxPowerResponse;
        private OnResponseDelegate _onGetBatteryLevelResponse;
        private OnResponseDelegate _onGetUAntADCResponse;
        private OnResponseDelegate _onGetUBattADCResponse;
        private OnResponseDelegate _onGetU80mADCResponse;
        private OnResponseDelegate _onGetUBattVoltsResponse;
        private OnResponseDelegate _onGetU80mVoltsResponse;
        private OnResponseDelegate _onGetLastFailureCodeResponse;
        private OnResponseDelegate _onNoOperationResponse;
        private OnResponseDelegate _onGetIdentificationDataResponse;
        private OnResponseDelegate _onGetAntennaMatchingStatusResponse;
        private OnResponseDelegate _onMarkMatchingAsSeenResponse;
        private OnResponseDelegate _onGetAntennaMatchingDataResponse;

        #endregion

        #region Events

        private OnEnteringSleepmodeEventDelegate _onEnteringSleepmodeEvent;
        private OnFoxArmingInitiatedEventDelegate _onFoxArmingInitiatedEvent;
        private OnFoxDisarmedEventDelegate _onFoxDisarmedEvent;
        private OnProfileSettingsChangedEventDelegate _onProfileSettingsChangedEvent;
        private OnProfileSwitchedEventDelegate _onProfileSwitchedEvent;

        #endregion

        private enum ReceiverState
        {
            WaitingForFirstByte,
            ReceivingNextBytes
        }

        private List<byte> _receiverBuffer = new List<byte>();

        private ReceiverState _receiverState = ReceiverState.WaitingForFirstByte;

        private int _expectedPacketLength;

        private bool _isWaitingForResponse;

        private Queue<CommandQueueElement> _commandsQueue = new Queue<CommandQueueElement>();

        private Thread _commandsSenderThread;

        private Object _commandQueueLock = new Object();

        public PacketsProcessor(IBluetoothCommunicator bluetoothCommunicator)
        {
            _bluetoothCommunicator = bluetoothCommunicator;

            _commandsSenderThread = new Thread(() => CommandsSenderThreadEntryPoint());
            _commandsSenderThread.Start();
        }

        public void NewByteReceived(byte data)
        {
            switch (_receiverState)
            {
                case ReceiverState.WaitingForFirstByte:

                    // First byte is always lenght
                    _expectedPacketLength = data;
                    var payloadLength = _expectedPacketLength - AddToPacketLenght;

                    if (payloadLength < MinPayloadLength || payloadLength > MaxPayloadLength)
                    {
                        // Wrong packet size, discarding
                        return;
                    }

                    _receiverBuffer.Clear();
                    _receiverBuffer.Add(data);
                    _receiverState = ReceiverState.ReceivingNextBytes;

                    break;

                case ReceiverState.ReceivingNextBytes:

                    _receiverBuffer.Add(data);

                    if (_receiverBuffer.Count == _expectedPacketLength)
                    {
                        // Packet completed
                        _receiverState = ReceiverState.WaitingForFirstByte;
                        NewRawPacketReceived(_receiverBuffer.AsReadOnly());
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

            _bluetoothCommunicator.SendMessage(fullPacket);
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
            lock (_commandQueueLock)
            {
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
                        CheckOnResponseDelegate(_onSetDateAndTimeResponse);

                        _onSetDateAndTimeResponse(responsePayload);
                        break;

                    // Set fox name response
                    case CommandType.SetFoxName:
                        CheckOnResponseDelegate(_onSetFoxNameResponse);

                        _onSetFoxNameResponse(responsePayload);
                        break;

                    // Get fox name response
                    case CommandType.GetFoxName:
                        CheckOnResponseDelegate(_onGetFoxNameResponse);

                        _onGetFoxNameResponse(responsePayload);
                        break;

                    // Get profiles count
                    case CommandType.GetProfilesCount:
                        CheckOnResponseDelegate(_onGetProfilesCountResponse);

                        _onGetProfilesCountResponse(responsePayload);
                        break;

                    // Get profile name
                    case CommandType.GetProfileName:
                        CheckOnResponseDelegate(_onGetProfileNameResponse);

                        _onGetProfileNameResponse(responsePayload);
                        break;

                    // Add new profile
                    case CommandType.AddNewProfile:
                        CheckOnResponseDelegate(_onAddNewProfileResponse);

                        _onAddNewProfileResponse(responsePayload);
                        break;

                    // Get current profile ID
                    case CommandType.GetCurrentProfileId:
                        CheckOnResponseDelegate(_onGetCurrentProfileIdResponse);

                        _onGetCurrentProfileIdResponse(responsePayload);
                        break;

                    // Switch to profile
                    case CommandType.SwitchToProfile:
                        CheckOnResponseDelegate(_onSwitchToProfileResponse);

                        _onSwitchToProfileResponse(responsePayload);
                        break;

                    // Set profile name
                    case CommandType.SetProfileName:
                        CheckOnResponseDelegate(_onSetProfileNameResponse);

                        _onSetProfileNameResponse(responsePayload);
                        break;

                    // Get frequency
                    case CommandType.GetFrequency:
                        CheckOnResponseDelegate(_onGetFrequencyResponse);

                        _onGetFrequencyResponse(responsePayload);
                        break;

                    // Set frequency
                    case CommandType.SetFrequency:
                        CheckOnResponseDelegate(_onSetFrequencyResponse);

                        _onSetFrequencyResponse(responsePayload);
                        break;

                    // Get code
                    case CommandType.GetCode:
                        CheckOnResponseDelegate(_onGetCodeResponse);

                        _onGetCodeResponse(responsePayload);
                        break;

                    // Set code
                    case CommandType.SetCode:
                        CheckOnResponseDelegate(_onSetCodeResponse);

                        _onSetCodeResponse(responsePayload);
                        break;

                    // Get speed
                    case CommandType.GetSpeed:
                        CheckOnResponseDelegate(_onGetSpeedResponse);

                        _onGetSpeedResponse(responsePayload);
                        break;

                    // Set speed
                    case CommandType.SetSpeed:
                        CheckOnResponseDelegate(_onSetSpeedResponse);

                        _onSetSpeedResponse(responsePayload);
                        break;

                    // Get cycle parameters
                    case CommandType.GetCycle:
                        CheckOnResponseDelegate(_onGetCycleResponse);

                        _onGetCycleResponse(responsePayload);
                        break;

                    // Set cycle parameters
                    case CommandType.SetCycle:
                        CheckOnResponseDelegate(_onSetCycleResponse);

                        _onSetCycleResponse(responsePayload);
                        break;

                    // Get ending tone duration
                    case CommandType.GetEndingToneDuration:
                        CheckOnResponseDelegate(_onGetEndingToneDurationResponse);

                        _onGetEndingToneDurationResponse(responsePayload);
                        break;

                    // Set ending tone duration
                    case CommandType.SetEndingToneDuration:
                        CheckOnResponseDelegate(_onSetEndingToneDurationResponse);

                        _onSetEndingToneDurationResponse(responsePayload);
                        break;

                    // Get begin and end times
                    case CommandType.GetBeginAndEndTimes:
                        CheckOnResponseDelegate(_onGetBeginAndEndTimesResponse);

                        _onGetBeginAndEndTimesResponse(responsePayload);
                        break;

                    // Is fox armed?
                    case CommandType.IsFoxArmed:
                        CheckOnResponseDelegate(_onIsFoxArmedResponse);

                        _onIsFoxArmedResponse(responsePayload);
                        break;

                    // Arm fox
                    case CommandType.ArmFox:
                        CheckOnResponseDelegate(_onArmFoxResponse);

                        _onArmFoxResponse(responsePayload);
                        break;

                    // Disarm fox
                    case CommandType.DisarmFox:
                        CheckOnResponseDelegate(_onDisarmFoxResponse);

                        _onDisarmFoxResponse(responsePayload);
                        break;

                    // Set begin and end times
                    case CommandType.SetBeginAndEndTimes:
                        CheckOnResponseDelegate(_onSetBeginAndEndTimesResponse);

                        _onSetBeginAndEndTimesResponse(responsePayload);
                        break;

                    // Get fox power
                    case CommandType.GetFoxPower:
                        CheckOnResponseDelegate(_onGetFoxPowerResponse);

                        _onGetFoxPowerResponse(responsePayload);
                        break;

                    // Set fox power
                    case CommandType.SetFoxPower:
                        CheckOnResponseDelegate(_onSetFoxPowerResponse);

                        _onSetFoxPowerResponse(responsePayload);
                        break;

                    // Get battery level
                    case CommandType.GetBatteryLevel:
                        CheckOnResponseDelegate(_onGetBatteryLevelResponse);

                        _onGetBatteryLevelResponse(responsePayload);
                        break;

                    // Get 3.5MHz antenna voltage (ADC)
                    case CommandType.GetUAntADC:
                        CheckOnResponseDelegate(_onGetUAntADCResponse);

                        _onGetUAntADCResponse(responsePayload);
                        break;

                    // Get battery voltage (ADC)
                    case CommandType.GetUBattADC:
                        CheckOnResponseDelegate(_onGetUBattADCResponse);

                        _onGetUBattADCResponse(responsePayload);
                        break;

                    // Get 3.5MHz output stage power source voltage (ADC)
                    case CommandType.GetU80mADC:
                        CheckOnResponseDelegate(_onGetU80mADCResponse);

                        _onGetU80mADCResponse(responsePayload);
                        break;

                    // Get battery voltage (volts)
                    case CommandType.GetUBattVolts:
                        CheckOnResponseDelegate(_onGetUBattVoltsResponse);

                        _onGetUBattVoltsResponse(responsePayload);
                        break;

                    // Get 3.5MHz output stage power source voltage (volts)
                    case CommandType.GetU80mVolts:
                        CheckOnResponseDelegate(_onGetU80mVoltsResponse);

                        _onGetU80mVoltsResponse(responsePayload);
                        break;

                    // Get last failure code
                    case CommandType.GetLastFailureCode:
                        CheckOnResponseDelegate(_onGetLastFailureCodeResponse);

                        _onGetLastFailureCodeResponse(responsePayload);
                        break;

                    // No operation
                    case CommandType.NoOperation:
                        CheckOnResponseDelegate(_onNoOperationResponse);

                        _onNoOperationResponse(responsePayload);
                        break;

                    // Get identification data
                    case CommandType.GetIdentificationData:
                        CheckOnResponseDelegate(_onGetIdentificationDataResponse);

                        _onGetIdentificationDataResponse(responsePayload);
                        break;

                    // Get antenna matching status
                    case CommandType.GetAntennaMatchingStatus:
                        CheckOnResponseDelegate(_onGetAntennaMatchingStatusResponse);

                        _onGetAntennaMatchingStatusResponse(responsePayload);
                        break;

                    // Get antenna matching data
                    case CommandType.GetAntennaMatchingData:
                        CheckOnResponseDelegate(_onGetAntennaMatchingDataResponse);

                        _onGetAntennaMatchingDataResponse(responsePayload);
                        break;

                    // Mark matching as seen
                    case CommandType.MarkMatchingAsSeen:
                        CheckOnResponseDelegate(_onMarkMatchingAsSeenResponse);

                        _onMarkMatchingAsSeenResponse(responsePayload);
                        break;

                    default:
                        return; // We've got some junk
                }

                StopWaitingForResponse();
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

                // Settings changed
                case EventType.ProfileSettingsChanged:
                    OnProfileSettingsChangedEvent(eventPayload);
                    break;

                // Profile switched
                case EventType.ProfileSwitched:
                    OnProfileSwitchedEvent(eventPayload);
                    break;

                // We've got some junk
                default:
                    return;
            }
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

        private void OnProfileSettingsChangedEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onProfileSettingsChangedEvent ?? throw new InvalidOperationException("Handler for Profile Settings Changed event isn't registered");

            _onProfileSettingsChangedEvent(new ProfileSettingsChangedEvent());
        }

        private void OnProfileSwitchedEvent(IReadOnlyCollection<byte> payload)
        {
            _ = _onProfileSwitchedEvent ?? throw new InvalidOperationException("Handler for Profile Switched event isn't registered");

            _onProfileSwitchedEvent(new ProfileSwitchedEvent());
        }

        #region Commands queue

        public void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload)
        {
            lock (_commandQueueLock)
            {
                _commandsQueue.Enqueue(new CommandQueueElement(command, commandPayload));

                Debug.WriteLine($"Enqueued command: { command }");
                Debug.WriteLine($"Queue length: { _commandsQueue.Count() }");
            }
        }

        private void CommandsSenderThreadEntryPoint()
        {
            while(true)
            {
                bool isQueueNotEmpty;

                lock(_commandQueueLock)
                {
                    isQueueNotEmpty = _commandsQueue.Any();
                }

                if (!isQueueNotEmpty)
                {
                    Thread.Sleep(WaitForNewCommandsSleepDuration);
                    continue;
                }

                // Are we waiting for response of previous command?
                var timeoutCounter = 0;
                while (_isWaitingForResponse)
                {
                    Thread.Sleep(WaitForResponseSleepDuration);
                    timeoutCounter++;

                    if (timeoutCounter >= WaitForResponseIterationsTimeout)
                    {
                        throw new InvalidOperationException("Timeout while waiting for command response");
                    }
                }

                lock (_commandQueueLock)
                {
                    StartWaitingForResponse();

                    var commandToSend = _commandsQueue.Dequeue();

                    var resultPayload = new List<byte>();
                    resultPayload.Add((byte)PayloadType.CommandToFox);
                    resultPayload.Add((byte)commandToSend.Command);
                    resultPayload.AddRange(commandToSend.Payload);

                    Debug.WriteLine($"Sending command: { commandToSend.Command }");

                    SendPacket(resultPayload);
                }
            }
        }

        private void StartWaitingForResponse()
        {
            lock (_commandQueueLock)
            {
                _isWaitingForResponse = true;
            }
        }

        private void StopWaitingForResponse()
        {
            lock (_commandQueueLock)
            {
                _isWaitingForResponse = false;
            }
        }

        #endregion

        public void SetOnSetDateAndTimeResponse(OnResponseDelegate onSetDateAndTimeResponse)
        {
            _onSetDateAndTimeResponse = onSetDateAndTimeResponse ?? throw new ArgumentNullException(nameof(onSetDateAndTimeResponse));
        }

        public void SetOnSetFoxNameResponse(OnResponseDelegate onSetFoxNameResponse)
        {
            _onSetFoxNameResponse = onSetFoxNameResponse ?? throw new ArgumentNullException(nameof(onSetFoxNameResponse));
        }

        public void SetOnGetFoxNameResponse(OnResponseDelegate onGetFoxNameResponse)
        {
            _onGetFoxNameResponse = onGetFoxNameResponse ?? throw new ArgumentNullException(nameof(onGetFoxNameResponse));
        }

        public void SetOnGetProfilesCountResponse(OnResponseDelegate onGetProfilesCountResponse)
        {
            _onGetProfilesCountResponse = onGetProfilesCountResponse ?? throw new ArgumentNullException(nameof(onGetProfilesCountResponse));
        }

        public void SetOnGetProfileNameResponse(OnResponseDelegate onGetProfileNameResponse)
        {
            _onGetProfileNameResponse = onGetProfileNameResponse ?? throw new ArgumentNullException(nameof(onGetProfileNameResponse));
        }

        public void SetOnAddNewProfileResponse(OnResponseDelegate onAddNewProfileResponse)
        {
            _onAddNewProfileResponse = onAddNewProfileResponse ?? throw new ArgumentNullException(nameof(onAddNewProfileResponse));
        }

        public void SetOnGetCurrentProfileIdResponse(OnResponseDelegate onGetCurrentProfileIdResponse)
        {
            _onGetCurrentProfileIdResponse = onGetCurrentProfileIdResponse ?? throw new ArgumentNullException(nameof(onGetCurrentProfileIdResponse));
        }

        public void SetOnSwitchToProfileResponse(OnResponseDelegate onSwitchToProfileResponse)
        {
            _onSwitchToProfileResponse = onSwitchToProfileResponse ?? throw new ArgumentNullException(nameof(onSwitchToProfileResponse));
        }

        public void SetOnSetProfileNameResponse(OnResponseDelegate onSetProfileNameResponse)
        {
            _onSetProfileNameResponse = onSetProfileNameResponse ?? throw new ArgumentNullException(nameof(onSetProfileNameResponse));
        }

        public void SetOnGetFrequencyResponse(OnResponseDelegate onGetFrequencyResponse)
        {
            _onGetFrequencyResponse = onGetFrequencyResponse ?? throw new ArgumentNullException(nameof(onGetFrequencyResponse));
        }

        public void SetOnSetFrequencyResponse(OnResponseDelegate onSetFrequencyResponse)
        {
            _onSetFrequencyResponse = onSetFrequencyResponse ?? throw new ArgumentNullException(nameof(onSetFrequencyResponse));
        }

        public void SetOnGetCodeResponse(OnResponseDelegate onGetCodeResponse)
        {
            _onGetCodeResponse = onGetCodeResponse ?? throw new ArgumentNullException(nameof(onGetCodeResponse));
        }

        public void SetOnSetCodeResponse(OnResponseDelegate onSetCodeResponse)
        {
            _onSetCodeResponse = onSetCodeResponse ?? throw new ArgumentNullException(nameof(onSetCodeResponse));
        }

        public void SetOnGetSpeedResponse(OnResponseDelegate onGetSpeedResponse)
        {
            _onGetSpeedResponse = onGetSpeedResponse ?? throw new ArgumentNullException(nameof(onGetSpeedResponse));
        }

        public void SetOnSetSpeedResponse(OnResponseDelegate onSetSpeedResponse)
        {
            _onSetSpeedResponse = onSetSpeedResponse ?? throw new ArgumentNullException(nameof(onSetSpeedResponse));
        }

        public void SetOnGetCycleResponse(OnResponseDelegate onGetCycleResponse)
        {
            _onGetCycleResponse = onGetCycleResponse ?? throw new ArgumentNullException(nameof(onGetCycleResponse));
        }

        public void SetOnSetCycleResponse(OnResponseDelegate onSetCycleResponse)
        {
            _onSetCycleResponse = onSetCycleResponse ?? throw new ArgumentNullException(nameof(onSetCycleResponse));
        }

        public void SetOnGetEndingToneDurationResponse(OnResponseDelegate onGetEndingToneDurationResponse)
        {
            _onGetEndingToneDurationResponse = onGetEndingToneDurationResponse ?? throw new ArgumentNullException(nameof(onGetEndingToneDurationResponse));
        }

        public void SetOnSetEndingToneDurationResponse(OnResponseDelegate onSetEndingToneDurationResponse)
        {
            _onSetEndingToneDurationResponse = onSetEndingToneDurationResponse ?? throw new ArgumentNullException(nameof(onSetEndingToneDurationResponse));
        }

        public void SetOnGetBeginAndEndTimesResponse(OnResponseDelegate onGetBeginAndEndTimesResponse)
        {
            _onGetBeginAndEndTimesResponse = onGetBeginAndEndTimesResponse ?? throw new ArgumentNullException(nameof(onGetBeginAndEndTimesResponse));
        }

        public void SetOnIsFoxArmedResponse(OnResponseDelegate onIsFoxArmedResponse)
        {
            _onIsFoxArmedResponse = onIsFoxArmedResponse ?? throw new ArgumentNullException(nameof(onIsFoxArmedResponse));
        }

        public void SetOnArmFoxResponse(OnResponseDelegate onArmFoxResponse)
        {
            _onArmFoxResponse = onArmFoxResponse ?? throw new ArgumentNullException(nameof(onArmFoxResponse));
        }

        public void SetOnDisarmFoxResponse(OnResponseDelegate onDisarmFoxResponse)
        {
            _onDisarmFoxResponse = onDisarmFoxResponse ?? throw new ArgumentNullException(nameof(onDisarmFoxResponse));
        }

        public void SetOnSetBeginAndEndTimesResponse(OnResponseDelegate onSetBeginAndEndTimesResponse)
        {
            _onSetBeginAndEndTimesResponse = onSetBeginAndEndTimesResponse ?? throw new ArgumentNullException(nameof(onSetBeginAndEndTimesResponse));
        }

        public void SetOnGetFoxPowerResponse(OnResponseDelegate onGetFoxPowerResponse)
        {
            _onGetFoxPowerResponse = onGetFoxPowerResponse ?? throw new ArgumentNullException(nameof(onGetFoxPowerResponse));
        }

        public void SetOnSetFoxPowerResponse(OnResponseDelegate onSetFoxPowerResponse)
        {
            _onSetFoxPowerResponse = onSetFoxPowerResponse ?? throw new ArgumentNullException(nameof(onSetFoxPowerResponse));
        }

        public void SetOnGetBatteryLevelResponse(OnResponseDelegate onGetBatteryLevelResponse)
        {
            _onGetBatteryLevelResponse = onGetBatteryLevelResponse ?? throw new ArgumentNullException(nameof(onGetBatteryLevelResponse));
        }

        public void SetOnGetUAntADCResponse(OnResponseDelegate onGetUAntADCResponse)
        {
            _onGetUAntADCResponse = onGetUAntADCResponse ?? throw new ArgumentNullException(nameof(onGetUAntADCResponse));
        }

        public void SetOnGetUBattADCResponse(OnResponseDelegate onGetUBattADCResponse)
        {
            _onGetUBattADCResponse = onGetUBattADCResponse ?? throw new ArgumentNullException(nameof(onGetUBattADCResponse));
        }

        public void SetOnGetU80mADCResponse(OnResponseDelegate onGetU80mADCResponse)
        {
            _onGetU80mADCResponse = onGetU80mADCResponse ?? throw new ArgumentNullException(nameof(onGetU80mADCResponse));
        }

        public void SetOnGetUBattVoltsResponse(OnResponseDelegate onGetUBattVoltsResponse)
        {
            _onGetUBattVoltsResponse = onGetUBattVoltsResponse ?? throw new ArgumentNullException(nameof(onGetUBattVoltsResponse));
        }

        public void SetOnGetU80mVoltsResponse(OnResponseDelegate onGetU80mVoltsResponse)
        {
            _onGetU80mVoltsResponse = onGetU80mVoltsResponse ?? throw new ArgumentNullException(nameof(onGetU80mVoltsResponse));
        }

        public void SetOnGetLastFailureCodeResponse(OnResponseDelegate onGetLastFailureCodeResponse)
        {
            _onGetLastFailureCodeResponse = onGetLastFailureCodeResponse ?? throw new ArgumentNullException(nameof(onGetLastFailureCodeResponse));
        }

        public void RegisterOnEnteringSleepmodeEventHandler(OnEnteringSleepmodeEventDelegate onEnteringSleepmodeEvent)
        {
            _onEnteringSleepmodeEvent = onEnteringSleepmodeEvent ?? throw new ArgumentNullException(nameof(onEnteringSleepmodeEvent));
        }

        public void SetOnNoOperationResponse(OnResponseDelegate onNoOperationResponse)
        {
            _onNoOperationResponse = onNoOperationResponse ?? throw new ArgumentNullException(nameof(onNoOperationResponse));
        }

        public void SetOnGetIdentificationDataResponse(OnResponseDelegate onGetIdentificationDataResponse)
        {
            _onGetIdentificationDataResponse = onGetIdentificationDataResponse ?? throw new ArgumentNullException(nameof(onGetIdentificationDataResponse));
        }

        public void RegisterOnFoxArmingInitiatedEventHandler(OnFoxArmingInitiatedEventDelegate onFoxArmingInitiatedEvent)
        {
            _onFoxArmingInitiatedEvent = onFoxArmingInitiatedEvent ?? throw new ArgumentNullException(nameof(onFoxArmingInitiatedEvent));
        }

        public void RegisterOnFoxDisarmedEventHandler(OnFoxDisarmedEventDelegate onFoxDisarmedEvent)
        {
            _onFoxDisarmedEvent = onFoxDisarmedEvent ?? throw new ArgumentNullException(nameof(onFoxDisarmedEvent));
        }

        public void RegisterOnProfileSettingsChangedEventHandler(OnProfileSettingsChangedEventDelegate onProfileSettingsChangedEvent)
        {
            _onProfileSettingsChangedEvent = onProfileSettingsChangedEvent ?? throw new ArgumentNullException(nameof(onProfileSettingsChangedEvent));
        }

        public void RegisterOnProfileSwitchedEventHandler(OnProfileSwitchedEventDelegate onProfileSwitchedEvent)
        {
            _onProfileSwitchedEvent = onProfileSwitchedEvent ?? throw new ArgumentNullException(nameof(onProfileSwitchedEvent));
        }

        public void SetOnGetAntennaMatchingStatusResponse(OnResponseDelegate onGetAntennaMatchingStatusResponse)
        {
            _onGetAntennaMatchingStatusResponse = onGetAntennaMatchingStatusResponse
                ?? throw new ArgumentNullException(nameof(onGetAntennaMatchingStatusResponse));
        }

        public void SetOnMarkMatchingAsSeenResponse(OnResponseDelegate onMarkMatchingAsSeenResponse)
        {
            _onMarkMatchingAsSeenResponse = onMarkMatchingAsSeenResponse
                ?? throw new ArgumentNullException(nameof(onMarkMatchingAsSeenResponse));
        }

        public void SetOnGetAntennaMatchingDataResponse(OnResponseDelegate onGetAntennaMatchingDataResponse)
        {
            _onGetAntennaMatchingDataResponse = onGetAntennaMatchingDataResponse
                ?? throw new ArgumentNullException(nameof(onGetAntennaMatchingDataResponse));
        }
    }
}
