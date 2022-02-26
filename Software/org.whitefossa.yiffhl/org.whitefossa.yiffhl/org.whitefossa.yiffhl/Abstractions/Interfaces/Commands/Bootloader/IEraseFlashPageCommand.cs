namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader
{
    public delegate void OnEraseFlashPageResponseDelegate(bool isSuccessful);

    public interface IEraseFlashPageCommand
    {
        void SetResponseDelegate(OnEraseFlashPageResponseDelegate onEraseFlashPageResponse);

        void SendEraseFlashPageCommand(uint pageStartAddress);
    }
}
