using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    public delegate void OnSetBeginAndEndTimesResponseDelegate(bool isSuccess);

    public interface ISetBeginAndEndTimesCommand
    {
        void SetResponseDelegate(OnSetBeginAndEndTimesResponseDelegate onSetBeginAndEndTimesResponse);

        void SendSetBeginAndEndTimesCommand(DateTime beginTime, DateTime endTime);
    }
}
