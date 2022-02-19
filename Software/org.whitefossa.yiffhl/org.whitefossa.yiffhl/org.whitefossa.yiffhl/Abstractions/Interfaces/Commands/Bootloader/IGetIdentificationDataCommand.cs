﻿using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader
{
    public delegate void OnGetIdentificationDataResponseDelegate
    (
        bool isFoxBootloader,
        UInt16 protocolVersion,
        UInt16 hardwareRevision,
        UInt16 softwareVersion,
        UInt32 flashStartAddress,
        UInt32 mainFirmwareStartAddress,
        UInt32 flashEndAddress
     );

    public interface IGetIdentificationDataCommand
    {
        void SetResponseDelegate(OnGetIdentificationDataResponseDelegate onGetIdentificationDataResponse);

        void SendGetBootloaderIdentificationDataCommand();
    }
}