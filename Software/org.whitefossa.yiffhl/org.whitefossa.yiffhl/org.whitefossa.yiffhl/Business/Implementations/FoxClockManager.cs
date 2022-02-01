using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class FoxClockManager : IFoxClockManager
    {
        private readonly ISetDateAndTimeCommand _setDateAndTimeCommand;

        private OnClockSynchronizedDelegate _onClockSynchronized;

        public FoxClockManager(ISetDateAndTimeCommand setDateAndTimeCommand)
        {
            _setDateAndTimeCommand = setDateAndTimeCommand;
        }

        public async Task SynchronizeClockAsync(OnClockSynchronizedDelegate onSynchronized)
        {
            _onClockSynchronized = onSynchronized ?? throw new ArgumentNullException(nameof(onSynchronized));

            _setDateAndTimeCommand.SetResponseDelegate(OnSetDateAndTimeResponse);
            _setDateAndTimeCommand.SendSetDateAndTimeCommand(DateTime.Now);
        }

        private void OnSetDateAndTimeResponse(bool isSuccessful)
        {
            _onClockSynchronized(isSuccessful);
        }
    }
}
