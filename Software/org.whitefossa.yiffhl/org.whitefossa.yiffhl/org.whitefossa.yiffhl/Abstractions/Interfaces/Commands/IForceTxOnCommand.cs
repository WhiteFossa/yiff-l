namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnForceTxOnResponseDelegate(bool isSuccessful);

    public interface IForceTxOnCommand
    {
        void SetResponseDelegate(OnForceTxOnResponseDelegate onForceTxOnResponse);

        void SendForceTxOnCommand();
    }
}
