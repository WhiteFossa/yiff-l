namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnDisarmFoxResponseDelegate(bool isSuccessful);

    public interface IDisarmFoxCommand
    {
        void SetResponseDelegate(OnDisarmFoxResponseDelegate onDisarmFoxResponse);

        void SendDisarmFoxCommand();
    }
}
