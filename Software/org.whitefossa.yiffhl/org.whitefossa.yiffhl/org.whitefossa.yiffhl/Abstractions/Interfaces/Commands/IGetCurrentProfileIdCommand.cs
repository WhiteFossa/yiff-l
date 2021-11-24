namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetCurrentProfileIdResponseDelegate(int profileId);

    public interface IGetCurrentProfileIdCommand
    {
        void SetResponseDelegate(OnGetCurrentProfileIdResponseDelegate onGetCurrentProfileIdResponse);

        void SendGetCurrentProfileIdCommand();
    }
}
