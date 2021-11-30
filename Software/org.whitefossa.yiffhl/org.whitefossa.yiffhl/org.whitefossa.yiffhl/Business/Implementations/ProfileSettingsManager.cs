using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ProfileSettingsManager : IProfileSettingsManager
    {
        private readonly IGetFrequencyCommand _getFrequencyCommand;
        private readonly ISetFrequencyCommand _setFrequencyCommand;
        private readonly IGetCodeCommand _getCodeCommand;
        private readonly IGetSpeedCommand _getSpeedCommand;

        private OnGetFrequencySettingsDelegate _onGetFrequencySettings;

        private OnSetFrequencySettingsDelegate _onSetFrequencySettings;

        private OnGetCallsignSettingsDelegate _onGetCallsignSettings;

        private Callsign _callsign;

        public ProfileSettingsManager(IGetFrequencyCommand getFrequencyCommand,
            ISetFrequencyCommand setFrequencyCommand,
            IGetCodeCommand getCodeCommand,
            IGetSpeedCommand getSpeedCommand)
        {
            _getFrequencyCommand = getFrequencyCommand;
            _setFrequencyCommand = setFrequencyCommand;
            _getCodeCommand = getCodeCommand;
            _getSpeedCommand = getSpeedCommand;
        }

        public async Task<IReadOnlyCollection<Callsign>> GetCallsignsAsync()
        {
            var result = new List<Callsign>();

            result.Add(new Callsign()
            {
                Code = FoxCode.Finish,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Fox1,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Fox2,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Fox3,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Fox4,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Fox5,
            });

            result.Add(new Callsign()
            {
                Code = FoxCode.Beacon,
            });

            return result;
        }

        public async Task LoadCallsignSettingsAsync(OnGetCallsignSettingsDelegate onGetCallsingSettings)
        {
            _onGetCallsignSettings = onGetCallsingSettings ?? throw new ArgumentNullException(nameof(onGetCallsingSettings));

            _getCodeCommand.SetResponseDelegate(OnGetCodeResponse);
            _getCodeCommand.SendGetCodeCommand();
        }

        public async Task LoadFrequencySettingsAsync(OnGetFrequencySettingsDelegate onGetFrequencySettings)
        {
            _onGetFrequencySettings = onGetFrequencySettings ?? throw new ArgumentNullException(nameof(onGetFrequencySettings));

            _getFrequencyCommand.SetResponseDelegate(OnGetFrequencyResponse);
            _getFrequencyCommand.SendGetFrequencyCommand();
        }

        public async Task SetFrequencySettingsAsync(FrequencySettings settings, OnSetFrequencySettingsDelegate onSetFrequencySettings)
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

        private void OnGetCodeResponse(FoxCode code)
        {
            _callsign = new Callsign() { Code = code };

            _getSpeedCommand.SetResponseDelegate(OnGetSpeedResponse);
            _getSpeedCommand.SendGetSpeedCommand();
        }

        private void OnGetSpeedResponse(bool isFast)
        {
            var callsignSettings = new CallsignSettings()
            {
                Callsing = _callsign,
                IsFast = isFast
            };

            _onGetCallsignSettings(callsignSettings);
        }
    }
}
