using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetEndingToneDurationResponseDelegate(bool isSuccess);

    public interface ISetEndingToneDurationCommand
    {
        void SetResponseDelegate(OnSetEndingToneDurationResponseDelegate onSetEndingToneDurationResponse);

        void SendSetEndingToneResponseDurationCommand(TimeSpan endingToneDuration);
    }
}
