namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetP80mFactorsResponseDelegate(float a, float b);

    public interface IGetP80mFactorsCommand
    {
        void SetResponseDelegate(OnGetP80mFactorsResponseDelegate onGetP80mFactorsResponse);

        void SendGetP80mFactorsCommand();
    }
}
