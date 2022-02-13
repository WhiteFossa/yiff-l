using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class RebootToBootloaderCommand : IRebootToBootloaderCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private Abstractions.Interfaces.Commands.OnRebootToBootloaderDelegate _onRebootToBootloader;

        public RebootToBootloaderCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnRebootToBootloaderResponse(OnRebootToBootloaderResponse);
        }

        public void SetResponseDelegate(Abstractions.Interfaces.Commands.OnRebootToBootloaderDelegate onRebootToBootloader)
        {
            _onRebootToBootloader = onRebootToBootloader;
        }

        public void SendRebootToBootloaderCommand()
        {
            _packetsProcessor.SendCommand(CommandType.RebootToBootloader, new List<byte>());
        }

        private void OnRebootToBootloaderResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onRebootToBootloader == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onRebootToBootloader(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
