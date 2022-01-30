namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetU80mVoltsResponseDelegate(float averageVoltage);

    public interface IGetU80mVoltsCommand
    {
        void SetResponseDelegate(OnGetU80mVoltsResponseDelegate onGetU80MVoltsResponse);

        void SendGetU80mVoltsCommand();
    }
}
