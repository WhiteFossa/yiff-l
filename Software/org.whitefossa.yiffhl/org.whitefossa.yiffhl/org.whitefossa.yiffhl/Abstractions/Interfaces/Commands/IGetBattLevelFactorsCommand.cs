namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetBattLevelFactorsResponseDelegate(float a, float b);

    public interface IGetBattLevelFactorsCommand
    {
        void SetResponseDelegate(OnGetBattLevelFactorsResponseDelegate onGetBattLevelFactorsResponse);

        void SendGetBattLevelFactorsCommand();
    }
}
