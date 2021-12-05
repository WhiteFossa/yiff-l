using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetEndingToneDurationResponseDelegate(TimeSpan duration);

    public interface IGetEndingToneDurationCommand
    {
        void SetResponseDelegate(OnGetEndingToneDurationResponseDelegate onGetEndingToneDurationResponse);

        void SendGetEndingToneDurationCommand();
    }
}
