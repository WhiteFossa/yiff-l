using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class StaticFoxStatusManager : IStaticFoxStatusManager
    {
        private readonly IIsFoxArmedCommand _isFoxArmedCommand;
        private readonly IArmFoxCommand _armFoxCommand;
        private readonly IDisarmFoxCommand _disarmFoxCommand;

        private OnGetStaticFoxStatusDelegate _onGetStaticFoxStatus;
        private OnArmFoxDelegate _onArmFox;
        private OnDisarmFoxDelegate _onDisarmFox;

        private StaticFoxStatus _statusToLoad = new StaticFoxStatus();

        public StaticFoxStatusManager(IIsFoxArmedCommand isFoxArmedCommand,
            IArmFoxCommand armFoxCommand,
            IDisarmFoxCommand disarmFoxCommand)
        {
            _isFoxArmedCommand = isFoxArmedCommand;
            _armFoxCommand = armFoxCommand;
            _disarmFoxCommand = disarmFoxCommand;
        }

        public async Task GetStaticFoxStatusAsync(OnGetStaticFoxStatusDelegate onGetStaticFoxStatus)
        {
            _onGetStaticFoxStatus = onGetStaticFoxStatus ?? throw new ArgumentNullException(nameof(onGetStaticFoxStatus));

            _isFoxArmedCommand.SetResponseDelegate(OnIsFoxArmedResponse);
            _isFoxArmedCommand.SendIsFoxArmedCommand();
        }

        private void OnIsFoxArmedResponse(bool isArmed)
        {
            _statusToLoad.IsFoxArmed = isArmed;

            _onGetStaticFoxStatus(_statusToLoad);
        }

        public async Task ArmFoxAsync(OnArmFoxDelegate onArmFox)
        {
            _onArmFox = onArmFox ?? throw new ArgumentNullException(nameof(onArmFox));

            _armFoxCommand.SetResponseDelegate(OnArmFoxResponse);
            _armFoxCommand.SendArmFoxCommand();
        }

        private void OnArmFoxResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to initiate fox arming");
            }

            _onArmFox();
        }

        public async Task DisarmFoxAsync(OnDisarmFoxDelegate onDisarmFox)
        {
            _onDisarmFox = onDisarmFox ?? throw new ArgumentNullException(nameof(onDisarmFox));

            _disarmFoxCommand.SetResponseDelegate(OnDisarmFoxResponse);
            _disarmFoxCommand.SendDisarmFoxCommand();
        }

        private void OnDisarmFoxResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to disarm fox.");
            }

            _onDisarmFox();
        }
    }
}
