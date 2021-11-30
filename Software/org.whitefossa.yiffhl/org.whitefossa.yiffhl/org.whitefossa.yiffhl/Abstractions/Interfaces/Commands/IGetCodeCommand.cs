using org.whitefossa.yiffhl.Abstractions.Enums;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetCodeResponseDelegate(FoxCode code);

    public interface IGetCodeCommand
    {
        void SetResponseDelegate(OnGetCodeResponseDelegate onGetCodeResponse);

        void SendGetCodeCommand();
    }
}
