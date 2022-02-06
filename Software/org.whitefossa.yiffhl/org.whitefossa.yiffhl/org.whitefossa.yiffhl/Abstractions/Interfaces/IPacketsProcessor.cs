using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Delegate for responses to commands. Payload is just a response payload, whithout
    /// response marker and command code
    /// </summary>
    public delegate void OnResponseDelegate(IReadOnlyCollection<byte> payload);

    #region Events delegats

    public delegate void OnEnteringSleepmodeEventDelegate(IEnteringSleepmodeEvent enteringSleepmodeEvent);

    #endregion

    /// <summary>
    /// Interface to work with packets
    /// </summary>
    public interface IPacketsProcessor
    {
        /// <summary>
        /// Send packet with given payload to fox
        /// </summary>
        void SendPacket(IReadOnlyCollection<byte> payload);

        void SendCommand(CommandType command, IReadOnlyCollection<byte> commandPayload);

        /// <summary>
        /// Call this when new byte arrive
        /// </summary>
        void NewByteReceived(byte data);

        /// <summary>
        /// Call this when connection with fox is estabilished
        /// </summary>
        void OnConnect();

        /// <summary>
        /// Call this if connection lost
        /// </summary>
        void OnDisconnect();

        #region Responses to commands

        void SetOnSetDateAndTimeResponse(OnResponseDelegate onSetDateAndTimeResponse);
        void SetOnSetFoxNameResponse(OnResponseDelegate onSetFoxNameResponse);
        void SetOnGetFoxNameResponse(OnResponseDelegate onGetFoxNameResponse);
        void SetOnGetProfilesCountResponse(OnResponseDelegate onGetProfilesCountResponse);
        void SetOnGetProfileNameResponse(OnResponseDelegate onGetProfileNameResponse);
        void SetOnAddNewProfileResponse(OnResponseDelegate onAddNewProfileResponse);
        void SetOnGetCurrentProfileIdResponse(OnResponseDelegate onGetCurrentProfileIdResponse);
        void SetOnSwitchToProfileResponse(OnResponseDelegate onSwitchToProfileResponse);
        void SetOnSetProfileNameResponse(OnResponseDelegate onSetProfileNameResponse);
        void SetOnGetFrequencyResponse(OnResponseDelegate onGetFrequencyResponse);
        void SetOnSetFrequencyResponse(OnResponseDelegate onSetFrequencyResponse);
        void SetOnGetCodeResponse(OnResponseDelegate onGetCodeResponse);
        void SetOnSetCodeResponse(OnResponseDelegate onSetCodeResponse);
        void SetOnGetSpeedResponse(OnResponseDelegate onGetSpeedResponse);
        void SetOnSetSpeedResponse(OnResponseDelegate onSetSpeedResponse);
        void SetOnGetCycleResponse(OnResponseDelegate onGetCycleResponse);
        void SetOnSetCycleResponse(OnResponseDelegate onSetCycleResponse);
        void SetOnGetEndingToneDurationResponse(OnResponseDelegate onGetEndingToneDurationResponse);
        void SetOnSetEndingToneDurationResponse(OnResponseDelegate onSetEndingToneDurationResponse);
        void SetOnGetBeginAndEndTimesResponse(OnResponseDelegate onGetBeginAndEndTimesResponse);
        void SetOnIsFoxArmedResponse(OnResponseDelegate onIsFoxArmedResponse);
        void SetOnArmFoxResponse(OnResponseDelegate onArmFoxResponse);
        void SetOnDisarmFoxResponse(OnResponseDelegate onDisarmFoxResponse);
        void SetOnSetBeginAndEndTimesResponse(OnResponseDelegate onSetBeginAndEndTimesResponse);
        void SetOnGetFoxPowerResponse(OnResponseDelegate onGetFoxPowerResponse);
        void SetOnSetFoxPowerResponse(OnResponseDelegate onSetFoxPowerResponse);
        void SetOnGetBatteryLevelResponse(OnResponseDelegate onGetBatteryLevelResponse);
        void SetOnGetUAntADCResponse(OnResponseDelegate onGetUAntADCResponse);
        void SetOnGetUBattADCResponse(OnResponseDelegate onGetUBattADCResponse);
        void SetOnGetU80mADCResponse(OnResponseDelegate onGetU80mADCResponse);
        void SetOnGetUBattVoltsResponse(OnResponseDelegate onGetUBattVoltsResponse);
        void SetOnGetU80mVoltsResponse(OnResponseDelegate onGetU80mVoltsResponse);
        void SetOnGetLastFailureCodeResponse(OnResponseDelegate onGetLastFailureCodeResponse);
        void SetOnNoOperationResponse(OnResponseDelegate onNoOperationResponse);
        void SetOnGetIdentificationDataResponse(OnResponseDelegate onGetIdentificationDataResponse);
        void SetOnGetAntennaMatchingStatusResponse(OnResponseDelegate onGetAntennaMatchingStatusResponse);
        void SetOnMarkMatchingAsSeenResponse(OnResponseDelegate onMarkMatchingAsSeenResponse);
        void SetOnGetAntennaMatchingDataResponse(OnResponseDelegate onGetAntennaMatchingDataResponse);
        void SetOnCheckForProfileSettingsChangesResponse(OnResponseDelegate onCheckForProfileSettingsChangesResponse);
        void SetOnResetLastErrorCodeResponse(OnResponseDelegate onResetLastErrorCodeResponse);
        void SetOnSerialNumberUpdatedResponse(OnResponseDelegate onSerialNumberUpdatedResponse);
        void SetOnGetUBattFactorsResponse(OnResponseDelegate onGetUBattFactorsResponse);
        void SetOnSetUBattFactorsResponse(OnResponseDelegate onSetUBattFactorsResponse);
        void SetOnGetBattLevelFactorsResponse(OnResponseDelegate onGetBattLevelFactorsResponse);
        void SetOnSetBattLevelFactorsResponse(OnResponseDelegate onSetBattLevelFactorsResponse);
        void SetOnGetU80mFactorsResponse(OnResponseDelegate onGetU80mFactorsResponse);
        void SetOnSetU80mFactorsResponse(OnResponseDelegate onSetU80mFactorsResponse);
        void SetOnGetP80mFactorsResponse(OnResponseDelegate onGetP80mFactorsResponse);
        void SetOnSetP80mFactorsResponse(OnResponseDelegate onSetP80mFactorsResponse);
        void SetOnGetUAntVoltsResponse(OnResponseDelegate onGetUAntVoltsResponse);
        void SetOnGetUantFactorsResponse(OnResponseDelegate onGetUantFactorsResponse);
        void SetOnSetUantFactorsResponse(OnResponseDelegate onSetUantFactorsResponse);
        void SetOnForceTxOnResponse(OnResponseDelegate onForceTxOnResponse);
        void SetOnReturnToNormalTxResponse(OnResponseDelegate onReturnToNormalTxResponse);
        void SetOnGetRTCCalibrationValueResponse(OnResponseDelegate onGetRTCCalibrationValueResponse);
        void SetOnSetRTCCalibrationValueResponse(OnResponseDelegate onSetRTCCalibrationValueResponse);
        void SetOnGetDisarmOnDischargeThresholdResponse(OnResponseDelegate onGetDisarmOnDischargeThresholdResponse);

        #endregion

        #region Events handlers registration

        void RegisterOnEnteringSleepmodeEventHandler(OnEnteringSleepmodeEventDelegate onEnteringSleepmodeEvent);

        #endregion
    }
}
