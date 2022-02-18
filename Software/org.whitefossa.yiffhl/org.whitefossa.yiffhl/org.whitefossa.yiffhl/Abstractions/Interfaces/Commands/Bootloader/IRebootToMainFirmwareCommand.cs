namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader
{
    public delegate void OnRebootToMainFirmwareDelegate();

    public interface IRebootToMainFirmwareCommand
    {
        void SetResponseDelegate(OnRebootToMainFirmwareDelegate onRebootToMainFirmwareResponse);

        void RebootToMainFirmware();
    }
}
