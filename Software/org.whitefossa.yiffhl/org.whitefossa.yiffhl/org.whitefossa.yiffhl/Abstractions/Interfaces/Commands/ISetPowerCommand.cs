namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetPowerResponseDelegate(bool isSuccessfull);

    public interface ISetPowerCommand
    {
        void SetResponseDelegate(OnSetPowerResponseDelegate onSetFoxPowerResponse);

        void SendSetPowerCommand(float power);
    }
}
