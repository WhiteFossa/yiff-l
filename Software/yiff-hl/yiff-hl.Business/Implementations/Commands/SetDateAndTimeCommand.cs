using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetDateAndTimeResponseDelegate(bool isSuccessfull);

    public class SetDateAndTimeCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetDateAndTimeResponseDelegate onSetDateAndTimeResponse;

        public SetDateAndTimeCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetDateAndTimeResponse(OnSetCurrentDateAndTimeResponse);
        }

        public void SendSetDateAndTimeCommand(DateTime time)
        {
            var payload = new List<byte>();

            // 2th (from 0th) byte - year
            payload.Add((byte)(time.Year - 2000));

            // 3th byte - month
            payload.Add((byte)time.Month);

            // 4th byte - day of month
            payload.Add((byte)time.Day);

            // 5th byte - day of week
            payload.Add(GetDayOfWeek(time.DayOfWeek));

            // 6th byte - hour
            payload.Add((byte)time.Hour);

            // 7th byte - minute
            payload.Add((byte)time.Minute);

            // 8th byte - second
            payload.Add((byte)time.Second);

            packetsProcessor.SendCommand(CommandType.SetDateAndTime, payload);
        }

        private void OnSetCurrentDateAndTimeResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetDateAndTimeResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetDateAndTimeResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }

        public void SetResponseDelegate(OnSetDateAndTimeResponseDelegate onSetDateAndTimeResponse)
        {
            this.onSetDateAndTimeResponse = onSetDateAndTimeResponse;
        }

        private byte GetDayOfWeek(DayOfWeek dow)
        {
            switch (dow)
            {
                case DayOfWeek.Monday:
                    return 1;

                case DayOfWeek.Tuesday:
                    return 2;

                case DayOfWeek.Wednesday:
                    return 3;

                case DayOfWeek.Thursday:
                    return 4;

                case DayOfWeek.Friday:
                    return 5;

                case DayOfWeek.Saturday:
                    return 6;

                case DayOfWeek.Sunday:
                    return 7;

                default:
                    throw new InvalidOperationException("Impossible day of week");
            }
        }
    }
}
