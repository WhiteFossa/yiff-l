using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader;
using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands.Bootloader
{
    public class RebootToMainFirmwareCommand : IRebootToMainFirmwareCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private Abstractions.Interfaces.Commands.Bootloader.OnRebootToMainFirmwareDelegate _onRebootToMainFirmwareResponse;

        public RebootToMainFirmwareCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnRebootToMainFirmwareResponse(OnRebootToMainFirmwareResponse);
        }

        public void SetResponseDelegate(Abstractions.Interfaces.Commands.Bootloader.OnRebootToMainFirmwareDelegate onRebootToMainFirmwareResponse)
        {
            _onRebootToMainFirmwareResponse = onRebootToMainFirmwareResponse;
        }

        public void RebootToMainFirmware()
        {
            _packetsProcessor.SendCommand(CommandType.BootloaderRebootToMainFirmware, new List<byte>());
        }

        private void OnRebootToMainFirmwareResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onRebootToMainFirmwareResponse == null)
            {
                return;
            }

            if (payload.Count != 0)
            {
                return;
            }

            _onRebootToMainFirmwareResponse();
        }
    }
}
