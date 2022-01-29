namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUantADCResponseDelegate(float uantADC);

    public interface IGetUantADCCommand
    {
        void SetResponseDelegate(OnGetUantADCResponseDelegate onGetUantADCResponse);

        void SendGetUantADCCommand();
    }
}
