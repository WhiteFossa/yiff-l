using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
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
        /// Arming status, formatted
        /// </summary>
        public string ArmingStatusFormatted
        {
            get
            {
                switch (MainModel.ArmingModel.Status)
                {
                    case ArmingStatus.Initiated:
                        return "Initiated";

                    case ArmingStatus.MatchingInProgress:
                        return $"Antenna matching: { MainModel.ArmingModel.CurrentMatchingPosition } of " +
                               $"{ MainModel.ArmingModel.TotalMatchingPositions }";

                    case ArmingStatus.Completed:
                        return "Completed";

                    default:
                        throw new InvalidOperationException("Invalid arming status");
                }
            }
        }

        public ArmingViewModel()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();

            MainModel = App.Container.Resolve<IMainModel>() as MainModel;

            MainModel.OnFoxArmed += async (e) => await OnFoxArmedAsync(e);
            MainModel.OnAntennaMatchingMeasurement += async (e) => await OnAntennaMatchingMeasurementAsync(e);

            _packetsProcessor.RegisterOnFoxArmedEventHandler(MainModel.OnFoxArmed);
            _packetsProcessor.RegisterOnAntennaMatchingMeasurementEventHandler(MainModel.OnAntennaMatchingMeasurement);
        }

        private async Task OnAntennaMatchingMeasurementAsync(IAntennaMatchingMeasurementEvent antennaMatchingMeasurementEvent)
        {
            MainModel.ArmingModel.Status = ArmingStatus.MatchingInProgress;
            MainModel.ArmingModel.CurrentMatchingPosition = antennaMatchingMeasurementEvent.GetMatchingPosition() + 1; // Cause counting from 0
            MainModel.ArmingModel.TotalMatchingPositions = antennaMatchingMeasurementEvent.GetTotalMatchingPositionsCount();
            OnPropertyChanged(nameof(ArmingStatusFormatted));
        }

        private async Task OnFoxArmedAsync(IFoxArmedEvent foxArmedEvent)
        {
            MainModel.ArmingModel.Status = ArmingStatus.Completed;
            OnPropertyChanged(nameof(ArmingStatusFormatted));
        }

    }
}
