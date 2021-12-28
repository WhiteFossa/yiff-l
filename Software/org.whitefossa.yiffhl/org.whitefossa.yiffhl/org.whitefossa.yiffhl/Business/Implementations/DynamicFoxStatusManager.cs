using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class DynamicFoxStatusManager : IDynamicFoxStatusManager
    {
        private readonly IGetBatteryLevelCommand _getBatteryLevelCommand;
        private readonly IGetAntennaMatchingStatusCommand _getAntennaMatchingStatusCommand;

        private OnGetDynamicFoxStatus _onGetDynamicFoxStatus;

        private DynamicFoxStatus _statusToLoad = new DynamicFoxStatus();

        public DynamicFoxStatusManager
            (
                IGetBatteryLevelCommand getBatteryLevelCommand,
                IGetAntennaMatchingStatusCommand getAntennaMatchingStatusCommand
            )
        {
            _getBatteryLevelCommand = getBatteryLevelCommand;
            _getAntennaMatchingStatusCommand = getAntennaMatchingStatusCommand;
        }

        public async Task GetDynamicFoxStatusAsync(OnGetDynamicFoxStatus onGetDynamicFoxStatus)
        {
            _onGetDynamicFoxStatus = onGetDynamicFoxStatus ?? throw new ArgumentNullException(nameof(onGetDynamicFoxStatus));

            _getBatteryLevelCommand.SetResponseDelegate(OnGetBatteryLevelResponse);
            _getBatteryLevelCommand.SendGetBatteryLevelCommand();
        }

        private void OnGetBatteryLevelResponse(float level)
        {
            _statusToLoad.BatteryLevel = level;

            _getAntennaMatchingStatusCommand.SetResponseDelegate(OnGetAntennaMatchingStatus);
            _getAntennaMatchingStatusCommand.SendGetAntennaMatchingStatusCommand();
        }

        private void OnGetAntennaMatchingStatus
        (
            AntennaMatchingStatus status,
            TimeSpan timeSinceLastInitiation,
            int totalMatcherPositions,
            int currentMatcherPosition,
            int currentBestMatchPosition,
            float currentBestMatchVoltage
        )
        {
            Debug.WriteLine($"Antenna matching:");
            Debug.WriteLine($"Status: { status }");
            Debug.WriteLine($"Time since last initiation: { timeSinceLastInitiation }");
            Debug.WriteLine($"Total matcher positions: { totalMatcherPositions }");
            Debug.WriteLine($"Current matcher position: { currentBestMatchPosition }");
            Debug.WriteLine($"Current best match voltage: { currentBestMatchVoltage }");

            _onGetDynamicFoxStatus(_statusToLoad);
        }
    }
}
