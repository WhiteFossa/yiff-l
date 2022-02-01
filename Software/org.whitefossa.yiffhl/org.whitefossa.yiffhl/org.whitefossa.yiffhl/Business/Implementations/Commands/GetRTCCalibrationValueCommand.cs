using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetRTCCalibrationValueCommand : IGetRTCCalibrationValueCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetRTCCalibrationValueResponseDelegate _onGetRTCCalibrationValueResponse;

        public GetRTCCalibrationValueCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetRTCCalibrationValueResponse(OnGetRTCCalibrationValueResponse);
        }

        public void SetResponseDelegate(OnGetRTCCalibrationValueResponseDelegate onGetRTCCalibrationValueResponse)
        {
            _onGetRTCCalibrationValueResponse = onGetRTCCalibrationValueResponse;
        }

        public void SendGetRTCCalibrationValue()
        {
            _packetsProcessor.SendCommand(CommandType.GetRTCCalibrationValue, new List<byte>());
        }

        private void OnGetRTCCalibrationValueResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetRTCCalibrationValueResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onGetRTCCalibrationValueResponse(payload.First());
        }
    }
}
