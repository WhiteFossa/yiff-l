using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.DTOs.Bootloader;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Business.Helpers;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Input;
using Xamarin.Essentials;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    public class ServicePageViewModel : BindableObject
    {
        /// <summary>
        /// Interval in milliseconds to poll service data
        /// </summary>
        private const int PollServiceDataInterval = 2000;

        /// <summary>
        /// Expected bootloader protocol version
        /// </summary>
        private const UInt16 ExpectedBootloaderProtocolVersion = 1;

        private readonly IServiceCommandsManager _serviceCommandsManager;
        private readonly IFoxIdentificationManager _foxIdentificationManager;
        private readonly IUserNotifier _userNotifier;
        private readonly IPacketsProcessor _packetsProcessor;
        private readonly IFoxConnector _foxConnector;
        private readonly IAppCloser _appCloser;
        private readonly IFilesManager _filesManager;

        private MainModel _mainModel;

        public INavigation Navigation;

        /// <summary>
        /// Timer to poll service data
        /// </summary>
        private System.Timers.Timer PollServiceDataTimer;

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

        /// <summary>
        /// A factor for P80m -> U80m
        /// </summary>
        private string _p80mAFactorAsString;

        public string P80mAFactorAsString
        {
            get => _p80mAFactorAsString;
            set
            {
                _p80mAFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// B factor for P80m -> U80m
        /// </summary>
        private string _p80mBFactorAsString;

        public string P80mBFactorAsString
        {
            get => _p80mBFactorAsString;
            set
            {
                _p80mBFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get P80m factors
        /// </summary>
        public ICommand GetP80mFactorsCommand { get; }

        /// <summary>
        /// Reset P80m factors
        /// </summary>
        public ICommand ResetP80mFactorsCommand { get; }

        /// <summary>
        /// Set P80m factors
        /// </summary>
        public ICommand SetP80mFactorsCommand { get; }

        /// <summary>
        /// Uant averaged ADC level
        /// </summary>
        public string UantADCValueAsString
        {
            get => String.Format("{0:0.00}", _mainModel.ServiceSettingsModel.UantAveragedADCValue);
        }

        /// <summary>
        /// Uant averaged voltage
        /// </summary>
        public string UantVoltageAsString
        {
            get => String.Format("{0:0.0000}V", _mainModel.ServiceSettingsModel.UantAveragedVoltage);
        }

        /// <summary>
        /// A factor for Uant(ADC)->Uant(Volts)
        /// </summary>
        private string _uantAFactorAsString;

        public string UantAFactorAsString
        {
            get => _uantAFactorAsString;
            set
            {
                _uantAFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// B factor for Uant(ADC)->Uant(Volts)
        /// </summary>
        private string _uantBFactorAsString;

        public string UantBFactorAsString
        {
            get => _uantBFactorAsString;
            set
            {
                _uantBFactorAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get Uant factors
        /// </summary>
        public ICommand GetUantFactorsCommand { get; }

        /// <summary>
        /// Reset Uant factors
        /// </summary>
        public ICommand ResetUantFactorsCommand { get; }

        /// <summary>
        /// Set Uant factors
        /// </summary>
        public ICommand SetUantFactorsCommand { get; }

        /// <summary>
        /// Force TX on
        /// </summary>
        public ICommand ForceTxOnCommand { get; }

        /// <summary>
        /// Return to normal TX operations
        /// </summary>
        public ICommand TxNormalCommand { get; }

        /// <summary>
        /// RTC calibration value (as a string)
        /// </summary>
        private string _rtcCalibrationValueAsString;

        public string RTCCalibrationValueAsString
        {
            get => _rtcCalibrationValueAsString;
            set
            {
                _rtcCalibrationValueAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get RTC calibration value
        /// </summary>
        public ICommand GetRTCCalibrationValueCommand { get; }

        /// <summary>
        /// Set RTC calibration value
        /// </summary>
        public ICommand SetRTCCalibrationValueCommand { get; }

        /// <summary>
        /// Disarm on discharge threshold
        /// </summary>
        private string _disarmOnDischargeThresholdAsString;

        public string DisarmOnDischargeThresholdAsString
        {
            get => _disarmOnDischargeThresholdAsString;
            set
            {
                _disarmOnDischargeThresholdAsString = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Get disarm on discharge threshold command
        /// </summary>
        public ICommand GetDisarmOnDischargeThresholdCommand { get; }

        /// <summary>
        /// Set disarm on discharge threshold command
        /// </summary>
        public ICommand SetDisarmOnDischargeThresholdCommand { get; }

        /// <summary>
        /// Reboot to bootloader command
        /// </summary>
        public ICommand RebootToBootloaderCommand { get; }

        /// <summary>
        /// Reconnect to this fox when going to bootloader mode
        /// </summary>
        private PairedFoxDTO _foxToReconnect { get; set; }

        /// <summary>
        /// True if fox in bootloader mode and successfully identified
        /// </summary>
        private bool _isBootloaderReady { get; set; }

        /// <summary>
        /// Is "Reboot to bootloader" button enabled?
        /// </summary>
        public bool IsRebootToBootloaderButtonEnabled
        {
            get => !_isBootloaderReady;
        }

        /// <summary>
        /// Is bootloader controls enabled?
        /// </summary>
        public bool IsBootloaderControlsEnabled
        {
            get => _isBootloaderReady;
        }

        /// <summary>
        /// Fox hardware revision as string
        /// </summary>
        public string FoxHardwareRevisionAsString
        {
            get => _mainModel.ServiceSettingsModel.BootloaderIdentificationData != null
                ? _mainModel.ServiceSettingsModel.BootloaderIdentificationData.HardwareRevision.ToString()
                : "N/A";
        }

        /// <summary>
        /// Fox firmware version as string
        /// </summary>
        public string FoxFirmwareVersionAsString
        {
            get => _mainModel.ServiceSettingsModel.BootloaderIdentificationData != null
                ? _mainModel.ServiceSettingsModel.BootloaderIdentificationData.FirmwareVersion.ToString()
                : "N/A";
        }

        /// <summary>
        /// FLASH start address as sting
        /// </summary>
        public string FlashStartAddressAsString
        {
            get => _mainModel.ServiceSettingsModel.BootloaderIdentificationData != null
                ? $"0x{ Convert.ToString(_mainModel.ServiceSettingsModel.BootloaderIdentificationData.FlashStartAddress, 16) }"
                : "N/A";
        }

        /// <summary>
        /// Main firmware start address as sting
        /// </summary>
        public string MainFirmwareStartAddressAsString
        {
            get => _mainModel.ServiceSettingsModel.BootloaderIdentificationData != null
                ? $"0x{ Convert.ToString(_mainModel.ServiceSettingsModel.BootloaderIdentificationData.MainFirmwareStartAddress, 16) }"
                : "N/A";
        }

        /// <summary>
        /// FLASH end address as sting
        /// </summary>
        public string FlashEndAddressAsString
        {
            get => _mainModel.ServiceSettingsModel.BootloaderIdentificationData != null
                ? $"0x{ Convert.ToString(_mainModel.ServiceSettingsModel.BootloaderIdentificationData.FlashEndAddress, 16) }"
                : "N/A";
        }

        /// <summary>
        /// Return from bootloader to main firmware
        /// </summary>
        public ICommand ReturnFromBootloaderCommand { get; }

        /// <summary>
        /// Dump firmware to file
        /// </summary>
        public ICommand DumpFirmwareCommand { get; }

        /// <summary>
        /// Update fox firmware
        /// </summary>
        public ICommand UpdateFirmwareCommand { get; }

        public ServicePageViewModel()
        {

            _mainModel = App.Container.Resolve<IMainModel>() as MainModel;
            _serviceCommandsManager = App.Container.Resolve<IServiceCommandsManager>();
            _foxIdentificationManager = App.Container.Resolve<IFoxIdentificationManager>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _appCloser = App.Container.Resolve<IAppCloser>();
            _filesManager = App.Container.Resolve<IFilesManager>();

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

            GetP80mFactorsCommand = new Command(async () => await OnGetP80mFactorsAsync());
            ResetP80mFactorsCommand = new Command(async () => await OnResetP80mFactorsAsync());
            SetP80mFactorsCommand = new Command(async () => await OnSetP80mFactorsAsync());

            GetUantFactorsCommand = new Command(async () => await OnGetUantFactorsAsync());
            ResetUantFactorsCommand = new Command(async () => await OnResetUantFactorsAsync());
            SetUantFactorsCommand = new Command(async () => await OnSetUantFactorsAsync());

            ForceTxOnCommand = new Command(async () => await OnForceTxOnAsync());
            TxNormalCommand = new Command(async () => await OnTxNormalAsync());

            GetRTCCalibrationValueCommand = new Command(async () => await OnGetRTCCalibrationValueAsync());
            SetRTCCalibrationValueCommand = new Command(async () => await OnSetRTCCalibrationValueAsync());

            GetDisarmOnDischargeThresholdCommand = new Command(async () => await OnGetDisarmOnDischargeThresholdAsync());
            SetDisarmOnDischargeThresholdCommand = new Command(async () => await OnSetDisarmOnDischargeThresholdAsync());

            RebootToBootloaderCommand = new Command(async () => await OnRebootToBootloaderAsync());
            
            ReturnFromBootloaderCommand = new Command(async() => await OnReturnFromBootloaderAsync());

            DumpFirmwareCommand = new Command(async () => await OnDumpFirmwareAsync());

            UpdateFirmwareCommand = new Command(async () => await OnUpdateFirmwareAsync());

            _isBootloaderReady = false;

            // Setting up poll service data timer
            PollServiceDataTimer = new System.Timers.Timer(PollServiceDataInterval);
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
            _mainModel.AppDisplays.Pop();
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
            if (_mainModel.AppDisplays.Peek() != ActiveDisplay.ServiceDisplay)
            {
                return;
            }

            if (_mainModel.ServiceSettingsModel.IsBootloaderMode)
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

            // Uant ADC level
            await _serviceCommandsManager.GetUantADCValueAsync(async (v) => await OnGetUantADCValueResponseAsync(v));

            // Uant voltage
            await _serviceCommandsManager.GetUantVoltageAsync(async (v) => await OnGetUantVoltageResponseAsync(v));
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

        private async Task OnGetUantADCValueResponseAsync(float averagedADCValue)
        {
            _mainModel.ServiceSettingsModel.UantAveragedADCValue = averagedADCValue;
            OnPropertyChanged(nameof(UantADCValueAsString));
        }

        private async Task OnGetUantVoltageResponseAsync(float averagedVoltage)
        {
            _mainModel.ServiceSettingsModel.UantAveragedVoltage = averagedVoltage;
            OnPropertyChanged(nameof(UantVoltageAsString));
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

        #region Get P80m factors

        private async Task OnGetP80mFactorsAsync()
        {
            await _serviceCommandsManager.GetP80mFactorsAsync(async (a, b) => await OnGetP80mFactorsResponseAsync(a, b));
        }

        private async Task OnGetP80mFactorsResponseAsync(float a, float b)
        {
            _mainModel.ServiceSettingsModel.P80mFactorA = a;
            _mainModel.ServiceSettingsModel.P80mFactorB = b;

            P80mAFactorAsString = _mainModel.ServiceSettingsModel.P80mFactorA.ToString();
            P80mBFactorAsString = _mainModel.ServiceSettingsModel.P80mFactorB.ToString();
        }

        #endregion

        #region Reset P80m factors

        private async Task OnResetP80mFactorsAsync()
        {
            await _serviceCommandsManager.ResetP80mFactorsAsync(async () => await OnResetP80mFactorsResponseAsync());
        }

        private async Task OnResetP80mFactorsResponseAsync()
        {
            await OnGetP80mFactorsAsync();
        }

        #endregion

        #region Set P80m factors

        private async Task OnSetP80mFactorsAsync()
        {
            float newA;
            if (!float.TryParse(P80mAFactorAsString, out newA))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor A is not a number!");
                return;
            }

            float newB;
            if (!float.TryParse(P80mBFactorAsString, out newB))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor B is not a number!");
                return;
            }

            await _serviceCommandsManager.SetP80mFactorsAsync(newA, newB, async () => await OnSetP80mFactorsResponseAsync());
        }

        private async Task OnSetP80mFactorsResponseAsync()
        {
            await OnGetP80mFactorsAsync();
        }

        #endregion

        #region Get Uant factors

        private async Task OnGetUantFactorsAsync()
        {
            await _serviceCommandsManager.GetUantFactorsAsync(async (a, b) => await OnGetUantFactorsResponseAsync(a, b));
        }

        private async Task OnGetUantFactorsResponseAsync(float a, float b)
        {
            _mainModel.ServiceSettingsModel.UantFactorA = a;
            _mainModel.ServiceSettingsModel.UantFactorB = b;

            UantAFactorAsString = _mainModel.ServiceSettingsModel.UantFactorA.ToString();
            UantBFactorAsString = _mainModel.ServiceSettingsModel.UantFactorB.ToString();
        }

        #endregion

        #region Reset Uant factors

        private async Task OnResetUantFactorsAsync()
        {
            await _serviceCommandsManager.ResetUantFactorsAsync(async () => await OnResetUantFactorsResponseAsync());
        }

        private async Task OnResetUantFactorsResponseAsync()
        {
            await OnGetUantFactorsAsync();
        }

        #endregion

        #region Set Uant factors

        private async Task OnSetUantFactorsAsync()
        {
            float newA;
            if (!float.TryParse(UantAFactorAsString, out newA))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor A is not a number!");
                return;
            }

            float newB;
            if (!float.TryParse(UantBFactorAsString, out newB))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Factor B is not a number!");
                return;
            }

            await _serviceCommandsManager.SetUantFactorsAsync(newA, newB, async () => await OnSetUantFactorsResponseAsync());
        }

        private async Task OnSetUantFactorsResponseAsync()
        {
            await OnGetUantFactorsAsync();
        }

        #endregion

        #region Force TX On

        private async Task OnForceTxOnAsync()
        {
            await _serviceCommandsManager.ForceTxOnAsync(async (s) => await OnForceTxOnResponseAsync(s));
        }

        private async Task OnForceTxOnResponseAsync(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                await _userNotifier.ShowErrorMessageAsync("Failure", "Unable to turn TX ON.\nIs fox armed?\nIs it already ON?");
            }
        }

        #endregion

        #region Return to normal TX

        private async Task OnTxNormalAsync()
        {
            await _serviceCommandsManager.ReturnToNormalTxAsync(async (s) => await OnTxNormalResponseResponseAsync(s));
        }

        private async Task OnTxNormalResponseResponseAsync(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                await _userNotifier.ShowErrorMessageAsync("Failure", "Unable to return to normal operations. Is fox already in normal mode?");
            }
        }

        #endregion

        #region Get RTC calibration value

        private async Task OnGetRTCCalibrationValueAsync()
        {
            await _serviceCommandsManager.GetRTCCalibrationValueAsync(async(v) => await OnGetRTCCalibrationValueResponseAsync(v));
        }

        private async Task OnGetRTCCalibrationValueResponseAsync(uint value)
        {
            _mainModel.ServiceSettingsModel.RTCCalibrationValue = value;

            RTCCalibrationValueAsString = _mainModel.ServiceSettingsModel.RTCCalibrationValue.ToString();
        }

        #endregion

        #region Set RTC calibration value

        private async Task OnSetRTCCalibrationValueAsync()
        {
            uint newValue;

            if (!uint.TryParse(RTCCalibrationValueAsString, out newValue))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Not a valid calibration value!");
                return;
            }

            await _serviceCommandsManager.SetRTCCalibrationValueAsync(newValue, async (s) => await OnSetRTCCalibrationValueResponseAsync(s));
        }

        private async Task OnSetRTCCalibrationValueResponseAsync(bool isSuccessful)
        {
            await OnGetRTCCalibrationValueAsync();

            if (!isSuccessful)
            {
                await _userNotifier.ShowErrorMessageAsync("Failure", "Unable to set new RTC calibration value.\nIs value in a valid range?");
            }
        }

        #endregion

        #region Get disarm on discharge threshold

        private async Task OnGetDisarmOnDischargeThresholdAsync()
        {
            await _serviceCommandsManager.GetDisarmOnDischargeThresholdAsync(async (v) => await OnGetDisarmOnDischargeThresholdResponseAsync(v));
        }

        private async Task OnGetDisarmOnDischargeThresholdResponseAsync(float value)
        {
            _mainModel.ServiceSettingsModel.DisarmOnDischargeThreshold = value;

            DisarmOnDischargeThresholdAsString = (_mainModel.ServiceSettingsModel.DisarmOnDischargeThreshold * 100).ToString();
        }

        #endregion

        #region Set RTC calibration value

        private async Task OnSetDisarmOnDischargeThresholdAsync()
        {
            float newThreshold;

            if (!float.TryParse(DisarmOnDischargeThresholdAsString, out newThreshold))
            {
                // Invalid value
                await _userNotifier.ShowErrorMessageAsync("Wrong value", "Not a valid disarm on discharge value!");
                return;
            }

            await _serviceCommandsManager
                .SetDisarmOnDischargeThresholdAsync(newThreshold / 100.0f, async (s) => await OnSetDisarmOnDischargeThresholdAsync(s));
        }

        private async Task OnSetDisarmOnDischargeThresholdAsync(bool isSuccessful)
        {
            await OnGetDisarmOnDischargeThresholdAsync();

            if (!isSuccessful)
            {
                await _userNotifier.ShowErrorMessageAsync("Failure", @"Unable to set new disarm on discharge threshold calibration value.
Is value in a valid range?");
            }
        }

        #endregion

        #region Reboot to bootloader

        private async Task OnRebootToBootloaderAsync()
        {
            // Assuming that the fox will successfully reboot to bootloader we enabling bootloader mode (now to avoid sending commands to fox)
            _mainModel.ServiceSettingsModel.IsBootloaderMode = true;

            // Waiting for completion of all pending commands
            while (!_packetsProcessor.IsNoPendingCommands())
            {
                Thread.Sleep(100);
            }

            await _serviceCommandsManager.RebootToBootloaderAsync(async (s) => await OnRebootToBootloaderResponseAsync(s));
        }

        private async Task OnRebootToBootloaderResponseAsync(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                _mainModel.ServiceSettingsModel.IsBootloaderMode = false;

                await _userNotifier.ShowErrorMessageAsync("Failure", @"Unable to reboot into bootloader.
Is fox armed?
Is TX forced?");

                return;
            }

            // Re-estabilishing connection (it is lost because of fox powercycle), but now in bootloader mode
            _foxToReconnect = _mainModel.ConnectedFox;

            _mainModel.OnFoxConnectorDisconnected = DelegatesHelper.RemoveAllMethods(_mainModel.OnFoxConnectorDisconnected);
            _mainModel.OnFoxConnectorDisconnected += async() => await OnFoxDisconnectedAsync();

            _mainModel.OnFoxConnectorConnected = DelegatesHelper.RemoveAllMethods(_mainModel.OnFoxConnectorConnected);
            _mainModel.OnFoxConnectorConnected += async (f) => await OnBootloaderConnectedAsync(f);

            DelegatesHelper.RemoveAllMethods(_mainModel.OnFoxConnectorFailedToConnect);
            _mainModel.OnFoxConnectorFailedToConnect += async (e) => await OnBootloaderFailedToConnectAsync(e);

            _foxConnector.SetupDelegates
            (
                _mainModel.OnFoxConnectorNewByteRead,
                _mainModel.OnFoxConnectorConnected,
                _mainModel.OnFoxConnectorDisconnected,
                _mainModel.OnFoxConnectorFailedToConnect
            );

            await _foxConnector.DisconnectAsync();
        }

        /// <summary>
        /// Called when fox connection (non-bootloader) is dropped
        /// </summary>
        private async Task OnFoxDisconnectedAsync()
        {
            _packetsProcessor.OnDisconnect();

            _mainModel.OnFoxConnectorDisconnected = DelegatesHelper.RemoveAllMethods(_mainModel.OnFoxConnectorDisconnected);
            _mainModel.OnFoxConnectorDisconnected += async () => await OnBootloaderDisconnectAsync();

            _foxConnector.SetupDelegates
            (
                _mainModel.OnFoxConnectorNewByteRead,
                _mainModel.OnFoxConnectorConnected,
                _mainModel.OnFoxConnectorDisconnected,
                _mainModel.OnFoxConnectorFailedToConnect
            );

            await _userNotifier.ShowNotificationMessageAsync("Success", "The fox have to be in DFU mode now.");

            await _foxConnector.ConnectAsync(_foxToReconnect);
        }

        private async Task OnBootloaderConnectedAsync(PairedFoxDTO connectedFox)
        {
            _packetsProcessor.OnConnect();

            // Reqeuesting identification
            await _serviceCommandsManager.GetBootloaderIdentificationData(
                async (isfb, pv, hr, sv, fs, mfs, fe) => await OnGetBootloaderIdentificationDataResponseAsync(isfb, pv, hr, sv, fs, mfs, fe));
        }

        private async Task OnBootloaderDisconnectAsync()
        {
            _packetsProcessor.OnDisconnect();

            await _userNotifier.ShowNotificationMessageAsync("Disconnected", @"Bootloader disconnected.
Application will be terminated.");

            _appCloser.Close();
        }

        private async Task OnBootloaderFailedToConnectAsync(Exception exception)
        {
            await _userNotifier.ShowNotificationMessageAsync("Disconnected", $@"Bootloader disconnected.
Application will be terminated.
Reason: {exception.Message}");

            _appCloser.Close();
        }

        #endregion

        #region Bootloader identification

        private async Task OnGetBootloaderIdentificationDataResponseAsync
        (
            bool isFoxBootloader,
            UInt16 protocolVersion,
            UInt16 hardwareRevision,
            UInt16 firmwareVersion,
            UInt32 flashStartAddress,
            UInt32 mainFirmwareStartAddress,
            UInt32 flashEndAddress
        )
        {
            if (!isFoxBootloader)
            {
                await _userNotifier.ShowErrorMessageAsync("Error", @"Bootloader responded, but it's not a fox bootloader.
Application will be terminated.");

                _appCloser.Close();
            }

            if (protocolVersion != ExpectedBootloaderProtocolVersion)
            {
                await _userNotifier.ShowErrorMessageAsync("Error", $@"Wrong bootloader protocol version.
Expected: { ExpectedBootloaderProtocolVersion },
Got: { protocolVersion }.
Application will be terminated.");

                _appCloser.Close();
            }

            _mainModel.ServiceSettingsModel.BootloaderIdentificationData = new BootloaderIdentificationData()
            {
                HardwareRevision = hardwareRevision,
                FirmwareVersion = firmwareVersion,
                FlashStartAddress = flashStartAddress,
                MainFirmwareStartAddress = mainFirmwareStartAddress,
                FlashEndAddress = flashEndAddress
            };

            _isBootloaderReady = true;
            OnPropertyChanged(nameof(IsRebootToBootloaderButtonEnabled));
            OnPropertyChanged(nameof(IsBootloaderControlsEnabled));
            OnPropertyChanged(nameof(FoxHardwareRevisionAsString));
            OnPropertyChanged(nameof(FoxFirmwareVersionAsString));
            OnPropertyChanged(nameof(FlashStartAddressAsString));
            OnPropertyChanged(nameof(MainFirmwareStartAddressAsString));
            OnPropertyChanged(nameof(FlashEndAddressAsString));
        }

        #endregion

        #region Return from bootloader

        private async Task OnReturnFromBootloaderAsync()
        {
            await _serviceCommandsManager.RebootToMainFirmware(async () => await OnRebootToMainFirmwareResponseAsync());
        }

        private async Task OnRebootToMainFirmwareResponseAsync()
        {
            await _foxConnector.DisconnectAsync();
        }

        #endregion

        #region Read main firmware

        private async Task OnDumpFirmwareAsync()
        {
            await _serviceCommandsManager.ReadMainFirmware(_mainModel.ServiceSettingsModel.BootloaderIdentificationData,
              async (fd) => await OnReadMainFirmwareAsync(fd));
        }

        private async Task OnReadMainFirmwareAsync(List<byte> firmwareDump)
        {
            var dumpFilename = await _filesManager.GenerateDumpFilename(_mainModel);

            await _filesManager.SaveFileAsync(dumpFilename, firmwareDump);

            var foxDirectory = await _filesManager.GetFirmwareDirectoryPath();
            var fullPath = Path.Combine(foxDirectory, dumpFilename);

            await _userNotifier.ShowNotificationMessageAsync("Success", $"Firmware dump saved to { fullPath } file.");
        }

        #endregion

        #region Flash main firmware

        private async Task OnUpdateFirmwareAsync()
        {
            // Selecting file with update
            try
            {
                var options = new PickOptions()
                {
                    PickerTitle = "Select firmware file"
                };

                var result = await FilePicker.PickAsync(options);
                if (result != null)
                {
                    var updateFile = await _filesManager.ReadFileAsync(result.FullPath);

                    var isFirmwareValid = await _serviceCommandsManager.VaildateMainFirmwareAsync
                        (
                            _mainModel.ServiceSettingsModel.BootloaderIdentificationData,
                            updateFile
                        );

                    if (!isFirmwareValid)
                    {
                        await _userNotifier.ShowErrorMessageAsync("Wrong file", "Selected file doesn't contain valid firmware!");
                        return;
                    }

                    // Dumping existing firmware
                    await _serviceCommandsManager.ReadMainFirmware(_mainModel.ServiceSettingsModel.BootloaderIdentificationData,
                        async (fd) => await OnReadMainFirmwareBackupAsync(fd));
                }
            }
            catch (Exception)
            {
                // User cancelled
            }
        }

        private async Task OnReadMainFirmwareBackupAsync(List<byte> firmwareDump)
        {
            var backupFilename = await _filesManager.GenerateBackupFilename(_mainModel);

            await _filesManager.SaveFileAsync(backupFilename, firmwareDump);

            // Erasing main firmware FLASH
        }

        #endregion
    }
}
