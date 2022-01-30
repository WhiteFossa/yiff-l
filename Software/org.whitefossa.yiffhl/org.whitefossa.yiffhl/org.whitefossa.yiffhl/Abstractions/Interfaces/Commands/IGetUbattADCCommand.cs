namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUBattADCResponseDelegate(float uBattADC);

    public interface IGetUbattADCCommand
    {
        void SetResponseDelegate(OnGetUBattADCResponseDelegate onGetUBattADCResponse);

        void SendGetUbattADCCommand();
    }
}
