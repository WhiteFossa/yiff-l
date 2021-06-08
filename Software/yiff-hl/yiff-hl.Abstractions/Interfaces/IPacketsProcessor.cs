﻿using System.Collections.Generic;
using yiff_hl.Abstractions.Enums;

namespace yiff_hl.Abstractions.Interfaces
{
    /// <summary>
    /// Delegate for responses to commands. Payload is just a response payload, whithout
    /// response marker and command code
    /// </summary>
    public delegate void OnResponseDelegate(IReadOnlyCollection<byte> payload);

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


        #endregion

    }
}
