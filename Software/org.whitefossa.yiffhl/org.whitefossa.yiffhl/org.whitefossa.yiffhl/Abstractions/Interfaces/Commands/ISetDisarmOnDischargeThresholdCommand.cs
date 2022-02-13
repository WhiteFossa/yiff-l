namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetDisarmOnDischargeThresholdResponseDelegate(bool isSuccessful);

    public interface ISetDisarmOnDischargeThresholdCommand
    {
        void SetResponseDelegate(OnSetDisarmOnDischargeThresholdResponseDelegate onSetDisarmOnDischargeThresholdResponseDelegate);

        void SendSetDisarmOnDischargeThresholdCommand(float newThreshold);
    }
}
