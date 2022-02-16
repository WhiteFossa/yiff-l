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
        private OnGetIdentificationDataDelegate _onGetIdentificationDataResponse;

        public GetIdentificationDataCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnBootloaderGetIdentificationDataResponse(OnGetIdentificationDataResponse);
        }

        public void SetResponseDelegate(OnGetIdentificationDataDelegate onGetIdentificationDataResponse)
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

            if (payload.Count != 10)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0);
                return;
            }

            var signatureBytes = payload
                .ToList()
                .GetRange(0, 4)
                .ToArray();

            var signature = BitConverter.ToUInt32(signatureBytes, 0);

            if (signature != FoxBootloaderSignature)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0);
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

            var softwareVersion = BitConverter.ToUInt16(softwareVersionBytes, 0);

            _onGetIdentificationDataResponse(true, protocolVersion, hardwareRevision, softwareVersion);
        }
    }
}
