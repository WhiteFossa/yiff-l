namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetU80mFactorsResponseDelegate(bool isSuccessfull);

    public interface ISetU80mFactorsCommand
    {
        void SetResponseDelegate(OnSetU80mFactorsResponseDelegate onSetU80mFactorsResponse);

        void SendSetU80mFactors(bool resetToDefaults, float a, float b);
    }
}
