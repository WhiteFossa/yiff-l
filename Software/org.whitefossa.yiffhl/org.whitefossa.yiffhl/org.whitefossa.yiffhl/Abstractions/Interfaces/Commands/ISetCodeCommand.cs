using org.whitefossa.yiffhl.Abstractions.Enums;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetCodeResponseDelegate(bool isSuccessful);

    public interface ISetCodeCommand
    {
        void SetResponseDelegate(OnSetCodeResponseDelegate onSetCodeResponse);

        void SendSetCodeCommand(FoxCode code);
    }
}
