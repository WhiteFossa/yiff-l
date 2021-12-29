namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnMarkMatchingAsSeenResponseDelegate();

    public interface IMarkMatchingAsSeenCommand
    {
        void SetResponseDelegate(OnMarkMatchingAsSeenResponseDelegate onMarkMatchingAsSeenResponse);

        void SendMarkMatchingAsSeenCommand();
    }
}
