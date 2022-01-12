namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetLastErrorCodeDelegate(uint lastErrorCode);

    public interface IGetLastErrorCodeCommand
    {
        void SetResponseDelegate(OnGetLastErrorCodeDelegate onGetLastErrorCode);

        void SendGetLastErrorCodeCommand();
    }
}
