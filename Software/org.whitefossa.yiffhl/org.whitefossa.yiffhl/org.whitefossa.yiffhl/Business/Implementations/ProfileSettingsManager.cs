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
        private readonly ISetSpeedCommand _setSpeedCommand;
        private readonly ISetCodeCommand _setCodeCommand;

        private OnGetFrequencySettingsDelegate _onGetFrequencySettings;
        private OnSetFrequencySettingsDelegate _onSetFrequencySettings;
        private OnGetCallsignSettingsDelegate _onGetCallsignSettings;
        private OnSetSpeedDelegate _onSetSpeed;
        private OnSetCallsignDelegate _onSetCallsign;

        private Callsign _callsign;

        private bool _speedToSet;

        private Callsign _callsignToSet;

        public ProfileSettingsManager(IGetFrequencyCommand getFrequencyCommand,
            ISetFrequencyCommand setFrequencyCommand,
            IGetCodeCommand getCodeCommand,
            IGetSpeedCommand getSpeedCommand,
            ISetSpeedCommand setSpeedCommand,
            ISetCodeCommand setCodeCommand)
        {
            _getFrequencyCommand = getFrequencyCommand;
            _setFrequencyCommand = setFrequencyCommand;
            _getCodeCommand = getCodeCommand;
            _getSpeedCommand = getSpeedCommand;
            _setSpeedCommand = setSpeedCommand;
            _setCodeCommand = setCodeCommand;
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

        public async Task SetSpeedAsync(bool isFast, OnSetSpeedDelegate onSetSpeed)
        {
            _onSetSpeed = onSetSpeed ?? throw new ArgumentNullException(nameof(onSetSpeed));
            _speedToSet = isFast;

            // Checking if speed changed
            _getSpeedCommand.SetResponseDelegate(OnGetSpeedResponse_SetSpeedPathway);
            _getSpeedCommand.SendGetSpeedCommand();
        }

        private void OnGetSpeedResponse_SetSpeedPathway(bool isFast)
        {
            if (_speedToSet == isFast)
            {
                return;
            }

            _setSpeedCommand.SetResponseDelegate(OnSetSpeedResponse);
            _setSpeedCommand.SendSetSpeedCommand(_speedToSet);
        }

        private void OnSetSpeedResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set speed");
            }

            _onSetSpeed();
        }

        public async Task SetCallsingAsync(Callsign callsign, OnSetCallsignDelegate onSetCallsign)
        {
            _callsignToSet = callsign ?? throw new ArgumentNullException(nameof(callsign));
            _onSetCallsign = onSetCallsign ?? throw new ArgumentNullException(nameof(onSetCallsign));

            // Checking if callsign changed
            _getCodeCommand.SetResponseDelegate(OnGetCodeResponse_SetCallsignPathway);
            _getCodeCommand.SendGetCodeCommand();
        }

        private void OnGetCodeResponse_SetCallsignPathway(FoxCode code)
        {
            if (_callsignToSet.Code == code)
            {
                return;
            }

            _setCodeCommand.SetResponseDelegate(OnSetCodeResponse);
            _setCodeCommand.SendSetCodeCommand(_callsignToSet.Code);
        }

        private void OnSetCodeResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set fox code");
            }

            _onSetCallsign();
        }
    }
}
