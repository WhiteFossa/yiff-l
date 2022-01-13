using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using OnGetLastErrorCodeDelegate = org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.OnGetLastErrorCodeDelegate;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetLastErrorCodeCommand : IGetLastErrorCodeCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetLastErrorCodeDelegate _onGetLastErrorCodeResponse;

        public GetLastErrorCodeCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetLastFailureCodeResponse(OnGetLastErrorCodeResponse);
        }

        public void SetResponseDelegate(OnGetLastErrorCodeDelegate onGetLastErrorCode)
        {
            _onGetLastErrorCodeResponse = onGetLastErrorCode;
        }

        public void SendGetLastErrorCodeCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetLastFailureCode, new List<byte>());
        }

        private void OnGetLastErrorCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetLastErrorCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var errcode = BitConverter.ToUInt32(payload.ToArray(), 0);

            _onGetLastErrorCodeResponse(errcode);
        }

    }
}
