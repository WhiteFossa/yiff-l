using org.whitefossa.yiffhl.Abstractions.Enums;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetAntennaMatchingStatus
    (
        AntennaMatchingStatus status,
        bool isNewForApp,
        int totalMatcherPositions,
        int currentMatcherPosition,
        float currentAntennaVoltage,
        int currentBestMatchPosition,
        float currentBestMatchVoltage
    );

    public delegate void OnMarkAntennaMatchingAsSeen();

    public delegate void OnGetAntennaMatchingData(int matcherPosition, float antennaVoltage);

    public interface IAntennaMatchingManager
    {
        Task GetAntennaMatchingStatusAsync(OnGetAntennaMatchingStatus onGetAntennaMatchingStatus);

        Task MarkAntennaMatchingAsSeenAsync(OnMarkAntennaMatchingAsSeen onMarkAntennaMatchingAsSeen);

        Task GetAntennaMatchingDataAsync(int matcherPosition, OnGetAntennaMatchingData onGetAntennaMatchingData);
    }
}
