using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetDynamicFoxStatus(DynamicFoxStatus status);

    public delegate void OnMarkAntennaMatchingAsSeen();

    public delegate void OnGetAntennaMatchingData(int matcherPosition, float antennaVoltage);

    public interface IDynamicFoxStatusManager
    {
        Task GetDynamicFoxStatusAsync(OnGetDynamicFoxStatus onGetDynamicFoxStatus);

        Task MarkAntennaMatchingAsSeenAsync(OnMarkAntennaMatchingAsSeen onMarkAntennaMatchingAsSeen);

        Task GetAntennaMatchingDataAsync(int matcherPosition, OnGetAntennaMatchingData onGetAntennaMatchingData);
    }
}
