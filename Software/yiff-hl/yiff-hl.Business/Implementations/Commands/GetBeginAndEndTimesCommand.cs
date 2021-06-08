using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    /// <summary>
    /// Times returned relative to BaseDate
    /// </summary>
    public delegate void OnGetBeginAndEndTimesResponseDelegate(DateTime beginTime, DateTime dateTime);

    public class GetBeginAndEndTimesCommand
    {
        public static readonly DateTime BaseDate = new DateTime(1970, 1, 1);

        private readonly IPacketsProcessor packetsProcessor;
        private OnGetBeginAndEndTimesResponseDelegate onGetBeginAndEndTimesResponse;

        private GetBeginAndEndTimesCommand()
        {

        }

        public GetBeginAndEndTimesCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetBeginAndEndTimesResponse(OnGetBeginAndEndTimesResponse);
        }

        public void SetResponseDelegate(OnGetBeginAndEndTimesResponseDelegate onGetBeginAndEndTimesResponse)
        {
            this.onGetBeginAndEndTimesResponse = onGetBeginAndEndTimesResponse;
        }

        public void SendGetBeginAndEndTimesCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetBeginAndEndTimes, new List<byte>());
        }

        private void OnGetBeginAndEndTimesResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetBeginAndEndTimesResponse == null)
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

            onGetBeginAndEndTimesResponse(beginTime, endTime);
        }

        private DateTime BytesToDateTime(byte[] payload)
        {
            if (payload.Count() != 4)
            {
                throw new ArgumentException("Payload must be 4 bytes", nameof(payload));
            }

            var secondsSinceMidnight = BitConverter.ToUInt32(payload, 0);
            var time = BaseDate; // We don't care about day, month and year yet
            time = time.AddSeconds(secondsSinceMidnight);

            return time;
        }
    }
}
