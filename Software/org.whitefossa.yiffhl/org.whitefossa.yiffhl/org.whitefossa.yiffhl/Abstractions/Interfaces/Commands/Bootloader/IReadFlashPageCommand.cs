using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader
{
    public delegate void OnReadFlashPageResponseDelegate
        (
            bool isSuccessful,
            List<byte> data
        );

    public interface IReadFlashPageCommand
    {
        void SetResponseDelegate(OnReadFlashPageResponseDelegate onReadFlashPageResponse);

        void SendReadFlashPageCommand(uint pageNumber);
    }
}
