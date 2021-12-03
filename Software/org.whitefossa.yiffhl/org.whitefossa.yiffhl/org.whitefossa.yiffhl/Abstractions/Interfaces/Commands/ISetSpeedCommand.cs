namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetSpeedResponseDelegate(bool isSuccessfull);

    public interface ISetSpeedCommand
    {
        void SetResponseDelegate(OnSetSpeedResponseDelegate onSetSpeedResponse);

        void SendSetSpeedCommand(bool isFast);
    }
}
