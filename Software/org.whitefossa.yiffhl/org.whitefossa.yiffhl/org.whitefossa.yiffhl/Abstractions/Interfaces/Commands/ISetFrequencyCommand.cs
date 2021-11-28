namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetFrequencyResponseDelegate(bool isSuccess);

    public interface ISetFrequencyCommand
    {
        void SetResponseDelegate(OnSetFrequencyResponseDelegate onSetFrequencyResponse);

        void SendSetFrequencyCommand(bool is144MHz, int frequency);
    }
}
