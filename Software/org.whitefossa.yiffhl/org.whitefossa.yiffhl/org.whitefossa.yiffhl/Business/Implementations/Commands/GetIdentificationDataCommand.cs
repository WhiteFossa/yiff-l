using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public delegate void OnGetIdentificationDataDelegate
    (
        bool isFox,
        UInt16 protocolVersion,
        UInt16 hardwareRevision,
        UInt16 softwareVersion,
        UInt32 serialNumber
     );

    public class GetIdentificationDataCommand
    {
        /// <summary>
        /// Signature reported by fox
        /// </summary>
        private const UInt32 FoxSignature = 0xF055AF06;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetIdentificationDataDelegate _onGetIdentificationDataResponse;

        public GetIdentificationDataCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetIdentificationDataResponse(OnGetIdentificationDataResponse);
        }

        public void SetResponseDelegate(OnGetIdentificationDataDelegate onGetIdentificationDataResponse)
        {
            _onGetIdentificationDataResponse = onGetIdentificationDataResponse;
        }

        public void SendGetIdentificationDataCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetIdentificationData, new List<byte>());
        }

        private void OnGetIdentificationDataResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetIdentificationDataResponse == null)
            {
                return;
            }

            if (payload.Count != 14)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0, 0);
                return;
            }

            var signatureBytes = payload
                .ToList()
                .GetRange(0, 4)
                .ToArray();

            var signature = BitConverter.ToUInt32(signatureBytes, 0);

            if (signature != FoxSignature)
            {
                _onGetIdentificationDataResponse(false, 0, 0, 0, 0);
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

            var serialNumberBytes = payload
                .ToList()
                .GetRange(10, 4)
                .ToArray();

            var serialNumber = BitConverter.ToUInt32(serialNumberBytes, 0);

            _onGetIdentificationDataResponse(true, protocolVersion, hardwareRevision, softwareVersion, serialNumber);
        }
    }
}
