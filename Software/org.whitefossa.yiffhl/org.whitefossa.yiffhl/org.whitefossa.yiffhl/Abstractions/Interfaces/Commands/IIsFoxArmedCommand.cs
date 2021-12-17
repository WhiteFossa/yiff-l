namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnIsFoxArmedResponseDelegate(bool isArmed);

    public interface IIsFoxArmedCommand
    {
        void SetResponseDelegate(OnIsFoxArmedResponseDelegate onIsFoxArmedResponse);

        void SendIsFoxArmedCommand();
    }
}
