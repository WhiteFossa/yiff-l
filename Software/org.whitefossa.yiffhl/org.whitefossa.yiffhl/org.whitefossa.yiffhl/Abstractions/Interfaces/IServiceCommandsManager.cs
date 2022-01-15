using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetLastErrorCodeDelegate(uint lastErrorCode);

    public delegate void OnResetLastErrorCodeDelegate();

    public delegate void OnSerialNumberUpdateDelegate();

    public interface IServiceCommandsManager
    {
        Task GetLastErrorCode(OnGetLastErrorCodeDelegate onGetLastErrorCode);

        Task ResetLastErrorCode(OnResetLastErrorCodeDelegate onResetLastErrorCode);

        Task UpdateSerialNumber(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate);
    }
}
