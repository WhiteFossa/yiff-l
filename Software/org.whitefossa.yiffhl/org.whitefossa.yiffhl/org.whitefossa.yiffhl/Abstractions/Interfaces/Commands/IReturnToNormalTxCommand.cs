namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnReturnToNormalTxResponseDelegate(bool isSuccessful);

    public interface IReturnToNormalTxCommand
    {
        void SetResponseDelegate(OnReturnToNormalTxResponseDelegate onReturnToNormalTxResponse);

        void SendReturnToNormalTxCommand();
    }
}
