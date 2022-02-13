namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnRebootToBootloaderDelegate(bool isSuccessful);

    public interface IRebootToBootloaderCommand
    {
        void SetResponseDelegate(OnRebootToBootloaderDelegate onRebootToBootloader);

        void SendRebootToBootloaderCommand();
    }
}
