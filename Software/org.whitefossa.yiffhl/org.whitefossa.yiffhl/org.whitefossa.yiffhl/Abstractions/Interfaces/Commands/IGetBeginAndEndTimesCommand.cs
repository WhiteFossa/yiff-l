using System;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces.Commands
{
    /// <summary>
    /// Times returned relative to BaseDate
    /// </summary>
    public delegate void OnGetBeginAndEndTimesResponseDelegate(DateTime beginTime, DateTime endTime);

    public interface IGetBeginAndEndTimesCommand
    {
        void SetResponseDelegate(OnGetBeginAndEndTimesResponseDelegate onGetBeginAndEndTimesResponse);

        void SendGetBeginAndEndTimesCommand();
    }
}
