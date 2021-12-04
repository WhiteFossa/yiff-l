using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnGetCycleResponseDelegate(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime);

    public interface IGetCycleCommand
    {
        void SetResponseDelegate(OnGetCycleResponseDelegate onGetCycleResponse);

        void SendGetCycleCommand();
    }
}
