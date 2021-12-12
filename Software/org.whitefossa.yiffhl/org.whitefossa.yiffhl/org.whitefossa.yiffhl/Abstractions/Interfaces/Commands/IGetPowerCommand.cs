namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetPowerResponseDelegate(float power);

    public interface IGetPowerCommand
    {
        void SetResponseDelegate(OnGetPowerResponseDelegate onGetPowerResponse);

        void SendGetPowerCommand();
    }
}
