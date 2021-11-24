using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetIdentificationDataDelegate
    (
        bool isFox,
        UInt16 protocolVersion,
        UInt16 hardwareRevision,
        UInt16 softwareVersion,
        UInt32 serialNumber
     );

    public interface IGetIdentificationDataCommand
    {
        void SetResponseDelegate(OnGetIdentificationDataDelegate onGetIdentificationDataResponse);

        void SendGetIdentificationDataCommand();
    }
}
