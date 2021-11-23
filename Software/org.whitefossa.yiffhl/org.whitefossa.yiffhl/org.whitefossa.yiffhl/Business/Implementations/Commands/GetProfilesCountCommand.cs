using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public delegate void OnGetProfilesCountResponseDelegate(int count);

    public class GetProfilesCountCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetProfilesCountResponseDelegate _onGetProfilesCountResponse;

        public GetProfilesCountCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetProfilesCountResponse(OnGetProfilesCountResponse);
        }

        public void SetResponseDelegate(OnGetProfilesCountResponseDelegate onGetProfilesCountResponse)
        {
            _onGetProfilesCountResponse = onGetProfilesCountResponse;
        }

        public void SendGetProfilesCountCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetProfilesCount, new List<byte>());
        }

        private void OnGetProfilesCountResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetProfilesCountResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            var count = payload.ElementAt(0);
            _onGetProfilesCountResponse(count);
        }
    }
}
