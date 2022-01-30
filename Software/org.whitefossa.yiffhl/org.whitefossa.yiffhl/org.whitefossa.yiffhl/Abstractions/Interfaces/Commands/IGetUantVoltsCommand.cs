namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUantVoltsResponseDelegate(float uantVolts);

    public interface IGetUantVoltsCommand
    {
        void SetResponseDelegate(OnGetUantVoltsResponseDelegate onGetUantVoltsResponse);

        void SendGetUantVoltsCommand();
    }
}
