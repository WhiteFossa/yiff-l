namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetU80mFactorsResponseDelegate(float a, float b);

    public interface IGetU80mFactorsCommand
    {
        void SetResponseDelegate(OnGetU80mFactorsResponseDelegate onGetU80mFactorsResponse);

        void SendGetU80mFactorsCommand();
    }
}
