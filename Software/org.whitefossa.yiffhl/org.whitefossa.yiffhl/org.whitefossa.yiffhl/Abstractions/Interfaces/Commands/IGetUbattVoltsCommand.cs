namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUBattVoltsResponseDelegate(float uBattVolts);

    public interface IGetUbattVoltsCommand
    {
        void SetResponseDelegate(OnGetUBattVoltsResponseDelegate onGetUBattVoltsResponse);

        void SendGetUbattVoltsCommand();
    }
}
