using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxStatusManager : IFoxStatusManager
    {
        private readonly IGetBatteryLevelCommand _getBatteryLevelCommand;

        private OnGetFoxStatus _onGetFoxStatus;

        private FoxStatus _statusToLoad = new FoxStatus();

        public FoxStatusManager(IGetBatteryLevelCommand getBatteryLevelCommand)
        {
            _getBatteryLevelCommand = getBatteryLevelCommand;
        }

        public async Task GetFoxStatusAsync(OnGetFoxStatus onGetFoxStatus)
        {
            _onGetFoxStatus = onGetFoxStatus ?? throw new ArgumentNullException(nameof(onGetFoxStatus));

            _getBatteryLevelCommand.SetResponseDelegate(OnGetBatteryLevelResponse);
            _getBatteryLevelCommand.SendGetBatteryLevelCommand();
        }

        private void OnGetBatteryLevelResponse(float level)
        {
            _statusToLoad.BatteryLevel = level;

            _onGetFoxStatus(_statusToLoad);
        }
    }
}
