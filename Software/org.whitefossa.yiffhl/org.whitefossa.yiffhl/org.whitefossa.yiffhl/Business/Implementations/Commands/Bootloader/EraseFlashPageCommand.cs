using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands.Bootloader
{
    public class EraseFlashPageCommand : IEraseFlashPageCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnEraseFlashPageResponseDelegate _onEraseFlashPageResponse;

        public EraseFlashPageCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnEraseFlashPageResponse(OnEraseFlashPageResponse);
        }

        public void SetResponseDelegate(OnEraseFlashPageResponseDelegate onEraseFlashPageResponse)
        {
            _onEraseFlashPageResponse = onEraseFlashPageResponse;
        }

        public void SendEraseFlashPageCommand(uint pageStartAddress)
        {
            var payload = new List<byte>();
            payload.AddRange(BitConverter.GetBytes(pageStartAddress));

            _packetsProcessor.SendCommand(CommandType.BootloaderEraseFlashPage, payload);
        }

        private void OnEraseFlashPageResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onEraseFlashPageResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                _onEraseFlashPageResponse(false);
            }

            _onEraseFlashPageResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
