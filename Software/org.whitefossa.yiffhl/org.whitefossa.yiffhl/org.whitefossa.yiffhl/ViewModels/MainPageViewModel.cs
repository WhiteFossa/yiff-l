using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Business.Implementations.Commands;
using org.whitefossa.yiffhl.Models;
using org.whitefossa.yiffhl.Views;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Input;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    internal class MainPageViewModel : BindableObject
    {
        /// <summary>
        /// We support this version of fox protocol
        /// </summary>
        private const UInt16 SupportedProtocolVersion = 1;

        /// <summary>
        /// Minimal frequency for 80m range
        /// </summary>
        private const int MinFrequency80m = 3500000;

        /// <summary>
        /// Maximal frequency for 80m range
        /// </summary>
        private const int MaxFrequency80m = 3650000;

        /// <summary>
        /// Frequency step for 80m range
        /// </summary>
        private const int FrequencyStep80m = 5000;

        /// <summary>
        /// Default frequency for 80m range
        /// </summary>
        private const int DefaultFrequency80m = 3575000;

        /// <summary>
        /// Minimal frequency for 2m range
        /// </summary>
        private const int MinFrequency2m = 144000000;

        /// <summary>
        /// Maximal frequency for 2m range
        /// </summary>
        private const int MaxFrequency2m = 146000000;

        /// <summary>
        /// Frequency step for 2m range
        /// </summary>
        private const int FrequencyStep2m = 50000;

        /// <summary>
        /// Default frequency for 2m range
        /// </summary>
        private const int DefaultFrequency2m = 145000000;

        /// <summary>
        /// Minimal TX duration in seconds
        /// </summary>
        private const int MinTxDuration = 6;

        /// <summary>
        /// Maximal TX duration in seconds
        /// </summary>
        private const int MaxTxDuration = 3599;

        /// <summary>
        /// Minimal pause duration in seconds
        /// </summary>
        private const int MinPauseDuration = 6;

        /// <summary>
        /// Maximal pause duration in seconds
        /// </summary>
        private const int MaxPauseDuration = 3599;

        /// <summary>
        /// Minimal ending tone duration in seconds
        /// </summary>
        private const int MinEndingToneDuration = 0;

        /// <summary>
        /// Maximal ending tone duration in seconds
        /// </summary>
        private const int MaxEndingToneDuration = 5;

        /// <summary>
        /// Change durations by this value when "small step" buttons are pressed
        /// </summary>
        private const int DurationStepSmall = 1;

        /// <summary>
        /// Change durations by this value when "big step" buttons are pressed
        /// </summary>
        private const int DurationStepBig = 15;

        /// <summary>
        /// Change ending tone duration by this value
        /// </summary>
        private const int EndingToneDurationStep = 1;

        /// <summary>
        /// Fine-tune fox run times by this step
        /// </summary>
        private const int RunTimesStep = 1;

        /// <summary>
        /// Minimal allowed power
        /// </summary>
        private const float MinPower = 2.0f;

        /// <summary>
        /// Maximal allowed power
        /// </summary>
        private const float MaxPower = 5.0f;

        /// <summary>
        /// Power change step
        /// </summary>
        private const float PowerStep = 0.1f;

        /// <summary>
        /// Interval in milliseconds to poll fox status
        /// </summary>
        private const int PollFoxStatusInterval = 2000;

        private readonly IFoxConnector _foxConnector;
        private readonly IPairedFoxesEnumerator _pairedFoxesEnumerator;
        private readonly IUserNotifier _userNotifier;
        private readonly IPacketsProcessor _packetsProcessor;
        private readonly IUserRequestor _userRequestor;
        private readonly IBluetoothManager _bluetoothManager;
        private readonly IFoxProfilesEnumerator _foxProfilesEnumerator;
        private readonly IFoxProfilesManager _foxProfilesManager;
        private readonly IFoxIdentificationManager _foxIdentificationManager;
        private readonly IFoxNameManager _foxNameManager;
        private readonly IFoxClockManager _foxClockManager;
        private readonly IProfileSettingsManager _profileSettingsManager;
        private readonly IDynamicFoxStatusManager _dynamicFoxStatusManager;
        private readonly IStaticFoxStatusManager _staticFoxStatusManager;
        private readonly IEventsGenerator _eventsGenerator;

        #region Views

        /// <summary>
        /// Arming view
        /// </summary>
        private ArmingView _matchingView = new ArmingView();

        #endregion

        /// <summary>
        /// Main model
        /// </summary>
        public MainModel MainModel;

        /// <summary>
        /// We use it for navigation
        /// </summary>
        public INavigation Navigation { get; set; }

        /// <summary>
        /// List of foxes, paired to a phone
        /// </summary>
        private ObservableCollection<PairedFoxDTO> _pairedFoxes = new ObservableCollection<PairedFoxDTO>();

        public ObservableCollection<PairedFoxDTO> PairedFoxes
        {
            get
            {
                return _pairedFoxes;
            }
            set
            {
                _pairedFoxes = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Selected fox
        /// </summary>
        private PairedFoxDTO _selectedFox = null;

        public PairedFoxDTO SelectedFox
        {
            get => _selectedFox;
            set
            {
                _selectedFox = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on selected fox change
        /// </summary>
        public ICommand SelectedFoxChangedCommand { get; }

        /// <summary>
        /// True if connect button enabled
        /// </summary>
        private bool _isConnectButtonEnabled;

        public bool IsConnectButtonEnabled
        {
            get => _isConnectButtonEnabled;
            set
            {
                _isConnectButtonEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// True if connect-related controls are enabled
        /// </summary>
        private bool _isConnectRelatedControlsEnabled;

        public bool IsConnectRelatedControlsEnabled
        {
            get => _isConnectRelatedControlsEnabled;
            set
            {
                _isConnectRelatedControlsEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on Connect button press
        /// </summary>
        public ICommand ConnectButtonClickedCommand { get; }

        /// <summary>
        /// Command, executed on Refresh Foxes List button press
        /// </summary>
        public ICommand RefreshFoxesListClickedCommand { get; }

        /// <summary>
        /// Is Disconnect button enabled
        /// </summary>
        private bool _isBtnDisconnectEnabled;

        public bool IsBtnDisconnectEnabled
        {
            get => _isBtnDisconnectEnabled;
            set
            {
                _isBtnDisconnectEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on Disconnect button press
        /// </summary>
        public ICommand DisconnectButtonClickedCommand { get; }

        /// <summary>
        /// Fox name
        /// </summary>
        public string FoxName
        {
            get => MainModel.FoxName;
            set
            {
                MainModel.FoxName = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox hardware revision
        /// </summary>
        public uint FoxHardwareRevision
        {
            get => MainModel.IdentificationData.HardwareRevision;
            set
            {
                MainModel.IdentificationData.HardwareRevision = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox firmware version
        /// </summary>
        public uint FoxFirmwareVersion
        {
            get => MainModel.IdentificationData.FirmwareVersion;
            set
            {
                MainModel.IdentificationData.FirmwareVersion = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Fox serial number
        /// </summary>
        public uint FoxSerialNumber
        {
            get => MainModel.IdentificationData.SerialNumber;
            set
            {
                MainModel.IdentificationData.SerialNumber = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Is fox connected
        /// </summary>
        public bool IsFoxConnected
        {
            get => MainModel.IsConnected;
            set
            {
                MainModel.IsConnected = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, called when user tries to rename a fox
        /// </summary>
        public ICommand RenameFoxClickedCommand { get; }

        /// <summary>
        /// True if fox picker need to be enabled
        /// </summary>
        private bool _isFoxPickerEnabled;

        public bool IsFoxPickerEnabled
        {
            get => _isFoxPickerEnabled;
            set
            {
                _isFoxPickerEnabled = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// List of profiles in fox
        /// </summary>
        private ObservableCollection<Profile> _profiles = new ObservableCollection<Profile>();

        public ObservableCollection<Profile> Profiles
        {
            get => _profiles;
            set
            {
                _profiles = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ProfilesCountText));
                OnPropertyChanged(nameof(IsAddProfileButtonEnabled));
            }
        }

        /// <summary>
        /// Command, called when Refresh Profiles List button is clicked
        /// </summary>
        public ICommand RefreshProfilesListClickedCommand { get; }

        /// <summary>
        /// Can we add profiles
        /// </summary>
        public bool CanWeAddProfiles
        {
            get => Profiles.Count < Constants.MaxProfilesCount;
        }

        /// <summary>
        /// Profiles count
        /// </summary>
        public string ProfilesCountText
        {
            get
            {
                var countAsText = MainModel.IsConnected ? $"{ Profiles.Count }" : "N/A";

                var maxProfilesWarning = CanWeAddProfiles ? String.Empty : ". No space left!";

                return $"Profiles count: { countAsText } {maxProfilesWarning}";
            }
        }

        /// <summary>
        /// Is add profile button enabled
        /// </summary>
        public bool IsAddProfileButtonEnabled
        {
            get => MainModel.IsConnected
                   &&
                   !MainModel.StaticFoxStatus.IsFoxArmed
                   &&
                   CanWeAddProfiles;
        }

        /// <summary>
        /// Command, called when user clicks Add Profile button
        /// </summary>
        public ICommand AddProfileClickedCommand { get; }

        /// <summary>
        /// Selected profile
        /// </summary>
        public Profile SelectedProfile
        {
            get => MainModel.CurrentProfile;
            set
            {
                MainModel.CurrentProfile = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Command, executed on selected profile change
        /// </summary>
        public ICommand SelectedProfileChangedCommand { get; }

        /// <summary>
        /// Command, executed when Rename Profile button is clicked
        /// </summary>
        public ICommand RenameProfileClickedCommand { get; }

        /// <summary>
        /// Formatted frequency range
        /// </summary>
        public string FoxRangeFormatted
        {
            get
            {
                if (MainModel.CurrentProfileSettings.FrequencySettings.Is2m)
                {
                    return "2m";
                }
                else
                {
                    return "80m";
                }
            }
        }

        /// <summary>
        /// Toggle fox frequency range
        /// </summary>
        public ICommand ToggleFoxFrequencyRangeCommand { get; }

        /// <summary>
        /// Formatted frequency for a label
        /// </summary>
        public string FrequencyFormatted
        {
            get
            {
                if (MainModel.CurrentProfileSettings.FrequencySettings.Frequency == 0)
                {
                    return "N/A";
                }
                else
                {
                    return String.Format("{0:0.000 MHz}", MainModel.CurrentProfileSettings.FrequencySettings.Frequency / 1000000.0);
                }
            }
        }

        /// <summary>
        /// Decrease fox frequency
        /// </summary>
        public ICommand DecreaseFoxFrequencyCommand { get; }

        /// <summary>
        /// Increase fox frequency
        /// </summary>
        public ICommand IncreaseFoxFrequencyCommand { get; }

        /// <summary>
        /// Possible fox callsigns
        /// </summary>
        private ObservableCollection<Callsign> _callsigns = new ObservableCollection<Callsign>();

        public ObservableCollection<Callsign> Callsigns
        {
            get => _callsigns;

            set
            {
                _callsigns = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Selected callsign
        /// </summary>
        private Callsign _selectedCallsign = null;

        public Callsign SelectedCallsign
        {
            get => _selectedCallsign;
            set
            {
                _selectedCallsign = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Formatted TX speed
        /// </summary>
        public string TxSpeedFormatted
        {
            get
            {
                if (MainModel.CurrentProfileSettings.CallsignSettings.IsFast)
                {
                    return "Fast";
                }
                else
                {
                    return "Slow";
                }
            }
        }

        /// <summary>
        /// Switch fox speed
        /// </summary>
        public ICommand ToggleFoxSpeedCommand { get; }

        /// <summary>
        /// Command, executed on selected callsign change
        /// </summary>
        public ICommand SelectedCallsignChangedCommand { get; }

        /// <summary>
        /// Fox cycle type
        /// </summary>
        public string FoxCycleTypeFormatted
        {
            get
            {
                if (MainModel.CurrentProfileSettings.CycleSettings.IsContinuous)
                {
                    return "Continuous";
                }
                else
                {
                    return "Cyclic";
                }
            }
        }

        /// <summary>
        /// Transmission duration formatted
        /// </summary>
        public string TxDurationFormatted
        {
            get
            {
                return MainModel.CurrentProfileSettings.CycleSettings.TxDuration.ToString(@"mm\:ss");
            }
        }

        /// <summary>
        /// Is cycle controls enabled (except cycle type switch)
        /// </summary>
        public bool IsCycleControlsEnabled
        {
            get => MainModel.IsConnected
                   &&
                   !MainModel.StaticFoxStatus.IsFoxArmed
                   &&
                   !MainModel.CurrentProfileSettings.CycleSettings.IsContinuous;
        }

        /// <summary>
        /// Pause duration formatted
        /// </summary>
        public string PauseDurationFormatted
        {
            get
            {
                return MainModel.CurrentProfileSettings.CycleSettings.PauseDuration.ToString(@"mm\:ss");
            }
        }

        /// <summary>
        /// Command to togle fox cycle mode
        /// </summary>
        public ICommand ToggleCycleModeCommand { get; }

        /// <summary>
        /// Command to increase TX duration (small)
        /// </summary>
        public ICommand IncreaseTxDurationSmallCommand { get; }

        /// <summary>
        /// Command to increase TX duration (big)
        /// </summary>
        public ICommand IncreaseTxDurationBigCommand { get; }

        /// <summary>
        /// Command to decrease TX duration (small)
        /// </summary>
        public ICommand DecreaseTxDurationSmallCommand { get; }

        /// <summary>
        /// Command to decrease TX duration (big)
        /// </summary>
        public ICommand DecreaseTxDurationBigCommand { get; }

        /// <summary>
        /// Command to increase pause duration (small)
        /// </summary>
        public ICommand IncreasePauseDurationSmallCommand { get; }

        /// <summary>
        /// Command to increase pause duration (big)
        /// </summary>
        public ICommand IncreasePauseDurationBigCommand { get; }

        /// <summary>
        /// Command to decrease pause duration (small)
        /// </summary>
        public ICommand DecreasePauseDurationSmallCommand { get; }

        /// <summary>
        /// Command to decrease pause duration (big)
        /// </summary>
        public ICommand DecreasePauseDurationBigCommand { get; }

        /// <summary>
        /// Command to increase ending tone duration
        /// </summary>
        public ICommand IncreaseEndingToneDurationCommand { get; }

        /// <summary>
        /// Command to decrease ending tone duration
        /// </summary>
        public ICommand DecreaseEndingToneDurationCommand { get; }

        /// <summary>
        /// Ending tone duration formatted
        /// </summary>
        public string EndingToneDurationFormatted
        {
            get
            {
                return $"{ MainModel.CurrentProfileSettings.CycleSettings.EndingToneDuration.TotalSeconds }s";
            }
        }

        /// <summary>
        /// Fox start time
        /// </summary>
        public TimeSpan StartTime
        {
            get
            {
                return MainModel.CurrentProfileSettings.RunTimesSettings.StartTime - DateTime.MinValue;
            }
        }

        /// <summary>
        /// Fox finish time
        /// </summary>
        public TimeSpan FinishTime
        {
            get
            {
                return MainModel.CurrentProfileSettings.RunTimesSettings.FinishTime - DateTime.MinValue;
            }
        }

        /// <summary>
        /// Set fox start time
        /// </summary>
        public ICommand SetStartTimeCommand { get; }

        /// <summary>
        /// Set fox finish time
        /// </summary>
        public ICommand SetFinishTimeCommand { get; }

        /// <summary>
        /// Increase fox start time
        /// </summary>
        public ICommand IncreaseStartTime { get; }

        /// <summary>
        /// Decrease fox start time
        /// </summary>
        public ICommand DecreaseStartTime { get; }

        /// <summary>
        /// Increase fox finish time
        /// </summary>
        public ICommand IncreaseFinishTime { get; }

        /// <summary>
        /// Decrease fox finish time
        /// </summary>
        public ICommand DecreaseFinishTime { get; }

        /// <summary>
        /// Fox power, formatted
        /// </summary>
        public string PowerFormatted
        {
            get
            {
                if (MainModel.CurrentProfileSettings.FrequencySettings.Is2m)
                {
                    return @"N/A";
                }
                else
                {
                    return String.Format("{0:0.0 W}", MainModel.CurrentProfileSettings.PowerSettings.Power);
                }
            }
        }

        /// <summary>
        /// Is power controls enabled
        /// </summary>
        public bool IsPowerControlsEnabled
        {
            get => MainModel.IsConnected
                   &&
                   !MainModel.StaticFoxStatus.IsFoxArmed
                   &&
                   !MainModel.CurrentProfileSettings.FrequencySettings.Is2m;
        }

        /// <summary>
        /// Increase power
        /// </summary>
        public ICommand IncreasePower { get; }

        /// <summary>
        /// Decrease power
        /// </summary>
        public ICommand DecreasePower { get; }

        public string BatteryLevelFormatted
        {
            get => String.Format("{0:0.0%}", MainModel.DynamicFoxStatus.BatteryLevel);
        }

        /// <summary>
        /// Timer to poll fox dynamic status
        /// </summary>
        private Timer PollFoxStatusTimer;

        /// <summary>
        /// Is fox armed? (as a string)
        /// </summary>
        public string IsFoxArmedFormatted
        {
            get => MainModel.StaticFoxStatus.IsFoxArmed ? "Armed" : "Not armed";
        }

        /// <summary>
        /// Is Arm button enabled
        /// </summary>
        public bool IsArmButtonEnabled
        {
            get => MainModel.IsConnected && !MainModel.StaticFoxStatus.IsFoxArmed;
        }

        /// <summary>
        /// Is Disarm button enabled
        /// </summary>
        public bool IsDisarmButtonEnabled
        {
            get => MainModel.IsConnected && MainModel.StaticFoxStatus.IsFoxArmed;
        }

        /// <summary>
        /// Arm fox
        /// </summary>
        public ICommand ArmFoxCommand { get; }

        /// <summary>
        /// Disarm fox
        /// </summary>
        public ICommand DisarmFoxCommand { get; }

        /// <summary>
        /// Is settings controls enabled
        /// </summary>
        public bool IsSettingsControlsEnabled
        {
            get => MainModel.IsConnected && !MainModel.StaticFoxStatus.IsFoxArmed;
        }

        public MainPageViewModel()
        {
            MainModel = App.Container.Resolve<IMainModel>() as MainModel;

            _foxConnector = App.Container.Resolve<IFoxConnector>();
            _pairedFoxesEnumerator = App.Container.Resolve<IPairedFoxesEnumerator>();
            _userNotifier = App.Container.Resolve<IUserNotifier>();
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _userRequestor = App.Container.Resolve<IUserRequestor>();
            _bluetoothManager = App.Container.Resolve<IBluetoothManager>();
            _foxProfilesEnumerator = App.Container.Resolve<IFoxProfilesEnumerator>();
            _foxProfilesManager = App.Container.Resolve<IFoxProfilesManager>();
            _foxIdentificationManager = App.Container.Resolve<IFoxIdentificationManager>();
            _foxNameManager = App.Container.Resolve<IFoxNameManager>();
            _foxClockManager = App.Container.Resolve<IFoxClockManager>();
            _profileSettingsManager = App.Container.Resolve<IProfileSettingsManager>();
            _dynamicFoxStatusManager = App.Container.Resolve<IDynamicFoxStatusManager>();
            _staticFoxStatusManager = App.Container.Resolve<IStaticFoxStatusManager>();
            _eventsGenerator = App.Container.Resolve<IEventsGenerator>();

            // Setting up fox connector delegates
            MainModel.OnFoxConnectorNewByteRead += OnNewByteRead;
            MainModel.OnFoxConnectorConnected += OnConnectedAsync;
            MainModel.OnFoxConnectorDisconnected += OnDisconnected;
            MainModel.OnFoxConnectorFailedToConnect += OnFailedToConnect;

            _foxConnector.SetupDelegates
            (
                MainModel.OnFoxConnectorNewByteRead,
                MainModel.OnFoxConnectorConnected,
                MainModel.OnFoxConnectorDisconnected,
                MainModel.OnFoxConnectorFailedToConnect
            );

            // Setting up fox events delegates
            MainModel.OnEnteringSleepmode += OnEnteringSleepmode;

            
            MainModel.OnProfileSettingsChanged += async (e) => await OnProfileSettingsChangedFromMenuAsync(e);
            MainModel.OnProfileSwitched += async (e) => await OnProfileSwitchedFromMenuAsync(e);

            _packetsProcessor.RegisterOnEnteringSleepmodeEventHandler(MainModel.OnEnteringSleepmode);
            _packetsProcessor.RegisterOnProfileSettingsChangedEventHandler(MainModel.OnProfileSettingsChanged);
            _packetsProcessor.RegisterOnProfileSwitchedEventHandler(MainModel.OnProfileSwitched);

            // Setting up events generato events handlers
            _eventsGenerator.RegisterOnShowMatchingDisplayHandler(NavigateToMatchingPage);

            MainModel.OnFoxArmed += async () => await OnFoxArmedAsync();
            _eventsGenerator.RegisterOnFoxArmedHandler(MainModel.OnFoxArmed);

            MainModel.OnFoxDisarmed += async () => await OnFoxDisarmedAsync();
            _eventsGenerator.RegisterOnFoxDisarmedHandler(MainModel.OnFoxDisarmed);

            // Binding commands to handlers
            SelectedFoxChangedCommand = new Command<PairedFoxDTO>(async (f) => await OnSelectedFoxChangedAsync(f));
            ConnectButtonClickedCommand = new Command(async () => await OnConnectButtonCLickedAsync());
            RefreshFoxesListClickedCommand = new Command(async () => await OnRefreshFoxesListClickedAsync());
            DisconnectButtonClickedCommand = new Command(async () => await OnDisconnectButtonClickedAsync());
            RenameFoxClickedCommand = new Command(async () => await OnRenameFoxClickedAsync());
            RefreshProfilesListClickedCommand = new Command(async () => await OnRefreshProfilesListClickedAsync());
            AddProfileClickedCommand = new Command(async () => await OnAddProfileClickedAsync());
            SelectedProfileChangedCommand = new Command<Profile>(async (p) => await OnChangeSelectedProfileAsync(p));
            RenameProfileClickedCommand = new Command(async () => await OnRenameProfileClickedAsync());
            ToggleFoxFrequencyRangeCommand = new Command(async () => await OnToggleFoxFrequencyRangeAsync());
            DecreaseFoxFrequencyCommand = new Command(async () => await OnDecreaseFoxFrequencyAsync());
            IncreaseFoxFrequencyCommand = new Command(async () => await OnIncreaseFoxFrequencyAsync());
            ToggleFoxSpeedCommand = new Command(async () => await OnToggleFoxSpeedAsync());
            SelectedCallsignChangedCommand = new Command<Callsign>(async (c) => await OnChangeSelectedCallsignAsync(c));
            ToggleCycleModeCommand = new Command(async () => await OnToggleCycleModeAsync());

            IncreaseTxDurationSmallCommand = new Command(async () => await OnIncreaseTxDurationAsync(DurationStepSmall));
            IncreaseTxDurationBigCommand = new Command(async () => await OnIncreaseTxDurationAsync(DurationStepBig));

            DecreaseTxDurationSmallCommand = new Command(async () => await OnDecreaseTxDurationAsync(DurationStepSmall));
            DecreaseTxDurationBigCommand = new Command(async () => await OnDecreaseTxDurationAsync(DurationStepBig));

            IncreasePauseDurationSmallCommand = new Command(async () => await OnIncreasePauseDurationAsync(DurationStepSmall));
            IncreasePauseDurationBigCommand = new Command(async () => await OnIncreasePauseDurationAsync(DurationStepBig));

            DecreasePauseDurationSmallCommand = new Command(async() => await OnDecreasePauseDurationAsync(DurationStepSmall));
            DecreasePauseDurationBigCommand = new Command(async () => await OnDecreasePauseDurationAsync(DurationStepBig));

            IncreaseEndingToneDurationCommand = new Command(async() => await OnIncreaseEndingToneDurationAsync(EndingToneDurationStep));
            DecreaseEndingToneDurationCommand = new Command(async () => await OnDecreaseEndingToneDurationAsync(EndingToneDurationStep));

            SetStartTimeCommand = new Command<TimeSpan>(async(ts) => await OnSetStartTimeAsync(ts));
            SetFinishTimeCommand = new Command<TimeSpan>(async (ts) => await OnSetFinishTimeAsync(ts));

            IncreaseStartTime = new Command(async() => await OnApplyDeltaToStartTimeAsync(RunTimesStep));
            DecreaseStartTime = new Command(async () => await OnApplyDeltaToStartTimeAsync(-1 * RunTimesStep));

            IncreaseFinishTime = new Command(async () => await OnApplyDeltaToFinishTimeAsync(RunTimesStep));
            DecreaseFinishTime = new Command(async () => await OnApplyDeltaToFinishTimeAsync(-1 * RunTimesStep));

            IncreasePower = new Command(async () => await OnIncreasePowerAsync(PowerStep));
            DecreasePower = new Command(async () => await OnDecreasePowerAsync(PowerStep));

            ArmFoxCommand = new Command(async () => await OnArmFoxAsync());
            DisarmFoxCommand = new Command(async () => await OnDisarmFoxAsync());

            // Initial state
            IsConnectButtonEnabled = false;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
            IsBtnDisconnectEnabled = false;

            ResetFoxRelatedData();

            // Requesting foxes list
            Task.WaitAll(OnRefreshFoxesListClickedAsync());

            // Callsigns list
            Task.WaitAll(LoadCallsignsAsync());

            // Setting up fox poll timer
            PollFoxStatusTimer = new Timer(PollFoxStatusInterval);
            PollFoxStatusTimer.Elapsed += async (s, e) => await OnDynamicFoxStatusPollRequest(s, e);
            PollFoxStatusTimer.AutoReset = true;
            PollFoxStatusTimer.Start();
        }

        public async Task OnSelectedFoxChangedAsync(PairedFoxDTO selectedFox)
        {
            IsConnectRelatedControlsEnabled = selectedFox != null && !IsFoxConnected;
            IsConnectButtonEnabled = IsConnectRelatedControlsEnabled;
        }

        public async Task OnConnectButtonCLickedAsync()
        {
            IsConnectButtonEnabled = false;
            IsFoxPickerEnabled = false;
            IsConnectRelatedControlsEnabled = false;

            await _foxConnector.ConnectAsync(SelectedFox);
        }

        public async Task OnRefreshFoxesListClickedAsync()
        {
            PairedFoxes = await EnumerateFoxesAsync();
        }

        public async Task OnDisconnectButtonClickedAsync()
        {
            IsBtnDisconnectEnabled = false;

            await _foxConnector.DisconnectAsync();
        }

        private async Task<ObservableCollection<PairedFoxDTO>> EnumerateFoxesAsync()
        {
            try
            {
                return new ObservableCollection<PairedFoxDTO>(await _pairedFoxesEnumerator.EnumerateAsync());
            }
            catch (InvalidOperationException ex)
            {
                Device.BeginInvokeOnMainThread(async () =>
                {
                    await _userNotifier.ShowErrorMessageAsync("Bluetooth error:", ex.Message);
                });

                return new ObservableCollection<PairedFoxDTO>();
            }
        }

        private void OnNewByteRead(byte data)
        {
            _packetsProcessor.NewByteReceived(data);
        }

        private async void OnConnectedAsync(PairedFoxDTO connectedFox)
        {
            MainModel.ConnectedFox = connectedFox;
            IsFoxConnected = true;

            IsBtnDisconnectEnabled = true;

            NotifyControlsOnConnectionStatusChange();

            // Requesting fox identification
            await _foxIdentificationManager.IdentifyFoxAsync(async (isFox, pVer, hwRev, fwVer, sn)
                => await OnGetIdentificationDataResponseAsync(isFox, pVer, hwRev, fwVer, sn));
        }

        private void OnDisconnected()
        {
            IsFoxConnected = false;
            MainModel.ConnectedFox = null;

            ResetFoxRelatedData();

            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;

            NotifyControlsOnConnectionStatusChange();
        }

        private void NotifyControlsOnConnectionStatusChange()
        {
            #region Notifying controls

            OnPropertyChanged(nameof(IsArmButtonEnabled));
            OnPropertyChanged(nameof(IsDisarmButtonEnabled));
            OnPropertyChanged(nameof(IsSettingsControlsEnabled));
            OnPropertyChanged(nameof(IsCycleControlsEnabled));
            OnPropertyChanged(nameof(IsPowerControlsEnabled));
            OnPropertyChanged(nameof(IsAddProfileButtonEnabled));

            #endregion
        }

        private void OnFailedToConnect(Exception exception)
        {
            IsConnectButtonEnabled = true;
            IsFoxPickerEnabled = true;
            IsConnectRelatedControlsEnabled = true;
        }

        /// <summary>
        /// Called when fox identification is received
        /// </summary>
        private async Task OnGetIdentificationDataResponseAsync
        (
            bool isFox,
            uint protocolVersion,
            uint hardwareRevision,
            uint firmwareVersion,
            uint serialNumber
        )
        {
            if (!isFox)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:", "You've tried to connect to not a fox!");
                return;
            }

            if (protocolVersion != SupportedProtocolVersion)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:",
                    $"Unsupported fox protocol version. Got { protocolVersion } while expecting { SupportedProtocolVersion }");

                return;
            }

            FoxHardwareRevision = hardwareRevision;
            FoxFirmwareVersion = firmwareVersion;
            FoxSerialNumber = serialNumber;

            // Requesting fox status
            await LoadStaticFoxStatusAsync();

            // Requesting fox name
            await _foxNameManager.GetNameAsync(async (name) => await OnGetFoxNameResponseAsync(name));

            // Enumerating profiles
            await EnumerateProfilesAsync();
        }

        /// <summary>
        /// Called when fox name is received
        /// </summary>
        private async Task OnGetFoxNameResponseAsync(string name)
        {
            FoxName = name;
        }

        /// <summary>
        /// Resets fox-related data (empties fox name, sets hardware version to zero and so on)
        /// </summary>
        private void ResetFoxRelatedData()
        {
            FoxName = "N/A";
            FoxHardwareRevision = 0;
            FoxFirmwareVersion = 0;
            FoxSerialNumber = 0;
        }

        private async Task OnSetFoxDateAndTimeResponseAsync(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                await OnDisconnectButtonClickedAsync();

                await _userNotifier.ShowErrorMessageAsync("Error:", "Failed to set fox date and time!");
                return;
            }
        }

        private async Task OnRenameFoxClickedAsync()
        {
            var isConfirmed = await _userNotifier.ShowYesNoRequestAsync("Confirmation",
                @"Please take into account, that after fox rename the Bluetooth connection will be lost.
You will have to un-pair old device and pair with a fox under a new name in a phone settings, then reconnect to it.
Do you want to continue?");

            if (!isConfirmed)
            {
                return;
            }

            var newNameData = await _userRequestor.EnterStringAsync
            (
                "Fox name",
                "Enter new fox name",
                FoxName,
                SetFoxNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetFoxNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Digits, numbers, whitespaces
                    if (!args.Value.All(c => (char.IsWhiteSpace(c) || char.IsLetterOrDigit(c))))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!newNameData.Item1)
            {
                return;
            }

            await _foxNameManager.SetNameAsync(newNameData.Item2, async (isSuccessfull) => await OnSetFoxNameResponseAsync(isSuccessfull));
        }

        private async Task OnSetFoxNameResponseAsync(bool isSuccessfull)
        {
            var fox = MainModel.ConnectedFox;

            await OnDisconnectButtonClickedAsync();

            if (!isSuccessfull)
            {
                await _userNotifier.ShowErrorMessageAsync("Error", "Failed to change fox name!");
                return;
            }

            // Un-pairing bluetooth device
            _bluetoothManager.UnPair(fox.MAC);

            await OnRefreshFoxesListClickedAsync();

            await _userNotifier.ShowErrorMessageAsync("Success", "Fox name is changed successfully.");
        }

        private async Task EnumerateProfilesAsync()
        {
            await _foxProfilesEnumerator.EnumerateProfilesAsync(async (profiles) => await OnFoxProfilesEnumeratedAsync(profiles));
        }

        private async Task OnFoxProfilesEnumeratedAsync(IReadOnlyCollection<Profile> profiles)
        {
            Profiles = new ObservableCollection<Profile>(profiles);

            await _foxProfilesManager.GetCurrentProfileId(OnGetCurrentProfileIdResponse);
        }

        private async Task OnRefreshProfilesListClickedAsync()
        {
            await EnumerateProfilesAsync();
        }

        private async Task OnAddProfileClickedAsync()
        {
            var profileNameData = await _userRequestor.EnterStringAsync
            (
                "New profile",
                "Enter new profile name",
                $"New profile { Profiles.Count + 1 }",
                SetProfileNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetProfileNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Digits, numbers, whitespaces and dot
                    if (!args.Value.All(c => (char.IsWhiteSpace(c) || char.IsLetterOrDigit(c) || c == '.')))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!profileNameData.Item1)
            {
                return;
            }

            await _foxProfilesManager.AddProfileAsync(profileNameData.Item2, OnFoxProfileAdded);
        }

        private void OnFoxProfileAdded()
        {
            Task.WaitAll(EnumerateProfilesAsync());
        }

        public async Task OnChangeSelectedProfileAsync(Profile selectedProfile)
        {
            if (selectedProfile == null)
            {
                return;
            }

            MainModel.CurrentProfile = selectedProfile;

            // Switching profile in the fox
            await _foxProfilesManager.SwitchProfileAsync(selectedProfile.Id, async () => await OnSelectedProfileChangedAsync());
        }

        private async Task OnSelectedProfileChangedAsync()
        {
            // We don't need to re-read current profile, because SwitchProfileAsync() guarantee that no errors happened
            await LoadProfileSettingsAsync();
        }

        private void OnGetCurrentProfileIdResponse(int profileId)
        {
            SelectedProfile = Profiles
                .FirstOrDefault(p => p.Id == profileId);
        }

        private async Task OnRenameProfileClickedAsync()
        {
            // We are always renaming current profile, so there is no need to switch profile
            var profileNameData = await _userRequestor.EnterStringAsync
            (
                "Rename profile",
                $"Current name: { SelectedProfile.Name }",
                SelectedProfile.Name,
                SetProfileNameCommand.MaxNameLength,
                (args) =>
                {
                    args.IsValid = true;

                    // Minimal length
                    if (args.Value.Length < SetProfileNameCommand.MinNameLength)
                    {
                        args.IsValid = false;
                        return;
                    }

                    // Digits, numbers, whitespaces and dot
                    if (!args.Value.All(c => (char.IsWhiteSpace(c) || char.IsLetterOrDigit(c) || c == '.')))
                    {
                        args.IsValid = false;
                        return;
                    }
                }
            );

            if (!profileNameData.Item1)
            {
                return;
            }

            await _foxProfilesManager.RenameCurrentProfileAsync(profileNameData.Item2, async() => await OnProfileRenamedAsync());
        }

        private async Task OnProfileRenamedAsync()
        {
            await EnumerateProfilesAsync();
        }

        /// <summary>
        /// Entry point for profile data loading
        /// </summary>
        /// <returns></returns>
        private async Task LoadProfileSettingsAsync()
        {
            await LoadFrequencySettingsAsync();
        }

        private async Task LoadFrequencySettingsAsync()
        {
            await _profileSettingsManager.LoadFrequencySettingsAsync(OnGetFrequencySettings_LoadPathway);
        }

        private async Task ReloadFrequencySettingsAsync()
        {
            await _profileSettingsManager.LoadFrequencySettingsAsync(OnGetFrequencySettings_ReloadPathway);
        }

        #region Load frequency settings

        private async void OnGetFrequencySettings_LoadPathway(FrequencySettings settings)
        {
            OnGetFrequencySettings_Common(settings);

            // Load fox callsign next
            await _profileSettingsManager.LoadCallsignSettingsAsync(OnGetCallsignSettings_LoadPathway);
        }

        private void OnGetFrequencySettings_ReloadPathway(FrequencySettings settings)
        {
            OnGetFrequencySettings_Common(settings);
        }

        private void OnGetFrequencySettings_Common(FrequencySettings settings)
        {
            MainModel.CurrentProfileSettings.FrequencySettings = settings;
            OnPropertyChanged(nameof(FoxRangeFormatted));
            OnPropertyChanged(nameof(FrequencyFormatted));
            OnPropertyChanged(nameof(PowerFormatted));
        }

        #endregion

        #region Toggle frequency range

        private async Task OnToggleFoxFrequencyRangeAsync()
        {
            var settings = (FrequencySettings)MainModel.CurrentProfileSettings.FrequencySettings.Clone();

            settings.Is2m = !settings.Is2m;

            if (settings.Is2m)
            {
                settings.Frequency = DefaultFrequency2m;
            }
            else
            {
                settings.Frequency = DefaultFrequency80m;
            }

            await SetFrequencyAsync(settings);
        }

        private async Task SetFrequencyAsync(FrequencySettings frequencySettings)
        {
            await _profileSettingsManager.SetFrequencySettingsAsync(frequencySettings,
                async () => await OnSetFrequencySettingsAsync());
        }

        private async Task OnSetFrequencySettingsAsync()
        {
            // Re-reading data from fox
            await ReloadFrequencySettingsAsync();
        }

        #endregion

        #region Frequency change

        private async Task OnDecreaseFoxFrequencyAsync()
        {
            var settings = (FrequencySettings)MainModel.CurrentProfileSettings.FrequencySettings.Clone();

            int step;
            int minValue;

            if (settings.Is2m)
            {
                step = FrequencyStep2m;
                minValue = MinFrequency2m;
            }
            else
            {
                step = FrequencyStep80m;
                minValue = MinFrequency80m;
            }

            settings.Frequency -= step;
            if (settings.Frequency < minValue)
            {
                settings.Frequency = minValue;
            }

            await SetFrequencyAsync(settings);
        }

        private async Task OnIncreaseFoxFrequencyAsync()
        {
            var settings = (FrequencySettings)MainModel.CurrentProfileSettings.FrequencySettings.Clone();

            int step;
            int maxValue;

            if (settings.Is2m)
            {
                step = FrequencyStep2m;
                maxValue = MaxFrequency2m;
            }
            else
            {
                step = FrequencyStep80m;
                maxValue = MaxFrequency80m;
            }

            settings.Frequency += step;
            if (settings.Frequency > maxValue)
            {
                settings.Frequency = maxValue;
            }

            await SetFrequencyAsync(settings);
        }

        #endregion

        private async Task LoadCallsignsAsync()
        {
            Callsigns = new ObservableCollection<Callsign>(await _profileSettingsManager.GetCallsignsAsync());
        }

        #region Load callsign settings

        private async void OnGetCallsignSettings_LoadPathway(CallsignSettings settings)
        {
            OnGetSpeed_Common(settings);
            OnGetCallsign_Common(settings);

            // Loading cycle parameters next
            await _profileSettingsManager.LoadCycleSettingsAsync(OnGetCycleSettings_LoadPathway);
        }

        private void OnGetSpeed_ReloadPathway(CallsignSettings settings)
        {
            OnGetSpeed_Common(settings);
        }

        private void OnGetSpeed_Common(CallsignSettings settings)
        {
            MainModel.CurrentProfileSettings.CallsignSettings.IsFast = settings.IsFast;
            OnPropertyChanged(nameof(TxSpeedFormatted));
        }

        private void OnGetCallsign_ReloadPathway(CallsignSettings settings)
        {
            OnGetCallsign_Common(settings);
        }

        private void OnGetCallsign_Common(CallsignSettings settings)
        {
            MainModel.CurrentProfileSettings.CallsignSettings.Callsing = settings.Callsing;
            SelectedCallsign = Callsigns
                .FirstOrDefault(cs => cs.Code == MainModel.CurrentProfileSettings.CallsignSettings.Callsing.Code);
        }

        #endregion

        #region Toggle fox speed

        private async Task OnToggleFoxSpeedAsync()
        {
            await _profileSettingsManager.SetSpeedAsync(!MainModel.CurrentProfileSettings.CallsignSettings.IsFast,
                async () => await OnSetSpeedAsync());
        }

        private async Task OnSetSpeedAsync()
        {
            await _profileSettingsManager.LoadCallsignSettingsAsync(OnGetSpeed_ReloadPathway);
        }

        #endregion

        #region Change callsign

        public async Task OnChangeSelectedCallsignAsync(Callsign callsing)
        {
            await _profileSettingsManager.SetCallsingAsync(callsing, async () => await OnSetCallsignAsync());
        }

        private async Task OnSetCallsignAsync()
        {
            await _profileSettingsManager.LoadCallsignSettingsAsync(OnGetCallsign_ReloadPathway);
        }

        #endregion

        #region Load cycle settings

        private void OnGetCycleSettings_Common(CycleSettings settings)
        {
            MainModel.CurrentProfileSettings.CycleSettings = settings;
            OnPropertyChanged(nameof(FoxCycleTypeFormatted));
            OnPropertyChanged(nameof(TxDurationFormatted));
            OnPropertyChanged(nameof(PauseDurationFormatted));
            OnPropertyChanged(nameof(IsCycleControlsEnabled));
            OnPropertyChanged(nameof(EndingToneDurationFormatted));
        }

        private async void OnGetCycleSettings_LoadPathway(CycleSettings settings)
        {
            OnGetCycleSettings_Common(settings);

            // Loading run times
            await _profileSettingsManager.LoadRunTimesSettinesAsync(OnGetRunTimesSettings_LoadPathway);
        }

        private void OnGetCycleSettings_ReloadPathway(CycleSettings settings)
        {
            OnGetCycleSettings_Common(settings);
        }

        #endregion

        #region Toggle cycle mode

        private async Task OnToggleCycleModeAsync()
        {
            var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
            newSettings.IsContinuous = !newSettings.IsContinuous;

            await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
        }

        private async Task OnSetCycleSettingsAsync()
        {
            await _profileSettingsManager.LoadCycleSettingsAsync(OnGetCycleSettings_ReloadPathway);
        }

        #endregion

        #region TX duration

        public async Task OnIncreaseTxDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.TxDuration + new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds <= MaxTxDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.TxDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        public async Task OnDecreaseTxDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.TxDuration - new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds >= MinTxDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.TxDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        #endregion

        #region Pause duration

        public async Task OnIncreasePauseDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.PauseDuration + new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds <= MaxPauseDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.PauseDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        public async Task OnDecreasePauseDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.PauseDuration - new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds >= MinPauseDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.PauseDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        #endregion

        #region Ending tone duration

        public async Task OnIncreaseEndingToneDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.EndingToneDuration + new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds <= MaxEndingToneDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.EndingToneDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        public async Task OnDecreaseEndingToneDurationAsync(int delta)
        {
            var newValue = MainModel.CurrentProfileSettings.CycleSettings.EndingToneDuration - new TimeSpan(0, 0, delta);

            if (newValue.TotalSeconds >= MinEndingToneDuration)
            {
                var newSettings = MainModel.CurrentProfileSettings.CycleSettings;
                newSettings.EndingToneDuration = newValue;
                await _profileSettingsManager.SetCycleSettingsAsync(newSettings, async () => await OnSetCycleSettingsAsync());
            }
        }

        #endregion

        #region Load start and finish times

        private async void OnGetRunTimesSettings_LoadPathway(RunTimesSettings settings)
        {
            OnGetRunTimesSettings_Common(settings);

            await _profileSettingsManager.LoadPowerSettingsAsync(OnGetPowerSettings_LoadPathway);
        }

        private void OnGetRunTimesSettings_Common(RunTimesSettings settings)
        {
            MainModel.CurrentProfileSettings.RunTimesSettings = settings;
            OnPropertyChanged(nameof(StartTime));
            OnPropertyChanged(nameof(FinishTime));
        }

        private void OnGetRunTimesSettings_ReloadPathway(RunTimesSettings settings)
        {
            OnGetRunTimesSettings_Common(settings);
        }

        #endregion

        #region Set run times

        private async Task OnSetStartTimeAsync(TimeSpan timespan)
        {
            var newSettings = MainModel.CurrentProfileSettings.RunTimesSettings;
            newSettings.StartTime = PrepareFoxRunTime(timespan);
            await _profileSettingsManager.SetRunTimesSettingsAsync(newSettings, async () => await OnSetRunTimesAsync());
        }

        private async Task OnSetFinishTimeAsync(TimeSpan timespan)
        {
            var newSettings = MainModel.CurrentProfileSettings.RunTimesSettings;
            newSettings.FinishTime = PrepareFoxRunTime(timespan);
            await _profileSettingsManager.SetRunTimesSettingsAsync(newSettings, async () => await OnSetRunTimesAsync());
        }

        private async Task OnApplyDeltaToStartTimeAsync(int delta)
        {
            var newSettings = MainModel.CurrentProfileSettings.RunTimesSettings;
            newSettings.StartTime = ApplyDeltaToTimeWithLoop(newSettings.StartTime, new TimeSpan(0, 0, delta));
            await _profileSettingsManager.SetRunTimesSettingsAsync(newSettings, async () => await OnSetRunTimesAsync());
        }

        private async Task OnApplyDeltaToFinishTimeAsync(int delta)
        {
            var newSettings = MainModel.CurrentProfileSettings.RunTimesSettings;
            newSettings.FinishTime = ApplyDeltaToTimeWithLoop(newSettings.FinishTime, new TimeSpan(0, 0, delta));
            await _profileSettingsManager.SetRunTimesSettingsAsync(newSettings, async () => await OnSetRunTimesAsync());
        }

        private DateTime PrepareFoxRunTime(TimeSpan runTime)
        {
            var result = new DateTime(DateTime.MinValue.Year, DateTime.MinValue.Month, DateTime.MinValue.Day);
            result += runTime;

            return result;
        }

        public DateTime ApplyDeltaToTimeWithLoop(DateTime time, TimeSpan delta)
        {
            time = new DateTime(2000, 1, 1, time.Hour, time.Minute, time.Second); // Random date, don't care about it

            time += delta;

            return new DateTime(DateTime.MinValue.Year, DateTime.MinValue.Month, DateTime.MinValue.Day, time.Hour, time.Minute, time.Second);
        }

        private async Task OnSetRunTimesAsync()
        {
            await _profileSettingsManager.LoadRunTimesSettinesAsync(OnGetRunTimesSettings_ReloadPathway);
        }

        #endregion

        #region Load power settings

        private void OnGetPowerSettings_LoadPathway(PowerSettings settings)
        {
            OnGetPowerSettings_Common(settings);
        }

        private void OnGetPowerSettings_Common(PowerSettings settings)
        {
            MainModel.CurrentProfileSettings.PowerSettings = settings;
            OnPropertyChanged(nameof(PowerFormatted));
        }

        private void OnGetPowerSettings_ReloadPathway(PowerSettings settings)
        {
            OnGetPowerSettings_Common(settings);
        }

        #endregion

        #region Set power

        public async Task OnIncreasePowerAsync(float delta)
        {
            var newValue = MainModel.CurrentProfileSettings.PowerSettings.Power + delta;

            if (newValue <= MaxPower)
            {
                var newSettings = MainModel.CurrentProfileSettings.PowerSettings;
                newSettings.Power = newValue;
                await _profileSettingsManager.SetPowerSettingsAsync(newSettings, async () => await OnSetPowerSettingsAsync());
            }
        }

        public async Task OnDecreasePowerAsync(float delta)
        {
            var newValue = MainModel.CurrentProfileSettings.PowerSettings.Power - delta;

            if (newValue >= MinPower)
            {
                var newSettings = MainModel.CurrentProfileSettings.PowerSettings;
                newSettings.Power = newValue;
                await _profileSettingsManager.SetPowerSettingsAsync(newSettings, async () => await OnSetPowerSettingsAsync());
            }
        }

        private async Task OnSetPowerSettingsAsync()
        {
            await _profileSettingsManager.LoadPowerSettingsAsync(OnGetPowerSettings_ReloadPathway);
        }

        #endregion

        #region Fox status polling

        private async Task OnDynamicFoxStatusPollRequest(Object source, ElapsedEventArgs e)
        {
            if (!MainModel.IsConnected)
            {
                return;
            }

            await _dynamicFoxStatusManager.GetDynamicFoxStatusAsync(async (s) => await OnGetDynamicFoxStatusAsync(s));
        }

        private async Task OnGetDynamicFoxStatusAsync(DynamicFoxStatus status)
        {
            MainModel.DynamicFoxStatus = status;

            await _eventsGenerator.GenerateEventsAsync(MainModel);

            OnPropertyChanged(nameof(BatteryLevelFormatted));
            await _matchingView.OnMatchingStatusChangedAsync();
        }

        #endregion

        #region Fox-generated events

        private async Task OnFoxArmedAsync()
        {
            Debug.WriteLine("Fox armed.");

            await _staticFoxStatusManager.GetStaticFoxStatusAsync(async (s) => await OnGetStaticFoxStatusAsync_ReloadPathway(s));
        }

        private void OnEnteringSleepmode(IEnteringSleepmodeEvent enteringSleepmodeEvent)
        {
            Debug.WriteLine("Entering sleepmode");
        }

        private async Task OnFoxDisarmedAsync()
        {
            Debug.WriteLine("Fox disarmed.");

            await _matchingView.LeaveMatchingDisplayAsync();

            await _staticFoxStatusManager.GetStaticFoxStatusAsync(async (s) => await OnGetStaticFoxStatusAsync_ReloadPathway(s));
        }

        /// <summary>
        /// Called when profile settings changed from menu on fox
        /// </summary>
        private async Task OnProfileSettingsChangedFromMenuAsync(IProfileSettingsChangedEvent settingsChangedEvent)
        {
            await LoadProfileSettingsAsync();
        }

        /// <summary>
        /// Called when profile settings changed from menu on fox
        /// </summary>
        private async Task OnProfileSwitchedFromMenuAsync(IProfileSwitchedEvent profileSwitchedEvent)
        {
            await _foxProfilesManager.GetCurrentProfileId(OnGetCurrentProfileIdResponse);
        }

        #endregion

        #region Static fox status

        private async Task LoadStaticFoxStatusAsync()
        {
            await _staticFoxStatusManager.GetStaticFoxStatusAsync(async (s) => await OnGetStaticFoxStatusAsync_LoadPathway(s));
        }

        private async Task OnGetStaticFoxStatusAsync_LoadPathway(StaticFoxStatus status)
        {
            OnGetStaticFoxStatus_Common(status);

            if (!MainModel.StaticFoxStatus.IsFoxArmed)
            {
                // Setting fox time
                await _foxClockManager.SynchronizeClockAsync(async (isSuccessfull) => await OnSetFoxDateAndTimeResponseAsync(isSuccessfull));
            }
        }

        private async Task OnGetStaticFoxStatusAsync_ReloadPathway(StaticFoxStatus status)
        {
            OnGetStaticFoxStatus_Common(status);
        }

        private void OnGetStaticFoxStatus_Common(StaticFoxStatus status)
        {
            MainModel.StaticFoxStatus = status;
            OnPropertyChanged(nameof(IsFoxArmedFormatted));
            OnPropertyChanged(nameof(IsArmButtonEnabled));
            OnPropertyChanged(nameof(IsDisarmButtonEnabled));
            OnPropertyChanged(nameof(IsSettingsControlsEnabled));
            OnPropertyChanged(nameof(IsCycleControlsEnabled));
            OnPropertyChanged(nameof(IsPowerControlsEnabled));
            OnPropertyChanged(nameof(IsAddProfileButtonEnabled));
        }

        #endregion

        #region Arm fox

        public async Task OnArmFoxAsync()
        {
            await _staticFoxStatusManager.ArmFoxAsync(async () => await OnArmFoxResponseAsync());
        }

        private async Task OnArmFoxResponseAsync()
        {
            // Do nothing, armed status will change in corresponding event
        }

        #endregion

        #region Disarm fox

        public async Task OnDisarmFoxAsync()
        {
            await _staticFoxStatusManager.DisarmFoxAsync(async () => await OnDisarmFoxResponseAsync());
        }

        private async Task OnDisarmFoxResponseAsync()
        {
            await _staticFoxStatusManager.GetStaticFoxStatusAsync(async (s) => await OnGetStaticFoxStatusAsync_ReloadPathway(s));
        }

        #endregion

        #region Navigation

        private void NavigateToMatchingPage()
        {
            MainModel.ActiveDisplay = ActiveDisplay.MatchingDisplay;

            Device.BeginInvokeOnMainThread(async () =>
            {
                await Navigation.PushModalAsync(_matchingView);
            });
        }

        #endregion
    }
}
