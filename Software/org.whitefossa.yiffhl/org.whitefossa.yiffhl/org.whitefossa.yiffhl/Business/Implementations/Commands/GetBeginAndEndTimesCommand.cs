using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetBeginAndEndTimesCommand : IGetBeginAndEndTimesCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetBeginAndEndTimesResponseDelegate _onGetBeginAndEndTimesResponse;

        public GetBeginAndEndTimesCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetBeginAndEndTimesResponse(OnGetBeginAndEndTimesResponse);
        }

        public void SetResponseDelegate(OnGetBeginAndEndTimesResponseDelegate onGetBeginAndEndTimesResponse)
        {
            _onGetBeginAndEndTimesResponse = onGetBeginAndEndTimesResponse;
        }

        public void SendGetBeginAndEndTimesCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetBeginAndEndTimes, new List<byte>());
        }

        private void OnGetBeginAndEndTimesResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetBeginAndEndTimesResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var beginTimeBytes = payload
                .ToList()
                .GetRange(0, 4)
                .ToArray();

            var beginTime = BytesToDateTime(beginTimeBytes);

            var endTimeBytes = payload
                .ToList()
                .GetRange(4, 4)
                .ToArray();

            var endTime = BytesToDateTime(endTimeBytes);

            _onGetBeginAndEndTimesResponse(beginTime, endTime);
        }

        private DateTime BytesToDateTime(byte[] payload)
        {
            if (payload.Count() != 4)
            {
                throw new ArgumentException("Payload must be 4 bytes", nameof(payload));
            }

            var secondsSinceMidnight = BitConverter.ToUInt32(payload, 0);

            var time = DateTime.MinValue;
            time = time.AddSeconds(secondsSinceMidnight);

            return new DateTime(DateTime.MinValue.Year, DateTime.MinValue.Month, DateTime.MinValue.Day, time.Hour, time.Minute, time.Second);
        }
    }
}
