using org.whitefossa.yiffhl.Abstractions.Enums;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetAntennaMatchingStatusDelegate
    (
        AntennaMatchingStatus status,
        bool isNewForApp,
        int totalMatcherPositions,
        int currentMatcherPosition,
        float currentAntennaVoltage,
        int currentBestMatchPosition,
        float currentBestMatchVoltage
    );

    public delegate void OnMarkAntennaMatchingAsSeenDelegate();

    public delegate void OnGetAntennaMatchingDataDelegate(int matcherPosition, float antennaVoltage);

    public interface IAntennaMatchingManager
    {
        Task GetAntennaMatchingStatusAsync(OnGetAntennaMatchingStatusDelegate onGetAntennaMatchingStatus);

        Task MarkAntennaMatchingAsSeenAsync(OnMarkAntennaMatchingAsSeenDelegate onMarkAntennaMatchingAsSeen);

        Task GetAntennaMatchingDataAsync(int matcherPosition, OnGetAntennaMatchingDataDelegate onGetAntennaMatchingData);
    }
}
