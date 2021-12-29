using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetAntennaMatchingDataCommand : IGetAntennaMatchingDataCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;

        private int _matcherPosition;

        private OnGetAntennaMatchingDataResponseDelegate _onGetAntennaMatchingDataResponse;

        public GetAntennaMatchingDataCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetAntennaMatchingDataResponse(OnGetAntennaMatchingDataResponse);
        }

        public void SetResponseDelegate(OnGetAntennaMatchingDataResponseDelegate onGetAntennaMatchingDataResponse)
        {
            _onGetAntennaMatchingDataResponse = onGetAntennaMatchingDataResponse;
        }

        public void SendGetAntennaMatchingDataCommand(int matcherPosition)
        {
            _matcherPosition = matcherPosition;

            var payload = new List<byte>();
            payload.Add((byte)matcherPosition);
            _packetsProcessor.SendCommand(CommandType.GetAntennaMatchingData, payload);
        }

        private void OnGetAntennaMatchingDataResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetAntennaMatchingDataResponse == null)
            {
                return;
            }

            if (payload.Count != 5)
            {
                return;
            }

            if (!CommandsHelper.IsSuccessful(payload.ElementAt(0)))
            {
                _onGetAntennaMatchingDataResponse(false, _matcherPosition, 0);
            }

            var voltageBytes = payload
                .ToList()
                .GetRange(1, 4)
                .ToArray();

            var voltage = BitConverter.ToSingle(voltageBytes, 0);

            _onGetAntennaMatchingDataResponse(true, _matcherPosition, voltage);
        }
    }
}
