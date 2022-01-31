namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnReturnToNormalTxResponseDelegate(bool isSuccessfull);

    public interface IReturnToNormalTxCommand
    {
        void SetResponseDelegate(OnReturnToNormalTxResponseDelegate onReturnToNormalTxResponse);

        void SendReturnToNormalTxCommand();
    }
}
