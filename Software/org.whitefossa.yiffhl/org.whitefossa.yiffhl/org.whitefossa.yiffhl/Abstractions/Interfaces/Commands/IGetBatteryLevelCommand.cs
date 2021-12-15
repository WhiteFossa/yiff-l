namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetBatteryLevelResponseDelegate(float level);

    public interface IGetBatteryLevelCommand
    {
        void SetResponseDelegate(OnGetBatteryLevelResponseDelegate onGetBatteryLevelResponse);

        void SendGetBatteryLevelCommand();
    }
}
