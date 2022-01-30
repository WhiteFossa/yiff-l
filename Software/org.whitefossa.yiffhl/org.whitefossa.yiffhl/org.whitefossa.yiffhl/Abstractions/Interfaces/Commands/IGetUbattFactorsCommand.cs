namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUbattFactorsResponseDelegate(float a, float b);

    public interface IGetUbattFactorsCommand
    {
        void SetResponseDelegate(OnGetUbattFactorsResponseDelegate onGetUbattFactorsResponse);

        void SendGetUbattFactorsCommand();
    }
}
