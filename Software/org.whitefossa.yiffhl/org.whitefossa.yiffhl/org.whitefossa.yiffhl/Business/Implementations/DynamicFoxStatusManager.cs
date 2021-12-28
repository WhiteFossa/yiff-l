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
        private readonly IMarkMatchingAsSeenCommand _markMatchingAsSeenCommand;

        private OnGetDynamicFoxStatus _onGetDynamicFoxStatus;
        private OnMarkAntennaMatchingAsSeen _onMarkAntennaMatchingAsSeen;

        private DynamicFoxStatus _statusToLoad = new DynamicFoxStatus();

        public DynamicFoxStatusManager
            (
                IGetBatteryLevelCommand getBatteryLevelCommand,
                IGetAntennaMatchingStatusCommand getAntennaMatchingStatusCommand,
                IMarkMatchingAsSeenCommand markMatchingAsSeenCommand
            )
        {
            _getBatteryLevelCommand = getBatteryLevelCommand;
            _getAntennaMatchingStatusCommand = getAntennaMatchingStatusCommand;
            _markMatchingAsSeenCommand = markMatchingAsSeenCommand;
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
            bool isNewForApp,
            int totalMatcherPositions,
            int currentMatcherPosition,
            float currentAntennaVoltage,
            int currentBestMatchPosition,
            float currentBestMatchVoltage
        )
        {
            _statusToLoad.AntennaMatchingStatus.Status = status;
            _statusToLoad.AntennaMatchingStatus.IsNewForApp = isNewForApp;
            _statusToLoad.AntennaMatchingStatus.TotalMatcherPositions = totalMatcherPositions;
            _statusToLoad.AntennaMatchingStatus.CurrentMatcherPosition = currentMatcherPosition;
            _statusToLoad.AntennaMatchingStatus.CurrentVoltage = currentAntennaVoltage;
            _statusToLoad.AntennaMatchingStatus.CurrentBestMatchPosition = currentBestMatchPosition;
            _statusToLoad.AntennaMatchingStatus.CurrentBestMatchVoltage = currentBestMatchVoltage;

            _onGetDynamicFoxStatus(_statusToLoad);
        }

        public async Task MarkAntennaMatchingAsSeen(OnMarkAntennaMatchingAsSeen onMarkAntennaMatchingAsSeen)
        {
            _onMarkAntennaMatchingAsSeen = onMarkAntennaMatchingAsSeen;

            _markMatchingAsSeenCommand.SetResponseDelegate(OnMarkAntennaMatchingAsSeen);
            _markMatchingAsSeenCommand.SendMarkMatchingAsSeenCommand();
        }

        private void OnMarkAntennaMatchingAsSeen()
        {
            _onMarkAntennaMatchingAsSeen();
        }
    }
}
