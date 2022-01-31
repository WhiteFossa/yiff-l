using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Models;
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    public class MatchingPageViewModel : BindableObject
    {
        public delegate void RedrawMatchingGraphDelegate();

        private readonly IAntennaMatchingManager _antennaMatchingManager;

        public MainModel MainModel;

        public INavigation Navigation;

        private AntennaMatchingStatus _previousMatchingStatus = AntennaMatchingStatus.NotSet;

        private IProgressDialog _progressDialog;

        public RedrawMatchingGraphDelegate RedrawMatchingGraph { get; set; }

        public bool ForceReloadMatchingData { get; set; }

        /// <summary>
        /// Matching status, formatted
        /// </summary>
        public string MatchingStatusFormatted
        {
            get
            {
                switch (MainModel.DynamicFoxStatus.AntennaMatchingStatus.Status)
                {
                    case AntennaMatchingStatus.NeverInitiated:
                        return "Not initiated";

                    case AntennaMatchingStatus.InProgress:
                        return "In progress";

                    case AntennaMatchingStatus.Completed:
                        return "Completed";

                    default:
                        throw new InvalidOperationException("Invalid arming status");
                }
            }
        }

        public string CurrentMatcherPositionFormatted
        {
            get
            {
                return $"{ MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentMatcherPosition + 1 } of " +
                       $"{ MainModel.DynamicFoxStatus.AntennaMatchingStatus.TotalMatcherPositions }";
            }
        }

        public string CurrentVoltageFormatted
        {
            get
            {
                return String.Format("{0:0.0V}", MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentVoltage);
            }
        }

        public string MaximalVoltageFormatted
        {
            get
            {
                return String.Format("{0:0.0V}", MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchVoltage);
            }
        }

        public MatchingPageViewModel()
        {
            MainModel = App.Container.Resolve<IMainModel>() as MainModel;
            _antennaMatchingManager = App.Container.Resolve<IAntennaMatchingManager>();
        }

        public async Task OnMatchingStatusChangedAsync()
        {
            OnPropertyChanged(nameof(MatchingStatusFormatted));
            OnPropertyChanged(nameof(CurrentMatcherPositionFormatted));
            OnPropertyChanged(nameof(CurrentVoltageFormatted));
            OnPropertyChanged(nameof(MaximalVoltageFormatted));

            var newMatchingStatus = MainModel.DynamicFoxStatus.AntennaMatchingStatus.Status;
            var isNewForApp = MainModel.DynamicFoxStatus.AntennaMatchingStatus.IsNewForApp;

            if (_previousMatchingStatus != AntennaMatchingStatus.InProgress
                &&
                newMatchingStatus == AntennaMatchingStatus.InProgress
                &&
                MainModel.AppDisplays.Peek() == ActiveDisplay.MatchingDisplay)
            {
                // Matching just initiated
                await OnMatchingInitiated();
            }

            if ((_previousMatchingStatus == AntennaMatchingStatus.NotSet || _previousMatchingStatus == AntennaMatchingStatus.InProgress)
                &&
                newMatchingStatus == AntennaMatchingStatus.Completed
                &&
                MainModel.AppDisplays.Peek() == ActiveDisplay.MatchingDisplay
                ||
                ForceReloadMatchingData)
            {
                ForceReloadMatchingData = false;

                // We just completed antenna matching
                await OnAntennaMatchingCompleted();
            }

            _previousMatchingStatus = newMatchingStatus;
        }

        public async Task OnLeavingMatchingDisplayAsync()
        {
            await _antennaMatchingManager.MarkAntennaMatchingAsSeenAsync(OnMarkAntennaMatchingAsSeen);

            MainModel.AppDisplays.Pop();
            await Navigation.PopModalAsync();
        }

        private void OnMarkAntennaMatchingAsSeen()
        {
            Debug.WriteLine("Antenna matching data marked as seen");
        }

        private async Task OnMatchingInitiated()
        {
            MainModel.MatchingModel.MatchingPositionsCount = 0;
            MainModel.MatchingModel.BestMatchingPosition = 0;
            MainModel.MatchingModel.BestMatchingPositionVoltage = 0;
            MainModel.MatchingModel.MatchingData.Clear();

            RedrawMatchingGraph();
        }

        private async Task OnAntennaMatchingCompleted()
        {
            // Making snapshot to arming model
            MainModel.MatchingModel.MatchingPositionsCount = MainModel.DynamicFoxStatus.AntennaMatchingStatus.TotalMatcherPositions;
            MainModel.MatchingModel.BestMatchingPosition = MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchPosition;
            MainModel.MatchingModel.BestMatchingPositionVoltage = MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchVoltage;
            MainModel.MatchingModel.MatchingData.Clear();

            // Loading first voltage
            _progressDialog = UserDialogs.Instance.Progress("Loading matching data...", null, null, true, MaskType.Clear);
            await _antennaMatchingManager.GetAntennaMatchingDataAsync(0, async (p, v) => await OnGetAntennaMatchingDataAsync(p, v));
        }

        private async Task OnGetAntennaMatchingDataAsync(int matcherPosition, float antennaVoltage)
        {
            MainModel.MatchingModel.MatchingData.Add(antennaVoltage);

            if (matcherPosition < MainModel.MatchingModel.MatchingPositionsCount - 1)
            {
                _progressDialog.PercentComplete = (int)Math.Round(100 * matcherPosition / (double)MainModel.MatchingModel.MatchingPositionsCount);
                await _antennaMatchingManager.GetAntennaMatchingDataAsync(matcherPosition + 1, async (p, v) => await OnGetAntennaMatchingDataAsync(p, v));
                return;
            }

            _progressDialog.Dispose();
            RedrawMatchingGraph();
        }

    }
}
