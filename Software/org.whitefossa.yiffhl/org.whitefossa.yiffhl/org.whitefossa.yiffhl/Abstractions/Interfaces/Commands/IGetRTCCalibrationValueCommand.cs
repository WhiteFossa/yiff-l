namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetRTCCalibrationValueResponseDelegate(uint value);

    public interface IGetRTCCalibrationValueCommand
    {
        void SetResponseDelegate(OnGetRTCCalibrationValueResponseDelegate onGetRTCCalibrationValueResponse);

        void SendGetRTCCalibrationValue();
    }
}
