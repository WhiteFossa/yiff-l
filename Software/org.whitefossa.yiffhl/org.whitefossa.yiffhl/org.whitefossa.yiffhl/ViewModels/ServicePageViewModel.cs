using org.whitefossa.yiffhl.Abstractions.Enums;
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

            // Setting up poll service data timer
            PollServiceDataTimer = new Timer(PollServiceDataInterval);
            PollServiceDataTimer.Elapsed += async (s, e) => await OnReloadServiceDataRequest(s, e);
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
            await _serviceCommandsManager.GetLastErrorCode(async (ec) => await OnGetLastErrorCodeResponseAsync(ec));
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
            await _serviceCommandsManager.ResetLastErrorCode(async () => await OnResetLastErrorCodeResponse());
        }

        private async Task OnResetLastErrorCodeResponse()
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

            await _serviceCommandsManager.UpdateSerialNumber(newSerialNumber, async () => await OnSerialNumberUpdatedAsync());
        }

        private async Task OnSerialNumberUpdatedAsync()
        {
            await OnReloadSerialNumberAsync();
        }

        #endregion

        #region Reload service data

        private async Task OnReloadServiceDataRequest(Object source, ElapsedEventArgs e)
        {
            if (_mainModel.ActiveDisplay != ActiveDisplay.ServiceDisplay)
            {
                return;
            }

            // Battery ADC level
            await _serviceCommandsManager.GetBatteryADCLevel(async (l) => await OnGetBatteryADCLevelResponse(l));

        }

        private async Task OnGetBatteryADCLevelResponse(float averagedADCLevel)
        {
            _mainModel.ServiceSettingsModel.BatteryAveragedADCLevel = averagedADCLevel;
            OnPropertyChanged(nameof(BatteryADCLevelAsString));
        }

        #endregion
    }
}
