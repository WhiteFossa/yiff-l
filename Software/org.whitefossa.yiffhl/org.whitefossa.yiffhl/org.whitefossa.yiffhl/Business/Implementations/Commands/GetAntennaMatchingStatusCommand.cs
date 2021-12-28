using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetAntennaMatchingStatusCommand : IGetAntennaMatchingStatusCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetAntennaMatchingStatusResponseDelegate _onGetAntennaMatchingStatusResponse;

        public GetAntennaMatchingStatusCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetAntennaMatchingStatusResponse(OnGetAntennaMatchingStatusResponse);
        }

        public void SetResponseDelegate(OnGetAntennaMatchingStatusResponseDelegate onGetAntennaMatchingStatusResponse)
        {
            _onGetAntennaMatchingStatusResponse = onGetAntennaMatchingStatusResponse;
        }

        public void SendGetAntennaMatchingStatusCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetAntennaMatchingStatus, new List<byte>());
        }

        private void OnGetAntennaMatchingStatusResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetAntennaMatchingStatusResponse == null)
            {
                return;
            }

            if (payload.Count != 12)
            {
                return;
            }

            var status = (AntennaMatchingStatus)(payload
                .ToList()
                .GetRange(0, 1)
                .First());

            var timeSinceLastInitiationBytes = payload
                .ToList()
                .GetRange(1, 4)
                .ToArray();

            // Overflow must never happen
            var timeSinceLastInitiation = new TimeSpan(0, 0, (Int32)BitConverter.ToUInt32(timeSinceLastInitiationBytes, 0));

            var totalMatcherPositions = payload
                .ToList()
                .GetRange(5, 1)
                .First();

            var currentMatcherPosition = payload
                .ToList()
                .GetRange(6, 1)
                .First();

            var currentBestMatchPosition = payload
                .ToList()
                .GetRange(7, 1)
                .First();

            var currentBestMatchVoltageBytes = payload
                .ToList()
                .GetRange(8, 4)
                .ToArray();

            var currentBestMatchVoltage = BitConverter.ToSingle(currentBestMatchVoltageBytes, 0);

            _onGetAntennaMatchingStatusResponse
            (
                status,
                timeSinceLastInitiation,
                totalMatcherPositions,
                currentMatcherPosition,
                currentBestMatchPosition,
                currentBestMatchVoltage
            );
        }
    }
}
