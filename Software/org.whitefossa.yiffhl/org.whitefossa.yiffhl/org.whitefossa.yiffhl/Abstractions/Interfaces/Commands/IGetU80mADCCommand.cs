namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetU80mADCResponseDelegate(float u80mADC);

    public interface IGetU80mADCCommand
    {
        void SetResponseDelegate(OnGetU80mADCResponseDelegate onGetU80mADCResponse);

        void SendGetU80mADCCommand();
    }
}
