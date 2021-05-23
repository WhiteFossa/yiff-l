using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetProfilesCountResponseDelegate(int count);

    public class GetProfilesCountCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetProfilesCountResponseDelegate onGetProfilesCountResponse;

        public GetProfilesCountCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;

            packetsProcessor.SetOnGetProfilesCountResponse(OnGetProfilesCountResponse);
        }

        public void SetResponseDelegate(OnGetProfilesCountResponseDelegate onGetProfilesCountResponse)
        {
            this.onGetProfilesCountResponse = onGetProfilesCountResponse;
        }

        public void SendGetProfilesCountCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetProfilesCount, new List<byte>());
        }

        private void OnGetProfilesCountResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetProfilesCountResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            var count = payload.ElementAt(0);
            onGetProfilesCountResponse(count);
        }

    }
}
