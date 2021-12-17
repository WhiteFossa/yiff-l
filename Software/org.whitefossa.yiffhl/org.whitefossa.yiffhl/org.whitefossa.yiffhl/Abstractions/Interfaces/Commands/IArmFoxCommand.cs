namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnArmFoxResponseDelegate(bool isSuccessful);

    public interface IArmFoxCommand
    {
        void SetResponseDelegate(OnArmFoxResponseDelegate onArmFoxResponse);

        void SendArmFoxCommand();
    }
}
