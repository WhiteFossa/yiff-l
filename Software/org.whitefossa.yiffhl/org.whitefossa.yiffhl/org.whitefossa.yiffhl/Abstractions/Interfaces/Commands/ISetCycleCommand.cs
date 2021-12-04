using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetCycleResponseDelegate(bool isSuccess);

    public interface ISetCycleCommand
    {
        void SetResponseDelegate(OnSetCycleResponseDelegate onSetCycleResponse);

        void SendSetCycleCommand(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime);
    }
}
