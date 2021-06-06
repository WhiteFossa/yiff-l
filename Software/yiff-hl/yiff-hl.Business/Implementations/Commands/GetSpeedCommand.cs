using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetSpeedResponseDelegate(bool isFast);

    public class GetSpeedCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetSpeedResponseDelegate onGetSpeedResponse;

        public GetSpeedCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnGetSpeedResponse(OnGetSpeedResponse);
        }

        public void SetResponseDelegate(OnGetSpeedResponseDelegate onGetSpeedResponse)
        {
            this.onGetSpeedResponse = onGetSpeedResponse;
        }

        public void SendGetSpeedCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetSpeed, new List<byte>());
        }

        private void OnGetSpeedResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetSpeedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onGetSpeedResponse(CommandsHelper.ToBool(payload.ElementAt(0)));
        }
    }
}
