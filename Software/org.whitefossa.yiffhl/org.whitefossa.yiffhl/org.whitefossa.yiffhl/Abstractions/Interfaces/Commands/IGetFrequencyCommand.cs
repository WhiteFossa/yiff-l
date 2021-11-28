namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetFrequencyResponseDelegate(bool is144MHz, int frequency);

    public interface IGetFrequencyCommand
    {
        void SetResponseDelegate(OnGetFrequencyResponseDelegate onGetFrequencyResponse);

        void SendGetFrequencyCommand();
    }
}
