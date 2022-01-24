using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetLastErrorCodeDelegate(uint lastErrorCode);

    public delegate void OnResetLastErrorCodeDelegate();

    public delegate void OnSerialNumberUpdateDelegate();

    public delegate void OnGetBatteryADCLevelDelegate(float averagedADCLevel);

    public delegate void OnGetBatteryVoltageLevelDelegate(float averagedVoltageLevel);

    public delegate void OnGetUbattFactorsDelegate(float a, float b);

    public delegate void OnResetUbattFactorsDelegate();

    public delegate void OnSetUbattFactorsDelegate();

    public delegate void OnGetBattLevelFactorsDelegate(float a, float b);

    public delegate void OnResetBattLevelFactorsDelegate();

    public delegate void OnSetBattLevelFactorsDelegate();

    public delegate void OnGetU80mADCValueDelegate(float averagedADCValue);

    public interface IServiceCommandsManager
    {
        Task GetLastErrorCodeAsync(OnGetLastErrorCodeDelegate onGetLastErrorCode);

        Task ResetLastErrorCodeAsync(OnResetLastErrorCodeDelegate onResetLastErrorCode);

        Task UpdateSerialNumberAsync(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate);

        Task GetBatteryADCLevelAsync(OnGetBatteryADCLevelDelegate onGetBatteryADCLevel);

        Task GetBatteryVoltageLevelAsync(OnGetBatteryVoltageLevelDelegate onGetBatteryVoltageLevel);

        Task GetUbattFactorsAsync(OnGetUbattFactorsDelegate onGetUBattFactors);

        Task ResetUbattFactorsAsync(OnResetUbattFactorsDelegate onResetUbattFactors);

        Task SetUbattFactorsAsync(float a, float b, OnSetUbattFactorsDelegate onSetUbattFactors);

        Task GetBattLevelFactorsAsync(OnGetBattLevelFactorsDelegate onGetBattLevelFactors);

        Task ResetBattLevelFactorsAsync(OnResetBattLevelFactorsDelegate onResetBattLevelFactors);

        Task SetBattLevelFactorsAsync(float a, float b, OnSetBattLevelFactorsDelegate onSetBattLevelFactors);

        Task GetU80mADCValueAsync(OnGetU80mADCValueDelegate onGetU80mADCValue);
    }
}
