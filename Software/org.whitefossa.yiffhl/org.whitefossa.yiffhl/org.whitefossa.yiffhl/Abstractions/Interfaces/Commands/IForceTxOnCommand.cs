namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnForceTxOnResponseDelegate(bool isSuccessfull);

    public interface IForceTxOnCommand
    {
        void SetResponseDelegate(OnForceTxOnResponseDelegate onForceTxOnResponse);

        void SendForceTxOnCommand();
    }
}
