﻿using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Models;
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Input;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    public class ServicePageViewModel : BindableObject
    {
        /// <summary>
        /// Interval in milliseconds to poll service data
        /// </summary>
        private const int PollServiceDataInterval = 2000;

        private readonly IServiceCommandsManager _serviceCommandsManager;
        private readonly IFoxIdentificationManager _foxIdentificationManager;
        private readonly IUserNotifier _userNotifier;
        private MainModel _mainModel;

        public INavigation Navigation;

        /// <summary>
        /// Timer to poll service data
        /// </summary>
        private Timer PollServiceDataTimer;

        /// <summary>
        /// Last error code
        /// </summary>
        public string LastErrorCodeFormatted
        {
            get
            {
                if (_mainModel.ServiceSettingsModel.LastErrorCode.HasValue)
                {
                    return _mainModel.ServiceSettingsModel.LastErrorCode.ToString();
                }
                else
                {
                    return "N/A";
                }
            }
        }

        /// <summary>
        /// Serial number (as a string)
        /// </summary>
        private string _serialNunberAsString;

        public string SerialNumberAsString
        {
            get => _serialNunberAsString;
            set
            {
                _serialNunberAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get last error code command
        /// </summary>
        public ICommand GetLastErrorCodeCommand { get; }

        /// <summary>
        /// Reset last error code command
        /// </summary>
        public ICommand ResetLastErrorCodeCommand { get; }

        /// <summary>
        /// Reload serial number
        /// </summary>
        public ICommand ReloadSerialNumberCommand { get; }

        /// <summary>
        /// Set serial number
        /// </summary>
        public ICommand SetSerialNumberCommand { get; }

        /// <summary>
        /// Battery averaged ADC level
        /// </summary>
        public string BatteryADCLevelAsString
        {
            get => String.Format("{0:0.00}", _mainModel.ServiceSettingsModel.BatteryAveragedADCLevel);
        }

        /// <summary>
        /// Battery averaged voltage level
        /// </summary>
        public string BatteryVoltageLevelAsString
        {
            get => String.Format("{0:0.0000}V", _mainModel.ServiceSettingsModel.BatteryAveragedVoltageLevel);
        }

        /// <summary>
        /// A factor for Ubatt(ADC) -> Ubatt(volts)
        /// </summary>
        private string _uBattAFactorAsString;

        public string UBattAFactorAsString
        {
            get => _uBattAFactorAsString;
            set
            {
                _uBattAFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// B factor for Ubatt(ADC) -> Ubatt(volts)
        /// </summary>
        private string _uBattBFactorAsString;

        public string UBattBFactorAsString
        {
            get => _uBattBFactorAsString;
            set
            {
                _uBattBFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get UBatt factors
        /// </summary>
        public ICommand GetUbattFactorsCommand { get; }

        /// <summary>
        /// Reset Ubatt factors
        /// </summary>
        public ICommand ResetUbattFactorsCommand { get; }

        /// <summary>
        /// Set Ubatt factors
        /// </summary>
        public ICommand SetUbattFactorsCommand { get; }

        /// <summary>
        /// Battery charge level
        /// </summary>
        public string BatteryLevelAsString
        {
            get => String.Format("{0:0.0%}", _mainModel.DynamicFoxStatus.BatteryLevel);
        }

        /// <summary>
        /// A factor for Ubatt(volts) -> Battery level
        /// </summary>
        private string _battLevelAFactorAsString;

        public string BattLevelAFactorAsString
        {
            get => _battLevelAFactorAsString;
            set
            {
                _battLevelAFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// B factor for Ubatt(volts) -> Battery level
        /// </summary>
        private string _battLevelBFactorAsString;

        public string BattLevelBFactorAsString
        {
            get => _battLevelBFactorAsString;
            set
            {
                _battLevelBFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get battery level factors
        /// </summary>
        public ICommand GetBattLevelFactorsCommand { get; }

        /// <summary>
        /// Reset battery level factors
        /// </summary>
        public ICommand ResetBattLevelFactorsCommand { get; }

        /// <summary>
        /// Set battery level factors
        /// </summary>
        public ICommand SetBattLevelFactorsCommand { get; }

        /// <summary>
        /// U80m averaged ADC level
        /// </summary>
        public string U80mADCValueAsString
        {
            get => String.Format("{0:0.00}", _mainModel.ServiceSettingsModel.U80mAveragedADCValue);
        }

        /// <summary>
        /// U80m averaged voltage
        /// </summary>
        public string U80mVoltageAsString
        {
            get => String.Format("{0:0.0000}V", _mainModel.ServiceSettingsModel.U80mAveragedVoltage);
        }

        /// <summary>
        /// A factor for U80m(ADC) -> U80m(Volts)
        /// </summary>
        private string _u80mAFactorAsString;

        public string U80mAFactorAsString
        {
            get => _u80mAFactorAsString;
            set
            {
                _u80mAFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// B factor for U80m(ADC) -> U80m(Volts)
        /// </summary>
        private string _u80mBFactorAsString;

        public string U80mBFactorAsString
        {
            get => _u80mBFactorAsString;
            set
            {
                _u80mBFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get U80m factors
        /// </summary>
        public ICommand GetU80mFactorsCommand { get; }

        /// <summary>
        /// Reset U80m factors
        /// </summary>
        public ICommand ResetU80mFactorsCommand { get; }

        /// <summary>
        /// Set U80m factors
        /// </summary>
        public ICommand SetU80mFactorsCommand { get; }

        public ServicePageViewModel()
        {

            _mainModel = App.Container.Resolve<IMainModel>() as MainModel;
            _serviceCommandsManager = App.Container.Resolve<IServiceCommandsManager>();
            _foxIdentificationManager = App.Container.Resolve<IFoxIdentificationManager>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();

            GetLastErrorCodeCommand = new Command(async () => await OnGetLastErrorAsync());
            ResetLastErrorCodeCommand = new Command(async () => await OnResetLastErrorCodeAsync());

            ReloadSerialNumberCommand = new Command(async () => await OnReloadSerialNumberAsync());
            SetSerialNumberCommand = new Command(async () => await OnSetSerialNumberAsync());

            GetUbattFactorsCommand = new Command(async() => await OnGetUbattFactorsAsync());
            ResetUbattFactorsCommand = new Command(async() => await OnResetUbattFactorsAsync());
            SetUbattFactorsCommand = new Command(async() => await OnSetUbattFactorsAsync());

            GetBattLevelFactorsCommand = new Command(async () => await OnGetBattLevelFactorsAsync());
            ResetBattLevelFactorsCommand = new Command(async () => await OnResetBattLevelFactorsAsync());
            SetBattLevelFactorsCommand = new Command(async () => await OnSetBattLevelFactorsAsync());

            GetU80mFactorsCommand = new Command(async () => await OnGetU80mFactorsAsync());
            ResetU80mFactorsCommand = new Command(async () => await OnResetU80mFactorsAsync());
            SetU80mFactorsCommand = new Command(async () => await OnSetU80mFactorsAsync());

            // Setting up poll service data timer
            PollServiceDataTimer = new Timer(PollServiceDataInterval);
            PollServiceDataTimer.Elapsed += async (s, e) => await OnReloadServiceDataRequestAsync(s, e);
            PollServiceDataTimer.AutoReset = true;
            PollServiceDataTimer.Start();
        }

        public async Task OnShowAsync()
        {
            SerialNumberAsString = _mainModel.IdentificationData.SerialNumber.ToString();
        }

        public async Task OnLeavingServiceDisplayAsync()
        {
            _mainModel.ActiveDisplay = ActiveDisplay.MainDisplay;
            await Navigation.PopModalAsync();
        }

        #region Get last error

        private async Task OnGetLastErrorAsync()
        {
            await _serviceCommandsManager.GetLastErrorCodeAsync(async (ec) => await OnGetLastErrorCodeResponseAsync(ec));
        }

        private async Task OnGetLastErrorCodeResponseAsync(uint errorCode)
        {
            _mainModel.ServiceSettingsModel.LastErrorCode = errorCode;
            OnPropertyChanged(nameof(LastErrorCodeFormatted));
        }

        #endregion

        #region Reset last error

        private async Task OnResetLastErrorCodeAsync()
        {
            await _serviceCommandsManager.ResetLastErrorCodeAsync(async () => await OnResetLastErrorCodeResponseAsync());
        }

        private async Task OnResetLastErrorCodeResponseAsync()
        {
            // Reloading last error code
            await OnGetLastErrorAsync();
        }

        #endregion

        #region Reload serial number

        private async Task OnReloadSerialNumberAsync()
        {
            await _foxIdentificationManager.IdentifyFoxAsync(async (isFox, pVer, hwRev, fwVer, sn)
                => await OnGetIdentificationDataResponseAsync(isFox, pVer, hwRev, fwVer, sn));
        }

        private async Task OnGetIdentificationDataResponseAsync
        (
            bool isFox,
            uint protocolVersion,
            uint hardwareRevision,
            uint firmwareVersion,
            uint serialNumber
        )
        {
            // Assuming that we are connected to fox and protocol version is OK
            _mainModel.MainPageViewModel.FoxSerialNumber = serialNumber;

            SerialNumberAsString = _mainModel.IdentificationData.SerialNumber.ToString();
        }

        #endregion

        #region Set serial number

        private async Task OnSetSerialNumberAsync()
        {
            uint newSerialNumber;
            if (!uint.TryParse(SerialNumberAsString, out newSerialNumber))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Not a valid serial number!");
                return;
            }

            await _serviceCommandsManager.UpdateSerialNumberAsync(newSerialNumber, async () => await OnSerialNumberUpdatedAsync());
        }

        private async Task OnSerialNumberUpdatedAsync()
        {
            await OnReloadSerialNumberAsync();
        }

        #endregion

        #region Reload service data

        private async Task OnReloadServiceDataRequestAsync(Object source, ElapsedEventArgs e)
        {
            if (_mainModel.ActiveDisplay != ActiveDisplay.ServiceDisplay)
            {
                return;
            }

            // Battery ADC level
            await _serviceCommandsManager.GetBatteryADCLevelAsync(async (l) => await OnGetBatteryADCLevelResponseAsync(l));

            // Battery voltage
            await _serviceCommandsManager.GetBatteryVoltageLevelAsync(async (l) => await OnGetBatteryVoltageLevelResponseAsync(l));

            // Charge level
            OnPropertyChanged(nameof(BatteryLevelAsString));

            // U80m ADC level
            await _serviceCommandsManager.GetU80mADCValueAsync(async (v) => await OnGetU80mADCValueResponseAsync(v));

            // U80m voltage
            await _serviceCommandsManager.GetU80mVoltageAsync(async (v) => await OnGetU80mVoltageResponseAsync(v));
        }

        private async Task OnGetBatteryADCLevelResponseAsync(float averagedADCLevel)
        {
            _mainModel.ServiceSettingsModel.BatteryAveragedADCLevel = averagedADCLevel;
            OnPropertyChanged(nameof(BatteryADCLevelAsString));
        }

        private async Task OnGetBatteryVoltageLevelResponseAsync(float averagedVoltageLevel)
        {
            _mainModel.ServiceSettingsModel.BatteryAveragedVoltageLevel = averagedVoltageLevel;
            OnPropertyChanged(nameof(BatteryVoltageLevelAsString));
        }

        private async Task OnGetU80mADCValueResponseAsync(float averagedADCValue)
        {
            _mainModel.ServiceSettingsModel.U80mAveragedADCValue = averagedADCValue;
            OnPropertyChanged(nameof(U80mADCValueAsString));
        }

        private async Task OnGetU80mVoltageResponseAsync(float averagedVoltage)
        {
            _mainModel.ServiceSettingsModel.U80mAveragedVoltage = averagedVoltage;
            OnPropertyChanged(nameof(U80mVoltageAsString));
        }

        #endregion

        #region Get UBatt factors

        private async Task OnGetUbattFactorsAsync()
        {
            await _serviceCommandsManager.GetUbattFactorsAsync(async (a, b) => await OnGetUbattFactorsResponseAsync(a, b));
        }

        private async Task OnGetUbattFactorsResponseAsync(float a, float b)
        {
            _mainModel.ServiceSettingsModel.UBattFactorA = a;
            _mainModel.ServiceSettingsModel.UBattFactorB = b;

            UBattAFactorAsString = _mainModel.ServiceSettingsModel.UBattFactorA.ToString();
            UBattBFactorAsString = _mainModel.ServiceSettingsModel.UBattFactorB.ToString();
        }

        #endregion

        #region Reset Ubatt factors

        private async Task OnResetUbattFactorsAsync()
        {
            await _serviceCommandsManager.ResetUbattFactorsAsync(async () => await OnResetUbattFactorsResponseAsync());
        }

        private async Task OnResetUbattFactorsResponseAsync()
        {
            await OnGetUbattFactorsAsync();
        }

        #endregion

        #region Set Ubatt factors

        private async Task OnSetUbattFactorsAsync()
        {
            float newA;
            if (!float.TryParse(UBattAFactorAsString, out newA))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor A is not a number!");
                return;
            }

            float newB;
            if (!float.TryParse(UBattBFactorAsString, out newB))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor B is not a number!");
                return;
            }

            await _serviceCommandsManager.SetUbattFactorsAsync(newA, newB, async () => await OnSetUbattFactorsResponseAsync());
        }

        private async Task OnSetUbattFactorsResponseAsync()
        {
            await OnGetUbattFactorsAsync();
        }

        #endregion

        #region Get battery level factors

        private async Task OnGetBattLevelFactorsAsync()
        {
            await _serviceCommandsManager.GetBattLevelFactorsAsync(async (a, b) => await OnGetBattLevelFactorsResponseAsync(a, b));
        }

        private async Task OnGetBattLevelFactorsResponseAsync(float a, float b)
        {
            _mainModel.ServiceSettingsModel.BattLevelFactorA = a;
            _mainModel.ServiceSettingsModel.BattLevelFactorB = b;

            BattLevelAFactorAsString = _mainModel.ServiceSettingsModel.BattLevelFactorA.ToString();
            BattLevelBFactorAsString = _mainModel.ServiceSettingsModel.BattLevelFactorB.ToString();
        }

        #endregion

        #region Reset battery level factors

        private async Task OnResetBattLevelFactorsAsync()
        {
            await _serviceCommandsManager.ResetBattLevelFactorsAsync(async () => await OnResetBattLevelFactorsResponseAsync());
        }

        private async Task OnResetBattLevelFactorsResponseAsync()
        {
            await OnGetBattLevelFactorsAsync();
        }

        #endregion

        #region Set battery level factors

        private async Task OnSetBattLevelFactorsAsync()
        {
            float newA;
            if (!float.TryParse(BattLevelAFactorAsString, out newA))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor A is not a number!");
                return;
            }

            float newB;
            if (!float.TryParse(BattLevelBFactorAsString, out newB))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor B is not a number!");
                return;
            }

            await _serviceCommandsManager.SetBattLevelFactorsAsync(newA, newB, async () => await OnSetBattLevelFactorsResponseAsync());
        }

        private async Task OnSetBattLevelFactorsResponseAsync()
        {
            await OnGetBattLevelFactorsAsync();
        }

        #endregion

        #region Get U80m factors

        private async Task OnGetU80mFactorsAsync()
        {
            await _serviceCommandsManager.GetU80mFactorsAsync(async (a, b) => await OnGetU80mFactorsResponseAsync(a, b));
        }

        private async Task OnGetU80mFactorsResponseAsync(float a, float b)
        {
            _mainModel.ServiceSettingsModel.U80mFactorA = a;
            _mainModel.ServiceSettingsModel.U80mFactorB = b;

            U80mAFactorAsString = _mainModel.ServiceSettingsModel.U80mFactorA.ToString();
            U80mBFactorAsString = _mainModel.ServiceSettingsModel.U80mFactorB.ToString();
        }

        #endregion

        #region Reset U80m factors

        private async Task OnResetU80mFactorsAsync()
        {
            await _serviceCommandsManager.ResetU80mFactorsAsync(async () => await OnResetU80mFactorsResponseAsync());
        }

        private async Task OnResetU80mFactorsResponseAsync()
        {
            await OnGetU80mFactorsAsync();
        }

        #endregion

        #region Set U80m factors

        private async Task OnSetU80mFactorsAsync()
        {
            float newA;
            if (!float.TryParse(U80mAFactorAsString, out newA))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor A is not a number!");
                return;
            }

            float newB;
            if (!float.TryParse(U80mBFactorAsString, out newB))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor B is not a number!");
                return;
            }

            await _serviceCommandsManager.SetU80mFactorsAsync(newA, newB, async () => await OnSetU80mFactorsResponseAsync());
        }

        private async Task OnSetU80mFactorsResponseAsync()
        {
            await OnGetU80mFactorsAsync();
        }

        #endregion
    }
}
