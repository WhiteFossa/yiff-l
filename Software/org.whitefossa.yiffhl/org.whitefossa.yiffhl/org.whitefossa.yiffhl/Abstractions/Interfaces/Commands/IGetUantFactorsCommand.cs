namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetUantFactorsResponseDelegate(float a, float b);

    public interface IGetUantFactorsCommand
    {
        void SetResponseDelegate(OnGetUantFactorsResponseDelegate onGetUantFactorsResponse);

        void SendGetUantFactorsCommand();
    }
}
