namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnResetLastErrorCodeDelegate();

    public interface IResetLastErrorCodeCommand
    {
        void SetResponseDelegate(OnResetLastErrorCodeDelegate onResetLastErrorCode);

        void SendResetLastErrorCodeCommand();
    }
}
