namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetUbattFactorsResponseDelegate(bool isSuccessful);

    public interface ISetUbattFactorsCommand
    {
        void SetResponseDelegate(OnSetUbattFactorsResponseDelegate onSetUbattFactorsResponse);

        void SendSetUbattFactors(bool resetToDefaults, float a, float b);
    }
}
