using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetLastErrorCodeDelegate(uint lastErrorCode);

    public delegate void OnResetLastErrorCodeDelegate();

    public delegate void OnSerialNumberUpdateDelegate();

    public delegate void OnGetBatteryADCLevelDelegate(float averagedADCLevel);

    public delegate void OnGetBatteryVoltageLevelDelegate(float averagedVoltageLevel);

    public delegate void OnGetUBattFactorsDelegate(float a, float b);

    public interface IServiceCommandsManager
    {
        Task GetLastErrorCodeAsync(OnGetLastErrorCodeDelegate onGetLastErrorCode);

        Task ResetLastErrorCodeAsync(OnResetLastErrorCodeDelegate onResetLastErrorCode);

        Task UpdateSerialNumberAsync(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate);

        Task GetBatteryADCLevelAsync(OnGetBatteryADCLevelDelegate onGetBatteryADCLevel);

        Task GetBatteryVoltageLevelAsync(OnGetBatteryVoltageLevelDelegate onGetBatteryVoltageLevel);

        Task GetUBattFactorsAsync(OnGetUBattFactorsDelegate onGetUBattFactors);
    }
}
