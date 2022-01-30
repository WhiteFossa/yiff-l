using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class UpdateSerialNumberCommand : IUpdateSerialNumberCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSerialNumberUpdatedDelegate _onSerialNumberUpdated;

        public UpdateSerialNumberCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSerialNumberUpdatedResponse(OnSerialNumberUpdatedResponse);
        }

        public void SetResponseDelegate(OnSerialNumberUpdatedDelegate onSerialNumberUpdated)
        {
            _onSerialNumberUpdated = onSerialNumberUpdated;
        }

        public void SendUpdateSerialNumberCommand(uint newSerialNuber)
        {
            var payload = new List<byte>();

            payload.AddRange(BitConverter.GetBytes((uint)newSerialNuber));

            _packetsProcessor.SendCommand(CommandType.UpdateSerialNumber, payload);
        }

        private void OnSerialNumberUpdatedResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSerialNumberUpdated == null)
            {
                return;
            }

            if (payload.Count != 0)
            {
                return;
            }

            _onSerialNumberUpdated();
        }


    }
}
