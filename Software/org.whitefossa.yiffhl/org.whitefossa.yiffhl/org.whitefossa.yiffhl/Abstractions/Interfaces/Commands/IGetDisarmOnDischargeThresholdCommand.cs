namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetDisarmOnDischargeThresholdResponseDelegate(float threshold);

    public interface IGetDisarmOnDischargeThresholdCommand
    {
        void SetResponseDelegate(OnGetDisarmOnDischargeThresholdResponseDelegate onGetDisarmOnDischargeThresholdResponse);

        void SendGetDisarmOnDischargeThreshold();
    }
}
