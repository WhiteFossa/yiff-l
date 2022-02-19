using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.Bootloader;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands.Bootloader
{
    public class ReadFlashPageCommand : IReadFlashPageCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnReadFlashPageResponseDelegate _onReadFlashPageResponse;

        public ReadFlashPageCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnReadFlashPageResponse(OnReadFlashPageResponse);
        }

        public void SetResponseDelegate(OnReadFlashPageResponseDelegate onReadFlashPageResponse)
        {
            _onReadFlashPageResponse = onReadFlashPageResponse;
        }

        public void SendReadFlashPageCommand(uint pageNumber)
        {
            var payload = new List<byte>();
            payload.AddRange(BitConverter.GetBytes(pageNumber));

            _packetsProcessor.SendCommand(CommandType.BootloaderReadFlashPage, payload);
        }

        private void OnReadFlashPageResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onReadFlashPageResponse == null)
            {
                return;
            }

            if (payload.Count != 33)
            {
                _onReadFlashPageResponse(false, new List<byte>());
            }

            if (!CommandsHelper.IsSuccessful(payload.ElementAt(0)))
            {
                _onReadFlashPageResponse(false, new List<byte>());
            }

            _onReadFlashPageResponse(true, payload.ToList().GetRange(1, 32));
        }
    }
}
