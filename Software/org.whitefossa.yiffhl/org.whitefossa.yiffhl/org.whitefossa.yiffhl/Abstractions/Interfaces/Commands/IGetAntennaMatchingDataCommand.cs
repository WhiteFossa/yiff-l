namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetAntennaMatchingDataResponseDelegate
        (
            bool isSuccessfull,
            int matcherPosition,
            float antennaVoltage
        );

    public interface IGetAntennaMatchingDataCommand
    {
        void SetResponseDelegate(OnGetAntennaMatchingDataResponseDelegate onGetAntennaMatchingDataResponse);

        void SendGetAntennaMatchingDataCommand(int matcherPosition);
    }
}
