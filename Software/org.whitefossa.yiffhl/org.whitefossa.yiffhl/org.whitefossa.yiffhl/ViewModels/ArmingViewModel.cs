using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace org.whitefossa.yiffhl.ViewModels
{
    public class ArmingViewModel : BindableObject
    {
        public delegate void RedrawMatchingGraphDelegate();

        private readonly IDynamicFoxStatusManager _dynamicFoxStatusManager;

        public MainModel MainModel;

        public INavigation Navigation;

        private AntennaMatchingStatus _previousMatchingStatus = AntennaMatchingStatus.InProgress;

        private IProgressDialog _progressDialog;

        public RedrawMatchingGraphDelegate RedrawMatchingGraph { get; set; }

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

        public ArmingViewModel()
        {
            MainModel = App.Container.Resolve<IMainModel>() as MainModel;
            _dynamicFoxStatusManager = App.Container.Resolve<IDynamicFoxStatusManager>();
        }

        //private async Task OnFoxArmingInitiatedAsync(IFoxArmingInitiatedEvent foxArmingInitiatedEvent)
        //{
        //    MainModel.ArmingModel.Status = ArmingStatus.Initiated;
        //    OnPropertyChanged(nameof(ArmingStatusFormatted));

        //    MainModel.ArmingModel.MatchingData.Clear();
        //    MainModel.ArmingModel.OrderdMatchingData = null;
        //    MainModel.ArmingModel.BestMatchingPosition = 0;
        //    MainModel.ArmingModel.BestMatchingPositionVoltage = 0;
        //}

        //private async Task OnAntennaMatchingMeasurementAsync(IAntennaMatchingMeasurementEvent antennaMatchingMeasurementEvent)
        //{
        //    var position = antennaMatchingMeasurementEvent.GetMatchingPosition();
        //    var voltage = antennaMatchingMeasurementEvent.GetAntennaVoltage();

        //    MainModel.ArmingModel.MatchingData.Add(position, voltage);
        //    MainModel.ArmingModel.OrderdMatchingData = MainModel.ArmingModel.MatchingData
        //        .OrderBy(kv => kv.Key);

        //    // Seeking for best matching
        //    if (voltage > MainModel.ArmingModel.BestMatchingPositionVoltage)
        //    {
        //        MainModel.ArmingModel.BestMatchingPositionVoltage = voltage;
        //        MainModel.ArmingModel.BestMatchingPosition = position;
        //    }

        //    MainModel.ArmingModel.Status = ArmingStatus.MatchingInProgress;

        //    MainModel.ArmingModel.CurrentMatchingPosition = antennaMatchingMeasurementEvent.GetMatchingPosition() + 1; // Cause counting from 0
        //    MainModel.ArmingModel.TotalMatchingPositions = antennaMatchingMeasurementEvent.GetTotalMatchingPositionsCount();
        //    OnPropertyChanged(nameof(ArmingStatusFormatted));
        //}

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
                newMatchingStatus == AntennaMatchingStatus.InProgress)
            {
                // Matching just initiated
                await OnMatchingInitiated();
            }

            if (_previousMatchingStatus == AntennaMatchingStatus.InProgress
                &&
                newMatchingStatus == AntennaMatchingStatus.Completed
                &&
                isNewForApp)
            {
                // We just completed antenna matching
                await OnAntennaMatchingCompleted();
            }

            _previousMatchingStatus = newMatchingStatus;
        }

        public async Task OnLeavingMatchingDisplayAsync()
        {
            await _dynamicFoxStatusManager.MarkAntennaMatchingAsSeen(OnMarkAntennaMatchingAsSeen);

            MainModel.ActiveDisplay = ActiveDisplay.MainDisplay;
            await Navigation.PopModalAsync();
        }

        private void OnMarkAntennaMatchingAsSeen()
        {
            Debug.WriteLine("Antenna matching data marked as seen");
        }

        private async Task OnMatchingInitiated()
        {
            MainModel.ArmingModel.BestMatchingPosition = 0;
            MainModel.ArmingModel.BestMatchingPositionVoltage = 0;
            MainModel.ArmingModel.MatchingData.Clear();

            RedrawMatchingGraph();
        }

        private async Task OnAntennaMatchingCompleted()
        {
            // Making snapshot to arming model
            MainModel.ArmingModel.BestMatchingPosition = MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchPosition;
            MainModel.ArmingModel.BestMatchingPositionVoltage = MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchVoltage;

            // Loading voltages
            var positionsCount = MainModel.DynamicFoxStatus.AntennaMatchingStatus.TotalMatcherPositions;

            MainModel.ArmingModel.MatchingData.Clear();

            _progressDialog = UserDialogs.Instance.Progress("Loading matching data...", null, null, true, MaskType.Clear);

            for (var position = 0; position < positionsCount; position ++)
            {
                _progressDialog.PercentComplete = (int)Math.Round(100 * position / (double)positionsCount);
            }

            _progressDialog.Dispose();

            RedrawMatchingGraph();
        }

    }
}
