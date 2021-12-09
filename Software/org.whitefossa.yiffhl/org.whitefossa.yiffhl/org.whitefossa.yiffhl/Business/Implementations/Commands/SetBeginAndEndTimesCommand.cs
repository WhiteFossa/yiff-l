using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetBeginAndEndTimesCommand : ISetBeginAndEndTimesCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetBeginAndEndTimesResponseDelegate _onSetBeginAndEndTimesResponse;

        public SetBeginAndEndTimesCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetBeginAndEndTimesResponse(OnSetBeginAndEndTimesResponse);
        }

        public void SetResponseDelegate(OnSetBeginAndEndTimesResponseDelegate onSetBeginAndEndTimesResponse)
        {
            _onSetBeginAndEndTimesResponse = onSetBeginAndEndTimesResponse;
        }

        public void SendSetBeginAndEndTimesCommand(DateTime beginTime, DateTime endTime)
        {
            var payload = new List<byte>();

            // 2th - 5th (from 0th) bytes - begin time
            payload.AddRange(BitConverter.GetBytes((uint)GetSecondsSinceMidnight(beginTime)));

            // 6th - 9th bytes - end time
            payload.AddRange(BitConverter.GetBytes((uint)GetSecondsSinceMidnight(endTime)));

            _packetsProcessor.SendCommand(CommandType.SetBeginAndEndTimes, payload);
        }

        private int GetSecondsSinceMidnight(DateTime dateTime)
        {
            var midnight = new DateTime(dateTime.Year, dateTime.Month, dateTime.Day);
            var timeSpan = dateTime - midnight;

            return (int)timeSpan.TotalSeconds;
        }

        private void OnSetBeginAndEndTimesResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetBeginAndEndTimesResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetBeginAndEndTimesResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
