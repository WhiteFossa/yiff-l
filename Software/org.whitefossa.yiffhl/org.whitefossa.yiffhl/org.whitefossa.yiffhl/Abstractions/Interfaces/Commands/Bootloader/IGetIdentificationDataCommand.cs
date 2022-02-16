using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader
{
    public delegate void OnGetIdentificationDataDelegate
    (
        bool isFoxBootloader,
        UInt16 protocolVersion,
        UInt16 hardwareRevision,
        UInt16 softwareVersion
     );

    public interface IGetIdentificationDataCommand
    {
        void SetResponseDelegate(OnGetIdentificationDataDelegate onGetIdentificationDataResponse);

        void SendGetBootloaderIdentificationDataCommand();
    }
}
