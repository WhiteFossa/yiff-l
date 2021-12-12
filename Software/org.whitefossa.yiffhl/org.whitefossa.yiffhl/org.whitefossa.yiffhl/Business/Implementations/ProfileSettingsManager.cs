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
        private readonly IGetCycleCommand _getCycleCommand;
        private readonly ISetCycleCommand _setCycleCommand;
        private readonly IGetEndingToneDurationCommand _getEndingToneDurationCommand;
        private readonly ISetEndingToneDurationCommand _setEndingToneDurationCommand;
        private readonly IGetBeginAndEndTimesCommand _getBeginAndEndTimesCommand;
        private readonly ISetBeginAndEndTimesCommand _setBeginAndEndTimesCommand;
        private readonly IGetPowerCommand _getPowerCommand;

        private OnGetFrequencySettingsDelegate _onGetFrequencySettings;
        private OnSetFrequencySettingsDelegate _onSetFrequencySettings;
        private OnGetCallsignSettingsDelegate _onGetCallsignSettings;
        private OnSetSpeedDelegate _onSetSpeed;
        private OnSetCallsignDelegate _onSetCallsign;
        private OnGetCycleSettingsDelegate _onGetCycleSettings;
        private OnSetCycleSettingsDelegate _onSetCycleSettings;
        private OnGetRunTimesSettingsDelegate _onGetRunTimesSettings;
        private OnSetRunTimesSettingsDelegate _onSetRunTimesSettings;
        private OnGetPowerSettingsDelegate _onGetPowerSettings;

        private Callsign _callsign;
        private CycleSettings _cycleSettings;

        private bool _speedToSet;
        private Callsign _callsignToSet;
        private FrequencySettings _frequencySettingsToSet;
        private CycleSettings _cycleSettingsToSet;
        private RunTimesSettings _runTimesSettingsToSet;

        public ProfileSettingsManager(IGetFrequencyCommand getFrequencyCommand,
            ISetFrequencyCommand setFrequencyCommand,
            IGetCodeCommand getCodeCommand,
            IGetSpeedCommand getSpeedCommand,
            ISetSpeedCommand setSpeedCommand,
            ISetCodeCommand setCodeCommand,
            IGetCycleCommand getCycleCommand,
            ISetCycleCommand setCycleCommand,
            IGetEndingToneDurationCommand getEndingToneDurationCommand,
            ISetEndingToneDurationCommand setEndingToneDurationCommand,
            IGetBeginAndEndTimesCommand getBeginAndEndTimesCommand,
            ISetBeginAndEndTimesCommand setBeginAndEndTimesCommand,
            IGetPowerCommand getPowerCommand)
        {
            _getFrequencyCommand = getFrequencyCommand;
            _setFrequencyCommand = setFrequencyCommand;
            _getCodeCommand = getCodeCommand;
            _getSpeedCommand = getSpeedCommand;
            _setSpeedCommand = setSpeedCommand;
            _setCodeCommand = setCodeCommand;
            _getCycleCommand = getCycleCommand;
            _setCycleCommand = setCycleCommand;
            _getEndingToneDurationCommand = getEndingToneDurationCommand;
            _setEndingToneDurationCommand = setEndingToneDurationCommand;
            _getBeginAndEndTimesCommand = getBeginAndEndTimesCommand;
            _setBeginAndEndTimesCommand = setBeginAndEndTimesCommand;
            _getPowerCommand = getPowerCommand;
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
            _frequencySettingsToSet = settings ?? throw new ArgumentNullException(nameof(settings));
            _onSetFrequencySettings = onSetFrequencySettings ?? throw new ArgumentNullException(nameof(onSetFrequencySettings));

            // Checking if frequency settings changed
            _getFrequencyCommand.SetResponseDelegate(OnGetFrequencyResponse_SetFrequencyPathway);
            _getFrequencyCommand.SendGetFrequencyCommand();
        }

        private void OnGetFrequencyResponse(bool is144MHz, int frequency)
        {
            _onGetFrequencySettings(new FrequencySettings() { Is2m = is144MHz, Frequency = frequency });
        }

        private void OnGetFrequencyResponse_SetFrequencyPathway(bool is144MHz, int frequency)
        {
            if (_frequencySettingsToSet.Is2m == is144MHz && _frequencySettingsToSet.Frequency == frequency)
            {
                _onSetFrequencySettings();
                return;
            }

            _setFrequencyCommand.SetResponseDelegate(OnSetFrequencyResponse);
            _setFrequencyCommand.SendSetFrequencyCommand(_frequencySettingsToSet.Is2m, _frequencySettingsToSet.Frequency);
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
                _onSetSpeed();
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
                _onSetCallsign();
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

        public async Task LoadCycleSettingsAsync(OnGetCycleSettingsDelegate onGetCycleSettings)
        {
            _onGetCycleSettings = onGetCycleSettings ?? throw new ArgumentNullException(nameof(onGetCycleSettings));

            _getCycleCommand.SetResponseDelegate(OnGetCycleResponse);
            _getCycleCommand.SendGetCycleCommand();
        }

        private void OnGetCycleResponse(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime)
        {
            _cycleSettings = new CycleSettings
            {
                IsContinuous = isContinuous,
                TxDuration = txTime,
                PauseDuration = pauseTime
            };

            _getEndingToneDurationCommand.SetResponseDelegate(OnGetEndingToneDurationResponse);
            _getEndingToneDurationCommand.SendGetEndingToneDurationCommand();
        }

        private void OnGetEndingToneDurationResponse(TimeSpan duration)
        {
            _cycleSettings.EndingToneDuration = duration;

            _onGetCycleSettings(_cycleSettings);
        }

        public async Task SetCycleSettingsAsync(CycleSettings settings, OnSetCycleSettingsDelegate onSetCycleSettings)
        {
            _cycleSettingsToSet = settings ?? throw new ArgumentNullException(nameof(settings));
            _onSetCycleSettings = onSetCycleSettings ?? throw new ArgumentNullException(nameof(onSetCycleSettings));

            // Checking if cycle settings changed
            await LoadCycleSettingsAsync(OnGetCycleSettings_SetCycleSettingsPathway);
        }

        private void OnGetCycleSettings_SetCycleSettingsPathway(CycleSettings settings)
        {
            if (settings.IsContinuous == _cycleSettingsToSet.IsContinuous
                &&
                settings.TxDuration == _cycleSettingsToSet.TxDuration
                &&
                settings.PauseDuration == _cycleSettingsToSet.PauseDuration
                &&
                settings.EndingToneDuration == _cycleSettingsToSet.EndingToneDuration)
            {
                _onSetCycleSettings();
                return;
            }

            _setCycleCommand.SetResponseDelegate(OnSetCycleResponse);
            _setCycleCommand.SendSetCycleCommand(_cycleSettingsToSet.IsContinuous, _cycleSettingsToSet.TxDuration, _cycleSettingsToSet.PauseDuration);
        }

        private void OnSetCycleResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set fox cycle");
            }

            // Setting ending tone duration
            _setEndingToneDurationCommand.SetResponseDelegate(OnSetEndingToneDurationResponse);
            _setEndingToneDurationCommand.SendSetEndingToneResponseDurationCommand(_cycleSettingsToSet.EndingToneDuration);
        }

        private void OnSetEndingToneDurationResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set ending tone duration");
            }

            _onSetCycleSettings();
        }

        public async Task LoadRunTimesSettinesAsync(OnGetRunTimesSettingsDelegate onGetRunTimesSettings)
        {
            _onGetRunTimesSettings = onGetRunTimesSettings ?? throw new ArgumentNullException(nameof(onGetRunTimesSettings));

            _getBeginAndEndTimesCommand.SetResponseDelegate(OnGetBeginAndEndTimesResponse);
            _getBeginAndEndTimesCommand.SendGetBeginAndEndTimesCommand();
        }

        private void OnGetBeginAndEndTimesResponse(DateTime beginTime, DateTime endTime)
        {
            var runTimesSettings = new RunTimesSettings()
            {
                StartTime = beginTime,
                FinishTime = endTime
            };

            _onGetRunTimesSettings(runTimesSettings);
        }

        public async Task SetRunTimesSettingsAsync(RunTimesSettings settings, OnSetRunTimesSettingsDelegate onSetRunTimesSettines)
        {
            _runTimesSettingsToSet = settings ?? throw new ArgumentNullException(nameof(settings));
            _onSetRunTimesSettings = onSetRunTimesSettines ?? throw new ArgumentNullException(nameof(onSetRunTimesSettines));

            // Do we need to change something?
            await LoadRunTimesSettinesAsync(OnGetBeginAndEndTimesResponse_SetRunTimesSettingsPathway);
        }

        private void OnGetBeginAndEndTimesResponse_SetRunTimesSettingsPathway(RunTimesSettings settings)
        {
            if (settings.StartTime == _runTimesSettingsToSet.StartTime
                &&
                settings.FinishTime == _runTimesSettingsToSet.FinishTime)
            {
                _onSetRunTimesSettings();
            }

            _setBeginAndEndTimesCommand.SetResponseDelegate(OnSetBeginAndEndTimesResponse);
            _setBeginAndEndTimesCommand.SendSetBeginAndEndTimesCommand(_runTimesSettingsToSet.StartTime, _runTimesSettingsToSet.FinishTime);
        }

        private void OnSetBeginAndEndTimesResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set start and finish times");
            }

            _onSetRunTimesSettings();
        }

        public async Task LoadPowerSettingsAsync(OnGetPowerSettingsDelegate onGetPowerSettings)
        {
            _onGetPowerSettings = onGetPowerSettings ?? throw new ArgumentNullException(nameof(onGetPowerSettings));

            _getPowerCommand.SetResponseDelegate(OnGetPowerResponse);
            _getPowerCommand.SendGetPowerCommand();
        }

        private void OnGetPowerResponse(float power)
        {
            var settings = new PowerSettings
            {
                Power = power
            };

            _onGetPowerSettings(settings);
        }
    }
}
