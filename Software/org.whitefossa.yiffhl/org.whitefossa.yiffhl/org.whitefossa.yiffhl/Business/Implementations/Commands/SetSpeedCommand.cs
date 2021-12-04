using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetSpeedCommand : ISetSpeedCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetSpeedResponseDelegate _onSetSpeedResponse;

        public SetSpeedCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetSpeedResponse(OnSetSpeedResponse);
        }

        public void SetResponseDelegate(OnSetSpeedResponseDelegate onSetSpeedResponse)
        {
            _onSetSpeedResponse = onSetSpeedResponse;
        }

        public void SendSetSpeedCommand(bool isFast)
        {
            var payload = new List<byte>();
            payload.Add(CommandsHelper.FromBool(isFast));

            _packetsProcessor.SendCommand(CommandType.SetSpeed, payload);
        }

        private void OnSetSpeedResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetSpeedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetSpeedResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
