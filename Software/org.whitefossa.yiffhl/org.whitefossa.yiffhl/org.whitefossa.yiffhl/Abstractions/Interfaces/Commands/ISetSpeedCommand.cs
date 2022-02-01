namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetSpeedResponseDelegate(bool isSuccessful);

    public interface ISetSpeedCommand
    {
        void SetResponseDelegate(OnSetSpeedResponseDelegate onSetSpeedResponse);

        void SendSetSpeedCommand(bool isFast);
    }
}
