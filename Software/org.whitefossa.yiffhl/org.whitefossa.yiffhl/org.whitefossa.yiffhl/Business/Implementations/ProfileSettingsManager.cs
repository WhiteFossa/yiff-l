using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ProfileSettingsManager : IProfileSettingsManager
    {
        private readonly IGetFrequencyCommand _getFrequencyCommand;
        private readonly ISetFrequencyCommand _setFrequencyCommand;

        private OnGetFrequencySettings _onGetFrequencySettings;

        private OnSetFrequencySettings _onSetFrequencySettings;

        public ProfileSettingsManager(IGetFrequencyCommand getFrequencyCommand,
            ISetFrequencyCommand setFrequencyCommand)
        {
            _getFrequencyCommand = getFrequencyCommand;
            _setFrequencyCommand = setFrequencyCommand;
        }

        public async Task LoadFrequencySettingsAsync(OnGetFrequencySettings onGetFrequencySettings)
        {
            _onGetFrequencySettings = onGetFrequencySettings ?? throw new ArgumentNullException(nameof(onGetFrequencySettings));

            _getFrequencyCommand.SetResponseDelegate(OnGetFrequencyResponse);
            _getFrequencyCommand.SendGetFrequencyCommand();
        }

        public async Task SetFrequencySettingsAsync(FrequencySettings settings, OnSetFrequencySettings onSetFrequencySettings)
        {
            _ = settings ?? throw new ArgumentNullException(nameof(settings));
            _onSetFrequencySettings = onSetFrequencySettings ?? throw new ArgumentNullException(nameof(onSetFrequencySettings));

            _setFrequencyCommand.SetResponseDelegate(OnSetFrequencyResponse);
            _setFrequencyCommand.SendSetFrequencyCommand(settings.Is2m, settings.Frequency);
        }

        private void OnGetFrequencyResponse(bool is144MHz, int frequency)
        {
            _onGetFrequencySettings(new FrequencySettings() { Is2m = is144MHz, Frequency = frequency });
        }

        private void OnSetFrequencyResponse(bool isSuccess)
        {
            if (!isSuccess)
            {
                throw new InvalidOperationException("Failed to set frequency");
            }

            _onSetFrequencySettings();
        }
    }
}
