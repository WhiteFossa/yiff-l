using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
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

            if (payload.Count != 13)
            {
                return;
            }

            var status = (AntennaMatchingStatus)(payload.ElementAt(0));

            var isNewForApp = CommandsHelper.ToBool(payload.ElementAt(1));

            var totalMatcherPositions = payload.ElementAt(2);

            var currentMatcherPosition = payload.ElementAt(3);

            var currentAntennaVoltageBytes = payload
                .ToList()
                .GetRange(4, 4)
                .ToArray();

            var currentAntennaVoltage = BitConverter.ToSingle(currentAntennaVoltageBytes, 0); ;

            var currentBestMatchPosition = payload.ElementAt(8);

            var currentBestMatchVoltageBytes = payload
                .ToList()
                .GetRange(9, 4)
                .ToArray();

            var currentBestMatchVoltage = BitConverter.ToSingle(currentBestMatchVoltageBytes, 0);

            _onGetAntennaMatchingStatusResponse
            (
                status,
                isNewForApp,
                totalMatcherPositions,
                currentMatcherPosition,
                currentAntennaVoltage,
                currentBestMatchPosition,
                currentBestMatchVoltage
            );
        }
    }
}
