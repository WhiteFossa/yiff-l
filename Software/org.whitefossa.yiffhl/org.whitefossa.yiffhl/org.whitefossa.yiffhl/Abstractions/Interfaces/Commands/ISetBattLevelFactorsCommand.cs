namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetBattLevelFactorsResponseDelegate(bool isSuccessfull);

    public interface ISetBattLevelFactorsCommand
    {
        void SetResponseDelegate(OnSetBattLevelFactorsResponseDelegate onSetBattLevelFactorsResponse);

        void SendSetBattLevelFactors(bool resetToDefaults, float a, float b);
    }
}
