using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetRTCCalibrationValueCommand : ISetRTCCalibrationValueCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetRTCCalibrationValueResponseDelegate _onSetRTCCalibrationValueResponse;

        public SetRTCCalibrationValueCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetRTCCalibrationValueResponse(OnSetRTCCalibrationValueResponse);
        }

        public void SetResponseDelegate(OnSetRTCCalibrationValueResponseDelegate onSetRTCCalibrationValueResponse)
        {
            _onSetRTCCalibrationValueResponse = onSetRTCCalibrationValueResponse;
        }

        public void SendSetRTCCalibrationValue(uint newValue)
        {
            var payload = new List<byte>();

            // Getting lower byte
            payload.Add((byte)(newValue & 0xFF));

            _packetsProcessor.SendCommand(CommandType.SetRTCCalibrationValue, payload);
        }

        private void OnSetRTCCalibrationValueResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetRTCCalibrationValueResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetRTCCalibrationValueResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
