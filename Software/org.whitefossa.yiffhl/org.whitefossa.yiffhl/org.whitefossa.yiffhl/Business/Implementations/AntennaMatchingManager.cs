using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class AntennaMatchingManager : IAntennaMatchingManager
    {
        private readonly IMarkMatchingAsSeenCommand _markMatchingAsSeenCommand;
        private readonly IGetAntennaMatchingDataCommand _getAntennaMatchingDataCommand;
        private readonly IGetAntennaMatchingStatusCommand _getAntennaMatchingStatusCommand;

        private OnMarkAntennaMatchingAsSeen _onMarkAntennaMatchingAsSeen;
        private OnGetAntennaMatchingData _onGetAntennaMatchingData;
        private OnGetAntennaMatchingStatus _onGetAntennaMatchingStatus;

        public AntennaMatchingManager
        (
            IMarkMatchingAsSeenCommand markMatchingAsSeenCommand,
            IGetAntennaMatchingDataCommand getAntennaMatchingDataCommand,
            IGetAntennaMatchingStatusCommand getAntennaMatchingStatusCommand
        )
        {
            _markMatchingAsSeenCommand = markMatchingAsSeenCommand;
            _getAntennaMatchingDataCommand = getAntennaMatchingDataCommand;
            _getAntennaMatchingStatusCommand = getAntennaMatchingStatusCommand;
        }

        #region Mark antenna matching as seen

        public async Task MarkAntennaMatchingAsSeenAsync(OnMarkAntennaMatchingAsSeen onMarkAntennaMatchingAsSeen)
        {
            _onMarkAntennaMatchingAsSeen = onMarkAntennaMatchingAsSeen ?? throw new ArgumentNullException(nameof(onMarkAntennaMatchingAsSeen));

            _markMatchingAsSeenCommand.SetResponseDelegate(OnMarkAntennaMatchingAsSeen);
            _markMatchingAsSeenCommand.SendMarkMatchingAsSeenCommand();
        }

        private void OnMarkAntennaMatchingAsSeen()
        {
            _onMarkAntennaMatchingAsSeen();
        }

        #endregion

        #region Get antenna matching data

        public async Task GetAntennaMatchingDataAsync(int matcherPosition, OnGetAntennaMatchingData onGetAntennaMatchingData)
        {
            _onGetAntennaMatchingData = onGetAntennaMatchingData ?? throw new ArgumentNullException(nameof(onGetAntennaMatchingData));

            _getAntennaMatchingDataCommand.SetResponseDelegate(OnGetAntennaMatchingData);
            _getAntennaMatchingDataCommand.SendGetAntennaMatchingDataCommand(matcherPosition);
        }


        private void OnGetAntennaMatchingData
        (
            bool isSuccessfull,
            int matcherPosition,
            float antennaVoltage
        )
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to get antenna matching data");
            }

            _onGetAntennaMatchingData(matcherPosition, antennaVoltage);
        }

        #endregion

        #region Get antenna matching status

        public async Task GetAntennaMatchingStatusAsync(OnGetAntennaMatchingStatus onGetAntennaMatchingStatus)
        {
            _onGetAntennaMatchingStatus = onGetAntennaMatchingStatus ?? throw new ArgumentNullException(nameof(onGetAntennaMatchingStatus));

            _getAntennaMatchingStatusCommand.SetResponseDelegate(OnGetAntennaMatchingStatus);
            _getAntennaMatchingStatusCommand.SendGetAntennaMatchingStatusCommand();
        }

        private void OnGetAntennaMatchingStatus
        (
            AntennaMatchingStatus status,
            bool isNewForApp,
            int totalMatcherPositions,
            int currentMatcherPosition,
            float currentAntennaVoltage,
            int currentBestMatchPosition,
            float currentBestMatchVoltage
        )
        {
            _onGetAntennaMatchingStatus
            (
                status,
                isNewForApp,
                totalMatcherPositions,
                currentMatcherPosition,
                currentAntennaVoltage,
                currentBestMatchPosition,
                currentBestMatchVoltage
            );
        }

        #endregion
    }
}
