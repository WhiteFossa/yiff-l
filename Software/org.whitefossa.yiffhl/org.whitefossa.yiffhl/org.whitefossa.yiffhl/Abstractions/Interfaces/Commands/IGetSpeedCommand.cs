namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetSpeedResponseDelegate(bool isFast);

    public interface IGetSpeedCommand
    {
        void SetResponseDelegate(OnGetSpeedResponseDelegate onGetSpeedResponse);

        void SendGetSpeedCommand();
    }
}
