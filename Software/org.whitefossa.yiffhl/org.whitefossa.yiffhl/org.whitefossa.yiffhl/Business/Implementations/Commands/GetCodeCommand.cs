using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetCodeCommand : IGetCodeCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetCodeResponseDelegate _onGetCodeResponse;

        public GetCodeCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetCodeResponse(OnGetCodeResponse);
        }

        public void SetResponseDelegate(OnGetCodeResponseDelegate onGetCodeResponse)
        {
            _onGetCodeResponse = onGetCodeResponse;
        }

        public void SendGetCodeCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetCode, new List<byte>());
        }

        private void OnGetCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            var foxCode = (FoxCode)payload.ElementAt(0);

            _onGetCodeResponse(foxCode);
        }
    }
}
