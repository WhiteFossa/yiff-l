using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetIdentificationDataDelegate(
        bool isFox,
        uint protocolVersion,
        uint hardwareRevision,
        uint softwareVersion,
        uint serialNumber);

    public class GetIdentificationDataCommand
    {
        /// <summary>
        /// Signature, reported by fox
        /// </summary>
        private const UInt32 FoxSignature = 0xF055AF06;

        private readonly IPacketsProcessor packetsProcessor;
        private OnGetIdentificationDataDelegate onGetIdentificationDataResponse;

        public GetIdentificationDataCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            this.packetsProcessor.SetOnGetIdentificationDataResponse(OnGetIdentificationDataResponse);
        }

        public void SetResponseDelegate(OnGetIdentificationDataDelegate onGetIdentificationDataResponse)
        {
            this.onGetIdentificationDataResponse = onGetIdentificationDataResponse;
        }

        public void SendGetIdentificationDataCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetIdentificationData, new List<byte>());
        }

        private void OnGetIdentificationDataResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetIdentificationDataResponse == null)
            {
                return;
            }

            if (payload.Count != 14)
            {
                onGetIdentificationDataResponse(false, 0, 0, 0, 0);
                return;
            }

            var signatureBytes = payload
                .ToList()
                .GetRange(0, 4)
                .ToArray();

            var signature = BitConverter.ToUInt32(signatureBytes, 0);

            if (signature != FoxSignature)
            {
                onGetIdentificationDataResponse(false, 0, 0, 0, 0);
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

            onGetIdentificationDataResponse(true, protocolVersion, hardwareRevision, softwareVersion, serialNumber);
        }

    }
}
