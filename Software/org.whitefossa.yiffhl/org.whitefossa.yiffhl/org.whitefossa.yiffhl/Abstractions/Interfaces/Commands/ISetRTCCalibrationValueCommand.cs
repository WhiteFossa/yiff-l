namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetRTCCalibrationValueResponseDelegate(bool isSuccessful);

    public interface ISetRTCCalibrationValueCommand
    {
        void SetResponseDelegate(OnSetRTCCalibrationValueResponseDelegate onSetRTCCalibrationValueResponse);

        void SendSetRTCCalibrationValue(uint newValue);
    }
}
