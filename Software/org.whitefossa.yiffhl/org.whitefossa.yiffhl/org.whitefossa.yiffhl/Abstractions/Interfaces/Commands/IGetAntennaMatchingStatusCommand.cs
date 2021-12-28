using org.whitefossa.yiffhl.Abstractions.Enums;
using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetAntennaMatchingStatusResponseDelegate
        (
            AntennaMatchingStatus status,
            TimeSpan timeSinceLastInitiation,
            int totalMatcherPositions,
            int currentMatcherPosition,
            int currentBestMatchPosition,
            float currentBestMatchVoltage
        );

    public interface IGetAntennaMatchingStatusCommand
    {
        void SetResponseDelegate(OnGetAntennaMatchingStatusResponseDelegate onGetAntennaMatchingStatusResponse);

        void SendGetAntennaMatchingStatusCommand();
    }
}
