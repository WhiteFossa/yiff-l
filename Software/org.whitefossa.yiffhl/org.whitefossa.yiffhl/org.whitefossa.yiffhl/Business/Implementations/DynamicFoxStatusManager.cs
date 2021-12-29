using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class DynamicFoxStatusManager : IDynamicFoxStatusManager
    {
        private readonly IGetBatteryLevelCommand _getBatteryLevelCommand;
        private readonly IAntennaMatchingManager _antennaMatchingManager;

        private OnGetDynamicFoxStatus _onGetDynamicFoxStatus;

        private DynamicFoxStatus _statusToLoad = new DynamicFoxStatus();

        public DynamicFoxStatusManager
            (
                IGetBatteryLevelCommand getBatteryLevelCommand,
                IAntennaMatchingManager antennaMatchingManager
            )
        {
            _getBatteryLevelCommand = getBatteryLevelCommand;
            _antennaMatchingManager = antennaMatchingManager;
        }

        public async Task GetDynamicFoxStatusAsync(OnGetDynamicFoxStatus onGetDynamicFoxStatus)
        {
            _onGetDynamicFoxStatus = onGetDynamicFoxStatus ?? throw new ArgumentNullException(nameof(onGetDynamicFoxStatus));

            _getBatteryLevelCommand.SetResponseDelegate(async (l) => await OnGetBatteryLevelResponseAsync(l));
            _getBatteryLevelCommand.SendGetBatteryLevelCommand();
        }

        private async Task OnGetBatteryLevelResponseAsync(float level)
        {
            _statusToLoad.BatteryLevel = level;

            await _antennaMatchingManager.GetAntennaMatchingStatusAsync(OnGetAntennaMatchingStatus);
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
    }
}
