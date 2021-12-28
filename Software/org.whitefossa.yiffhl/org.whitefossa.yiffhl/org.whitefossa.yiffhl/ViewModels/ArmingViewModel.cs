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
        private readonly IPacketsProcessor _packetsProcessor;

        public MainModel MainModel;

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
                return String.Format("{0:0.0V}", MainModel.DynamicFoxStatus.AntennaMatchingStatus.CurrentBestMatchVoltage);
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

            MainModel.OnFoxArmed += async (e) => await OnFoxArmedAsync(e);
            //MainModel.OnFoxArmingInitiated += async (e) => await OnFoxArmingInitiatedAsync(e);

            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();

            _packetsProcessor.RegisterOnFoxArmedEventHandler(MainModel.OnFoxArmed);
            //_packetsProcessor.RegisterOnFoxArmingInitiatedEventHandler(MainModel.OnFoxArmingInitiated);
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

        private async Task OnFoxArmedAsync(IFoxArmedEvent foxArmedEvent)
        {
            MainModel.ArmingModel.Status = ArmingStatus.Completed;
            OnPropertyChanged(nameof(MatchingStatusFormatted));
        }

        public void OnMatchingStatusChanged()
        {
            OnPropertyChanged(nameof(MatchingStatusFormatted));
            OnPropertyChanged(nameof(CurrentMatcherPositionFormatted));
            OnPropertyChanged(nameof(CurrentVoltageFormatted));
            OnPropertyChanged(nameof(MaximalVoltageFormatted));

        }

    }
}
