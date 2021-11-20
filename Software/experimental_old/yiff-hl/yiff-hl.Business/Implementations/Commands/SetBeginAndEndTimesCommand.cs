using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetBeginAndEndTimesResponseDelegate(bool isSuccess);

    public class SetBeginAndEndTimesCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetBeginAndEndTimesResponseDelegate onSetBeginAndEndTimesResponse;

        private SetBeginAndEndTimesCommand()
        {

        }

        public SetBeginAndEndTimesCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetBeginAndEndTimesResponse(OnSetBeginAndEndTimesResponse);
        }

        public void SetResponseDelegate(OnSetBeginAndEndTimesResponseDelegate onSetBeginAndEndTimesResponse)
        {
            this.onSetBeginAndEndTimesResponse = onSetBeginAndEndTimesResponse;
        }

        public void SendSetBeginAndEndTimesCommand(DateTime beginTime, DateTime endTime)
        {
            var payload = new List<byte>();

            // 2th - 5th (from 0th) bytes - begin time
            payload.AddRange(BitConverter.GetBytes((uint)GetSecondsSinceMidnight(beginTime)));

            // 6th - 9th bytes - end time
            payload.AddRange(BitConverter.GetBytes((uint)GetSecondsSinceMidnight(endTime)));

            packetsProcessor.SendCommand(CommandType.SetBeginAndEndTimes, payload);
        }

        private int GetSecondsSinceMidnight(DateTime dateTime)
        {
            var midnight = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day);
            var timeSpan = dateTime - midnight;

            return (int)timeSpan.TotalSeconds;
        }

        private void OnSetBeginAndEndTimesResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetBeginAndEndTimesResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetBeginAndEndTimesResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
