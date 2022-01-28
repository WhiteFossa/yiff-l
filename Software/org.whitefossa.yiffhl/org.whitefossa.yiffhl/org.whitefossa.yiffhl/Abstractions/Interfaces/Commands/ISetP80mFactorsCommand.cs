namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetP80mFactorsResponseDelegate(bool isSuccessfull);

    public interface ISetP80mFactorsCommand
    {
        void SetResponseDelegate(OnSetP80mFactorsResponseDelegate onSetP80mFactorsResponse);

        void SendSetP80mFactors(bool resetToDefaults, float a, float b);
    }
}
