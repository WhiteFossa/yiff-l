using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetSpeedCommand : IGetSpeedCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetSpeedResponseDelegate _onGetSpeedResponse;

        public GetSpeedCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetSpeedResponse(OnGetSpeedResponse);
        }

        public void SetResponseDelegate(OnGetSpeedResponseDelegate onGetSpeedResponse)
        {
            _onGetSpeedResponse = onGetSpeedResponse;
        }

        public void SendGetSpeedCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetSpeed, new List<byte>());
        }

        private void OnGetSpeedResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetSpeedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onGetSpeedResponse(CommandsHelper.ToBool(payload.ElementAt(0)));
        }
    }
}
