using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands.Bootloader
{
    public class GetIdentificationDataCommand : IGetIdentificationDataCommand
    {
        /// <summary>
        /// Signature reported by fox bootloader
        /// </summary>
        private const UInt32 FoxBootloaderSignature = 0xF055AF06;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetIdentificationDataResponseDelegate _onGetIdentificationDataResponse;

        public GetIdentificationDataCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnBootloaderGetIdentificationDataResponse(OnGetIdentificationDataResponse);
        }

        public void SetResponseDelegate(OnGetIdentificationDataResponseDelegate onGetIdentificationDataResponse)
        {
            _onGetIdentificationDataResponse = onGetIdentificationDataResponse;
        }

        public void SendGetBootloaderIdentificationDataCommand()
        {
            _packetsProcessor.SendCommand(CommandType.BootloaderGetIdentificationData, new List<byte>());
        }

        private void OnGetIdentificationDataResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetIdentificationDataResponse == null)
            {
                return;
            }

            if (payload.Count != 22)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0, 0, 0, 0);
                return;
            }

            var signatureBytes = payload
                .ToList()
                .GetRange(0, 4)
                .ToArray();

            var signature = BitConverter.ToUInt32(signatureBytes, 0);

            if (signature != FoxBootloaderSignature)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0, 0, 0, 0);
                return;
            }

            var protocolVersionBytes = payload
                .ToList()
                .GetRange(4, 2)
                .ToArray();

            var protocolVersion = BitConverter.ToUInt16(protocolVersionBytes, 0);


            var hardwareRevisionBytes = payload
                .ToList()
                .GetRange(6, 2)
                .ToArray();

            var hardwareRevision = BitConverter.ToUInt16(hardwareRevisionBytes, 0);

            var softwareVersionBytes = payload
                .ToList()
                .GetRange(8, 2)
                .ToArray();

            var firmwareVersion = BitConverter.ToUInt16(softwareVersionBytes, 0);

            var flashStartAddressBytes = payload
                .ToList()
                .GetRange(10, 4)
                .ToArray();

            var flashStartAddress = BitConverter.ToUInt32(flashStartAddressBytes, 0);

            var mainFirmwareStartAddressBytes = payload
                .ToList()
                .GetRange(14, 4)
                .ToArray();

            var mainFirmwareStartAddress = BitConverter.ToUInt32(mainFirmwareStartAddressBytes, 0);

            var flashEndAddressBytes = payload
                .ToList()
                .GetRange(18, 4)
                .ToArray();

            var flashEndAddress = BitConverter.ToUInt32(flashEndAddressBytes, 0);

            _onGetIdentificationDataResponse
             (
                true,
                protocolVersion,
                hardwareRevision,
                firmwareVersion,
                flashStartAddress,
                mainFirmwareStartAddress,
                flashEndAddress
             );
        }
    }
}
